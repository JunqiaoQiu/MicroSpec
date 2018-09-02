#include <iostream>

#include "Resources.hpp"
#include "MicroSpec.hpp"

using namespace std;

namespace microspec
{
	void Seq_DFA::run(const Table* table, const Input* input)
	{
		/*
		int* tableList_ = table->getTable();
		int state_ = table->getStartState();
		int nstate_ = table->getNumState();
		int nsymbol_ = table->getNumSymbol();
		
		int* Inputs_ = input->getPointer();
		int length_ = input->getLength();

		for (long i = 0; i < length_; i++)
		{
			int temp = tableList_[state_ * nsymbol_ + Inputs_[i]];
			state_ = temp & 0X0FFFFFFF;
		}

		printf("This is just a Test! And final state is  %d\n", state_);
		*/
		printf("This is just a Test ! \n");
	}
}
