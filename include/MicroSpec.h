/*
* @Author: Junqiao Qiu
* @Last Modified by:   Junqiao Qiu
*/

#ifndef MICROSPEC_H
#define MICROSPEC_H

#include <stdlib.h>
#include <stdio.h>

#include "Resources.h"
#include "Predictor.h"
#include "Action.h"

namespace microspec 
{
	// unroll factor in SpecUnroll
	#define MICROSPEC_UNROLLFACTOR 8 
	// unroll factor in SpecUnroll+ and SpecGather+
	#define MICROSPEC_SIMDUNROLLFACTOR 2
	// SIMD factor
	#define MICROSPEC_SIMDFACTOR 8

	class DFA
	{
	public:
		DFA();
		virtual ~DFA();
		virtual void run(const Table* table, const Input* input) = 0;

		virtual void setAction(char* actionType);
		virtual void printResults();

	protected:
		DFAResults* mEndingResults;
		// @mAction is what will execute in each step during DFA execution
		action mAction;
	};

	class SeqDFA:public DFA
	{
	public:
		SeqDFA():DFA(){}
		virtual ~SeqDFA() {}
		virtual void run(const Table* table, const Input* input);
	};

	class SpecDFA:public DFA
	{
	public:
		// @Brief In speculative DFA parallelization, a light-weight predictor
		// is used, and the look back length is the only parameter to control it 
		SpecDFA();
		SpecDFA(int lookBackLength);
		virtual ~SpecDFA() {}

		virtual void setAction(char* actionType);

	protected:
		// @Brief Provide reexecution. By comparing @predictStates and @endingStates,  
		// which both have size of @mChunks, If find dis-match, start re-execution. 
		virtual void re_execute(const Table* table, const Input* input, 
		 	int* predictStates, int* endingStates, int nChunks);

	protected:
		DFAResults* mEndingResultsPerChunk;
		actionReExecute mReExecuteAction;
		int mLookBack;
	};

	class SpecDFA_Unroll_Single:public SpecDFA
	{
	public:
		SpecDFA_Unroll_Single():SpecDFA(){}
		SpecDFA_Unroll_Single(int lookBackLength):SpecDFA(lookBackLength){}
		virtual ~SpecDFA_Unroll_Single() {}

		virtual void run(const Table* table, const Input* input);		
	};	

#ifdef AVX2_SUPPORT
	class SpecDFA_Gather_Single:public SpecDFA
	{
	public:
		SpecDFA_Gather_Single();
		SpecDFA_Gather_Single(int lookBackLength);
		virtual ~SpecDFA_Gather_Single() {}

		virtual void setAction(char* reExecuteActionType);
		virtual void run(const Table* table, const Input* input);	
		
	protected:
		actionSIMD mActionSIMD;
	};

	class SpecDFA_GatherUnroll_Single:public SpecDFA
	{
	public:
		SpecDFA_GatherUnroll_Single();
		SpecDFA_GatherUnroll_Single(int lookBackLength);
		virtual ~SpecDFA_GatherUnroll_Single() {}

		virtual void setAction(char* reExecuteActionType);
		virtual void run(const Table* table, const Input* input);	
		
	protected:
		actionSIMD mActionSIMD;	
	};

	class SpecDFA_UnrollGather_Single:public SpecDFA
	{
	public:
		SpecDFA_UnrollGather_Single();
		SpecDFA_UnrollGather_Single(int lookBackLength);
		virtual ~SpecDFA_UnrollGather_Single() {}

		virtual void setAction(char* reExecuteActionType);
		virtual void run(const Table* table, const Input* input);	
		
	protected:
		actionSIMD mActionSIMD;	
	};
#endif

}	// end of namespace microspec

#endif // MICROSPEC_H