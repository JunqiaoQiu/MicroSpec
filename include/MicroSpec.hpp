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

namespace microspec 
{
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

}	// end of namespace microspec

#endif // MICROSPEC_H