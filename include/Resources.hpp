/*
* @Author: Junqiao Qiu
* @Last Modified by:   Junqiao Qiu
* @Last Modified time: 2018-08-31 
*/

#ifndef RESOURCES_H
#define RESOURCES_H

#include <stdlib.h>
#include <stdio.h>

namespace microspec 
{
	#define ASCII256 256
	#define MAXSYMBOL ASCII256

	class  Input
	{
	public:
		Input();	
		Input(int* p, long s);	
		~Input();

		// @brief Provide @filename to access the target inputs, 
		// and then return an @Input type object  
		static Input* ReadFromFile(const char* filename);	
		
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

		// typedef void(*func)(void*, int) ;

		// @brief Providing @filename to access the given two-dimension table, 
		// with using @acceptFile to mark the accept state in the table, 
		// and applying required start state @s, then return an @Table-type object  
		static Table* ReadFromFile(const char* filename, const char* acceptFile, const int s);

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