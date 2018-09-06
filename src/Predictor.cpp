#include <iostream>
#include <fstream>
#include <stdint.h>
#include <stdlib.h>
#include <time.h> 
#include <stdio.h>
#include <pthread.h>
#include <algorithm>
#include <vector>
#include <sys/sysinfo.h>

#include <smmintrin.h> // sse4.2
#include <immintrin.h>   // avx

#include "Resources.h"
#include "Predictor.h"

using namespace std;

namespace microspec
{
	Predictor::Predictor()
	{
		mThreads = 0;
		mChunks = 0;
		mLook_Back = 0;
		mTable_ = NULL;
		mInputs_ = NULL;
		pthread_predict = NULL;
		pthread_final = NULL;
	}

	Predictor::Predictor(const Table* tb, const Input* ints, int nT, int nC, long nLB)
	{
		srand (time(NULL));
		
		mThreads = nT;
		mChunks = nC;
		mLook_Back = nLB;

		mTable_ = tb;
		mInputs_ = ints;

		pthread_predict = new int [mChunks];
		pthread_final = new int [mChunks];	
	}

	Predictor::~Predictor()
	{
		delete []pthread_predict;
		delete []pthread_final;
	}

	Predictor* Predictor::constructPredictor(const Table* table, const Input* input, 
			const int nthread, const int nchunk, const long nlookback)
	{
		Predictor* obj = new Predictor(table, input, nthread, nchunk, nlookback);
		return obj;
	}

	void Predictor::SequentialPrediction()
	{
		long ChunkLength = mInputs_->getLength() / mChunks;

		pthread_predict[0]= mTable_->getStartState();
		for(int t=1; t < mChunks; t++)
		{
			long START_POINT = ChunkLength * t - mLook_Back;
			pthread_predict[t]= SeqPredict_one(START_POINT);
		}
	}

	int Predictor::SeqPredict_one(long START_POINT)
	{
		int mNumState = mTable_->getNumState();
		int mNumSymbol = mTable_->getNumSymbol();
		int* mInput = mInputs_->getPointer();
		int* mTable = mTable_->getTable();

		long i;
		int counter;
		int ite=0;
		int maxt=0;

		int* predict_states;
		int* time_counter;
		predict_states = new int [mNumState];
		time_counter = new int [mNumState];
		
		for (i=0; i < mNumState; i++)
		{
			predict_states[i]=i;
			time_counter[i]=0;
		}

		for(i=0; i < mLook_Back; i++)
		{
			int symbol = mInput[START_POINT+i];
			for(counter=0; counter < mNumState; counter++)
				predict_states[counter] = ( mTable[predict_states[counter] * mNumSymbol + symbol ] & 0X0FFFFFFF );
		}

		for(counter=0; counter < mNumState; counter++)
				time_counter[predict_states[counter]]++;

		for(counter=0; counter < mNumState; counter++)
		{	
			if(time_counter[counter] > maxt)
			{
				maxt = time_counter[counter];
				ite = counter;
			}
		}
		if(maxt==1)
			ite = rand() % mNumState;

		delete []predict_states;
		delete []time_counter;

		return ite;
	}


	void Predictor::ParallelPrediction()
	{
		//PTHREAD--------------------------------
		int errorCheck1, errorCheck2;
		long t;

		pthread_t* threads;			
		threads=(pthread_t*)malloc(sizeof(pthread_t)* mChunks);
		cpu_set_t* cpu;		// thread binding variables
		int MAXCPU = (mChunks > mThreads ? mThreads:mChunks);
		cpu=(cpu_set_t*)malloc(sizeof(cpu_set_t) * MAXCPU);
		//PTHREAD-------------------------------

		// thread binding
		for(t=0; t < MAXCPU; t++)
		{
			CPU_ZERO(&cpu[t]);
			CPU_SET(t, &cpu[t]);
		}	

		for(t=0; t < mChunks; t++)
		{
			PassItem* var;
			var = new PassItem();
			var->pointer = this;
			var->tid = t;
			errorCheck1 = pthread_create(&threads[t], NULL, Caller1, (void*)var);
			if (errorCheck1)
			{
				printf("ERROR; return code from pthread_create() is %d\n", errorCheck1);
				exit(-1);
			}
			errorCheck2 = pthread_setaffinity_np(threads[t], sizeof(cpu_set_t), &cpu[t%MAXCPU]);
		}
		for(t=0; t< mChunks; t++)
	    	pthread_join(threads[t], NULL);
	}

	void* Predictor::Caller1(void* args)
	{
		PassItem* Arg = (PassItem*)args;
		Arg->pointer->ParPredict_one(Arg->tid);
		pthread_exit((void*)args);
	}

	void Predictor::ParPredict_one(int ID)
	{
		long tid =  ID;
		long ChunkLength = mInputs_->getLength() / mChunks;

		if(tid==0)	
			pthread_predict[0]= mTable_->getStartState();
		else
		{
			long START_POINT = ChunkLength * tid - mLook_Back;
			pthread_predict[tid]=this->SeqPredict_one(START_POINT);
		}
	}

	int Predictor::getPredictState(int i) const
	{
		return pthread_predict[i];
	}
	int Predictor::getFinalState(int i) const
	{
		return pthread_final[i];
	}

	void Predictor::setPredictState(int i, int state)
	{
		pthread_predict[i] = state;
	}

	void Predictor::setFinalState(int i, int state)
	{
		pthread_final[i] = state;
	}

	int* Predictor::getPredictStatePointer() const
	{
		return pthread_predict;
	}

	int* Predictor::getFinalStatePointer() const
	{
		return pthread_final;
	}
	
	const Table* Predictor::getUsedTable() const
	{
		return mTable_;
	}

	const Input* Predictor::getUsedInput() const
	{
		return mInputs_;
	}

}