/*
* @Author: Junqiao Qiu
* @Last Modified by:   Junqiao Qiu
* @Last Modified time: 2018-08-31 
*/

#ifndef MICROSPEC_H
#define MICROSPEC_H

#include <stdlib.h>
#include <stdio.h>

namespace microspec 
{
	class  Input
	{
	public:
		Input();	
		Input(int* p, long size);	
		~Input();

		// @brief Provide @filename to access the target inputs  
		static Input* ReadFromFile(const char* filename);	
		
		int* getPointer() const;
		long getLength() const;

	private:

		int* pointer;
		long size;
	};

	class Table
	{
	public:
		Table();
		Table(int* List, int nState, int nSymbol, int start);
		~Table();

		static Table* ReadFromFile(const char* filename);

		int* getTable() const;
		int getNumState() const;
		int getNumSymbol() const;
		int getStartState();

	private:
		int* tableList;
		int numState;
		int numSymbol;
		int start;
	};

	class DFA
	{
	public:
		DFA();
		~DFA();

		void run(const Table* table, const Input* input);

	private:
		int* mTable;
		int* mInputs;
		int mStart;

	};

}	// end of namespace microspec

#endif // MICROSPEC_H