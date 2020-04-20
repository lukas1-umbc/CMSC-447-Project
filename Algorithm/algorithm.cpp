// Algorithm.cpp:
// 	- Algorithm and driver for making a swing state.
// 	- Input: Preprocessed/formatted file of state data
// 	- Output: File containing list of the new districts, their party majorities, and their precincts
//	- Dependent files: 
//		- prescinct.h - contains the Prescinct class and methods
//		- district.h - contains the District class and methods
//
// Original Coder: David Ramsey
// Most Recent Change: 19 April 2020
//		- added loadPrecinctData, and splitString functions
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "precinct.h"
#include "district.h"

using namespace std;

/**** CONSTANTS ****/
const string IN_FILE_NAME = "2018_eligible_voters_by_precinct.csv";
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

// splitStrings(): Splits each line from the file into tokens
// - Inputs: string str, the string to be splitString
//           string* tokens, a given array that will be filled with the tokens
void splitString(string str, string* tokens)
{
	// For some reason is dropping the first character???
	// We don't need it, but I don't like it >=[
	string temp_str = "";
	stringstream ss(str);
	
	for(int i = 0; i < 10; i++)
	{
		getline(ss, temp_str, ',');
		tokens[i] = temp_str;
	}
}

// ---IN PROGRESS---
// loadPrecinctData(): Loads in the precinct data from the given file
void loadPrecinctData()
{
	// Each party will have a number associate with it in the precinct object,
	// this will be determined by how they are laid out in the data file.
	// They are as follows:
	// Democrat = 0, Green = 1, Libertarian = 2, Other Parties = 3, Republican = 4, Unaffiliated = 5
	
	int counter = 0;
	int totalVoterCount = 0;
	int voterCount[6];
	
	string precinctID = "";
	string data = "";
	string line[10];
	
	Precinct* newPrecinct;
	
	// Open the data file
	g_inFile.open(IN_FILE_NAME);
	if(g_inFile.is_open())
	{
		// Read in the first line of the file and discard it
		// The first line just has the column headers
		getline(g_inFile, data);
		
		// This loop only reads in the data for the first two
		// Precincts, it is incomplete
		// Still needed: edge cases (for odd entries in data)
		//               add county codes to precinct ids
		//               need to iterate through entire dataset
		// Start reading in valid data
		for(int k = 0; k < 13; k++)
		{
			getline(g_inFile, data);
			splitString(data, line);
			
			// If this is the first loop, get initial pricinct id
			if(precinctID == "")
			{
				newPrecinct = new Precinct();
				precinctID = line[3];
			}
			
			// If the new line has data for a different precinct, add current data to precinct
			// and then add it to g_Precincts, then create a new 
			if(precinctID != line[3])
			{
				newPrecinct->setId(precinctID);
				
				// get the total amount of voters to find percentage of each party
				for(int i = 0; i < 6; i++)
				{
					totalVoterCount += voterCount[i];
				}
				
				// add percentage for each party to m_partyPercents
				for(int i = 0; i < 6; i++)
				{
					newPrecinct->addPartyPercentage(voterCount[i]/(double)totalVoterCount);
				}

				// Add the precinct to g_Precincts, then reset data and create new precinct
				g_Precincts.push_back(newPrecinct);
				newPrecinct = new Precinct();
				precinctID = line[3];
				totalVoterCount = 0;
				counter = 0;
			}
			
			voterCount[counter] = stoi(line[9]);
			counter++;
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
	myPrecinct.setId("123321");
	cout << "My precinct's id is " << myPrecinct.getId() << endl;

	loadPrecinctData();
	g_Precincts[0]->print();
	g_Precincts[1]->print();
	
	// Free all data
	clearAll();
    return 0;
}
