// prescinct.h - contains the Prescinct class and methods
//	- Dependent files: 
//		None
//
// Original Coder: David Ramsey
// Most Recent Change: 15 April 2020
//		- Initial creation
//


#ifndef PRECINCT_H
#define PRECINCT_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Precinct {
private:
	int	m_id;						// precinct's id or index
	int m_totalPop;					// the total population
	vector<double> m_partyPercents; // percentage of party affiliation, one entry for each party
	vector<Precinct*> m_neighbors;	// list of pointers to all adjacent precincts

public:
	// Construct - Destruct
	Precinct();
	~Precinct();

	// Setters
	void setId(int newId);
	void setTotalPop(int newTotalPop);

	// Getters
	int getId();
	int getTotalPop();

	// Print
	void print();

	// Helpers
	int getPartyPop(int partyIndex);

};

/**** Construct - Destruct ****/
Precinct::Precinct()
{
	setId(0);
	setTotalPop(0);
}

Precinct::~Precinct()
{	
}

/**** Setters ****/
void Precinct::setId(int newId)
{
	m_id = newId;
}

void Precinct::setTotalPop(int newTotalPop)
{
	m_totalPop = newTotalPop;
}

/***** Getters *****/
int Precinct::getId()
{
	return m_id;
}

int Precinct::getTotalPop()
{
	return m_totalPop;
}

/**** Print ****/
// print(): prints all information about precinct
void Precinct::print()
{

}

/**** Helpers ****/
// getPartyPop(): gets the population of a given party
// - Input: int partyIndex, the index of the specified party
// - Return: int, the population of the given party within the precinct
int Precinct::getPartyPop(int partyIndex)
{
	return (int)(m_partyPercents[partyIndex] * (double)m_totalPop);
}

#endif
