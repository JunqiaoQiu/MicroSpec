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

	if (argc > 5)
		rules_ = MappingRule::defineMappingRule(argv[5]);
	else
		rules_ = MappingRule::defineMappingRule();

	Timer T1;
	startTime(&T1);
	Input* inputs_ = Input::readFromFile(InputFile, rules_);
	Table* table_ = Table::readFromFile(TableFile, AcceptFile, start, rules_);
	stopTime(&T1);
	double datatime =  elapsedTime(T1);	
	cout << endl << "Data construct time : "<< datatime << " s"<< endl << endl;

	startTime(&T1);
	DFA* obj_seq = new SeqDFA();
	obj_seq->run(table_, inputs_);
	stopTime(&T1);
	double SeqTime =  elapsedTime(T1);

	startTime(&T1);
	DFA* obj_unroll_single = new SpecDFA_Unroll_Single();
	obj_unroll_single->run(table_, inputs_);
	stopTime(&T1);
	double SpecUnrollSingleTime = elapsedTime(T1);

	startTime(&T1);
	DFA* obj_gather_single = new SpecDFA_Gather_Single();
	obj_gather_single->run(table_, inputs_);
	stopTime(&T1);
	double SpecGatherSingleTime = elapsedTime(T1);

	startTime(&T1);
	DFA* obj_gatherunroll_single = new SpecDFA_GatherUnroll_Single();
	obj_gatherunroll_single->run(table_, inputs_);
	stopTime(&T1);
	double SpecGatherUnrollSingleTime = elapsedTime(T1);

	startTime(&T1);
	DFA* obj_unrollgather_single = new SpecDFA_UnrollGather_Single();
	obj_unrollgather_single->run(table_, inputs_);
	stopTime(&T1);
	double SpecUnrollGatherSingleTime = elapsedTime(T1);	

	cout << "SeqDFA Execution "<< SeqTime << " s " << endl;
	cout << "SpecDFA Gather Single Execution "<< SpecGatherSingleTime << " s " << endl;
	cout << "SpecDFA Unroll Single Execution "<< SpecUnrollSingleTime << " s " << endl;
	cout << "SpecDFA GatherUnroll Single Execution "<< SpecGatherUnrollSingleTime << " s " << endl;
	cout << "SpecDFA UnrollGather Single Execution "<< SpecUnrollGatherSingleTime << " s " << endl;

	cout << endl;
	cout << "SpecDFA Gather Single Speedup " <<  SeqTime / SpecGatherSingleTime << endl;	
	cout << "SpecDFA Unroll Single Speedup " <<  SeqTime / SpecUnrollSingleTime << endl;	
	cout << "SpecDFA GatherUnroll Single Speedup " <<  SeqTime / SpecGatherUnrollSingleTime << endl;	
	cout << "SpecDFA UnrollGather Single Speedup " <<  SeqTime / SpecUnrollGatherSingleTime << endl;		

	return 0;
}
	