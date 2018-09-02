#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "MicroSpec.hpp"
#include "Resources.hpp"

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

	DFA* framework = new Seq_DFA();
	framework->run(table_, inputs_);

	return 0;
}
