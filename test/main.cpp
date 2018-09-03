#include <iostream>
#include <stdlib.h>
#include <stdio.h>

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

	Input* inputs_ = Input::ReadFromFile(InputFile);
	Table* table_ = Table::ReadFromFile(TableFile, AcceptFile, start);

	Timer T1;
	startTime(&T1);
	DFA* obj_seq = new Seq_DFA();
	obj_seq->run(table_, inputs_);
	stopTime(&T1);
	double SeqTime =  elapsedTime(T1);

	startTime(&T1);
	DFA* obj_unroll_single = new Spec_DFA_Pthread_Gather(100, 4);
	obj_unroll_single->run(table_, inputs_);
	stopTime(&T1);
	double UnrollSingleTime = elapsedTime(T1);

	cout << SeqTime / UnrollSingleTime << endl;	

	return 0;
}
