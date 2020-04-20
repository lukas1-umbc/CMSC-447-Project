// district.h: - Contains the District class and it's methods.
//	- Dependent files: 
//		- prescinct.h - contains the Prescinct class and methods
//
// Original Coder: David Ramsey
// Most Recent Change: 20 April 2020
//		- Implemented manageEdges() function.
//              - Moved m_precincts, and m_edge precincts to be public members
//


#ifndef DISTRICT_H
#define DISTRICT_H

#include <iostream>
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
    void print();

    // Helpers
    int checkMajorityParty();
    void manageEdges();
    vector<Precinct*> m_precincts;          // list of pointers to district's precincts
    vector<Precinct*> m_edgePrecincts;      // member precinct's that are at the district's border \
, semi-redundant to m_precincts


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
void District::print()
{
	
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
    for(int i=0; i<m_edgePrecincts.size();i++)
    {
        //Iterate through the neighbors of each edge precinct
        for(int j=0; j<m_edgePrecincts[i]->m_neighbors.size(); j++)
        {

            //Check which neighbors are inside of the current district
            for(int k=0; k<m_precincts.size(); k++)
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
