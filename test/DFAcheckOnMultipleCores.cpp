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

	cout << "Table Information "<< table_->getNumState() << " " << table_->getNumSymbol() << endl << endl;

	char* acction = "accumulate";

	startTime(&T1);
	DFA* obj_seq = new SeqDFA();
	obj_seq->run(table_, inputs_);
	stopTime(&T1);
	double SeqTime =  elapsedTime(T1);

	startTime(&T1);
	DFA* obj_specPthread = new SpecDFA_Pthread(4);
	obj_specPthread->run(table_, inputs_);
	stopTime(&T1);
	double SpecPthreadTime =  elapsedTime(T1);	

	startTime(&T1);
	DFA* obj_specUnrollPthread = new SpecDFA_Unroll_Pthread(4);
	obj_specUnrollPthread->run(table_, inputs_);
	stopTime(&T1);
	double SpecUnrollPthreadTime = elapsedTime(T1);

	startTime(&T1);
	DFA* obj_specGatherPthread = new SpecDFA_Gather_Pthread(4);
	obj_specGatherPthread->run(table_, inputs_);
	stopTime(&T1);
	double SpecGatherPthreadTime = elapsedTime(T1);

	startTime(&T1);
	DFA* obj_specGatherUnrollPthread = new SpecDFA_GatherUnroll_Pthread(4);
	obj_specGatherUnrollPthread->run(table_, inputs_);
	stopTime(&T1);
	double SpecGatherUnrllPthreadTime = elapsedTime(T1);

	startTime(&T1);
	DFA* obj_specUnrollGatherPthread = new SpecDFA_UnrollGather_Pthread(4);
	obj_specUnrollGatherPthread->run(table_, inputs_);
	stopTime(&T1);
	double SpecUnrollGatherPthreadTime = elapsedTime(T1);			

	cout << "SeqDFA Execution "<< SeqTime << " s " << endl;
	cout << "SpecDFA Pthread Execution "<< SpecPthreadTime << " s " << endl;

	cout << endl;	
	cout << "SpecDFA Pthread Speedup " <<  SeqTime / SpecPthreadTime << endl;		
	cout << "SpecDFA Unroll Pthread Speedup " <<  SeqTime / SpecUnrollPthreadTime << endl;	
	cout << "SpecDFA Gather Pthread Speedup " <<  SeqTime / SpecGatherPthreadTime << endl;	
	cout << "SpecDFA GatherUnroll Pthread Speedup " <<  SeqTime / SpecGatherUnrllPthreadTime << endl;	
	cout << "SpecDFA UnrollGahter Pthread Speedup " <<  SeqTime / SpecUnrollGatherPthreadTime << endl;	



	return 0;
}
	