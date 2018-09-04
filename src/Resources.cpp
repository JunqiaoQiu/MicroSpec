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

#include "Resources.hpp"

using namespace std;

namespace microspec
{
	
	MappingRule::MappingRule()
	{
		mrules = new int [ASCII256];
		msize = ASCII256;		
		for (int i = 0; i<ASCII256; i++)
			mrules[i] = i;
	}

	MappingRule::MappingRule(int* rules, int size)
	{
		mrules = new int [ASCII256];
		msize = size;
		for (int i = 0; i<ASCII256; i++)
			mrules[i] = rules[i];
	}

	MappingRule::~MappingRule()
	{
		delete []mrules;
	}

	MappingRule* MappingRule::DefineMappingRule()
	{
		MappingRule* obj = new MappingRule();
		return obj;
	}

	MappingRule* MappingRule::DefineMappingRule(char* rs_key)
	{
		char* rs_key_low;
		rs_key_low = new char [strlen(rs_key)];
		for (int i = 0; i < strlen(rs_key); ++i)
	    	rs_key_low[i] = tolower(rs_key[i]);

		// Currently allow size of 256
		int* temprules;
		int tempsize;

		temprules = new int [ASCII256];
		for (int j=0; j<ASCII256; j++)
			temprules[j] = 0;

		if (rs_key_low == std::string("dna"))
		{
			tempsize = 4;
			temprules['A'] = 0;
			temprules['T'] = 1;
			temprules['G'] = 2;
			temprules['C'] = 3;
		}
		else if (rs_key_low == std::string("protn") )
		{
			tempsize = 20;
			temprules['A'] = 0;
			temprules['C'] = 1;
			temprules['D'] = 2;
			temprules['E'] = 3;
			temprules['F'] = 4;
			temprules['G'] = 5;
			temprules['H'] = 6;
			temprules['I'] = 7;
			temprules['K'] = 8;
			temprules['L'] = 9;
			temprules['M'] = 10;
			temprules['N'] = 11;
			temprules['P'] = 12;
			temprules['Q'] = 13;
			temprules['R'] = 14;
			temprules['S'] = 15;
			temprules['T'] = 16;
			temprules['V'] = 17;
			temprules['W'] = 18;
			temprules['Y'] = 19;
		}
		else if (rs_key_low == std::string("evenodd") )
		{
			tempsize = 4;
			temprules['a'] = 0;
			temprules['b'] = 1;
			temprules['c'] = 2;
			temprules['d'] = 3;			
		}
		else if (rs_key_low == std::string("div") )
		{
			tempsize = 2;
			temprules['0'] = 0;
			temprules['1'] = 1;			
		}
		else
		{
			tempsize = ASCII256;
			for (int i = 0; i<tempsize; i++)
				temprules[i] = i;
		}

		MappingRule* obj = new MappingRule(temprules, tempsize);
		return obj;
	}

	int MappingRule::Char2Int(char character) const
	{
		int temp;
		if (character < 0 || character >= ASCII256)
			temp = 0;
		else
			temp = mrules[character];
		return temp;
	}

	int MappingRule::RuleSize() const
	{
		return msize;
	}

	void MappingRule::PrintRules() const
	{
		int it;
		for (it = 0; it < ASCII256; it++)
			cout << (char)it << " : " << mrules[it] << endl;
		cout << endl;
	}

	Input::Input()
	{
		pointer = NULL;
		size = 0;
	}

	Input::Input(int* p, long s)
	{
		pointer = p;
		size = s;
	}

	Input::~Input()
	{
		delete []pointer;
		size = 0;
	}

	Input* Input::ReadFromFile(const char* filename, const MappingRule* ruleset)
	{
		long length_;
		int* inputs_;
		char* inputs_Char;

		struct stat infile_sb;
		int fdSrc = open(filename, O_RDONLY);
	    if (fdSrc == -1 || fstat(fdSrc, &infile_sb) == -1 )
	    {
	        cout << "Error: cannot open " << filename << " for processing. Skipped." << endl;
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
	                cout << "Error:  mmap of " << filename << " failed: out of memory\n";
	            else
	                cout << "Error: mmap of " << filename << " failed with errno " << errno << ". Skipped.\n";
	            return NULL;
	        }	    	
	    }

	    inputs_ = new int [length_];
	    for (int l = 0; l < length_; l++)
	    	inputs_[l] = ruleset->Char2Int(inputs_Char[l]);

	    // mmap and input file close
    	munmap((void *)inputs_Char, length_);
    	close(fdSrc);

	    Input *object = new Input(inputs_, length_);
	    return object;
	}

	int* Input::getPointer() const
	{
		return pointer;
	}

	long Input::getLength() const
	{
		return size;
	}
	 

	Table::Table()
	{
		tableList = NULL;
		numState = 0;
		numSymbol = 0;
		start = 0;
	} 

	Table::Table(int* list, int nstate, int nsymbol, int s)
	{
		tableList = list;
		numState = nstate;
		numSymbol = nsymbol;
		start = s;
	}

	Table::~Table()
	{
		delete []tableList;
		numState = 0;
		numSymbol = 0;
		start = 0;
	}

	Table* Table::ReadFromFile(const char* filename, const char* acceptFile, 
			const int s, const MappingRule* ruleset)
	{	
		int* list_;
		int nstate_;
		int nsymbol_;

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

		int MAXSYMBOL = ruleset->RuleSize();
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
        				currentLineNum ++;
        				if (find (acceptVec.begin(), acceptVec.end(), temp_n) != acceptVec.end())
        					temp_n = temp_n | 0XF0000000;
        				else
        					temp_n = temp_n | 0X00000000;
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
		return tableList;
	}

	int Table::getNumState() const
	{
		return numState;
	}
	int Table::getNumSymbol() const
	{
		return numSymbol;
	}

	int Table::getStartState() const
	{
		return start;
	}

	void Table::printTable() const
	{
		cout << "(" << this->getNumState() << " " << this->getNumSymbol() << ")" << endl;
		for (int i = 0; i < numState * numSymbol; i++)
		{
			int realMember =  tableList[i] & 0X0FFFFFFF ;
			cout << realMember << " ";
			if ((i+1) % numSymbol == 0)
				cout << endl;
		}
		cout << endl;
	}

}	// End of namespace microspec
