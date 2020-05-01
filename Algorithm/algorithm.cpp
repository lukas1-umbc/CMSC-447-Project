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
#include <ctime> //Use time to seed a random number

#include "precinct.h"
#include "district.h"

using namespace std;


//*** Temp variables for testing, delete later
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

	srand(time(nullptr));// Create semi-random set of numbers

	string currentLine; //For reading line by line
	char* charArray = new char[256]; // for converting string line into char*
	char* charArray2 = new char[256]; //for secondary tokenizing

	string precinctId;
	int numDistricts;





	//TODO     Read in first data, which is number of districts
	//TODO	   New parsed data will have a line after active party booleans, giving corresponding string names









	//***** Read in Precinct Data ****

	g_inFile.open(IN_FILE);
	if (!(g_inFile.is_open()))
	{
		printf("ERROR trying to open precinct file. /n");
		return 1;
	}

	//First Line = number of Districts
	getline(g_inFile, currentLine);
	strcpy(charArray, currentLine.c_str());
	numDistricts = atoi(strtok(charArray, " \t\v\r\n\f,()"));

	//Second line = number of dominant parties
	getline(g_inFile, currentLine);
	strcpy(charArray, currentLine.c_str());
	g_NumParties = atoi(strtok(charArray, " \t\v\r\n\f,()"));


	//Third line = active parties
	getline(g_inFile, currentLine);
	strcpy(charArray, currentLine.c_str());

	activeParties[DEMOCRAT] = atoi(strtok(charArray, " \t\v\r\n\f,()"));
	activeParties[GREEN] = atoi(strtok(NULL, " \t\v\r\n\f,()"));
	activeParties[LIBERTARIAN] = atoi(strtok(NULL, " \t\v\r\n\f,()"));
	activeParties[OTHER] = atoi(strtok(NULL, " \t\v\r\n\f,()"));
	activeParties[REPUBLICAN] = atoi(strtok(NULL, " \t\v\r\n\f,()"));

	//TODO Read in name of Parties, place in vector of strings
	getline(g_inFile, currentLine); //For now, skip

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

		readInPrecinct->updateMajorParty();

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


	int debug = 0; // DELETE LaTER



	//loop through each line in neighbor list
	while(getline(g_inFile, currentLine) && debug <= 1849)
	{
		printf("%i\n",debug);
		//printf("\n \n %s", currentLine);
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

		char* precinctIdChar = strtok(NULL, " ,'\n");
		//precinctId = strtok(NULL, " ,'\n");
		while(precinctIdChar != NULL)
		{
			precinctId = precinctIdChar;
			iter = precinctMap.find(precinctId);

			if(iter == precinctMap.end())
			{
				//Somehow this precinct isn't listed, print error for now
				//printf("ERROR at %i! Precinct with id(%s) does not already exist! Cannot add as neighbor \n",debug, precinctId);
				continue;
			}

			int index = iter->second;
			Precinct* targetNeighborPrecinct = g_Precincts[index];

			targetPrecinct->m_neighbors.push_back(targetNeighborPrecinct);

			precinctIdChar = strtok(NULL, " ,'\n");
		}
		debug++;
	}


	for(int i = 0; i < (int) g_Precincts.size(); i++)
	{
		g_Precincts[i]->print();
		printf("----- Neighbors: ");
		for(int jj = 0; jj < g_Precincts[i]->m_neighbors.size(); jj++)
		{
			printf("%s, ",g_Precincts[i]->m_neighbors[jj]->getId());
		}
		printf("\n _____________________________\n");
	}

	g_inFile.close();

	//**********   Setup Districts   **************

	//Setup an array of iterators for both precincts owned, and for neighbors, for each district
	vector<Precinct*>::iterator arrayOfIter[numDistricts];
	vector<Precinct*>::iterator arrayOfNeighborIter[numDistricts];

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


	for(int ii = 0; ii < numDistricts; ii++)
	{
		District* newDistrict = new District();
		newDistrict->setId(ii);
		newDistrict->setParty(partyTargetAssignment);

		//give district a random starting precinct

		int randomIndex;
		Precinct* randomPrecinct;
		int majorPartyIndex;
		map<string,int>::iterator iter;

		do
		{
			randomIndex = rand() % (int)precinctMap.size();
			randomPrecinct = g_Precincts[randomIndex];
			majorPartyIndex = randomPrecinct->getMajorPartyIndex();
			iter = precinctMap.find(randomPrecinct->getId());
		}while(majorPartyIndex != partyTargetAssignment || iter == precinctMap.end());

		newDistrict->m_precincts.push_back(randomPrecinct);

		arrayOfIter[ii] = newDistrict->m_precincts.begin();
		arrayOfNeighborIter[ii] = (randomPrecinct->m_neighbors).begin(); //Will handle empty neighbors in main loop

		//TODO double check if doing insertion right and calling manageEdges() appropriately
		newDistrict->m_edgePrecincts.push_back(randomPrecinct);
		newDistrict->manageEdges();



		//Remove precinct from map, indicating it has already been added to a district
		precinctMap.erase(iter);

		g_Districts.push_back(newDistrict);

		for(int jj = (partyTargetAssignment + 1) % 5 ;; jj++) //TODO again, change from hard coding 5
		{
			if(activeParties[jj])
			{
				partyTargetAssignment = jj;
				break;
			}
		}

	}



	//********** Add Precincts to Districts ********
	//REMINDER: After precinct added to district, run district.manageEdges() function
	// Using algorithm_practice.cpp as guidance for what to do for now...
	
	for(int ii=0; precinctMap.size()>0; (ii++)%numDistricts)
	{

		bool precinctAdded = false;
		/*Each district will go through its neighbors to
		find one to add */
		vector<Precinct*>::iterator neighborIter = arrayOfNeighborIter[ii];
		vector<Precinct*>::iterator addedPrecinctIter = arrayOfIter[ii];

		for(; neighborIter != (*addedPrecinctIter)->m_neighbors.end(); neighborIter++)
		{

			/*If the neighboring district, really the precinct,
			is of the same party, then add it and continue */

			//Check if the neighbor is same party as already listed precinct, and is not already taken (missing from map)
			if ((*neighborIter)->getMajorPartyIndex() == (*addedPrecinctIter)->getMajorPartyIndex()
					&& precinctMap.find((*neighborIter)->getId()) != precinctMap.end())
			{

				g_Districts[ii]->m_precincts.push_back(*neighborIter);
				g_Districts[ii]->m_edgePrecincts.push_back(*neighborIter);
				g_Districts[ii]->manageEdges();
				precinctMap.erase((*neighborIter)->getId());

				precinctAdded = true;
				break;

			}


		}
		if(!precinctAdded)
		{
			//end of a neighbor list, try next added precinct list of neighbors
			if((arrayOfIter[ii]++) != g_Districts[ii]->m_precincts.end())
			{
				//arrayOfIter[ii]++;
				arrayOfNeighborIter[ii] = (*arrayOfIter[ii])->m_neighbors.begin();
				ii--; //Reset for loop counter, redo adding a precinct to same District
			}
			else //End of all possible neighbors, try random picking of a precinct
			{

				//Note: Possible edge case that no more precincts of same party exist, in which case need to only loop a set number of times

				int maxLoop = precinctMap.size();//Use to ensure we won't infinitely loop

				int randomIndex;
				Precinct* randomPrecinct;
				int majorPartyIndex;
				map<string,int>::iterator iter;

				do
				{
					randomIndex = rand() % (int)precinctMap.size();
					randomPrecinct = g_Precincts[randomIndex];
					majorPartyIndex = randomPrecinct->getMajorPartyIndex();
					iter = precinctMap.find(randomPrecinct->getId());

					(majorPartyIndex == partyTargetAssignment) ? precinctAdded = true : precinctAdded = false;

					if(precinctAdded)
						break;

					maxLoop--;
				}while((majorPartyIndex != partyTargetAssignment || iter == precinctMap.end()) && maxLoop!= 0 ); //TODO double check

				if(precinctAdded)
				{
					g_Districts[ii]->m_precincts.push_back(randomPrecinct);
					g_Districts[ii]->m_edgePrecincts.push_back(randomPrecinct);
					g_Districts[ii]->manageEdges();
					precinctMap.erase(randomPrecinct->getId());
				}
				else
				{
					//Edge case, no more precincts of same party left, SKIP?
				}
			}
		}




	}
	
	
	//All precincts should be added, print out
	//DEBUG, delete later
	
	for(int i = 0; i < g_Districts.size(); i++)
	{
		printf("District %i \n", i);
		printf("Party: %s \n", g_Districts[i]->getParty());
		for(int j = 0; j < g_Districts[i]->m_precincts.size(); j++)
		{
			printf("%s \n", g_Districts[i]->m_precincts[j]->getId());
		}

		printf("\n");
	}


	// Free all data
	clearAll();
    return 0;
}
