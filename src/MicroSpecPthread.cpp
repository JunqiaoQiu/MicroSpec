#include <iostream>
#include <sys/sysinfo.h>
#include <smmintrin.h> // sse4.2
#include <immintrin.h>   // avx

#include <pthread.h>

#include "Resources.h"
#include "Predictor.h"

#include "MicroSpec.h"
#include "MicroSpecPthread.h"

using namespace std;

namespace microspec
{
	void Spec_DFA_Pthread::run(const Table* table, const Input* input)
	{
		int* tableList_ = table->getTable();
		int state_ = table->getStartState();
		int nstate_ = table->getNumState();
		int nsymbol_ = table->getNumSymbol();

		int* Inputs_ = input->getPointer();
		int length_ = input->getLength();

		Predictor* objPredictor = Predictor::constructPredictor(table, input, mthreads, mchunks, mLookBack);
		objPredictor->parallelPrediction();

		//PTHREAD--------------------------------
		int errorCheck1, errorCheck2;
		long t;
		pthread_t* threads;			
		threads=(pthread_t*)malloc(sizeof(pthread_t)* mthreads);
		cpu_set_t* cpu;		// thread binding variables
		int MAXCPU = (mthreads > get_nprocs() ? get_nprocs():mthreads);
		cpu=(cpu_set_t*)malloc(sizeof(cpu_set_t) * MAXCPU);
		//PTHREAD-------------------------------	

		// thread binding
		for(t=0; t < MAXCPU; t++)
		{
			CPU_ZERO(&cpu[t]);
			CPU_SET(t, &cpu[t]);
		}	

		for(t=0; t < mthreads; t++)
		{
			DFAPassPointer* var;
			var = new DFAPassPointer();
			var->pointer = this;
			var->tid = t;
			var->p_predictor = objPredictor;

			errorCheck1 = pthread_create(&threads[t], NULL, caller, (void*)var);
			if (errorCheck1)
			{
				printf("ERROR; return code from pthread_create() is %d\n", errorCheck1);
				exit(-1);
			}
			errorCheck2 = pthread_setaffinity_np(threads[t], sizeof(cpu_set_t), &cpu[t%MAXCPU]);
		}
		for(t=0; t< mthreads; t++)
	    	pthread_join(threads[t], NULL);

	    int* currentfinal;
	    currentfinal = new int [mchunks];
	    for (t = 0; t < mchunks; t++)
	    	currentfinal[t] = objPredictor->getEndingState(t);

		this->re_execute(table, input, objPredictor->getPredictStatePointer(), currentfinal, mchunks);

		printf("The final state is  %d\n", currentfinal[mchunks-1]);
	}

	void* Spec_DFA_Pthread::caller(void* args)
	{
		DFAPassPointer* Arg = (DFAPassPointer*)args;
		Arg->pointer->parallel_run(Arg->tid, Arg->p_predictor);
		pthread_exit((void*)args);
	}

	void Spec_DFA_Pthread::parallel_run(int tid, Predictor* p)
	{
		const Table* table = p->getTableUsed();
		const Input* input = p->getInputUsed();

		int* tableList_ = table->getTable();
		int nstate_ = table->getNumState();
		int nsymbol_ = table->getNumSymbol();
		
		int* Inputs_ = input->getPointer();
		long length_ = input->getLength();

		int state_ = p->getPredictState(tid);
		long ChunkLength_ = length_ / mthreads;

		for (long i = tid * ChunkLength_; i < (tid + 1) * ChunkLength_; i++)
		{
			int temp = tableList_[state_ * nsymbol_ + Inputs_[i]];
			state_ = temp & 0X0FFFFFFF;

			// Action Function (state_)
			//  table->getAction()(...., state_);
		}
		p->setEndingState(tid, state_);
	}

	void Spec_DFA_Gather_Pthread::parallel_run(int tid, Predictor* p)
	{
		const Table* table = p->getTableUsed();
		const Input* input = p->getInputUsed();

		int* tableList_ = table->getTable();
		int nstate_ = table->getNumState();
		int nsymbol_ = table->getNumSymbol();
		
		int* Inputs_ = input->getPointer();
		long length_ = input->getLength();
		long i,j,k;

		__m256i scurrent_v;
		__m256i avxsymbol_v;

		__m256i avxmask_v;
		int* scurrent __attribute__ ((aligned (32)));
		scurrent= new int [MICROSPEC_SIMDFACTOR];

		avxmask_v = _mm256_set1_epi32(0XFFFFFFFF);
		for(i=0; i<MICROSPEC_SIMDFACTOR; i++)
			scurrent[i] = p->getPredictState(tid * MICROSPEC_SIMDFACTOR + i);
		scurrent_v=_mm256_maskload_epi32 ((int*)scurrent, avxmask_v);	
		avxmask_v  = _mm256_set1_epi32(0X0FFFFFFF);

		long ChunkLength_ = length_ / mthreads;
		long bound = ChunkLength_ / MICROSPEC_SIMDFACTOR;

		long cal[8];
		for(i=0; i<8; i++)
			cal[i]=i*bound;

		for(i = tid*ChunkLength_; i < bound + tid*ChunkLength_; i++)
		{
			// get the address of next state in the table 
			// 1, set the input symbols as avx2 format; 2, calculate the table address  
        	avxsymbol_v=_mm256_set_epi32(Inputs_[i+cal[7]], Inputs_[i+cal[6]], Inputs_[i+cal[5]],
        		Inputs_[i+cal[4]], Inputs_[i+cal[3]], Inputs_[i+cal[2]], Inputs_[i+cal[1]], Inputs_[i]);
        	__m256i addr_v = _mm256_add_epi32(_mm256_mullo_epi32(scurrent_v, _mm256_set1_epi32(nsymbol_)), avxsymbol_v);

        	// access the table to update the states
			scurrent_v = _mm256_i32gather_epi32 (tableList_, addr_v, 4);
			scurrent_v = _mm256_and_si256(scurrent_v, avxmask_v);
    	}    

    	printf("Current Cores is %d \n", sched_getcpu());
    	
    	_mm256_store_si256((__m256i*)scurrent, scurrent_v);

    	for(i = 0; i < MICROSPEC_SIMDFACTOR; i++)
			p->setEndingState(tid*MICROSPEC_SIMDFACTOR+i, scurrent[i]);
	}

	void Spec_DFA_Unroll_Pthread::parallel_run(int tid, Predictor* p)
	{
		const Table* table = p->getTableUsed();
		const Input* input = p->getInputUsed();

		int* tableList_ = table->getTable();
		int nsymbol_ = table->getNumSymbol();
		
		int* Inputs_ = input->getPointer();
		long length_ = input->getLength();

		long i,gap;
		int* scurrent;
		int* avxsymbol;
		
		scurrent = new int [MICROSPEC_UNROLLFACTOR];
		avxsymbol = new int [MICROSPEC_UNROLLFACTOR];
	
		for(i=0; i<MICROSPEC_UNROLLFACTOR; i++)
			scurrent[i] = p->getPredictState(tid * MICROSPEC_UNROLLFACTOR + i);
		
		long ChunkLength_ = length_ / mthreads;
		long bound = ChunkLength_ / MICROSPEC_UNROLLFACTOR;

		for(i=tid*ChunkLength_; i<bound+tid*ChunkLength_; i++)
		{
			gap = 0;
			scurrent[0]=(tableList_[scurrent[0]*nsymbol_+Inputs_[i+gap]] & 0X0FFFFFFF );

			gap = gap + bound;
			scurrent[1]=(tableList_[scurrent[1]*nsymbol_+Inputs_[i+gap]] & 0X0FFFFFFF );

			gap = gap + bound;
			scurrent[2]=(tableList_[scurrent[2]*nsymbol_+Inputs_[i+gap]] & 0X0FFFFFFF );

			gap = gap + bound;
			scurrent[3]=(tableList_[scurrent[3]*nsymbol_+Inputs_[i+gap]] & 0X0FFFFFFF );

			gap = gap + bound;
			scurrent[4]=(tableList_[scurrent[4]*nsymbol_+Inputs_[i+gap]] & 0X0FFFFFFF );

			gap = gap + bound;
			scurrent[5]=(tableList_[scurrent[5]*nsymbol_+Inputs_[i+gap]] & 0X0FFFFFFF );

			gap = gap + bound;
			scurrent[6]=(tableList_[scurrent[6]*nsymbol_+Inputs_[i+gap]] & 0X0FFFFFFF );

			gap = gap + bound;
			scurrent[7]=(tableList_[scurrent[7]*nsymbol_+Inputs_[i+gap]] & 0X0FFFFFFF );
		}

		for(i=0; i<MICROSPEC_UNROLLFACTOR; i++)
			p->setEndingState(tid*MICROSPEC_UNROLLFACTOR+i, scurrent[i]);	
	}

	void Spec_DFA_GatherUnroll_Pthread::parallel_run(int tid, Predictor* p)
	{
		const Table* table = p->getTableUsed();
		const Input* input = p->getInputUsed();

		int* tableList_ = table->getTable();
		int nsymbol_ = table->getNumSymbol();
		
		int* Inputs_ = input->getPointer();
		long length_ = input->getLength();

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
				scurrent[j] = p->getPredictState(j*MICROSPEC_SIMDUNROLLFACTOR + i + tid * MICROSPEC_SIMDFACTOR * MICROSPEC_SIMDUNROLLFACTOR);
			scurrent_v[i] = _mm256_maskload_epi32 ((int*)scurrent, avxmask_v); 			
		}
		avxmask_v  = _mm256_set1_epi32(0X0FFFFFFF);

		long bound = length_ / mchunks;

		long cal[MICROSPEC_SIMDUNROLLFACTOR][MICROSPEC_SIMDFACTOR];
		for(i=0; i<MICROSPEC_SIMDUNROLLFACTOR; i++)
			for(j=0; j<MICROSPEC_SIMDFACTOR; j++)
				cal[i][j]= i*bound + j*bound*MICROSPEC_SIMDUNROLLFACTOR + tid * MICROSPEC_SIMDFACTOR * MICROSPEC_SIMDUNROLLFACTOR * bound;

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

	    for(i=0; i<MICROSPEC_SIMDUNROLLFACTOR; i++)
	    {
	        _mm256_store_si256((__m256i*)scurrent, scurrent_v[i]);
	        for(j = 0; j < MICROSPEC_SIMDFACTOR; j++)
	        	p->setEndingState(tid*MICROSPEC_SIMDUNROLLFACTOR*MICROSPEC_SIMDFACTOR + j*MICROSPEC_SIMDUNROLLFACTOR+i , scurrent[j]);	
	    }	    			
	}

	void Spec_DFA_UnrollGather_Pthread::parallel_run(int tid, Predictor* p)
	{
		const Table* table = p->getTableUsed();
		const Input* input = p->getInputUsed();

		int* tableList_ = table->getTable();
		int nsymbol_ = table->getNumSymbol();
		
		int* Inputs_ = input->getPointer();
		long length_ = input->getLength();

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
				scurrent[j] = p->getPredictState(i*MICROSPEC_SIMDFACTOR + j + tid * MICROSPEC_SIMDFACTOR * MICROSPEC_SIMDUNROLLFACTOR);
			scurrent_v[i] = _mm256_maskload_epi32 ((int*)scurrent, avxmask_v); 			
		}
		avxmask_v  = _mm256_set1_epi32(0X0FFFFFFF);

		long bound = length_ / mchunks;

		long cal[MICROSPEC_SIMDUNROLLFACTOR][MICROSPEC_SIMDFACTOR];
		for(i=0; i<MICROSPEC_SIMDUNROLLFACTOR; i++)
			for(j=0; j<MICROSPEC_SIMDFACTOR; j++)
				cal[i][j]= i*MICROSPEC_SIMDFACTOR*bound + j*bound+ tid * MICROSPEC_SIMDFACTOR * MICROSPEC_SIMDUNROLLFACTOR * bound;

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

	    for(i=0; i<MICROSPEC_SIMDUNROLLFACTOR; i++)
	    {
	        _mm256_store_si256((__m256i*)scurrent, scurrent_v[i]);
	        for(j = 0; j < MICROSPEC_SIMDFACTOR; j++)
	        	p->setEndingState(tid*MICROSPEC_SIMDUNROLLFACTOR*MICROSPEC_SIMDFACTOR + i*MICROSPEC_SIMDFACTOR+j , scurrent[j]);	
	    }	    			
	}

}
