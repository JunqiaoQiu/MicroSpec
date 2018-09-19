/*
* @Author: Junqiao Qiu
* @Last Modified by:   Junqiao Qiu
*/

#ifndef MICROSPECPTHREAD_H
#define MICROSPECPTHREAD_H

#include <stdlib.h>
#include <stdio.h>

#include "Resources.h"
#include "Predictor.h"
#include "MicroSpec.h"

namespace microspec 
{
	class SpecDFA_Pthread;
	struct DFAPassPointer
	{
		SpecDFA_Pthread* pointer;
		int tid;
		Predictor* p_predictor;
	};

	class SpecDFA_Pthread:public SpecDFA
	{
	public:
		SpecDFA_Pthread():SpecDFA(){}
		SpecDFA_Pthread(int threads_used):SpecDFA(), mThreads(threads_used), mChunks(threads_used){}
		SpecDFA_Pthread(int look_back_length, int threads_used):SpecDFA(look_back_length), mThreads(threads_used), mChunks(threads_used){}	
		virtual ~SpecDFA_Pthread() {}

		virtual void run(const Table* table, const Input* input);

	protected:
		static void* callFunc_parallelRun(void* args);
		virtual void parallelRun(int tid, Predictor* p);

	protected:
		int mThreads;
		int mChunks;
	};

	class SpecDFA_Unroll_Pthread:public SpecDFA_Pthread
	{
	public:
		SpecDFA_Unroll_Pthread():SpecDFA_Pthread(){}
		SpecDFA_Unroll_Pthread(int threads_used):SpecDFA_Pthread(threads_used){mChunks = threads_used * MICROSPEC_UNROLLFACTOR;}	
		SpecDFA_Unroll_Pthread(int look_back_length, int threads_used):SpecDFA_Pthread(look_back_length, threads_used){mChunks = threads_used * MICROSPEC_UNROLLFACTOR;}	
		virtual ~SpecDFA_Unroll_Pthread() {}
		
	protected:
		virtual void parallelRun(int tid, Predictor* p);
	};	

#ifdef AVX2_SUPPORT
	class SpecDFA_Gather_Pthread:public SpecDFA_Pthread
	{
	public:
		SpecDFA_Gather_Pthread():SpecDFA_Pthread(){}
		SpecDFA_Gather_Pthread(int threads_used):SpecDFA_Pthread(threads_used){mChunks = threads_used * MICROSPEC_SIMDFACTOR;}	
		SpecDFA_Gather_Pthread(int look_back_length, int threads_used):SpecDFA_Pthread(look_back_length, threads_used){mChunks = threads_used * MICROSPEC_SIMDFACTOR;}	
		virtual ~SpecDFA_Gather_Pthread() {}
		
	protected:
		virtual void parallelRun(int tid, Predictor* p);
	};

	class SpecDFA_GatherUnroll_Pthread:public SpecDFA_Pthread
	{
	public:
		SpecDFA_GatherUnroll_Pthread():SpecDFA_Pthread(){}
		SpecDFA_GatherUnroll_Pthread(int threads_used):SpecDFA_Pthread(threads_used){mChunks = threads_used * MICROSPEC_SIMDFACTOR * MICROSPEC_SIMDUNROLLFACTOR;}	
		SpecDFA_GatherUnroll_Pthread(int look_back_length, int threads_used):SpecDFA_Pthread(look_back_length, threads_used){mChunks = threads_used * MICROSPEC_SIMDFACTOR * MICROSPEC_SIMDUNROLLFACTOR;}	
		virtual ~SpecDFA_GatherUnroll_Pthread() {}
		
	protected:
		virtual void parallelRun(int tid, Predictor* p);
	};

	class SpecDFA_UnrollGather_Pthread:public SpecDFA_Pthread
	{
	public:
		SpecDFA_UnrollGather_Pthread():SpecDFA_Pthread(){}
		SpecDFA_UnrollGather_Pthread(int threads_used):SpecDFA_Pthread(threads_used){mChunks = threads_used * MICROSPEC_SIMDUNROLLFACTOR * MICROSPEC_SIMDFACTOR;}	
		SpecDFA_UnrollGather_Pthread(int look_back_length, int threads_used):SpecDFA_Pthread(look_back_length, threads_used){mChunks = threads_used * MICROSPEC_SIMDUNROLLFACTOR * MICROSPEC_SIMDFACTOR;}	
		virtual ~SpecDFA_UnrollGather_Pthread() {}
		
	protected:
		virtual void parallelRun(int tid, Predictor* p);
	};	
#endif
}	// end of namespace microspec

#endif // MICROSPECPTHREAD_H