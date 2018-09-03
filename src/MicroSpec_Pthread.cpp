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
	void Spec_DFA_Pthread::run(const Table* table, const Input* input)
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

		this->ReExecute(table, input, objPredictor->getPredictStatePointer(), currentfinal, mchunks);

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

	void Spec_DFA_Pthread_Gather::parallel_run(int tid, Predictor* p)
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
		int* current __attribute__ ((aligned (32)));
		current= new int [AVX2FACTOR];
		int* avxsymbol __attribute__ ((aligned (32)));
		avxsymbol=new int [AVX2FACTOR] ;
		int* addr __attribute__ ((aligned (32)));
		addr=new int [AVX2FACTOR];

		avxmask_v = _mm256_set1_epi32(0XFFFFFFFF);
		for(i=0; i<AVX2FACTOR; i++)
			scurrent[i] = p->getPredictState(tid * AVX2FACTOR + i);
		scurrent_v=_mm256_maskload_epi32 ((int*)scurrent, avxmask_v);	
	
		long ChunkLength_ = length_ / mthreads;
		long bound = ChunkLength_ / AVX2FACTOR;

		long cal[8];
		for(i=0; i<8; i++)
			cal[i]=i*bound;

		for(i = tid*ChunkLength_; i < bound + tid*ChunkLength_; i++)
		{
        	avxsymbol_v=_mm256_set_epi32(Inputs_[i+cal[7]], Inputs_[i+cal[6]], Inputs_[i+cal[5]],
        		Inputs_[i+cal[4]], Inputs_[i+cal[3]], Inputs_[i+cal[2]], Inputs_[i+cal[1]], Inputs_[i]);
        	__m256i addr_v = _mm256_add_epi32(_mm256_mullo_epi32(scurrent_v, _mm256_set1_epi32(nsymbol_)), avxsymbol_v);
			scurrent_v = _mm256_i32gather_epi32 (tableList_, addr_v, 4);
    	}    

    	printf("Current Cores is %d \n", sched_getcpu());

    	if (tid == 2)
    		_mm256_store_si256((__m256i*)scurrent, scurrent_v);

    	for(i = 0; i < AVX2FACTOR; i++)
			p->setFinalState(tid*AVX2FACTOR+i, scurrent[i]);
	}
}
