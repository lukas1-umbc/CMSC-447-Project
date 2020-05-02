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


	int debug = 0; // DELETE LaTER



	//loop through each line in neighbor list
	while(getline(g_inFile, currentLine))
	{
		//printf("%i \n ", debug);
		//charArray[0] = '\0';

		strcpy(charArray, currentLine.c_str());
		//printf("%s \n", charArray);

		precinctId = strtok(charArray, " :");
		map<string,int>::iterator iter = precinctMap.find(precinctId);

		if(iter == precinctMap.end())
		{
			//Somehow this precinct isn't listed, print error for now
			printf("\n ERROR! Precinct with id(%s) does not already exist! \n", precinctId.c_str());

			debug++;

			continue;
		}

		int index = iter->second;
		Precinct* targetPrecinct = g_Precincts[index]; //The precinct we want to add neighbors to


		char* precinctIdChar = strtok(NULL, " ,'\n");
		//precinctId = strtok(NULL, " ,'\n");

		int debug2 = 0;

		while(precinctIdChar != NULL && debug2 != 15)
		{
			debug2++;

			precinctId = precinctIdChar;
			iter = precinctMap.find(precinctId);

			if(iter == precinctMap.end())
			{
				//Somehow this precinct isn't listed, print error for now
				printf("\n ERROR at %i! Precinct with id(%s) does not already exist! Cannot add as neighbor \n",debug, precinctId.c_str());

				precinctIdChar = strtok(NULL, " ,'\n");

				continue;
			}

			int index = iter->second;
			Precinct* targetNeighborPrecinct = g_Precincts[index];

			targetPrecinct->m_neighbors.push_back(targetNeighborPrecinct);

			precinctIdChar = strtok(NULL, " ,'\n");
		}
		debug++;
	}


	for(int i = 0; i < g_Precincts.size(); i++)
	{
		if(g_Precincts[i]->m_neighbors.size() != 0)
		{
			//g_Precincts[i]->print();
			printf("----- Neighbors: ");
			for(int jj = 0; jj < g_Precincts[i]->m_neighbors.size(); jj++)
			{
				printf("%s, ",g_Precincts[i]->m_neighbors[jj]->getId().c_str());
			}
			printf("\n _____________________________\n");
		}

	}

	printf("\n\n\n\n .............");
	g_inFile.close();






	//**********   Setup Districts   **************

	printf("\n\n\n\n .............");

	//Setup an array of iterators for both precincts owned, and for neighbors, for each district
	//vector<Precinct*>::iterator arrayOfIter[numDistricts];
	//vector<Precinct*>::iterator arrayOfNeighborIter[numDistricts];

	int arrayOfIndex[numDistricts];

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

		printf("!!!!  %s: \n", randomPrecinct->getId().c_str());

		newDistrict->m_precincts.push_back(randomPrecinct);

		//arrayOfIter[ii] = newDistrict->m_precincts.begin();
		//arrayOfNeighborIter[ii] = (randomPrecinct->m_neighbors).begin(); //Will handle empty neighbors in main loop
		arrayOfIndex[ii] = 0;

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



	for(int i = 0; i < g_Districts.size(); i++)
	{
		printf("District %i \n", i);
		printf("Party: %i \n", g_Districts[i]->getParty());

		Precinct* aPrecinct = g_Districts[i]->m_precincts[0];
			printf("%s: ", aPrecinct->getId().c_str());

			for(int ii = 0; ii < ((aPrecinct)->m_neighbors).size(); ii++)
			{


						printf("%s , ",(aPrecinct->m_neighbors[ii])->getId().c_str());


			}
			printf("\n _____________________________\n");



	}


#if 1
	//********** Add Precincts to Districts ********
	//REMINDER: After precinct added to district, run district.manageEdges() function
	// Using algorithm_practice.cpp as guidance for what to do for now...
	
	for(int ii=0, maxLoop = precinctMap.size() ; precinctMap.size()>0 && maxLoop != 0; ii= (ii+1)%numDistricts, maxLoop--)
	{
		printf("\n \n *****District %i : \n", ii);

		bool precinctAdded = false;
		/*Each district will go through its neighbors to
		find one to add */
		//vector<Precinct*>::iterator neighborIter = arrayOfNeighborIter[ii];
		//vector<Precinct*>::iterator addedPrecinctIter = arrayOfIter[ii];

		Precinct* rootPrecinct = g_Districts[ii]->m_precincts[arrayOfIndex[ii]];


/*
		printf("Main: %s, Number of neighbors: %i \n", (*addedPrecinctIter)->getId().c_str() ,(*addedPrecinctIter)->m_neighbors.size());

		if((*addedPrecinctIter)->m_neighbors.size() != 0)
		{
			for(int ij =0; ij != (*addedPrecinctIter)->m_neighbors.size(); ij++)
			{
				printf("%s, ", (*addedPrecinctIter)->m_neighbors[ij]->getId().c_str());
			}
			printf("\n");
		}

		for(; neighborIter != (*addedPrecinctIter)->m_neighbors.end(); neighborIter++)
*/
		vector<Precinct*>::iterator anIt = rootPrecinct->m_neighbors.begin();
		for(int i = 0; i < rootPrecinct->m_neighbors.size(); i++, anIt++)
		{
			Precinct* neighbor = rootPrecinct->m_neighbors[i];

			//printf("Current Neighbor: %s \n", (*neighborIter)->getId().c_str());
			printf("Current Neighbor: %s \n", neighbor->getId().c_str());

			/*If the neighboring district, really the precinct,
			is of the same party, then add it and continue */

			//Check if the neighbor is same party as already listed precinct, and is not already taken (missing from map)
//			if ((*neighborIter)->getMajorPartyIndex() == (*addedPrecinctIter)->getMajorPartyIndex()
//					&& precinctMap.find((*neighborIter)->getId()) != precinctMap.end())
//			{
			if (neighbor->getMajorPartyIndex() == rootPrecinct->getMajorPartyIndex()
								&& precinctMap.find(neighbor->getId()) != precinctMap.end())
			{

//				g_Districts[ii]->m_precincts.push_back(*neighborIter);
//				g_Districts[ii]->m_edgePrecincts.push_back(*neighborIter);
//				//g_Districts[ii]->manageEdges();
//				precinctMap.erase((*neighborIter)->getId());
//
//				neighborIter++;
//				arrayOfNeighborIter[ii] = neighborIter;
//				precinctAdded = true;
//				break;

				g_Districts[ii]->m_precincts.push_back(neighbor);
				g_Districts[ii]->m_edgePrecincts.push_back(neighbor);
				//g_Districts[ii]->manageEdges();
				precinctMap.erase(neighbor->getId());

				rootPrecinct->m_neighbors.erase(rootPrecinct->m_neighbors.begin(), anIt);


				precinctAdded = true;
				break;

			}
			//Try this for optimizing
//			else
//			{
//				vector<Precinct*>::iterator anIt = rootPrecinct->m_neighbors.begin()+i;
//				rootPrecinct->m_neighbors.erase(anIt);
//				i--; //since we removed an element, need to adjust loop counter
//			}

		}
		if(!precinctAdded)
		{

			//end of a neighbor list, try next added precinct list of neighbors
//			if(next(arrayOfIter[ii],1) != g_Districts[ii]->m_precincts.end()  && arrayOfIter[ii]!= g_Districts[ii]->m_precincts.end())
//			{
//				arrayOfIter[ii]++;
//				arrayOfNeighborIter[ii] = (*arrayOfIter[ii])->m_neighbors.begin();
//				ii--; //Reset for loop counter, redo adding a precinct to same District
//			}
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

//					arrayOfIter[ii]++; // = newDistrict->m_precincts.begin();
//					arrayOfNeighborIter[ii] = (randomPrecinct->m_neighbors).begin();

					arrayOfIndex[ii]++; // = newDistrict->m_precincts.begin();

				}
				else
				{
					//Edge case, no more precincts of same party left, SKIP?
				}
			}
		}




	}
#endif

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



//
//
//		//Optional debug can be outputted to tester
//		if(1)
//		{
//		  cout << "District " << g_Districts[i]->getId() << endl;
//		  cout << "Party " << g_Districts[i] -> getParty() << endl;
//		}
//
//		//First we check if the output file already exists
//		ifstream algOutputTest("algOutput.txt");
//		int firstLine = 0;
//		if(!algOutputTest.is_open()) firstLine = 1;
//		algOutputTest.close();
//
//		//Opens the output file in append mode
//		ofstream algOutput;
//		algOutput.open("algOutput.txt", ios::app);
//
//		//Testing variables for optional debug info
//		int districtPop = 0;
//		int partyPop = 0;
//
//		//First line describes the format of the file
//		if(firstLine) algOutput << "DistrictNum,DistrictParty,PrecinctID,...\n";
//
//		//Adding initial info
//		algOutput << g_Districts[i]->getId() << "," << g_Districts[i]->getParty() << ",";
//
//		//Printing out the contained precincts
//		for(int i=0; i<g_Districts[i]->m_precincts.size(); i++)
//		{
//			algOutput << g_Districts[i]->m_precincts[i]->getId().c_str() << ",";  ////!!! converted to c-string
//
//		//Optionally, we want to save the total population and party population for testing
//			if(1)
//			{
//			districtPop += g_Districts[i]->m_precincts[i]->getTotalPop();
//			partyPop += g_Districts[i]->m_precincts[i]->getPartyPop(g_Districts[i]->checkMajorityParty());
//		}
//		}
//
//		algOutput << "\n";
//
//		//Print out the majority party percentage and check with what is stored in the district
//		if(1)
//		{
//			double percentage = partyPop/districtPop;
//		cout << "Majority percentage: " << percentage << endl;
//		cout << "All party percentages: ";
//
//
//		//!!!partStats neve made
////		for(int i=0;i<g_Districts[i]->m_partyStats.size();i++)
////		{
////		  cout << g_Districts[i]->m_partyStats[i] << ", ";
////		}
//		cout << "\n";
//		}
//
//		algOutput.close();
//
//
//
	}


	// Free all data
	clearAll();
    return 0;
}
