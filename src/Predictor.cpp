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

#include "Resources.h"
#include "Predictor.h"

using namespace std;

namespace microspec
{
	Predictor::Predictor()
	{
		mThreads = 0;
		mChunks = 0;
		mLookBack = 0;
		mTransTable = NULL;
		mInputs = NULL;
		mPredictStates = NULL;
		mEndingStates = NULL;
	}

	Predictor::Predictor(const Table* tb, const Input* ints, int nT, int nC)
	{
		srand (time(NULL));
		mThreads = nT;
		mChunks = nC;
		mLookBack = DefaultLookBackLength;

		mTransTable = tb;
		mInputs = ints;

		mPredictStates = new int [mChunks];
		mEndingStates = new int [mChunks];	
	}	

	Predictor::Predictor(const Table* tb, const Input* ints, int nT, int nC, long nLB)
	{
		srand (time(NULL));
		mThreads = nT;
		mChunks = nC;
		mLookBack = nLB;

		mTransTable = tb;
		mInputs = ints;

		mPredictStates = new int [mChunks];
		mEndingStates = new int [mChunks];	
	}

	Predictor::~Predictor()
	{
		delete []mPredictStates;
		delete []mEndingStates;
	}

	Predictor* Predictor::constructPredictor(const Table* table, const Input* input, 
			const int nthread, const int nchunk)
	{
		Predictor* obj = new Predictor(table, input, nthread, nchunk);
		return obj;
	}

	Predictor* Predictor::constructPredictor(const Table* table, const Input* input, 
			const int nthread, const int nchunk, const long nlookback)
	{
		Predictor* obj = new Predictor(table, input, nthread, nchunk, nlookback);
		return obj;
	}

	void Predictor::sequentialPrediction()
	{
		long chunkLength = mInputs->getLength() / mChunks;

		mPredictStates[0]= mTransTable->getStartState();
		for(int t=1; t < mChunks; t++)
		{
			long lookBackStartIndex = chunkLength * t - mLookBack;
			mPredictStates[t]= seqPredict(lookBackStartIndex);
		}
	}

	int Predictor::seqPredict(long lookBackStartIndex)
	{
		int numState = mTransTable->getNumState();
		int numSymbol = mTransTable->getNumSymbol();
		int* transTable = mTransTable->getTable();
		int* inputs = mInputs->getPointer();

		long i;
		int counter;

		int* predict_states;
		int* time_counter;
		predict_states = new int [numState];
		time_counter = new int [numState];
		
		for (i=0; i < numState; i++)
		{
			predict_states[i]=i;
			time_counter[i]=0;
		}

		for(i=0; i < mLookBack; i++)
		{
			int symbol = inputs[lookBackStartIndex + i];
			for(counter=0; counter < numState; counter++)
				predict_states[counter] = ( transTable[predict_states[counter] * numSymbol + symbol ] & 0X0FFFFFFF );
		}

		int ite=0;
		int maxt=0;
		for(counter=0; counter < numState; counter++)
				time_counter[predict_states[counter]]++;

		for(counter=0; counter < numState; counter++)
		{	
			if(time_counter[counter] > maxt)
			{
				maxt = time_counter[counter];
				ite = counter;
			}
		}
		if(maxt==1)
			ite = rand() % numState;

		delete []predict_states;
		delete []time_counter;

		return ite;
	}


	void Predictor::parallelPrediction()
	{
		pthread_t* threads;			
		threads=(pthread_t*)malloc(sizeof(pthread_t)* mChunks);
		cpu_set_t* cpu;		// thread binding variables
		int MAXCPU = (mChunks > mThreads ? mThreads:mChunks);
		cpu=(cpu_set_t*)malloc(sizeof(cpu_set_t) * MAXCPU);
		//PTHREAD-------------------------------

		long t;
		// thread binding
		for(t=0; t < MAXCPU; t++)
		{
			CPU_ZERO(&cpu[t]);
			CPU_SET(t, &cpu[t]);
		}	

		int errorCode1, errorCode2;
		for(t=0; t < mChunks; t++)
		{
			PredictorPointerAndThread* var;
			var = new PredictorPointerAndThread();
			var->pointer = this;
			var->threadID = t;
			errorCode1 = pthread_create(&threads[t], NULL, callFunc_parPredict, (void*)var);
			if (errorCode1)
			{
				printf("ERROR; return code from pthread_create() is %d\n", errorCode1);
				exit(-1);
			}
			errorCode2 = pthread_setaffinity_np(threads[t], sizeof(cpu_set_t), &cpu[t%MAXCPU]);
		}
		for(t=0; t< mChunks; t++)
	    	pthread_join(threads[t], NULL);
	}

	void* Predictor::callFunc_parPredict(void* args)
	{
		PredictorPointerAndThread* Arg = (PredictorPointerAndThread*)args;
		Arg->pointer->parPredict(Arg->threadID);
		pthread_exit((void*)args);
	}

	void Predictor::parPredict(int threadID)
	{
		long tid =  threadID;
		long chunkLength = mInputs->getLength() / mChunks;

		if(tid==0)	
			mPredictStates[0]= mTransTable->getStartState();
		else
		{
			long lookBackStartIndex = chunkLength * tid - mLookBack;
			mPredictStates[tid]=this->seqPredict(lookBackStartIndex);
		}
	}

	int Predictor::getPredictState(int i) const
	{
		return mPredictStates[i];
	}
	int Predictor::getEndingState(int i) const
	{
		return mEndingStates[i];
	}

	void Predictor::setPredictState(int i, int state)
	{
		mPredictStates[i] = state;
	}

	void Predictor::setEndingState(int i, int state)
	{
		mEndingStates[i] = state;
	}

	int* Predictor::getPredictStatePointer() const
	{
		return mPredictStates;
	}

	int* Predictor::getEndingStatePointer() const
	{
		return mEndingStates;
	}
	
	const Table* Predictor::getTableUsed() const
	{
		return mTransTable;
	}

	const Input* Predictor::getInputUsed() const
	{
		return mInputs;
	}

	const int Predictor::getMaxThreads() const
	{
		return mThreads;
	}
	
	const int Predictor::getMaxChunks() const
	{
		return mChunks;
	}

	const long Predictor::getLookBackLength() const
	{
		return mLookBack;
	}
}