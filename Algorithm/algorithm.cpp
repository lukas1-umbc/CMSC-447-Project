// Algorithm.cpp:
// 	- Algorithm and driver for making a swing state.
// 	- Input: Preprocessed/formatted file of state data
// 	- Output: File containing list of the new districts, their party majorities, and their precincts
//	- Dependent files: 
//		- prescinct.h - contains the Prescinct class and methods
//		- district.h - contains the District class and methods
//
// Original Coder: David Ramsey
// Most Recent Change: 20 April 2020
//		- added loadCountyCodes, fixed splitString function, completed full file read in for precinct voting data
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

#include "precinct.h"
#include "district.h"

using namespace std;

/**** CONSTANTS ****/
const string IN_DATA_FILE_NAME = "2018_eligible_voters_by_precinct.csv";
const string IN_CODE_FILE_NAME = "md_county_codes.csv";
const string OUT_FILE_NAME = "newDistricts.txt";

/***** GLOBALS *****/
// File I/O
ifstream g_inFile;
ofstream g_outFile;

// Calculation
int g_NumParties;
vector<Precinct*> g_Precincts;
vector<District*> g_Districts;

// Data
map<string, string> g_CountyCodes;

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
	string delimiter = ",";
	string temp_str = "";
	
	size_t pos = 0;
	
	int counter = 0;
	
	while ((pos = str.find(delimiter)) != std::string::npos)
	{
		temp_str = str.substr(0, pos);
		tokens[counter] = temp_str;
		str.erase(0, pos + delimiter.length());
		counter++;
	}
	
	tokens[counter] = str;
}

// loadCountyCodes(): Reads in the county names, and codes, and then
//                    loads them into a map for easy access
void loadCountyCodes()
{
	string data = "";
	string line[2];
	
	// Open the data file
	g_inFile.open(IN_CODE_FILE_NAME);
	if(g_inFile.is_open())
	{
		// Read in the first line of the file and discard it
		// The first line just has the column headers
		getline(g_inFile, data);
		while(getline(g_inFile, data))
		{
			// Split the data into the names, and codes
			// then add them to the map
			splitString(data, line);
			g_CountyCodes.insert(pair<string, string>(line[0], line[1]));
		}
			
		g_inFile.close();
	}
}


// loadPrecinctData(): Loads in the precinct data from the given file
void loadPrecinctData()
{
	/******************************* Notes **************************************
								---IN PROGRESS---
	TODO: Get precinct voter population from precinct 2010 data file
	
	Each party will have a number associate with it in the precinct object,
	this will be determined by how they are laid out in the data file.
	They are as follows:
	Democrat = 0, Green = 1, Libertarian = 2, Other Parties = 3, Republican = 4, Unaffiliated = 5
	
	Precinct IDs are added to county codes in the following form:
    Allegany county code = 24001, precinct 1 id = 001-000 => 24001001-000
	****************************************************************************/
	
	int counter = 0;
	int totalVoterCount = 0;
	int voterCount[6];
	
	string precinctID = "";
	string data = "";
	string line[10];
	
	Precinct* newPrecinct;
	
	// Load in the county codes for precinct IDs
	loadCountyCodes();
	
	// Open the data file
	g_inFile.open(IN_DATA_FILE_NAME);
	if(g_inFile.is_open())
	{
		// Read in the first line of the file and discard it
		// The first line just has the column headers
		getline(g_inFile, data);
		
		// Start reading in valid data
		while(getline(g_inFile, data))
		{
			splitString(data, line);
			
			// Some of the data entries are empty. They are signafied
			// by the phrase "Unable to Determine" in the precinct id line
			if(line[3] != "Unable to Determine")
			{
				// If this is the first loop, get initial pricinct id and create new precinct object
				if(precinctID == "")
				{
					newPrecinct = new Precinct();
					precinctID = line[3];
				}

				// Save voter count arranged by the party number formatted listed above
				voterCount[counter] = stoi(line[9]);
				counter++;
				
				// If all the data is read in for a precinct (6 lines of data), then complete
				// the precinct and add it to the precinct list
				if(counter == 6)
				{	
					// This segment is needed (I wasn't able to just append the county code because
					// there was some weird formatting) to clean up the county code, and then
					// get it formatted properly
					string temp = "";
					temp.append(to_string(stoi(g_CountyCodes.find(line[0])->second)));
					temp.append(line[3]);
					newPrecinct->setId(temp);
					
					// Get the total amount of voters to find percentage of each party
					for(int i = 0; i < 6; i++)
					{
						totalVoterCount += voterCount[i];
					}
					
					// Add percentage for each party to m_partyPercents
					for(int i = 0; i < 6; i++)
					{
						newPrecinct->addPartyPercentage(voterCount[i]/(double)totalVoterCount);
					}
						
					// Add the precinct to g_Precincts, then reset data and create new precinct
					g_Precincts.push_back(newPrecinct);
					newPrecinct = new Precinct();
					totalVoterCount = 0;
					counter = 0;

				}
			}
		}
		g_inFile.close();
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
	

	for(int i = 0; i < (int) g_Precincts.size(); i++)
	{
		g_Precincts[i]->print();
	}

	
	// Free all data
	clearAll();
    return 0;
}
