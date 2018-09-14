/*
* @Author: Junqiao Qiu
* @Last Modified by:   Junqiao Qiu
*/

#ifndef ACTION_H
#define ACTION_H

#include <stdlib.h>
#include <stdio.h>

#ifdef AVX2_SUPPORT
#include <smmintrin.h> // sse4.2
#include <immintrin.h>   // avx.
#endif

namespace microspec
{

	// The struct which stores the target results of DFA execution
	struct DFAResults
	{
		int mResults;
	};

	//	The User defined Actions during DFA transition
	typedef void(*action)(int, DFAResults*);
	typedef void(*actionReExecute)(int, int, DFAResults*);
#ifdef AVX2_SUPPORT	
	typedef void(*actionSIMD)(__m256i, __m256i*);
#endif

	// void setAction(func f_);
	// func getAction() const;	

	// Do noghitng action, which is the default one
	void doNothing(int state, DFAResults* dfaResultsPointer);
	void doNothingReExecute(int correctState, int wrongState, 
		DFAResults* dfaResultsPointer);
#ifdef AVX2_SUPPORT
	void doNothingSIMD(__m256i stateVector, __m256i* dfaResultsPointer);
#endif

	// Accumulate action, which adds 1 to results when meeting accept
	void accumulateAction(int state, DFAResults* dfaResultsPointer);
	void accumulateActionReExecute(int correctState, int wrongState, 
		DFAResults* dfaResultsPointer);
#ifdef AVX2_SUPPORT	
	void accumulateActionSIMD(__m256i stateVector, __m256i* dfaResultsPointer);	
#endif

}

#endif // ACTION_H