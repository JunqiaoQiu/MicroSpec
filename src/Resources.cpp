#include <iostream>

// mmap system
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <cstring>
#include <string>
#include <cctype>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <map>
#include <algorithm>  

#include "Resources.h"

using namespace std;

namespace microspec
{

/* ---------class MappingRule------------ */

	MappingRule::MappingRule()
	{
		mRules = new int [ASCII256];
		mSize = ASCII256;		
		for (int i = 0; i<ASCII256; i++)
			mRules[i] = i;
	}

	MappingRule::MappingRule(int* rules, int size)
	{
		mRules = new int [ASCII256];
		mSize = size;
		for (int i = 0; i<ASCII256; i++)
			mRules[i] = rules[i];
	}

	MappingRule::~MappingRule()
	{
		delete []mRules;
	}

	MappingRule* MappingRule::defineMappingRule()
	{
		MappingRule* obj = new MappingRule();
		return obj;
	}

	MappingRule* MappingRule::defineMappingRule(char* ruleKey)
	{
		char* ruleKeyLow;
		ruleKeyLow = new char [strlen(ruleKey)];
		for (int i = 0; i < strlen(ruleKey); ++i)
	    	ruleKeyLow[i] = tolower(ruleKey[i]);

		// Currently allow size of 256
		int* tempRules;
		int tempSize;

		tempRules = new int [ASCII256];
		for (int j=0; j<ASCII256; j++)
			tempRules[j] = 0;

		if (ruleKeyLow == std::string("dna"))
		{
			tempSize = 4;
			tempRules['A'] = 0;
			tempRules['T'] = 1;
			tempRules['G'] = 2;
			tempRules['C'] = 3;
		}
		else if (ruleKeyLow == std::string("protn") )
		{
			tempSize = 20;
			tempRules['A'] = 0;
			tempRules['C'] = 1;
			tempRules['D'] = 2;
			tempRules['E'] = 3;
			tempRules['F'] = 4;
			tempRules['G'] = 5;
			tempRules['H'] = 6;
			tempRules['I'] = 7;
			tempRules['K'] = 8;
			tempRules['L'] = 9;
			tempRules['M'] = 10;
			tempRules['N'] = 11;
			tempRules['P'] = 12;
			tempRules['Q'] = 13;
			tempRules['R'] = 14;
			tempRules['S'] = 15;
			tempRules['T'] = 16;
			tempRules['V'] = 17;
			tempRules['W'] = 18;
			tempRules['Y'] = 19;
		}
		else if (ruleKeyLow == std::string("evenodd") )
		{
			tempSize = 4;
			tempRules['a'] = 0;
			tempRules['b'] = 1;
			tempRules['c'] = 2;
			tempRules['d'] = 3;			
		}
		else if (ruleKeyLow == std::string("div") )
		{
			tempSize = 2;
			tempRules['0'] = 0;
			tempRules['1'] = 1;			
		}
		else
		{
			tempSize = ASCII256;
			for (int i = 0; i<tempSize; i++)
			{
				tempRules[i] = i;
			}
		}

		MappingRule* obj = new MappingRule(tempRules, tempSize);
		return obj;
	}

	int MappingRule::char2Int(char character) const
	{
		int temp;
		if (character < 0 || character >= ASCII256)
			temp = 0;
		else
			temp = mRules[character];
		return temp;
	}

	int MappingRule::ruleSize() const
	{
		return mSize;
	}

	void MappingRule::printRules() const
	{
		int it;
		for (it = 0; it < ASCII256; it++)
			cout << (char)it << " : " << mRules[it] << endl;
		cout << endl;
	}

/* ---------class Input------------ */

	Input::Input()
	{
		mPointer = NULL;
		mSize = 0;
	}

	Input::Input(int* inputsPointer, long inputSize)
	{
		mPointer = inputsPointer;
		mSize = inputSize;
	}

	Input::~Input()
	{
		delete []mPointer;
		mSize = 0;
	}

	Input* Input::readFromFile(const char* inputFileName, const MappingRule* ruleUsed)
	{
		int* inputs_ __attribute__ ((aligned (32)));
		long length_;
		char* inputs_Char;

		struct stat infile_sb;
		int fdSrc = open(inputFileName, O_RDONLY);
	    if (fdSrc == -1 || fstat(fdSrc, &infile_sb) == -1 )
	    {
	        cout << "Error: cannot open " << inputFileName << " for processing. Skipped." << endl;
	        return NULL;
	    }	
	    length_ = infile_sb.st_size;

	    if (length_ == 0)
	    	inputs_ = nullptr;
	    else
	    {
	    	inputs_Char = (char *) mmap(NULL, length_, PROT_READ, MAP_PRIVATE, fdSrc, 0);
	        if (inputs_Char == MAP_FAILED)
	        {
	            if (errno ==  ENOMEM)
	                cout << "Error:  mmap of " << inputFileName << " failed: out of memory\n";
	            else
	                cout << "Error: mmap of " << inputFileName << " failed with errno " << errno << ". Skipped.\n";
	            return NULL;
	        }	    	
	    }

	    inputs_ = new int [length_];
	    for (int l = 0; l < length_; l++)
	    	inputs_[l] = ruleUsed->char2Int(inputs_Char[l]);

	    // mmap and input file close
    	munmap((void *)inputs_Char, length_);
    	close(fdSrc);

	    Input *object = new Input(inputs_, length_);
	    return object;
	}

	int* Input::getPointer() const
	{
		return mPointer;
	}

	long Input::getLength() const
	{
		return mSize;
	}
	 
/* ---------class Table------------ */

	Table::Table()
	{
		mTableList = NULL;
		mNumState = 0;
		mNumSymbol = 0;
		mStartState = 0;
	} 

	Table::Table(int* list, int nstate, int nsymbol, int s)
	{
		mTableList = list;
		mNumState = nstate;
		mNumSymbol = nsymbol;
		mStartState = s;
	}

	Table::~Table()
	{
		delete []mTableList;
		mNumState = 0;
		mNumSymbol = 0;
		mStartState = 0;
	}

	Table* Table::readFromFile(const char* filename, const char* acceptFile, 
			const int s, const MappingRule* ruleset)
	{	
		int* list_  __attribute__ ((aligned (32)));
		int nstate_;
		int nsymbol_;

		// Loading the accept states from the acceptFile
		vector<int> acceptVec;
		ifstream in_ac;
		in_ac.open(acceptFile);
		if (in_ac.is_open())
		{
			while(in_ac)
			{
				int temp_ac;
				in_ac >> temp_ac;
				acceptVec.push_back(temp_ac);
			}
			in_ac.close();
		}
		else
		{
			cout << "Fail to open Accept file " << acceptFile << endl;
			return NULL;
		}

		int MAXSYMBOL = ruleset->ruleSize();
		vector<int> vecTable;
		ifstream in_table;
		in_table.open(filename);
		if (in_table.is_open())
		{
			string line;
    		while(!in_table.eof())
    		{
        		getline(in_table,line);
        		if(in_table.fail())
            		break;
        		if (line.size() > 2)
        		{
        			int currentLineNum = 0;
        			stringstream stream(line);
        			while(stream && currentLineNum != MAXSYMBOL)
        			{
        				int temp_n;
        				stream >> temp_n;
        				currentLineNum++;
        				if (find (acceptVec.begin(), acceptVec.end(), temp_n) != acceptVec.end())
        					temp_n = temp_n | 0XF0000000;
        				else
        					temp_n = temp_n & 0X0FFFFFFF;
        				vecTable.push_back(temp_n);
    				}
    				if (currentLineNum != MAXSYMBOL)
    				{
    					cout << "Number of Symbol does not match with current setting \n";
    					return NULL;
    				}
        		}
    		}
		}
		else
		{
			cout << "Fail to open Table file " << filename << endl;
			return NULL;
		}

		list_ = new int [(int)vecTable.size()];
		nsymbol_ = MAXSYMBOL;
		nstate_ = ((int)vecTable.size()) / nsymbol_;

		for (int i = 0 ; i < vecTable.size(); i++)
			list_[i] = vecTable[i];

	    Table *object = new Table(list_, nstate_, nsymbol_, s);
	    return object;		
	}

	int* Table::getTable() const
	{
		return mTableList;
	}

	int Table::getNumState() const
	{
		return mNumState;
	}
	int Table::getNumSymbol() const
	{
		return mNumSymbol;
	}

	int Table::getStartState() const
	{
		return mStartState;
	}

	void Table::printTable() const
	{
		vector<int> acceptStateVec;
		for (int i = 0; i < mNumState * mNumSymbol; i++)
		{
			int realMember =  mTableList[i] & 0X0FFFFFFF;
			if( (mTableList[i] & 0XF0000000) == 0XF0000000 )
			{
				if (find(acceptStateVec.begin(), acceptStateVec.end(), realMember) == acceptStateVec.end())
					acceptStateVec.push_back(realMember);
			}

			cout << realMember << " ";
			if ((i+1) % mNumSymbol == 0)
				cout << endl;
		}
		cout << endl;
		cout << "#State " << this->getNumState() << ", #Symbol " << this->getNumSymbol() << endl;
		cout << "Accept States include: " << endl;
		for (int j = 0; j < acceptStateVec.size(); j++)
			cout << acceptStateVec[j] << " ";
		cout << endl;
	}

}	// End of namespace microspec
