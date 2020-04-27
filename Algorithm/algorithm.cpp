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
//		- added loadCountyCodes, fixed splitString function, completed full file read in for precinct voting data, added loadPrecinctPop
//

#include <iostream>
#include <fstream>
#include <string>
#include <string.h> //For c-strings
#include <sstream>
#include <map>
#include <stdio.h>

#include "precinct.h"
#include "district.h"

using namespace std;

//*** Temp variables for testing, delete later
const int NUM_DISTRICTS = 8; //cheating, using as place holder for now
const string IN_FILE = "md_parsed_data.txt";
const string NEIGHBOR_FILE = "md_precinct_neighbors.txt";


enum {DEMOCRAT, GREEN, LIBERTARIAN, OTHER, REPUBLICAN};
bool activeParties[5];
//*********



/**** CONSTANTS ****/
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

	string currentLine; //For reading line by line
	char* charArray = new char[256]; // for converting string line into char*
	char* charArray2 = new char[256]; //for secondary tokenizing

	string precinctId;



	//***** Read in Precinct Data ****

	g_inFile.open(IN_FILE);
	if (!(g_inFile.is_open()))
	{
		printf("ERROR trying to open precinct file. /n");
		return 1;
	}

	//First line = number of dominant parties
	getline(g_inFile, currentLine);
	strcpy(charArray, currentLine.c_str());
	g_NumParties = atoi(strtok(charArray, " \t\v\r\n\f,()"));


	//Second line = active parties
	getline(g_inFile, currentLine);
	strcpy(charArray, currentLine.c_str());

	activeParties[DEMOCRAT] = atoi(strtok(charArray, " \t\v\r\n\f,()"));
	activeParties[GREEN] = atoi(strtok(NULL, " \t\v\r\n\f,()"));
	activeParties[LIBERTARIAN] = atoi(strtok(NULL, " \t\v\r\n\f,()"));
	activeParties[OTHER] = atoi(strtok(NULL, " \t\v\r\n\f,()"));
	activeParties[REPUBLICAN] = atoi(strtok(NULL, " \t\v\r\n\f,()"));

	//Header line, skip over
	getline(g_inFile, currentLine);

	map<string, int> precinctMap; //use this to track precinct in vector, will use for adding neighbors
	int precinctCount = 0;

	//Loop to add precincts
	while(getline(g_inFile, currentLine))
	{
		/*
		 * //Blank line case TODO: Implement later
		 * if(currentLine == NULL)
		 * continue;
		 *
		*/


		strcpy(charArray, currentLine.c_str());

		//Create new Precinct
		Precinct* readInPrecinct = new Precinct();
		precinctId = strtok(charArray, " \t\v\r\n\f,()");

		readInPrecinct->setId(precinctId);
		readInPrecinct->setTotalPop(atoi(strtok(NULL, " \t\v\r\n\f,()")));

		//TODO Figure out if better to grab all party percentages, or just majority. For now, all

		readInPrecinct->addPartyPercentage(stof(strtok(NULL, " \t\v\r\n\f,()")));
		readInPrecinct->addPartyPercentage(stof(strtok(NULL, " \t\v\r\n\f,()")));
		readInPrecinct->addPartyPercentage(stof(strtok(NULL, " \t\v\r\n\f,()")));
		readInPrecinct->addPartyPercentage(stof(strtok(NULL, " \t\v\r\n\f,()")));
		readInPrecinct->addPartyPercentage(stof(strtok(NULL, " \t\v\r\n\f,()")));

		//Add precinct to vector
		g_Precincts.push_back(readInPrecinct);

		//Add to map
		precinctMap.insert(pair<string,int>(precinctId, precinctCount));
		precinctCount++;

	}

	g_inFile.close();


	//********* Add neighbor precincts  *********

	g_inFile.open(NEIGHBOR_FILE);
	if (!(g_inFile.is_open()))
	{
		printf("ERROR trying to open neighbor file. /n");
		return 1;
	}

	//loop through each line in neighbor list
	while(getline(g_inFile, currentLine))
	{
		strcpy(charArray, currentLine.c_str());

		precinctId = strtok(charArray, ":");
		map<string,int>::iterator iter = precinctMap.find(precinctId);

		if(iter == precinctMap.end())
		{
			//Somehow this precinct isn't listed, print error for now
			printf("ERROR! Precinct with id(%s) does not already exist! \n", precinctId);
			continue;
		}

		int index = iter->second;
		Precinct* targetPrecinct = g_Precincts[index]; //The precinct we want to add neighbors to


		//Read up to first '[', signaling start of neighbors
		precinctId = strtok(NULL, "[");

		string listNeighbors;
		//Grab everything up to ']'
		listNeighbors = strtok(NULL, "]");

		//Move everything grabbed into charArray2 for further tokenizing
		strcpy(charArray2, listNeighbors.c_str());

		precinctId = strtok(charArray2, " ,'");
		if(precinctId == nullptr)
		{
			//A (') doesn't show up, meaning we have an empty neighbor list ( 401918-001: [])
			//in that case continue to next line
			continue;
		}

		//else, there is at least one neighbor, so keep tonkenizing till we hit the end
		precinctId = strtok(NULL, " ,'");
		while(precinctId != nullptr)
		{

			iter = precinctMap.find(precinctId);

			if(iter == precinctMap.end())
			{
				//Somehow this precinct isn't listed, print error for now
				printf("ERROR! Precinct with id(%s) does not already exist! Cannot add as neighbor \n", precinctId);
				continue;
			}

			int index = iter->second;
			Precinct* targetNeighborPrecinct = g_Precincts[index];

			targetPrecinct->m_neighbors.push_back(targetNeighborPrecinct);

			precinctId = strtok(NULL, " ,'");
		}
	}




	//**********   Setup Districts   **************


	int partyTargetAssignment; //Tracks what party to assign district
	//TODO remove hardcode of 5, this represents the number of possible parties
	for(int jj = 0; jj < 5; jj++)
	{
		if(activeParties[jj])
		{
			partyTargetAssignment = jj;
			break;
		}
	}


	for(int ii = 0; ii < NUM_DISTRICTS; ii++)
	{
		District* newDistrict = new District();
		newDistrict->setId(ii);
		newDistrict->setParty(partyTargetAssignment);

		for(int jj = (partyTargetAssignment + 1) % 5 ;; jj++) //TODO again, change from hard coding 5
		{
			if(activeParties[jj])
			{
				partyTargetAssignment = jj;
				break;
			}

		}

		g_Districts.push_back(newDistrict);
	}



	//********** Add Precincts to Districts ********
	//REMINDER: After precinct added to district, run district.manageEdges() function
	
	
	
	
	
	
/*
	loadPrecinctData();
	loadPrecinctPop();
	
	for(int i = 0; i < (int) g_Precincts.size(); i++)
	{
		g_Precincts[i]->print();
	}
	
*/
	// Free all data
	clearAll();
    return 0;
}
