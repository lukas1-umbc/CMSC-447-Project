// district.h: - Contains the District class and it's methods.
//	- Dependent files: 
//		- prescinct.h - contains the Prescinct class and methods
//
// Original Coder: David Ramsey
// Most Recent Change: 15 April 2020
//		- Initial creation
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
	int m_id;							// district's id or index
	int m_party;						// district's target party affiliation
	vector<int> m_partyStats;			// stat for each party, used to determine district's current party majority, not sure if this would hold #of precincts or populations for each party
	vector<Precinct*> m_precincts;		// list of pointers to district's precincts
	vector<Precinct*> m_edgePrecincts;	// member precinct's that are at the district's border, semi-redundant to m_precincts

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

#endif
