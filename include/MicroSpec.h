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

namespace microspec 
{
	// unroll factor in SpecUnroll
	#define MICROSPEC_UNROLLFACTOR 8 

	// unroll factor in SpecUnroll+ and SpecGather+
	#define MICROSPEC_SIMDUNROLLFACTOR 2

	#define MICROSPEC_SIMDFACTOR 8

	class DFA
	{
	public:
		DFA() {}
		virtual ~DFA() {}
		virtual void run(const Table* table, const Input* input) = 0;
	};

	class Seq_DFA:public DFA
	{
	public:
		Seq_DFA() {}
		virtual ~Seq_DFA() {}
		virtual void run(const Table* table, const Input* input);
	};

	class Spec_DFA:public DFA
	{
	public:
		// @Brief In speculative DFA parallelization, a light-weight predictor
		// is used, and the look back length is the only parameter to control it 
		Spec_DFA():mLookBack(0) {}
		Spec_DFA(int lookBackLength):mLookBack(lookBackLength) {}
		virtual ~Spec_DFA() {}
	
	protected:

		// @Brief Provide reexecution. By comparing @predictStates and @endingStates,  
		// which both have size of @mChunks, If find dis-match, start re-execution. 
		virtual void re_execute(const Table* table, const Input* input, 
		 	int* predictStates, int* endingStates, int nChunks);		
		int mLookBack;
	};

	class Spec_DFA_Gather_Single:public Spec_DFA
	{
	public:
		Spec_DFA_Gather_Single():Spec_DFA(){}
		Spec_DFA_Gather_Single(int lookBackLength):Spec_DFA(lookBackLength){}
		virtual ~Spec_DFA_Gather_Single() {}

		virtual void run(const Table* table, const Input* input);	
	};

	class Spec_DFA_Unroll_Single:public Spec_DFA
	{
	public:
		Spec_DFA_Unroll_Single():Spec_DFA(){}
		Spec_DFA_Unroll_Single(int lookBackLength):Spec_DFA(lookBackLength){}
		virtual ~Spec_DFA_Unroll_Single() {}

		virtual void run(const Table* table, const Input* input);		
	};

	class Spec_DFA_GatherUnroll_Single:public Spec_DFA
	{
	public:
		Spec_DFA_GatherUnroll_Single():Spec_DFA(){}
		Spec_DFA_GatherUnroll_Single(int lookBackLength):Spec_DFA(lookBackLength){}
		virtual ~Spec_DFA_GatherUnroll_Single() {}

		virtual void run(const Table* table, const Input* input);	
	};

	class Spec_DFA_UnrollGather_Single:public Spec_DFA
	{
	public:
		Spec_DFA_UnrollGather_Single():Spec_DFA(){}
		Spec_DFA_UnrollGather_Single(int lookBackLength):Spec_DFA(lookBackLength){}
		virtual ~Spec_DFA_UnrollGather_Single() {}

		virtual void run(const Table* table, const Input* input);		
	};

}	// end of namespace microspec

#endif // MICROSPEC_H