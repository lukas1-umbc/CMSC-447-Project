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
const string IN_FILE = "E:/UMBC/CMSC/CMSC 447 - SE/Project/CMSC-447-Project/Parsers/MD_Vote_Parser/md_parsed_data.txt";
const string NEIGHBOR_FILE = "E:/UMBC/CMSC/CMSC 447 - SE/Project/CMSC-447-Project/Parsers/MD_GEO_Parser/md_precinct_neighbors.txt";


enum {DEMOCRAT, GREEN, LIBERTARIAN, OTHER, REPUBLICAN};
bool activeParties[5];

//*********



/**** CONSTANTS ****/
const string OUT_FILE_NAME = "E:/UMBC/CMSC/CMSC 447 - SE/Project/CMSC-447-Project/Algorithm/newDistricts.txt";



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
	char* charArray = new char[2048]; // for converting string line into char* (update, bumped up character count, maybe fix issues?)

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

	//loop through each line in neighbor list
	while(getline(g_inFile, currentLine))
	{

		strcpy(charArray, currentLine.c_str());

		precinctId = strtok(charArray, " :");
		map<string,int>::iterator iter = precinctMap.find(precinctId);

		if(iter == precinctMap.end())
		{
			//Somehow this precinct isn't listed, print error for now
			//printf("\n ERROR! Precinct with id(%s) does not already exist! \n", precinctId.c_str());

			continue;
		}

		int index = iter->second;
		Precinct* targetPrecinct = g_Precincts[index]; //The precinct we want to add neighbors to

		char* precinctIdChar = strtok(NULL, " ,'\n");

		//look at each neighbor for the current precinct
		while(precinctIdChar != NULL)
		{

			precinctId = precinctIdChar;
			iter = precinctMap.find(precinctId);

			if(iter == precinctMap.end())
			{
				//Somehow this precinct isn't listed, print error for now
				//printf("\n ERROR at %i! Precinct with id(%s) does not already exist! Cannot add as neighbor \n",debug, precinctId.c_str());

				precinctIdChar = strtok(NULL, " ,'\n");

				continue;
			}

			int index = iter->second;
			Precinct* targetNeighborPrecinct = g_Precincts[index];

			targetPrecinct->m_neighbors.push_back(targetNeighborPrecinct);

			precinctIdChar = strtok(NULL, " ,'\n");
		}
		
	}

	//print out each precinct and its neighbors
	/*
	for(int i = 0; i < g_Precincts.size(); i++)
	{
		if(g_Precincts[i]->m_neighbors.size() != 0)
		{
			printf("----- Neighbors: ");
			for(int jj = 0; jj < g_Precincts[i]->m_neighbors.size(); jj++)
			{
				printf("%s, ",g_Precincts[i]->m_neighbors[jj]->getId().c_str());
			}
			printf("\n _____________________________\n");
		}
	}

	printf("\n\n\n\n .............");*/
	g_inFile.close();

	//**********   Setup Districts   **************

	//printf("\n\n\n\n .............");

	//Setup an array of iterators for both precincts owned, and for neighbors, for each district
	//vector<Precinct*>::iterator arrayOfIter[numDistricts];
	//vector<Precinct*>::iterator arrayOfNeighborIter[numDistricts];

	int arrayOfIndex[7];

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

	vector <string> initPrecs = {};

	//create first district
	District * newDistrict = new District();
	newDistrict -> setId(0);
	newDistrict -> setParty(partyTargetAssignment);
	int randomIndex = rand() % (int)precinctMap.size();
	Precinct * randomPrecinct = g_Precincts[randomIndex];
	int majorPartyIndex = randomPrecinct -> getMajorPartyIndex();
	map<string,int>::iterator iter = precinctMap.find(randomPrecinct -> getId());
	newDistrict -> m_precincts.push_back(randomPrecinct);
	arrayOfIndex[0] = 0;
	precinctMap.erase(iter);
	g_Districts.push_back(newDistrict);
	initPrecs.push_back(randomPrecinct -> getId());

	//create other districts
	for(int ii = 1; ii < numDistricts; ii++)
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
			int count = 0; //used to check whether the random precinct's county is not shared by a precinct already being used by another district
			randomIndex = rand() % (int)precinctMap.size();
			randomPrecinct = g_Precincts[randomIndex];
			for (int prec = 0; prec < initPrecs.size(); prec++)
			{
				if (initPrecs[prec].substr(0, 5) != (randomPrecinct -> getId()).substr(0, 5))
				{
					count++;
				}

				if (count == initPrecs.size())
				{
					majorPartyIndex = randomPrecinct->getMajorPartyIndex();
					iter = precinctMap.find(randomPrecinct->getId());
					initPrecs.push_back(randomPrecinct -> getId());
				}
			}
			
			
		}while(majorPartyIndex != partyTargetAssignment || iter == precinctMap.end());

		//printf("!!!!  %s: \n", randomPrecinct->getId().c_str());

		newDistrict->m_precincts.push_back(randomPrecinct);

		//arrayOfIter[ii] = newDistrict->m_precincts.begin();
		//arrayOfNeighborIter[ii] = (randomPrecinct->m_neighbors).begin(); //Will handle empty neighbors in main loop
		arrayOfIndex[ii] = 0;

		//TODO double check if doing insertion right and calling manageEdges() appropriately
		//newDistrict->m_edgePrecincts.push_back(randomPrecinct);
		//newDistrict->manageEdges();

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
	

	for(int district = 0; district < g_Districts.size(); district++)
	{
		cout << "District " << district + 1 << "(" << g_Districts[district] -> getParty() << ")" << ": ";
		for(int precinct = 0; precinct < (g_Districts[district] -> m_precincts).size(); precinct++)
		{
			cout << g_Districts[district] -> m_precincts[precinct] -> getId() << ", ";
		}
		cout << endl;
	}

	//********** Add Precincts to Districts ********
	//REMINDER: After precinct added to district, run district.manageEdges() function
	// Using algorithm_practice.cpp as guidance for what to do for now...
	/*
	for(int ii=0, maxLoop = precinctMap.size() ; precinctMap.size()>0 && maxLoop != 0; ii= (ii+1)%numDistricts, maxLoop--)
	{
		printf("\n \n *****District %i : \n", ii);

		bool precinctAdded = false;

		Precinct* rootPrecinct = g_Districts[ii]->m_precincts[arrayOfIndex[ii]];

		vector<Precinct*>::iterator anIt = rootPrecinct->m_neighbors.begin();

		for(int i = 0; i < rootPrecinct->m_neighbors.size(); i++, anIt++)
		{
			Precinct* neighbor = rootPrecinct->m_neighbors[i];

			printf("Current Neighbor: %s \n", neighbor->getId().c_str());

			if (neighbor->getMajorPartyIndex() == rootPrecinct->getMajorPartyIndex()
								&& precinctMap.find(neighbor->getId()) != precinctMap.end())
			{
				g_Districts[ii]->m_precincts.push_back(neighbor);
				g_Districts[ii]->m_edgePrecincts.push_back(neighbor);
				//g_Districts[ii]->manageEdges();
				precinctMap.erase(neighbor->getId());

				rootPrecinct->m_neighbors.erase(rootPrecinct->m_neighbors.begin(), anIt);

				precinctAdded = true;
				break;
			}
		}
		
		if(!precinctAdded)
		{
			if(arrayOfIndex[ii]+1 != g_Districts[ii]->m_precincts.size())
			{
				arrayOfIndex[ii]++;
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

					(majorPartyIndex == g_Districts[ii]->getParty()) ? precinctAdded = true : precinctAdded = false;

					if(precinctAdded && iter != precinctMap.end())
						break;

					precinctAdded = false;
					maxLoop--;
					
				}while( maxLoop!= 0 );

				if(precinctAdded)
				{
					g_Districts[ii]->m_precincts.push_back(randomPrecinct);
					g_Districts[ii]->m_edgePrecincts.push_back(randomPrecinct);
					//g_Districts[ii]->manageEdges();
					precinctMap.erase(randomPrecinct->getId());

					arrayOfIndex[ii]++; // = newDistrict->m_precincts.begin();

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
	
	printf("\n\n -----------------------------\n\n");

	for(int i = 0; i < g_Districts.size(); i++)
	{
		printf("District %i \n", i);
		printf("Party: %i \n", g_Districts[i]->getParty());


		for(int j = 0; j < g_Districts[i]->m_precincts.size(); j++)
		{
			printf("%s, ", g_Districts[i]->m_precincts[j]->getId().c_str());
		}

		printf("\n");

		g_Districts[i]->print();
	}*/


	// Free all data
	clearAll();
    return 0;
}
