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
	// The current rule range
	const int ASCII256 = 256;

	// @Brief The class @MappingRule defines the mapping rule about transforming 
	// the possible appear char-type input characters into an integer.   
	class MappingRule
	{
	public:
		MappingRule();
		MappingRule(int* rules, int size);
		~MappingRule();
		
		// Transform the char type character to int type
		int char2Int(char character) const;
		void printRules() const;
		int ruleSize() const;

		// @Brief Provide @rs_key to denote the rules aim to set, 
		// which is current supported to be dna/div/protn/evenodd. And the 
		// other cases will be consider as default one, i.e., ASCII256. 
		static MappingRule* defineMappingRule();
		static MappingRule* defineMappingRule(char* rs_key);
	private:
		// @Brief character mapping rules, i.e., rules about transforming 
		// char-type character into int-type integer
		int* mrules;
		// the number of real effective (i.e., possible appear) characters
		int msize;
	};

	// @Brief The class @Input is used to store the necessary input contents 
	// where DFAs execute on
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

	// @Brief The class @Table is used to store the main information about  
	// the target DFA, more specefically, the table information
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
