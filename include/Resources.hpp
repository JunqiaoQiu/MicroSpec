/*
* @Author: Junqiao Qiu
* @Last Modified by:   Junqiao Qiu
*/

#ifndef RESOURCES_H
#define RESOURCES_H

#include <stdlib.h>
#include <stdio.h>

namespace microspec 
{
	// Current rule range
	const int ASCII256 = 256;

	// @Brief The class @MappingRule defines the mapping rule of 
	// input characters in the following @Input class.   
	class MappingRule
	{
	public:
		MappingRule();
		MappingRule(int* rules, int size);
		~MappingRule();
		
		// Transform the char type character to int type
		int Char2Int(char character) const;
		void PrintRules() const;
		int RuleSize() const;

		// @brief Provide @rs_key to denote the rules aim to set. 
		// Current support dna/div/protn/evenodd, and the others will 
		// be consider as ASCII256. 
		static MappingRule* DefineMappingRule();
		static MappingRule* DefineMappingRule(char* rs_key);
	private:
		// character mapping rules
		int* mrules;
		// the number of real effective (i.e., possible appear) characters
		int msize;
	};

	class  Input
	{
	public:
		Input();	
		Input(int* p, long s);	
		~Input();

		// @brief Provide @filename to access the target inputs, 
		// and @reuleset to denote the possible appear characters,  
		// then return an @Input type object  
		static Input* ReadFromFile(const char* filename, 
			const MappingRule* ruleset);	
		
		int* getPointer() const;
		long getLength() const;

	private:
		int* pointer __attribute__ ((aligned (32)));
		long size;
	};

	class Table
	{
	public:
		Table();
		Table(int* list, int nstate, int nsymbol, int s);
		~Table();

		// typedef void(*func)(void*, int);

		// @brief Providing @filename to access the given two-dimension table, 
		// with using @acceptFile to mark the accept state in the table, 
		// and applying required start state @s while also define the mapping rule @ruleset, 
		// then return an @Table-type object  
		static Table* ReadFromFile(const char* filename, const char* acceptFile, 
			const int s, const MappingRule* ruleset);

		// void setAction(func f_);
		// func getAction() const;

		int* getTable() const;
		int getNumState() const;
		int getNumSymbol() const;
		int getStartState() const;
		void printTable() const;

	private:
		int* tableList __attribute__ ((aligned (32)));
		int numState;
		int numSymbol;
		int start;
		// func mFunc;
	};
}	// end of namespace microspec

#endif // RESOURCES_H
