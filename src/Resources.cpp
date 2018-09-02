#include <iostream>

// mmap system
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>  

#include "Resources.hpp"

using namespace std;

namespace microspec
{
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

	Input* Input::ReadFromFile(const char* filename)
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
	    {
	    	if ((int)inputs_Char[l] < 0 || (int)inputs_Char[l] > MAXSYMBOL)
	    		inputs_[l] = 0;
	    	else
	    		inputs_[l] = inputs_Char[l];
	    }

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

	Table* Table::ReadFromFile(const char* filename, const char* acceptFile, const int s)
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
        		if (line.size() > 3)
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
		for (int i = 0; i < numState * numSymbol; i++)
		{
			int realMember =  tableList[i] & 0X0FFFFFFF ;
			cout << realMember << " ";
			if (i % numSymbol == 0 && i != 0)
				cout << endl;
		}
		cout << endl;
	}

}	// End of namespace microspec
