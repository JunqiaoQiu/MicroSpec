/*
* @Author: Junqiao Qiu
* @Last Modified by:   Junqiao Qiu
*/

#ifndef MICROSPEC_PTHREAD_H
#define MICROSPEC_PTHREAD_H

#include <stdlib.h>
#include <stdio.h>

#include "Resources.hpp"
#include "Predictor.hpp"
#include "MicroSpec.hpp"

namespace microspec 
{
	class Spec_DFA_Pthread;
	struct DFAPassPointer
	{
		Spec_DFA_Pthread* pointer;
		int tid;
		Predictor* p_predictor;
	};

	class Spec_DFA_Pthread:public Spec_DFA
	{
	public:
		Spec_DFA_Pthread():Spec_DFA(){}
		Spec_DFA_Pthread(int look_back_length, int threads_used):Spec_DFA(look_back_length), mthreads(threads_used), mchunks(threads_used){}	
		virtual ~Spec_DFA_Pthread() {}

		virtual void Run(const Table* table, const Input* input);

	protected:
		static void* caller(void* args);
		virtual void parallel_run(int tid, Predictor* p);

	protected:
		int mthreads;
		int mchunks;
	};

	class Spec_DFA_Gather_Pthread:public Spec_DFA_Pthread
	{
	public:
		Spec_DFA_Gather_Pthread():Spec_DFA_Pthread(){}
		Spec_DFA_Gather_Pthread(int look_back_length, int threads_used):Spec_DFA_Pthread(look_back_length, threads_used){mchunks = threads_used * AVX2FACTOR;}	
		virtual ~Spec_DFA_Gather_Pthread() {}
		
	protected:
		virtual void parallel_run(int tid, Predictor* p);
	};

	class Spec_DFA_Unroll_Pthread:public Spec_DFA_Pthread
	{
	public:
		Spec_DFA_Unroll_Pthread():Spec_DFA_Pthread(){}
		Spec_DFA_Unroll_Pthread(int look_back_length, int threads_used):Spec_DFA_Pthread(look_back_length, threads_used){mchunks = threads_used * UNROLLFACTOR;}	
		virtual ~Spec_DFA_Unroll_Pthread() {}
		
	protected:
		virtual void parallel_run(int tid, Predictor* p);
	};

	class Spec_DFA_GatherUnroll_Pthread:public Spec_DFA_Pthread
	{
	public:
		Spec_DFA_GatherUnroll_Pthread():Spec_DFA_Pthread(){}
		Spec_DFA_GatherUnroll_Pthread(int look_back_length, int threads_used):Spec_DFA_Pthread(look_back_length, threads_used){mchunks = threads_used * AVX2FACTOR * UNROLLFACTORAVX;}	
		virtual ~Spec_DFA_GatherUnroll_Pthread() {}
		
	protected:
		virtual void parallel_run(int tid, Predictor* p);
	};

	class Spec_DFA_UnrollGather_Pthread:public Spec_DFA_Pthread
	{
	public:
		Spec_DFA_UnrollGather_Pthread():Spec_DFA_Pthread(){}
		Spec_DFA_UnrollGather_Pthread(int look_back_length, int threads_used):Spec_DFA_Pthread(look_back_length, threads_used){mchunks = threads_used * UNROLLFACTORAVX * AVX2FACTOR;}	
		virtual ~Spec_DFA_UnrollGather_Pthread() {}
		
	protected:
		virtual void parallel_run(int tid, Predictor* p);
	};	

}	// end of namespace microspec

#endif // MICROSPEC_PTHREAD_H