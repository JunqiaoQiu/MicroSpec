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
		DFA();
		~DFA();
		virtual void run(const Table* table, const Input* input);

	};

	class Seq_DFA:public DFA
	{
	public:
		Seq_DFA();
		~Seq_DFA();
		void run(const Table* table, const Input* input);
	};

}	// end of namespace microspec

#endif // MICROSPEC_H