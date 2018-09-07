/*
	* This test is used to check whether class Predictor works. 
*/
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "Timer.h"
#include "Resources.h"
#include "Predictor.h"

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

	Timer T1;
	int chunkTest = 4;

	startTime(&T1);
	Predictor* objPredictor1 = Predictor::constructPredictor(tableTest, inputTest, chunkTest, chunkTest, 100);
	objPredictor1->sequentialPrediction();
	stopTime(&T1);

	cout << "Sequential Prediction " << endl;
	for (int i = 0; i < chunkTest; i++)
		cout << objPredictor1->getPredictState(i) << " " ;
	cout << endl;
	cout << "Spend Time " << elapsedTime(T1) << "s " << endl << endl;

	startTime(&T1);
	Predictor* objPredictor2 = Predictor::constructPredictor(tableTest, inputTest, chunkTest, chunkTest, 100);
	objPredictor2->parallelPrediction();
	stopTime(&T1);

	cout << "Parallel Prediction " << endl;
	for (int i = 0; i < chunkTest; i++)
		cout << objPredictor2->getPredictState(i) << " " ;
	cout << endl;
	cout << "Spend Time " << elapsedTime(T1) << "s " << endl << endl;

	return 0;
}
	