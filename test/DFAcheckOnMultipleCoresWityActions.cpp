/*
	* This test is used to check whether all the DFA classes can perform 
	* the run function normally with actions during the processing. 
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
	int numThread = atoi(argv[5]);
	MappingRule* rules_;

	if (argc > 6)
		rules_ = MappingRule::defineMappingRule(argv[6]);
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

	
	DFA* obj_seq = new SeqDFA();
	obj_seq->setAction(acction);
	startTime(&T1);
	obj_seq->run(table_, inputs_);
	stopTime(&T1);
	double SeqTime =  elapsedTime(T1);

	DFA* obj_specPthread = new SpecDFA_Pthread(numThread);
	obj_specPthread->setAction(acction);
	startTime(&T1);
	obj_specPthread->run(table_, inputs_);
	stopTime(&T1);
	double SpecPthreadTime =  elapsedTime(T1);	

	cout << "SeqDFA Execution "<< SeqTime << " s " << endl;
	cout << "SpecDFA Pthread Execution "<< SpecPthreadTime << " s " << endl;

	cout << endl;	
	cout << "SpecDFA Pthread Speedup " <<  SeqTime / SpecPthreadTime << endl;		

	return 0;
}
	