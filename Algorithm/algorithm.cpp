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
//Contributors: David Ramsey, Phillip Chow, Lukas Mueller

#include <iostream>
#include <fstream>
#include <string>
#include <string.h> //For c-strings
#include <sstream>
#include <map>
#include <stdio.h>
#include <ctime> //Use time to seed a random number
#include <set>
#include <algorithm>

#include "precinct.h"
#include "district.h"

using namespace std;

//*** File locations
const string IN_FILE = "E:/UMBC/CMSC/CMSC 447 - SE/Project/CMSC-447-Project/Parsers/SC_Vote_Parser/sc_parsed_voter_with_VTD.txt";
const string NEIGHBOR_FILE = "E:/UMBC/CMSC/CMSC 447 - SE/Project/CMSC-447-Project/Parsers/SC_Geo_Parser/sc_precinct_neighbors.txt";


//enum {DEMOCRAT, GREEN, LIBERTARIAN, OTHER, REPUBLICAN}; //Maryland
enum {DEMOCRAT, WORKING_FAMILIES, CONSTITUTION, INDEPENDENCE, GREEN, REPUBLICAN, AMERICAN, LIBERTARIAN}; //South Carolina
bool activeParties[5];
bool activePartiesSC[8];

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

int main() {

	srand(time(nullptr));// Create semi-random set of numbers

	string currentLine; //For reading line by line
	char* charArray = new char[2048]; // for when converting string into char* 

	string precinctId; //Holds the Id of a precinct
	int numDistricts; //Tracks the number of districts


//******** Read in Precinct Data ***************************************************


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

	//South Carolina
	activePartiesSC[DEMOCRAT] = atoi(strtok(charArray, " \t\v\r\n\f,()"));
	activePartiesSC[WORKING_FAMILIES] = atoi(strtok(NULL, " \t\v\r\n\f,()"));
	activePartiesSC[CONSTITUTION] = atoi(strtok(NULL, " \t\v\r\n\f,()"));
	activePartiesSC[INDEPENDENCE] = atoi(strtok(NULL, " \t\v\r\n\f,()"));
	activePartiesSC[GREEN] = atoi(strtok(NULL, " \t\v\r\n\f,()"));
	activePartiesSC[REPUBLICAN] = atoi(strtok(NULL, " \t\v\r\n\f,()"));
	activePartiesSC[AMERICAN] = atoi(strtok(NULL, " \t\v\r\n\f,()"));
	activePartiesSC[LIBERTARIAN] = atoi(strtok(NULL, " \t\v\r\n\f,()"));

	/*Maryland
	activeParties[DEMOCRAT] = atoi(strtok(charArray, " \t\v\r\n\f,()"));
	activeParties[GREEN] = atoi(strtok(NULL, " \t\v\r\n\f,()"));
	activeParties[LIBERTARIAN] = atoi(strtok(NULL, " \t\v\r\n\f,()"));
	activeParties[OTHER] = atoi(strtok(NULL, " \t\v\r\n\f,()"));
	activeParties[REPUBLICAN] = atoi(strtok(NULL, " \t\v\r\n\f,()"));*/

	getline(g_inFile, currentLine);

	//Header line, skip over
	getline(g_inFile, currentLine);

	map<string, int> precinctMap; //Tracks the precincts and if they are added
	int precinctCount = 0;

	//Loop to add precincts
	while(getline(g_inFile, currentLine))
	{
		strcpy(charArray, currentLine.c_str());

		//Create new Precinct
		Precinct* readInPrecinct = new Precinct();
		precinctId = strtok(charArray, " \t\v\r\n\f,()");

		readInPrecinct->setId(precinctId);
		readInPrecinct->setTotalPop(atoi(strtok(NULL, " \t\v\r\n\f,()")));

		readInPrecinct->addPartyPercentage(stof(strtok(NULL, " \t\v\r\n\f,()")));
		readInPrecinct->addPartyPercentage(stof(strtok(NULL, " \t\v\r\n\f,()")));
		readInPrecinct->addPartyPercentage(stof(strtok(NULL, " \t\v\r\n\f,()")));
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



//********* Add neighbor precincts  ************************************************


	g_inFile.open(NEIGHBOR_FILE);
	if (!(g_inFile.is_open()))
	{
		printf("ERROR trying to open neighbor file. /n");
		return 1;
	}

	vector<string>nbrFilePrecs;

	//loop through each line in neighbor list
	while(getline(g_inFile, currentLine))
	{
		strcpy(charArray, currentLine.c_str());

		precinctId = strtok(charArray, " :");

		map<string,int>::iterator iter = precinctMap.find(precinctId);

		//precinct is not in the map, so don't add it since we have no data for it
		if(iter == precinctMap.end())
		{
			continue;
		}
		else
		{

			nbrFilePrecs.push_back(precinctId);

			int index = iter->second;
			Precinct* targetPrecinct = g_Precincts[index]; //Precinct to find neighbors for
			char* precinctIdChar = strtok(NULL, " ,'\n");

			//Go through each available neighbor
			while(precinctIdChar != NULL)
			{
				precinctId = precinctIdChar;
				iter = precinctMap.find(precinctId);

				if(iter == precinctMap.end())
				{
					precinctIdChar = strtok(NULL, " ,'\n");
					continue;
				}

				int index = iter->second;
				Precinct* targetNeighborPrecinct = g_Precincts[index];
				targetPrecinct->m_neighbors.push_back(targetNeighborPrecinct);
				
				precinctIdChar = strtok(NULL, " ,'\n"); //read in next precinct(if available)
			}
		}
		
	}

	g_inFile.close();

	//remove any precincts in precinctMap and g_Precincts that were not in the neighbor file
	map<string, int>::iterator it = precinctMap.begin();
	//loop through all precincts in the map
	while (it != precinctMap.end())
	{
		string id = it -> first;
		
		bool found = false;

		//compare to all precincts from the neighbor file
		for (string prec : nbrFilePrecs)
		{
			//if found, all good
			if (prec == id)
			{
				found = true;
				it++;
				break;
			}
		}
		//if not found, remove it from precinctMap and g_Precincts since we won't be able to map it
		if (!found)
		{

			//map<string, int>::iterator temp = it;
			it = precinctMap.erase(it);

			//remove from g_Precincts
			for (Precinct* prec : g_Precincts)
			{
				if (prec -> getId() == it -> first)
				{
					g_Precincts.erase(remove(g_Precincts.begin(), g_Precincts.end(), prec), g_Precincts.end());
				}
			}

		}
	}


//**********   Setup Districts   *****************************************


	int arrayOfIndex[numDistricts]; //Each district will iterate this index through it's list of precincts
	int partyTargetAssignment; //Tracks what party to assign district
	
	for(int jj = DEMOCRAT; jj < numDistricts; jj++)
	{
		if(activePartiesSC[jj])
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

	for(int jj = (partyTargetAssignment + 1) % numDistricts ;; jj = (jj + 1) % numDistricts)
	{
		if(activePartiesSC[jj])
		{
			partyTargetAssignment = jj;
			break;
		}
	}

	
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
		bool foundPrecinct;
		do
		{
			foundPrecinct = false;
			int count = 0; //used to check whether the random precinct's county is not
						   //shared by a precinct already being used by another district
						   //and has at least 1 neighbor
			randomIndex = rand() % (int)precinctMap.size();
			randomPrecinct = g_Precincts[randomIndex];
			for (int prec = 0; prec < initPrecs.size(); prec++)
			{
				if (initPrecs[prec].substr(0, 5) != (randomPrecinct -> getId()).substr(0, 5) && (randomPrecinct ->m_neighbors).size() >= 10)
				{
					count++;
				}

				if (count == initPrecs.size())
				{
					majorPartyIndex = randomPrecinct->getMajorPartyIndex();
					iter = precinctMap.find(randomPrecinct->getId());
					initPrecs.push_back(randomPrecinct -> getId());
					foundPrecinct = true;
					break;
				}
			}
			
			
		}while(!foundPrecinct);

		newDistrict->m_precincts.push_back(randomPrecinct);

		arrayOfIndex[ii] = 0;

		//Remove precinct from map, indicating it has already been added to a district
		iter = precinctMap.erase(iter);

		g_Districts.push_back(newDistrict);

		for(int jj = (partyTargetAssignment + 1) % numDistricts ;; jj = (jj+1) %numDistricts)
		{
			if(activePartiesSC[jj])
			{
				partyTargetAssignment = jj;
				break;
			}
		}

	}
	

	//print out each district and its first precinct
	for(int district = 0; district < g_Districts.size(); district++)
	{	cout << "District " << g_Districts[district] -> getId() << "(" << g_Districts[district] -> getParty() << ")" << ": ";
		for(int precinct = 0; precinct < (g_Districts[district] -> m_precincts).size(); precinct++)
		{
			cout << "'" << g_Districts[district] -> m_precincts[precinct] -> getId() << "', ";
		}
	}
	cout << endl;

//********** Add Precincts to Districts **************************************


	int count = 0;
	int size = precinctMap.size();

	while (count < size - 500)
	{
		for(int ii=0 ; ii < numDistricts ; ii++)
		{

			bool precinctAdded = false;

			Precinct* rootPrecinct = g_Districts[ii]->m_precincts[arrayOfIndex[ii]];

			vector<Precinct*>::iterator anIt = rootPrecinct->m_neighbors.begin();

			for(int i = 0; i < rootPrecinct->m_neighbors.size(); i++, anIt++)
			{
				Precinct* neighbor = rootPrecinct->m_neighbors[i];

				if (neighbor->getMajorPartyIndex() == rootPrecinct->getMajorPartyIndex()
									&& precinctMap.find(neighbor->getId()) != precinctMap.end())
				{
					g_Districts[ii]->m_precincts.push_back(neighbor);
					//maxLoop--;
					count++;
					precinctMap.erase(neighbor->getId());

					precinctAdded = true;
					break;
				}
				//if the neighbor is not necessarily the same party but is still available
				else if (precinctMap.find(neighbor -> getId()) != precinctMap.end())
				{
					//check that adding it won't affect overall party of the district
					//need district's current democrat + republican populations
					int demPop = 0;
					int repubPop = 0;

					//get the current party populations for the current district
					for (int m = 0; m < g_Districts[ii] -> m_precincts.size(); m++)
					{
						demPop += g_Districts[ii] -> m_precincts[m] -> getPartyPop(DEMOCRAT);
						repubPop += g_Districts[ii] -> m_precincts[m] -> getPartyPop(REPUBLICAN);
					}

					//check whether adding the neighbor will change the majority population of the district
					//if the current district is republican
					int nbrPop = 0;
					if (g_Districts[ii] -> getParty() == DEMOCRAT)
					{
						nbrPop += neighbor -> getPartyPop(REPUBLICAN);
						//if adding so many republicans does not change the district's majority, add the neighbor
						if (repubPop + nbrPop < demPop)
						{
							g_Districts[ii] -> m_precincts.push_back(neighbor);
							//maxLoop--;
							count++;
							precinctMap.erase(neighbor -> getId());
							precinctAdded = true;
							break;
						}
						//if it does change the district's majority, don't add it
						else
						{
							continue;
						}
						
					}
					else if (g_Districts[ii] -> getParty() == REPUBLICAN)
					{
						nbrPop += neighbor -> getPartyPop(DEMOCRAT);
						if (demPop + nbrPop < repubPop)
						{
							g_Districts[ii] -> m_precincts.push_back(neighbor);
							//maxLoop--;
							count++;
							precinctMap.erase(neighbor -> getId());
							precinctAdded = true;
							break;
						}
						else
						{
							continue;
						}
						
					}
					
				}
			}
			
			if(!precinctAdded)
			{
				if(arrayOfIndex[ii]+1 != g_Districts[ii]->m_precincts.size())
				{
					arrayOfIndex[ii]++;
				}

				else //End of all possible neighbors
				{
					continue;	
				}
			}
		}
	}

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
	}

	// Free all data
	clearAll();
    return 0;
}
