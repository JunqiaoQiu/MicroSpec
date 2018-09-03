/*
* @Author: Junqiao Qiu
* @Last Modified by:   Junqiao Qiu
* @Last Modified time: 2018-08-31 
*/

#ifndef MICROSPEC_H
#define MICROSPEC_H

#include <stdlib.h>
#include <stdio.h>

#include "Resources.hpp"
#include "Predictor.hpp"

namespace microspec 
{
	#define UNROLLFACTOR 8
	#define AVX2FACTOR 8

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
		Spec_DFA():mLookBack(0) {}
		Spec_DFA(int lb):mLookBack(lb) {}
		virtual ~Spec_DFA() {}
		virtual void ReExecute(const Table* table, const Input* input, 
		 	int* p, int* f, int size);	
	protected:
		int mLookBack;
	};

	class Spec_DFA_Gather_Single:public Spec_DFA
	{
	public:
		Spec_DFA_Gather_Single():Spec_DFA(){}
		Spec_DFA_Gather_Single(int lb):Spec_DFA(lb){}
		virtual ~Spec_DFA_Gather_Single() {}
		virtual void run(const Table* table, const Input* input);	
	};

	class Spec_DFA_Unroll_Single:public Spec_DFA
	{
	public:
		Spec_DFA_Unroll_Single():Spec_DFA(){}
		Spec_DFA_Unroll_Single(int lb):Spec_DFA(lb){}
		virtual ~Spec_DFA_Unroll_Single() {}
		virtual void run(const Table* table, const Input* input);		
	};

}	// end of namespace microspec

#endif // MICROSPEC_H