// district.h: - Contains the District class and it's methods.
//	- Dependent files: 
//		- prescinct.h - contains the Prescinct class and methods
//
// Original Coder: David Ramsey
// Most Recent Change: 1 May 2020
//		- Implemented print() function.
//              - Changed 0 to be the default for the parameter
//

#ifndef DISTRICT_H
#define DISTRICT_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "precinct.h"

using namespace std;

class District {
private:
    int m_id;		                  // district's id or index
    int m_party;	                  // district's target party affiliation
    vector<int> m_partyStats;             // stat for each party, used to determine district's current party majority, not sure if this would hold #of precincts or populations for each party
    //vector<Precinct*> m_precincts;	   // list of pointers to district's precincts
    //vector<Precinct*> m_edgePrecincts;   // member precinct's that are at the district's border, semi-redundant to m_precincts

public:
    // Construct - Destruct
    District();
    ~District();

    // Setters
    void setId(int newId);
    void setParty(int newParty);

    // Getters
    int getId();
    int getParty();

    // Print
    void print(int debug);

    // Helpers
    int checkMajorityParty();
    void manageEdges();
    vector<Precinct*> m_precincts;          // list of pointers to district's precincts
    vector<Precinct*> m_edgePrecincts;      // member precinct's that are at the district's border, semi-redundant to m_precincts


};

/**** Construct - Destruct ****/
District::District()
{
    setId(0);
    setParty(0);
}

District::~District()
{
}

// Setters
void District::setId(int newId)
{
    m_id = newId;
}

void District::setParty(int newParty)
{
    m_party = newParty;
}

// Getters
int District::getId()
{
    return m_id;
}

int District::getParty()
{
    return m_party;
}

// Print
// print(): prints all information about district
//       - return: File containing the District information
//       - input:  If there is no input given, or the parameter is 0, only the file will be produced.
//                 Otherwise, some debug info will be displayed.
void District::print(int debug=0)
{

    //Optional debug can be outputted to tester
    if(debug)
    {
      cout << "District " << m_id << endl;
      cout << "Party " << m_party << endl;
    }

    //First we check if the output file already exists
    ifstream algOutputTest("algOutput.txt");
    int firstLine = 0;
    if(!algOutputTest.is_open()) firstLine = 1;
    algOutputTest.close();
    
    //Opens the output file in append mode
    ofstream algOutput;
    algOutput.open("algOutput.txt", ios::app);

    //Testing variables for optional debug info
    int districtPop = 0;
    int partyPop = 0;

    //First line describes the format of the file
    if(firstLine) algOutput << "DistrictNum,DistrictParty,PrecinctID,...\n";
    
    //Adding initial info
    algOutput << m_id << "," << m_party << ",";

    //Printing out the contained precincts
    for(int i=0; i<m_precincts.size(); i++)
    {
        algOutput << "'" << m_precincts[i]->getId() <<"'" << ",";

	//Optionally, we want to save the total population and party population for testing
        if(debug)
        {
	    districtPop += m_precincts[i]->getTotalPop();
	    partyPop += m_precincts[i]->getPartyPop(checkMajorityParty());
	}
    }

    algOutput << "\n \n";

    //Print out the majority party percentage and check with what is stored in the district
    if(debug)
    {
        double percentage = partyPop/districtPop;
	cout << "Majority percentage: " << percentage << endl;
	cout << "All party percentages: ";

	for(int i=0;i<m_partyStats.size();i++)
	{
	  cout << m_partyStats[i] << ", "; 
	}
	cout << "\n";
    }
    
    algOutput.close();
    
}

// Helpers
// checkMajorityParty(): finds the majority party within the district
// - return: int, index of the majority party for the district
int District::checkMajorityParty()
{
    int majority = -1;
    int highest = -1;

    // Find majority party, party with the highest stat
    for(int i = 0; i < (int)m_partyStats.size(); i++)
    {
        if(m_partyStats[i] > highest)
	{
	    highest = m_partyStats[i];
	    majority = i;
	}
    }

    // Return index of majority party
    return majority;
}

//manageEdges(): Manages the precincts located at the edge of the district
// - return: Updated list of edge precincts (m_edgePrecincts)
// - assumptions: This function assumes that a new precinct has already been
//                been added to m_precincts and m_edgePrecincts.
void District::manageEdges()
{
    //Iterate though all edge precincts
    for(int i=0; i< (int) m_edgePrecincts.size();i++)
    {
        //Iterate through the neighbors of each edge precinct
        for(int j=0; j< (int) m_edgePrecincts[i]->m_neighbors.size(); j++)
        {

            //Check which neighbors are inside of the current district
            for(int k=0; k< (int) m_precincts.size(); k++)
	        {
	            //Check if the neighbor is already in the current district
	            if(m_precincts[k]->getId() == m_edgePrecincts[i]->m_neighbors[j]->getId())
	            {
		  
		            //Remove the naighbor
		            m_edgePrecincts[i]->m_neighbors.erase(m_edgePrecincts[i]->m_neighbors.begin()+j);
		            j--;
		            break;
		        }     
	        }
	}

	//If the edge precinct has an empty neighbors list
	if(m_edgePrecincts[i]->m_neighbors.empty())
	{
	    //cout << "Removed precinct " << m_edgePrecincts[i]->getId() << endl;
	    m_edgePrecincts.erase(m_edgePrecincts.begin()+i);
	    i--;
	}
    }
}

#endif
