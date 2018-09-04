/*
* @Predictor.hpp
* @Author: Junqiao Qiu
* @Last Modified by:   Junqiao Qiu
*/

#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <stdio.h>
#include <stdlib.h>

#include <smmintrin.h> // sse4.2
#include <immintrin.h>   // avx

#include "Resources.hpp"

namespace microspec 
{
	class Predictor;
	struct PassItem
	{
		Predictor* pointer;
		int tid;
	};

	class Predictor
	{
	public:
		Predictor();
		Predictor(const Table* tb, const Input* ints, int nT, int nC, long nLB);
		~Predictor();

		static Predictor* ConstructPredictor(const Table* table, const Input* input, 
			const int nthread, const int nchunk, const long nlookback);	

		void SequentialPrediction();
		void ParallelPrediction();

		int getPredictState(int i) const;
		int getFinalState(int i) const;
		void setPredictState(int i, int state);
		void setFinalState(int i, int state);

		const Table* getUsedTable() const;
		const Input* getUsedInput() const;

		int* getPredictStatePointer() const;
		int* getFinalStatePointer() const;

	private:
		int SeqPredict_one(long START_POINT);

		// @ Brief The two private functions are used for ParallelPrediction
		static void* Caller1(void* args);
	 	void ParPredict_one(int tid);

	private:
		// Predictor Settings
		int mThreads;
		int mChunks;
		long mLook_Back;

		const Table* mTable_;
		const Input* mInputs_;

		// Prediction Results
		int* pthread_predict __attribute__ ((aligned (32)));
		int* pthread_final 	 __attribute__ ((aligned (32)));
	};

}	// end of namespace microspec

#endif // PREDICTOR_H
