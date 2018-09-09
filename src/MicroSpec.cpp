#include <iostream>
#include <smmintrin.h> // sse4.2
#include <immintrin.h>   // avx
#include <cstring>
#include <string>

#include "Resources.h"
#include "MicroSpec.h"
#include "Predictor.h"
#include "Action.h"

using namespace std;

namespace microspec
{
/* Implementation of class DFA */ 
	DFA::DFA()
	{
		mEndingResults = new DFAResults();
		mEndingResults->mResutls = 0;
		mAction = doNothing;
	}

	DFA::~DFA()
	{
		delete mEndingResults;
	}

	void DFA::setAction(char* actionType)
	{
		char* actionTypeLow;
		actionTypeLow = new char [strlen(actionType)];
		for (int i = 0; i < strlen(actionType); ++i)
	    	actionTypeLow[i] = tolower(actionType[i]);	

	    if (actionTypeLow == std::string("accumulate"))	
	    	mAction = accumulateAction;
	}

	void DFA::printResults()
	{
		cout << "DFA final Results are " << mEndingResults->mResutls << endl;
	}

/* Implementation of class SeqDFA */ 

	void SeqDFA::run(const Table* table, const Input* input)
	{	
		int* tableList_ = table->getTable();
		int state_ = table->getStartState();
		int nstate_ = table->getNumState();
		int nsymbol_ = table->getNumSymbol();
		
		int* Inputs_ = input->getPointer();
		int length_ = input->getLength();

		for (long i = 0; i < length_; i++)
		{
			int temp = tableList_[state_ * nsymbol_ + Inputs_[i]];
			state_ = temp & 0X0FFFFFFF;

			// Action Function (state_)
			mAction(state_, mEndingResults);
		}

		printf("The final state is  %d\n", state_);
		this->printResults();
	}

/* Implementation of class SpecDFA */

	SpecDFA::SpecDFA():DFA(), mLookBack(0) 
	{
		mReExecuteAction = doNothingReExecute;
	}

	SpecDFA::SpecDFA():DFA(), mLookBack(lookBackLength) 
	{
		mReExecuteAction = doNothingReExecute;
	}

	void SpecDFA::setAction(char* actionType)
	{
		char* actionTypeLow;
		actionTypeLow = new char [strlen(actionType)];
		for (int i = 0; i < strlen(actionType); ++i)
	    	actionTypeLow[i] = tolower(actionType[i]);	

	    if (actionTypeLow == std::string("accumulate"))	
	    {
	    	mAction = accumulateAction;
	    	mReExecuteAction = accumulateActionReExecute;
	    }
	}

	void SpecDFA::re_execute(const Table* table, const Input* input, 
		int* p, int* f, int size)
	{
		int* tableList_ = table->getTable();
		int state_ = table->getStartState();
		int nstate_ = table->getNumState();
		int nsymbol_ = table->getNumSymbol();
		int* Inputs_ = input->getPointer();	

		long chunkLength = input->getLength()/size;
		int i;
		long j;
		for (i = 1; i < size; i++)
		{
			if (p[i] != f[i-1])
			{
				int correctState = f[i-1];
				int predictState = p[i];

				for (j = i*chunkLength; j < (i+1)*chunkLength; j++)
				{
					int temp_correct = tableList_[correctState * nsymbol_ + Inputs_[j]];
					int temp_predict = tableList_[predictState * nsymbol_ + Inputs_[j]];
					correctState = temp_correct & 0X0FFFFFFF;
					predictState = temp_predict & 0X0FFFFFFF;
				
					// insert Action Function (temp_correct, temp_predict)
					mReExecuteAction(temp_correct, temp_predict, mEndingResults);

					if (correctState == predictState)
						break;
				}
				if (j == (i+1)*chunkLength)
					f[i] = correctState;
			}
		}
	}

/* Implementation of class SpecDFA_Gather_Single */

	SpecDFA_Gather_Single::SpecDFA_Gather_Single():SpecDFA()
	{
		mActionSIMD = doNothingSIMD;
	}

	SpecDFA_Gather_Single::SpecDFA_Gather_Single(int lookBackLength):SpecDFA(lookBackLength)
	{
		mActionSIMD = doNothingSIMD;
	}

	void SpecDFA_Gather_Single::setAction(char* actionType)
	{
		char* actionTypeLow;
		actionTypeLow = new char [strlen(actionType)];
		for (int i = 0; i < strlen(actionType); ++i)
	    	actionTypeLow[i] = tolower(actionType[i]);	

	    if (actionTypeLow == std::string("accumulate"))	
	    {
	    	mAction = accumulateAction;
	    	mReExecuteAction = accumulateActionReExecute;
	    	mActionSIMD = accumulateActionSIMD;
	    }
	}

	void SpecDFA_Gather_Single::run(const Table* table, const Input* input)
	{		
		int* tableList_ = table->getTable();
		int nstate_ = table->getNumState();
		int nsymbol_ = table->getNumSymbol();
		int* Inputs_ = input->getPointer();
		int length_ = input->getLength();

		// construct the predictor, and use sequential prediction
		Predictor* objPredictor = Predictor::constructPredictor(table, input, 1, MICROSPEC_SIMDFACTOR, mLookBack);
		objPredictor->sequentialPrediction(); 

		int i;
		__m256i scurrent_v;
		__m256i avxsymbol_v;
		__m256i avxmask_v;

		__m256i tempStore_v[1];
		tempStore_v[0] = _mm256_set1_epi32(0);

		int scurrent[MICROSPEC_SIMDFACTOR] __attribute__ ((aligned (32)));
		//scurrent= new int [MICROSPEC_SIMDFACTOR];
		int resultsPerChunk[MICROSPEC_SIMDFACTOR] __attribute__ ((aligned (32)));

		// set up the predicted starting states in avx2 format
		avxmask_v = _mm256_set1_epi32(0XFFFFFFFF);
		for(i=0; i<MICROSPEC_SIMDFACTOR; i++)
		{
			scurrent[i] = objPredictor->getPredictState(i);
			mEndingResultsPerChunk[i].mResutls = 0;
		}
		scurrent_v = _mm256_maskload_epi32 ((int*)scurrent, avxmask_v);
		avxmask_v  = _mm256_set1_epi32(0X0FFFFFFF);

		long bound = length_/MICROSPEC_SIMDFACTOR;

		long cal[MICROSPEC_SIMDFACTOR];
		for(i=0; i<MICROSPEC_SIMDFACTOR; i++)
			cal[i] = i*bound;

		for(i = 0; i < bound; i++)
		{
			// get the address of next state in the table 
			// 1, set the input symbols as avx2 format; 2, calculate the table address  
	        avxsymbol_v=_mm256_set_epi32(Inputs_[i+cal[7]],Inputs_[i+cal[6]],Inputs_[i+cal[5]],Inputs_[i+cal[4]],
	        	Inputs_[i+cal[3]],Inputs_[i+cal[2]],Inputs_[i+cal[1]],Inputs_[i]);
	        __m256i addr_v = _mm256_add_epi32(_mm256_mullo_epi32(scurrent_v, _mm256_set1_epi32(nsymbol_)), avxsymbol_v);

	        // access the table to update the states
			scurrent_v = _mm256_i32gather_epi32 (tableList_, addr_v, 4);
			mActionSIMD(scurrent_v, tempStore_v[0]);
			scurrent_v = _mm256_and_si256(scurrent_v, avxmask_v);
	    }
	    
	    _mm256_store_si256((__m256i*)scurrent, scurrent_v);
	    _mm256_store_si256(resultsPerChunk, tempStore_v[0]);

	    for(i=0; i<MICROSPEC_SIMDFACTOR; i++)
	    	mEndingResults->mResutls += resultsPerChunk[i];

		this->re_execute(table, input, objPredictor->getPredictStatePointer(), scurrent, MICROSPEC_SIMDFACTOR);

		printf("The final state is  %d\n", scurrent[MICROSPEC_SIMDFACTOR-1]);
		this->printResults();
		//delete []scurrent;
	}

/* Implementation of class SpecDFA_Unroll_Single */

	void SpecDFA_Unroll_Single::run(const Table* table, const Input* input)
	{	
		int* tableList_ = table->getTable();
		int nstate_ = table->getNumState();
		int nsymbol_ = table->getNumSymbol();
		
		int* Inputs_ = input->getPointer();
		int length_ = input->getLength();

		int* scurrent;
		scurrent = new int [MICROSPEC_UNROLLFACTOR];
	
		Predictor* objPredictor = Predictor::constructPredictor(table, input, 1, MICROSPEC_UNROLLFACTOR, mLookBack);
		objPredictor->sequentialPrediction();
		mEndingResultsPerChunk = new DFAResults [MICROSPEC_UNROLLFACTOR];

		long i,j,gap;	

		for(i=0; i<MICROSPEC_UNROLLFACTOR; i++)
		{
			scurrent[i]=objPredictor->getPredictState(i);
			mEndingResultsPerChunk[i].mResutls = 0;
		}
		long bound = length_/MICROSPEC_UNROLLFACTOR;

		for(i=0; i<bound; i++)
		{
			// manually Unroll 
			gap = 0;
			int temp0 = tableList_[scurrent[0]* nsymbol_ + Inputs_[i+gap]];
			scurrent[0] = ( temp0 & 0X0FFFFFFF );
			mAction(temp0, &mEndingResultsPerChunk[0]);

			gap = gap + bound;
			int temp1 = tableList_[scurrent[1]* nsymbol_ + Inputs_[i+gap]];
			scurrent[1] = ( temp1 & 0X0FFFFFFF );
			mAction(temp1, &mEndingResultsPerChunk[1]);
			
			gap = gap + bound;
			int temp2 = tableList_[scurrent[2]* nsymbol_ + Inputs_[i+gap]];
			scurrent[2] = ( temp2 & 0X0FFFFFFF );
			mAction(temp2, &mEndingResultsPerChunk[2]);
			
			gap = gap + bound;
			int temp3 = tableList_[scurrent[3]* nsymbol_ + Inputs_[i+gap]] ;
			scurrent[3] = ( temp3 & 0X0FFFFFFF );
			mAction(temp3, &mEndingResultsPerChunk[3]);
			
			gap = gap + bound;
			int temp4 = tableList_[scurrent[4]* nsymbol_ + Inputs_[i+gap]];
			scurrent[4] = ( temp4 & 0X0FFFFFFF );
			mAction(temp4, &mEndingResultsPerChunk[4]);
			
			gap = gap + bound;
			int temp5 = tableList_[scurrent[5]* nsymbol_ + Inputs_[i+gap]];
			scurrent[5] = ( temp5 & 0X0FFFFFFF );
			mAction(temp5, &mEndingResultsPerChunk[5]);
			
			gap = gap + bound;
			int temp6 = tableList_[scurrent[6]* nsymbol_ + Inputs_[i+gap]];
			scurrent[6] = ( temp6 & 0X0FFFFFFF );
			mAction(temp6, &mEndingResultsPerChunk[6]);
			
			gap = gap + bound;
			int temp7 = tableList_[scurrent[7]* nsymbol_ + Inputs_[i+gap]];
			scurrent[7] = ( & 0X0FFFFFFF );
			mAction(temp7, &mEndingResultsPerChunk[7]);
		}
		for(i=0; i<MICROSPEC_UNROLLFACTOR; i++)
		{
			objPredictor->setEndingState(i, scurrent[i]);
			mEndingResults->mResutls += mEndingResultsPerChunk[i].mResutls;
		}

		this->re_execute(table, input, objPredictor->getPredictStatePointer(), scurrent, MICROSPEC_UNROLLFACTOR);

		printf("The final state is  %d\n", scurrent[MICROSPEC_UNROLLFACTOR-1]);
		this->printResults();

		delete []scurrent;		
	}	

/* Implementation of class SpecDFA_GatherUnroll_Single */

	void SpecDFA_GatherUnroll_Single::run(const Table* table, const Input* input)
	{		
		int* tableList_ = table->getTable();
		int nstate_ = table->getNumState();
		int nsymbol_ = table->getNumSymbol();
		int* Inputs_ = input->getPointer();
		int length_ = input->getLength();

		// construct the predictor, and use sequential prediction
		Predictor* objPredictor = Predictor::constructPredictor(table, input, 1, MICROSPEC_SIMDFACTOR*MICROSPEC_UNROLLFACTOR, mLookBack);
		objPredictor->sequentialPrediction();

		int i, j;
		__m256i scurrent_v[MICROSPEC_SIMDUNROLLFACTOR];
		__m256i symbol_v[MICROSPEC_SIMDUNROLLFACTOR];
    	__m256i addr_v[MICROSPEC_SIMDUNROLLFACTOR];
		__m256i avxmask_v;

		int* scurrent __attribute__ ((aligned (32)));
		scurrent= new int [MICROSPEC_SIMDFACTOR];

		// set up the predicted starting states in avx2 format
		avxmask_v = _mm256_set1_epi32(0XFFFFFFFF);

		for (i = 0; i < MICROSPEC_SIMDUNROLLFACTOR; i++)
		{
			for(j=0; j<MICROSPEC_SIMDFACTOR; j++)
				scurrent[j] = objPredictor->getPredictState(j*MICROSPEC_SIMDUNROLLFACTOR + i);
			scurrent_v[i] = _mm256_maskload_epi32 ((int*)scurrent, avxmask_v); 			
		}
		avxmask_v  = _mm256_set1_epi32(0X0FFFFFFF);

		long bound = length_ / (MICROSPEC_SIMDFACTOR*MICROSPEC_SIMDUNROLLFACTOR);

		long cal[MICROSPEC_SIMDUNROLLFACTOR][MICROSPEC_SIMDFACTOR];
		for(i=0; i<MICROSPEC_SIMDUNROLLFACTOR; i++)
			for(j=0; j<MICROSPEC_SIMDFACTOR; j++)
				cal[i][j]= i*bound + j*bound*MICROSPEC_SIMDUNROLLFACTOR;

		for(i = 0; i < bound; i++)
		{
			// get the address of next state in the table 
			// 1, set the input symbols as avx2 format; 2, calculate the table address  
			for (j=0; j<MICROSPEC_SIMDUNROLLFACTOR; j++)
			{
		        symbol_v[j] = _mm256_set_epi32(Inputs_[i+cal[j][7]],Inputs_[i+cal[j][6]],Inputs_[i+cal[j][5]],Inputs_[i+cal[j][4]],
		        	Inputs_[i+cal[j][3]],Inputs_[i+cal[j][2]],Inputs_[i+cal[j][1]],Inputs_[i+cal[j][0]]);
		        addr_v[j] = _mm256_add_epi32(_mm256_mullo_epi32(scurrent_v[j], _mm256_set1_epi32(nsymbol_)), symbol_v[j]);

		        // access the table to update the states
				scurrent_v[j] = _mm256_i32gather_epi32 (tableList_, addr_v[j], 4);
				scurrent_v[j] = _mm256_and_si256(scurrent_v[j], avxmask_v);
			}
	    }

	    int* finalStateVec;
	    finalStateVec = new int [MICROSPEC_SIMDUNROLLFACTOR*MICROSPEC_SIMDFACTOR];
	    for(i=0; i<MICROSPEC_SIMDUNROLLFACTOR; i++)
	    {
	        _mm256_store_si256((__m256i*)scurrent, scurrent_v[i]);
	        for(j = 0; j < MICROSPEC_SIMDFACTOR; j++)
	        	finalStateVec[j*MICROSPEC_SIMDUNROLLFACTOR+i] = scurrent[j];
	    }	    

		this->re_execute(table, input, objPredictor->getPredictStatePointer(), finalStateVec, MICROSPEC_SIMDFACTOR*MICROSPEC_SIMDUNROLLFACTOR);

		printf("The final state is  %d\n", finalStateVec[MICROSPEC_SIMDFACTOR*MICROSPEC_SIMDUNROLLFACTOR-1]);
	}

/* Implementation of class SpecDFA_UnrollGather_Single */

	void SpecDFA_UnrollGather_Single::run(const Table* table, const Input* input)
	{		
		int* tableList_ = table->getTable();
		int nstate_ = table->getNumState();
		int nsymbol_ = table->getNumSymbol();
		int* Inputs_ = input->getPointer();
		int length_ = input->getLength();

		// construct the predictor, and use sequential prediction
		Predictor* objPredictor = Predictor::constructPredictor(table, input, 1, MICROSPEC_SIMDFACTOR*MICROSPEC_UNROLLFACTOR, mLookBack);
		objPredictor->sequentialPrediction();

		int i, j;
		__m256i scurrent_v[MICROSPEC_SIMDUNROLLFACTOR];
		__m256i symbol_v[MICROSPEC_SIMDUNROLLFACTOR];
    	__m256i addr_v[MICROSPEC_SIMDUNROLLFACTOR];
		__m256i avxmask_v;

		int* scurrent __attribute__ ((aligned (32)));
		scurrent= new int [MICROSPEC_SIMDFACTOR];

		// set up the predicted starting states in avx2 format
		avxmask_v = _mm256_set1_epi32(0XFFFFFFFF);

		for (i = 0; i < MICROSPEC_SIMDUNROLLFACTOR; i++)
		{
			for(j=0; j<MICROSPEC_SIMDFACTOR; j++)
				scurrent[j] = objPredictor->getPredictState(i*MICROSPEC_SIMDFACTOR + j);
			scurrent_v[i] = _mm256_maskload_epi32 ((int*)scurrent, avxmask_v); 			
		}
		avxmask_v  = _mm256_set1_epi32(0X0FFFFFFF);

		long bound = length_ / (MICROSPEC_SIMDFACTOR*MICROSPEC_SIMDUNROLLFACTOR);

		long cal[MICROSPEC_SIMDUNROLLFACTOR][MICROSPEC_SIMDFACTOR];
		for(i=0; i<MICROSPEC_SIMDUNROLLFACTOR; i++)
			for(j=0; j<MICROSPEC_SIMDFACTOR; j++)
				cal[i][j]= i*MICROSPEC_SIMDFACTOR*bound + j*bound;

		for(i = 0; i < bound; i++)
		{
			// get the address of next state in the table 
			// 1, set the input symbols as avx2 format; 2, calculate the table address  
			for (j=0; j<MICROSPEC_SIMDUNROLLFACTOR; j++)
			{
		        symbol_v[j] = _mm256_set_epi32(Inputs_[i+cal[j][7]],Inputs_[i+cal[j][6]],Inputs_[i+cal[j][5]],Inputs_[i+cal[j][4]],
		        	Inputs_[i+cal[j][3]],Inputs_[i+cal[j][2]],Inputs_[i+cal[j][1]],Inputs_[i+cal[j][0]]);
		        addr_v[j] = _mm256_add_epi32(_mm256_mullo_epi32(scurrent_v[j], _mm256_set1_epi32(nsymbol_)), symbol_v[j]);

		        // access the table to update the states
				scurrent_v[j] = _mm256_i32gather_epi32 (tableList_, addr_v[j], 4);
				scurrent_v[j] = _mm256_and_si256(scurrent_v[j], avxmask_v);
			}
	    }

	    int* finalStateVec;
	    finalStateVec = new int [MICROSPEC_SIMDUNROLLFACTOR*MICROSPEC_SIMDFACTOR];
	    for(i=0; i<MICROSPEC_SIMDUNROLLFACTOR; i++)
	    {
	        _mm256_store_si256((__m256i*)scurrent, scurrent_v[i]);
	        for(j = 0; j < MICROSPEC_SIMDFACTOR; j++)
	        	finalStateVec[i*MICROSPEC_SIMDFACTOR+j] = scurrent[j];
	    }	    

	    // Re-execution after finishing parallel execution
		this->re_execute(table, input, objPredictor->getPredictStatePointer(), finalStateVec, MICROSPEC_SIMDFACTOR*MICROSPEC_SIMDUNROLLFACTOR);

		printf("The final state is  %d\n", finalStateVec[MICROSPEC_SIMDFACTOR*MICROSPEC_SIMDUNROLLFACTOR-1]);
	}

}
