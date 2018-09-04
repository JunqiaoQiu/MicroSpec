#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "Resources.hpp"
#include "MicroSpec.hpp"
#include "MicroSpec_Pthread.hpp"
#include "Timer.hpp"

using namespace std;
using namespace microspec;

int main(int argc, char* argv[])
{
	char* InputFile = argv[1];
	char* TableFile = argv[2];
	char* AcceptFile = argv[3];
	int start = atoi(argv[4]);
	MappingRule* rules_;

	// if (argc > 5)
	// {
	// 	char* setrule_ = argv[5];
	// 	rules_ = MappingRule::DefineMappingRule(setrule_);
	// }
	// else
		rules_ = MappingRule::DefineMappingRule();

	//rules_->PrintRules();

	Timer T1;
	startTime(&T1);
	Input* inputs_ = Input::ReadFromFile(InputFile, rules_);
	Table* table_ = Table::ReadFromFile(TableFile, AcceptFile, start, rules_);
	stopTime(&T1);
	double datatime =  elapsedTime(T1);	
	cout << datatime << " s"<< endl;

	startTime(&T1);
	DFA* obj_unroll_single = new Spec_DFA_Pthread(100, 4);
	obj_unroll_single->Run(table_, inputs_);
	stopTime(&T1);
	double SpecTime = elapsedTime(T1);

	startTime(&T1);
	DFA* obj_seq = new Seq_DFA();
	obj_seq->Run(table_, inputs_);
	stopTime(&T1);
	double SeqTime =  elapsedTime(T1);

	cout << SeqTime << " " << SpecTime << " " << SeqTime / SpecTime << endl;	

	return 0;
}
	