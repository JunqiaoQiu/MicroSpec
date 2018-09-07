/*
	* This test is used to check whether the class MappingRule can perform 
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
	MappingRule* default_obj;
	default_obj = MappingRule::defineMappingRule();

	// Test the default mapping rules
	default_obj->printRules();
	cout << "# possible appear states in default rules is ";
	cout  << default_obj->ruleSize() << endl << endl;

	// Test the mapping rules for benchmark dna
	MappingRule* dna_obj;
	dna_obj = MappingRule::defineMappingRule("dna");
	dna_obj->printRules();
	cout << "# possible appear states in DNA rules is ";
	cout  << dna_obj->ruleSize() << endl << endl;	

	printf("In benchmark DNA\n");
	printf("Transform 'A' to %d\n", dna_obj->char2Int('A'));
	printf("Transform 'T' to %d\n", dna_obj->char2Int('T'));
	printf("Transform 'G' to %d\n", dna_obj->char2Int('G'));
	printf("Transform 'C' to %d\n", dna_obj->char2Int('C'));
	
	return 0;
}
	