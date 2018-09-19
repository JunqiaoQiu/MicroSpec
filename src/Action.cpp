#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <smmintrin.h> // sse4.2
#include <immintrin.h>   // avx

#include "Action.h"

using namespace std;

namespace microspec
{
	void doNothing(int state, DFAResults* dfaResultsPointer)
	{
		;
	}

	void doNothingReExecute(int correctState, int wrongState, DFAResults* dfaResultsPointer)
	{
		;
	}

	void doNothingSIMD(__m256i stateVector, __m256i* dfaResultsPointer)
	{
		;
	}

	void accumulateAction(int state, DFAResults* dfaResultsPointer)
	{
		if ( (state & 0XF0000000) == 0XF0000000 )
			dfaResultsPointer->mResults++;
	}

	void accumulateActionReExecute(int correctState, int wrongState, DFAResults* dfaResultsPointer)
	{
		if ( (correctState & 0XF0000000) == 0XF0000000 )
			dfaResultsPointer->mResults++;
		if ( (wrongState & 0XF0000000) == 0XF0000000 )
			dfaResultsPointer->mResults--;
	}
 
	void accumulateActionSIMD(__m256i stateVector, __m256i* dfaResultsPointer)
	{
		__m256i maskV = _mm256_set1_epi32(0XF0000000);
		__m256i maskV2 = _mm256_set1_epi32(0X00000001);
		__m256i addV = _mm256_cmpeq_epi32(_mm256_and_si256(stateVector, maskV), maskV);
		addV = _mm256_and_si256(addV, maskV2);

		dfaResultsPointer[0] = _mm256_add_epi32(dfaResultsPointer[0], addV);
	}

}