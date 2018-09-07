/*
	* This test is used to check whether all the DFA classes can perform 
	* the run function normally. 
	* 
*/
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "Resources.h"
#include "MicroSpec.h"
#include "MicroSpecPthread.h"
#include "Timer.h"

using namespace std;
using namespace microspec;

int main(int argc, char* argv[])
{
	char* InputFile = argv[1];
	char* TableFile = argv[2];
	char* AcceptFile = argv[3];
	int start = atoi(argv[4]);
	MappingRule* rules_;

	rules_ = MappingRule::defineMappingRule();

	cout << endl << "No User Actions!" << endl << endl;
	Timer T1;
	startTime(&T1);
	Input* inputs_ = Input::readFromFile(InputFile, rules_);
	Table* table_ = Table::readFromFile(TableFile, AcceptFile, start, rules_);
	stopTime(&T1);
	double datatime =  elapsedTime(T1);	
	cout << "Data construct time : "<< datatime << " s"<< endl;

	// startTime(&T1);
	// DFA* obj_unroll_single = new Spec_DFA_Pthread(100, 4);
	// obj_unroll_single->run(table_, inputs_);
	// stopTime(&T1);
	// double SpecTime = elapsedTime(T1);

	startTime(&T1);
	DFA* obj_seq = new Seq_DFA();
	obj_seq->run(table_, inputs_);
	stopTime(&T1);
	double SeqTime =  elapsedTime(T1);

	// cout << "SeqDFA Execution "<< SeqTime << " s " << endl;
	// cout << "SpecDFA Execution "<< SpecTime << " s " << endl;
	// cout << "SpecDFA Speedup " <<  SeqTime / SpecTime << endl;	

	return 0;
}
	