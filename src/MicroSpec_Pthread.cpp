#include <iostream>
#include <sys/sysinfo.h>
#include <smmintrin.h> // sse4.2
#include <immintrin.h>   // avx

#include <pthread.h>

#include "Resources.hpp"
#include "Predictor.hpp"

#include "MicroSpec.hpp"
#include "MicroSpec_Pthread.hpp"

using namespace std;

namespace microspec
{
	void Spec_DFA_Pthread::Run(const Table* table, const Input* input)
	{
		int* tableList_ = table->getTable();
		int state_ = table->getStartState();
		int nstate_ = table->getNumState();
		int nsymbol_ = table->getNumSymbol();

		int* Inputs_ = input->getPointer();
		int length_ = input->getLength();

		Predictor* objPredictor = Predictor::ConstructPredictor(table, input, mthreads, mchunks, mLookBack);
		objPredictor->ParallelPrediction();

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
	    	currentfinal[t] = objPredictor->getFinalState(t);

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
		const Table* table = p->getUsedTable();
		const Input* input = p->getUsedInput();

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
		p->setFinalState(tid, state_);
	}

	void Spec_DFA_Gather_Pthread::parallel_run(int tid, Predictor* p)
	{
		const Table* table = p->getUsedTable();
		const Input* input = p->getUsedInput();

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
		scurrent= new int [AVX2FACTOR];

		avxmask_v = _mm256_set1_epi32(0XFFFFFFFF);
		for(i=0; i<AVX2FACTOR; i++)
			scurrent[i] = p->getPredictState(tid * AVX2FACTOR + i);
		scurrent_v=_mm256_maskload_epi32 ((int*)scurrent, avxmask_v);	
		avxmask_v  = _mm256_set1_epi32(0X0FFFFFFF);

		long ChunkLength_ = length_ / mthreads;
		long bound = ChunkLength_ / AVX2FACTOR;

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

    	for(i = 0; i < AVX2FACTOR; i++)
			p->setFinalState(tid*AVX2FACTOR+i, scurrent[i]);
	}

	void Spec_DFA_Unroll_Pthread::parallel_run(int tid, Predictor* p)
	{
		const Table* table = p->getUsedTable();
		const Input* input = p->getUsedInput();

		int* tableList_ = table->getTable();
		int nsymbol_ = table->getNumSymbol();
		
		int* Inputs_ = input->getPointer();
		long length_ = input->getLength();

		long i,gap;
		int* scurrent;
		int* avxsymbol;
		
		scurrent = new int [UNROLLFACTOR];
		avxsymbol = new int [UNROLLFACTOR];
	
		for(i=0; i<UNROLLFACTOR; i++)
			scurrent[i] = p->getPredictState(tid * UNROLLFACTOR + i);
		
		long ChunkLength_ = length_ / mthreads;
		long bound = ChunkLength_ / UNROLLFACTOR;

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

		for(i=0; i<UNROLLFACTOR; i++)
			p->setFinalState(tid*UNROLLFACTOR+i, scurrent[i]);	
	}

	void Spec_DFA_GatherUnroll_Pthread::parallel_run(int tid, Predictor* p)
	{
		const Table* table = p->getUsedTable();
		const Input* input = p->getUsedInput();

		int* tableList_ = table->getTable();
		int nsymbol_ = table->getNumSymbol();
		
		int* Inputs_ = input->getPointer();
		long length_ = input->getLength();

		int i, j;
		__m256i scurrent_v[UNROLLFACTORAVX];
		__m256i symbol_v[UNROLLFACTORAVX];
    	__m256i addr_v[UNROLLFACTORAVX];
		__m256i avxmask_v;

		int* scurrent __attribute__ ((aligned (32)));
		scurrent= new int [AVX2FACTOR];

		// set up the predicted starting states in avx2 format
		avxmask_v = _mm256_set1_epi32(0XFFFFFFFF);

		for (i = 0; i < UNROLLFACTORAVX; i++)
		{
			for(j=0; j<AVX2FACTOR; j++)
				scurrent[j] = p->getPredictState(j*UNROLLFACTORAVX + i + tid * AVX2FACTOR * UNROLLFACTORAVX);
			scurrent_v[i] = _mm256_maskload_epi32 ((int*)scurrent, avxmask_v); 			
		}
		avxmask_v  = _mm256_set1_epi32(0X0FFFFFFF);

		long bound = length_ / mchunks;

		long cal[UNROLLFACTORAVX][AVX2FACTOR];
		for(i=0; i<UNROLLFACTORAVX; i++)
			for(j=0; j<AVX2FACTOR; j++)
				cal[i][j]= i*bound + j*bound*UNROLLFACTORAVX + tid * AVX2FACTOR * UNROLLFACTORAVX * bound;

		for(i = 0; i < bound; i++)
		{
			// get the address of next state in the table 
			// 1, set the input symbols as avx2 format; 2, calculate the table address  
			for (j=0; j<UNROLLFACTORAVX; j++)
			{
		        symbol_v[j] = _mm256_set_epi32(Inputs_[i+cal[j][7]],Inputs_[i+cal[j][6]],Inputs_[i+cal[j][5]],Inputs_[i+cal[j][4]],
		        	Inputs_[i+cal[j][3]],Inputs_[i+cal[j][2]],Inputs_[i+cal[j][1]],Inputs_[i+cal[j][0]]);
		        addr_v[j] = _mm256_add_epi32(_mm256_mullo_epi32(scurrent_v[j], _mm256_set1_epi32(nsymbol_)), symbol_v[j]);

		        // access the table to update the states
				scurrent_v[j] = _mm256_i32gather_epi32 (tableList_, addr_v[j], 4);
				scurrent_v[j] = _mm256_and_si256(scurrent_v[j], avxmask_v);
			}
	    }

	    for(i=0; i<UNROLLFACTORAVX; i++)
	    {
	        _mm256_store_si256((__m256i*)scurrent, scurrent_v[i]);
	        for(j = 0; j < AVX2FACTOR; j++)
	        	p->setFinalState(tid*UNROLLFACTORAVX*AVX2FACTOR + j*UNROLLFACTORAVX+i , scurrent[j]);	
	    }	    			
	}

	void Spec_DFA_UnrollGather_Pthread::parallel_run(int tid, Predictor* p)
	{
		const Table* table = p->getUsedTable();
		const Input* input = p->getUsedInput();

		int* tableList_ = table->getTable();
		int nsymbol_ = table->getNumSymbol();
		
		int* Inputs_ = input->getPointer();
		long length_ = input->getLength();

		int i, j;
		__m256i scurrent_v[UNROLLFACTORAVX];
		__m256i symbol_v[UNROLLFACTORAVX];
    	__m256i addr_v[UNROLLFACTORAVX];
		__m256i avxmask_v;

		int* scurrent __attribute__ ((aligned (32)));
		scurrent= new int [AVX2FACTOR];

		// set up the predicted starting states in avx2 format
		avxmask_v = _mm256_set1_epi32(0XFFFFFFFF);

		for (i = 0; i < UNROLLFACTORAVX; i++)
		{
			for(j=0; j<AVX2FACTOR; j++)
				scurrent[j] = p->getPredictState(i*AVX2FACTOR + j + tid * AVX2FACTOR * UNROLLFACTORAVX);
			scurrent_v[i] = _mm256_maskload_epi32 ((int*)scurrent, avxmask_v); 			
		}
		avxmask_v  = _mm256_set1_epi32(0X0FFFFFFF);

		long bound = length_ / mchunks;

		long cal[UNROLLFACTORAVX][AVX2FACTOR];
		for(i=0; i<UNROLLFACTORAVX; i++)
			for(j=0; j<AVX2FACTOR; j++)
				cal[i][j]= i*AVX2FACTOR*bound + j*bound+ tid * AVX2FACTOR * UNROLLFACTORAVX * bound;

		for(i = 0; i < bound; i++)
		{
			// get the address of next state in the table 
			// 1, set the input symbols as avx2 format; 2, calculate the table address  
			for (j=0; j<UNROLLFACTORAVX; j++)
			{
		        symbol_v[j] = _mm256_set_epi32(Inputs_[i+cal[j][7]],Inputs_[i+cal[j][6]],Inputs_[i+cal[j][5]],Inputs_[i+cal[j][4]],
		        	Inputs_[i+cal[j][3]],Inputs_[i+cal[j][2]],Inputs_[i+cal[j][1]],Inputs_[i+cal[j][0]]);
		        addr_v[j] = _mm256_add_epi32(_mm256_mullo_epi32(scurrent_v[j], _mm256_set1_epi32(nsymbol_)), symbol_v[j]);

		        // access the table to update the states
				scurrent_v[j] = _mm256_i32gather_epi32 (tableList_, addr_v[j], 4);
				scurrent_v[j] = _mm256_and_si256(scurrent_v[j], avxmask_v);
			}
	    }

	    for(i=0; i<UNROLLFACTORAVX; i++)
	    {
	        _mm256_store_si256((__m256i*)scurrent, scurrent_v[i]);
	        for(j = 0; j < AVX2FACTOR; j++)
	        	p->setFinalState(tid*UNROLLFACTORAVX*AVX2FACTOR + i*AVX2FACTOR+j , scurrent[j]);	
	    }	    			
	}

}
