#include <iostream>
#include <smmintrin.h> // sse4.2
#include <immintrin.h>   // avx

#include "Resources.hpp"
#include "MicroSpec.hpp"
#include "Predictor.hpp"

using namespace std;

namespace microspec
{
	void Seq_DFA::run(const Table* table, const Input* input)
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
			//  table->getAction()(...., state_);
		}

		printf("The final state is  %d\n", state_);
	}

	void Spec_DFA::ReExecute(const Table* table, const Input* input, 
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

					if (correctState == predictState)
						break;
				}
				if (j == (i+1)*chunkLength)
					f[i] = correctState;
			}
		}
	}

	void Spec_DFA_Gather_Single::run(const Table* table, const Input* input)
	{		
		int* tableList_ = table->getTable();
		int state_ = table->getStartState();
		int nstate_ = table->getNumState();
		int nsymbol_ = table->getNumSymbol();

		int* Inputs_ = input->getPointer();
		int length_ = input->getLength();

		Predictor* objPredictor = Predictor::ConstructPredictor(table, input, 1, AVX2FACTOR, mLookBack);
		objPredictor->SequentialPrediction();

		int i;
		__m256i scurrent_v;
		__m256i avxsymbol_v;
		__m256i avxmask_v;
		int* scurrent __attribute__ ((aligned (32)));
		scurrent= new int [AVX2FACTOR];
		int* current __attribute__ ((aligned (32)));
		current= new int [AVX2FACTOR];
		int* avxsymbol __attribute__ ((aligned (32)));
		avxsymbol=new int [AVX2FACTOR] ;
		int* addr __attribute__ ((aligned (32)));
		addr=new int [AVX2FACTOR];

		avxmask_v = _mm256_set1_epi32(0XFFFFFFFF);
		for(i=0; i<AVX2FACTOR; i++)
			scurrent[i] = objPredictor->getPredictState(i);

		scurrent_v = _mm256_maskload_epi32 ((int*)scurrent, avxmask_v);

		long bound = length_/AVX2FACTOR;

		long cal[AVX2FACTOR];
		for(i=0; i<AVX2FACTOR; i++)
			cal[i]=i*bound;

		for(i = 0; i < bound; i++)
		{
	        avxsymbol_v=_mm256_set_epi32(Inputs_[i+cal[7]],Inputs_[i+cal[6]],Inputs_[i+cal[5]],Inputs_[i+cal[4]],Inputs_[i+cal[3]],Inputs_[i+cal[2]],Inputs_[i+cal[1]],Inputs_[i]);
	        __m256i addr_v = _mm256_add_epi32(_mm256_mullo_epi32(scurrent_v, _mm256_set1_epi32(nsymbol_)), avxsymbol_v);

			scurrent_v = _mm256_i32gather_epi32 (tableList_, addr_v, 4);

	    }
	    
	    _mm256_store_si256((__m256i*)scurrent, scurrent_v);

		printf("The final state is  %d\n", state_);
	}

	void Spec_DFA_Unroll_Single::run(const Table* table, const Input* input)
	{	
		int* tableList_ = table->getTable();
		int state_ = table->getStartState();
		int nstate_ = table->getNumState();
		int nsymbol_ = table->getNumSymbol();
		
		int* Inputs_ = input->getPointer();
		int length_ = input->getLength();

		int* scurrent;
		int* avxsymbol;
		scurrent = new int [UNROLLFACTOR];
		avxsymbol = new int [UNROLLFACTOR];
	
		Predictor* objPredictor = Predictor::ConstructPredictor(table, input, 1, UNROLLFACTOR, mLookBack);
		objPredictor->SequentialPrediction();

		long i,j,gap;	

		for(i=0; i<UNROLLFACTOR; i++)
			scurrent[i]=objPredictor->getPredictState(i);
		long bound = length_/UNROLLFACTOR;

		for(i=0; i<bound; i++)
		{
			gap = 0;
			scurrent[0] = (tableList_[scurrent[0]* nsymbol_ + Inputs_[i+gap]] & 0X0FFFFFFF );

			gap = gap + bound;
			scurrent[1] = (tableList_[scurrent[1]* nsymbol_ + Inputs_[i+gap]] & 0X0FFFFFFF );
			
			gap = gap + bound;
			scurrent[2] = (tableList_[scurrent[2]* nsymbol_ + Inputs_[i+gap]] & 0X0FFFFFFF );
			
			gap = gap + bound;
			scurrent[3] = (tableList_[scurrent[3]* nsymbol_ + Inputs_[i+gap]] & 0X0FFFFFFF );
			
			gap = gap + bound;
			scurrent[4] = (tableList_[scurrent[4]* nsymbol_ + Inputs_[i+gap]] & 0X0FFFFFFF );
			
			gap = gap + bound;
			scurrent[5] = (tableList_[scurrent[5]* nsymbol_ + Inputs_[i+gap]] & 0X0FFFFFFF );
			
			gap = gap + bound;
			scurrent[6] = (tableList_[scurrent[6]* nsymbol_ + Inputs_[i+gap]] & 0X0FFFFFFF );
			
			gap = gap + bound;
			scurrent[7] = (tableList_[scurrent[7]* nsymbol_ + Inputs_[i+gap]] & 0X0FFFFFFF );
		}
		for(i=0; i<UNROLLFACTOR; i++)
			objPredictor->setFinalState(i, scurrent[i]);

		this->ReExecute(table, input, objPredictor->getPredictStatePointer(), scurrent, UNROLLFACTOR);

		printf("The final state is  %d\n", scurrent[UNROLLFACTOR-1]);

		delete []scurrent;
		delete []avxsymbol;		
	}	

}
