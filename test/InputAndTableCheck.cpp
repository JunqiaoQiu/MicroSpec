/*
	* This test is used to check whether the class Input and Table can perform 
	* as required. 
*/
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "Resources.h"

using namespace std;
using namespace microspec;

int main(int argc, char* argv[])
{
	char* inputFile = argv[1];
	char* tableFile = argv[2];
	char* acceptFile = argv[3];
	int startState = atoi(argv[4]);
	MappingRule* rulesDefault;

	if (argc > 5)
	{
		char* ruleSet = argv[5];
		rulesDefault = MappingRule::defineMappingRule(ruleSet);
	}
	else
		rulesDefault = MappingRule::defineMappingRule();

	Input* inputTest = Input::readFromFile(inputFile, rulesDefault);
	Table* tableTest = Table::readFromFile(tableFile, acceptFile, startState, rulesDefault);
	
	int testLength = (inputTest->getLength() > 10 ) ? 10 : inputTest->getLength();
	for (int i = 0; i < testLength; i++)
		cout << (inputTest->getPointer())[i] << " ";
	cout << endl << endl;

	tableTest->printTable();

	return 0;
}
	