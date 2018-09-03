/*
* @Author: Junqiao Qiu
* @Last Modified by:   Junqiao Qiu
* @Last Modified time: 2018-08-31 
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
		Spec_DFA_Pthread(int lb, int tds):Spec_DFA(lb), mthreads(tds), mchunks(tds){}	
		virtual ~Spec_DFA_Pthread() {}
		virtual void run(const Table* table, const Input* input);

	protected:
		static void* caller(void* args);
		virtual void parallel_run(int tid, Predictor* p);
		int mthreads;
		int mchunks;
	};

	class Spec_DFA_Pthread_Gather:public Spec_DFA_Pthread
	{
	public:
		Spec_DFA_Pthread_Gather():Spec_DFA_Pthread(){}
		Spec_DFA_Pthread_Gather(int lb, int tds):Spec_DFA_Pthread(lb, tds){mchunks = tds * AVX2FACTOR;}	
		virtual ~Spec_DFA_Pthread_Gather() {}
		//virtual void run(const Table* table, const Input* input);
		
	protected:
		virtual void parallel_run(int tid, Predictor* p);
	};

	class Spec_DFA_Pthread_Unroll:public Spec_DFA_Pthread
	{
	public:
		Spec_DFA_Pthread_Unroll():Spec_DFA_Pthread(){}
		Spec_DFA_Pthread_Unroll(int lb, int tds, int cks):Spec_DFA_Pthread(lb, tds){mchunks = tds * UNROLLFACTOR;}	
		virtual ~Spec_DFA_Pthread_Unroll() {}
		//virtual void run(const Table* table, const Input* input);
		
	protected:
		virtual void parallel_run(int tid, Predictor* p);
	};

}	// end of namespace microspec

#endif // MICROSPEC_PTHREAD_H