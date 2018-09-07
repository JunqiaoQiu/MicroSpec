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

		// @Brief Provide @ruleKey to denote the rules aim to set, 
		// which is current supported to be dna/div/protn/evenodd. And the 
		// other cases will be consider as default one, i.e., ASCII256. 
		static MappingRule* defineMappingRule();
		static MappingRule* defineMappingRule(char* ruleKey);
	private:
		// @Brief character mapping rules, i.e., rules about transforming 
		// char-type character into int-type integer.
		int* mRules;
		// the number of real effective (i.e., possible appear) characters
		int mSize;

	};

	// @Brief The class @Input is used to store the necessary input contents 
	// where DFAs execute on
	class  Input
	{
	public:
		Input();	
		Input(int* inputsPointer, long inputSize);	
		~Input();

		// @brief Provide @inputFileName to access the target inputs, 
		// and @ruleUsed to denote the possible appear characters and 
		// their mapping rule, then return an @Input type object  
		static Input* readFromFile(const char* inputFileName, 
			const MappingRule* ruleUsed);	
		
		int* getPointer() const;
		long getLength() const;

	private:
		// the pointer to the executing inputs, it should follow aligned (32)
		int* mPointer __attribute__ ((aligned (32)));
		long mSize;
	};

	// @Brief The class @Table is used to describe the transition table in DFA
	class Table
	{
	public:
		Table();
		Table(int* transTable, int nState, int nSymbol, int startState);
		~Table();

		// @brief Providing @tableFileName to access the given transition table, 
		// with using @acceptFileName to mark the accept state in the table.
		// And applying required start state @startState while also define the mapping rule @ruleUsed, 
		// then return an @Table-type object  
		static Table* readFromFile(const char* tableFileName, const char* acceptFileName, 
			const int startState, const MappingRule* ruleUsed);

		int* getTable() const;
		int getNumState() const;
		int getNumSymbol() const;
		int getStartState() const;
		void printTable() const;

	private:
		int* mTableList __attribute__ ((aligned (32)));
		int mNumState;
		int mNumSymbol;
		int mStartState;
	};
}	// end of namespace microspec

#endif // RESOURCES_H
