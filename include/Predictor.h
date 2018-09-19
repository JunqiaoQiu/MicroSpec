/*
* @Predictor.h
* @Author: Junqiao Qiu
* @Last Modified by:   Junqiao Qiu
*/

#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <stdio.h>
#include <stdlib.h>

#include "Resources.h"

namespace microspec 
{
	const int DefaultLookBackLength = 100;

	class Predictor;

	// @Brief The structure stores the pointer of a predictor object 
	// and ID of the thread which uses this object 
	struct PredictorPointerAndThread
	{
		Predictor* pointer;
		int threadID;
	};

	class Predictor
	{
	public:
		Predictor();		
		~Predictor();

		static Predictor* constructPredictor(const Table* transTable, const Input* inputStream, 
			const int numThreads, const int numChunks);	
		static Predictor* constructPredictor(const Table* transTable, const Input* inputStream, 
			const int numThreads, const int numChunks, const long lookBackLength);	

	protected:
		Predictor(const Table* transTable, const Input* inputStream, 
			int numThreads, int numChunks);		
		Predictor(const Table* transTable, const Input* inputStream, 
			int numThreads, int numChunks, long lookBackLength);		
	public:
		
		// @Brief function @sequentialPrediction provides single core prediction, while 
		// @parallelPrediction provides parallel prediction based on mThreads. 
		// Will construct array @mPredictStates and @mEndingStates.
		void sequentialPrediction();
		void parallelPrediction();

		int getPredictState(int index) const;
		int getEndingState(int index) const;
		void setPredictState(int index, int state);
		void setEndingState(int index, int state);

		const Table* getTableUsed() const;
		const Input* getInputUsed() const;

		int* getPredictStatePointer() const;
		int* getEndingStatePointer() const;

		// @Brief Provide the look back start location @predictStartIndex 
		// in the input stream, and then return the predicted state
		int seqPredict(long predictStartIndex);

		const int getMaxThreads() const;
		const int getMaxChunks() const;
		const long getLookBackLength() const;

	private:	
		// @ Brief The two private functions are used for ParallelPrediction
		static void* callFunc_parPredict(void* args_predictorPointerAndThread);
	 	void parPredict(int threadID);

	private:
		// Predictor Settings
		int mThreads;
		int mChunks;
		long mLookBack;

		const Table* mTransTable;
		const Input* mInputs;

		// Prediction Results
		int* mPredictStates __attribute__ ((aligned (32)));
		int* mEndingStates 	 __attribute__ ((aligned (32)));
	};

}	// end of namespace microspec

#endif // PREDICTOR_H
