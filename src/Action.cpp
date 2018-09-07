#include <iostream>
#include <stdlib.h>
#include <stdio.h>
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

// Accumulate action, which adds 1 to results when meeting accept

	void accumulateAction(int state, DFAResults* dfaResultsPointer)
	{
		;
	}

	void accumulateActionReExecute(int correctState, int wrongState, DFAResults* dfaResultsPointer)
	{
		;
	}


}

#endif // ACTION_H