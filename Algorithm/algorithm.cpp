// Algorithm.cpp:
// 	- Algorithm and driver for making a swing state.
// 	- Input: Preprocessed/formatted file of state data
// 	- Output: File containing list of the new districts, their party majorities, and their precincts
//	- Dependent files: 
//		- prescinct.h - contains the Prescinct class and methods
//		- district.h - contains the District class and methods
//
// Original Coder: David Ramsey
// Most Recent Change: 15 April 2020
//		- Initial creation, added globals and clearAll() function
//

#include <iostream>
#include <fstream>
#include <string>

#include "precinct.h"
#include "district.h"

using namespace std;

/**** CONSTANTS ****/
const string IN_FILE_NAME = "testFile.txt";
const string OUT_FILE_NAME = "newDistricts.txt";

/***** GLOBALS *****/
// File I/O
ifstream g_inFile;
ofstream g_outFile;

// Calculation
int g_NumParties;
vector<Precinct*> g_Precincts;
vector<District*> g_Districts;

/**** FUNCTIONS ****/
// clearAll(): Frees all dynamically allocated memory
void clearAll()
{
	// Free precincts
	for(int i = 0; i < (int)g_Precincts.size(); i++)
	{
		if(g_Precincts[i] != NULL)
		{
			delete g_Precincts[i];
			g_Precincts[i] = NULL;
		}
	}

	// Free districts
	for(int i = 0; i < (int)g_Districts.size(); i++)
	{
		if(g_Districts[i] != NULL)
		{
			delete g_Districts[i];
			g_Districts[i] = NULL;
		}
	}
}


/**** MAIN ****/
int main() {
	printf("Hello Windows.\n");

	District myDistrict;
	myDistrict.setId(123);
	printf("My district's id is %d\n", myDistrict.getId());

	Precinct myPrecinct;
	myPrecinct.setId(123321);
	printf("My precinct's id is %d\n", myPrecinct.getId());

	// Free all data
	clearAll();
    return 0;
}
