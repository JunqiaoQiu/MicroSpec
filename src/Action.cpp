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


// Accumulate action, which adds 1 to results when meeting accept

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
		;
	}

}