/*
* @Author: Junqiao Qiu
* @Last Modified by:   Junqiao Qiu
*/

#ifndef ACTION_H
#define ACTION_H

#include <stdlib.h>
#include <stdio.h>

namespace microspec
{

	// The struct which stores the target results of DFA execution
	struct DFAResults
	{
		int mResutls;
	};

	//	The User defined Actions during DFA transition
	typedef void(*action)(int, DFAResults*);

	// void setAction(func f_);
	// func getAction() const;	

	// Do noghitng action, which is the default one
	void doNothing(int state, DFAResults* dfaResultsPointer);
	void doNothingReExecute(int correctState, int wrongState, 
		DFAResults* dfaResultsPointer);

	// Accumulate action, which adds 1 to results when meeting accept
	void accumulateAction(int state, DFAResults* dfaResultsPointer);
	void accumulateActionReExecute(int correctState, int wrongState, 
		DFAResults* dfaResultsPointer);

}

#endif // ACTION_H