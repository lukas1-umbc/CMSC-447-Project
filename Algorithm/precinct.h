// prescinct.h - contains the Prescinct class and methods
//	- Dependent files: 
//		None
//
// Original Coder: David Ramsey
// Most Recent Change:
//		29 April 2020
//		- Added integer to track party majority index
//
//		20 April 2020
//		- Moved m_neighbors to be a public variable
//


#ifndef PRECINCT_H
#define PRECINCT_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Precinct {
private:
    string m_id;                    // precinct's id or index
    int m_totalPop;	            // the total population
    vector<double> m_partyPercents; // percentage of party affiliation, one entry for each party
    int m_majorPartyIndex;

public:
    // Construct - Destruct
    Precinct();
    ~Precinct();

    // Setters
    void setId(string newId);
    void setTotalPop(int newTotalPop);
    void setMajorPartyIndex(int newIndex);

    // Getters
    string getId();
    int getTotalPop();
    int getMajorPartyIndex();

    // Print
    void print();

    // Helpers
    int getPartyPop(int partyIndex);
    void addPartyPercentage(double percentage);
    void updateMajorParty();
    vector<Precinct*> m_neighbors; //list of pointers to all adjacent precincts
};

/**** Construct - Destruct ****/
Precinct::Precinct()
{
    setId("");
    setTotalPop(0);
    setMajorPartyIndex(-1);

}

Precinct::~Precinct()
{	
}

/**** Setters ****/
void Precinct::setId(string newId)
{
    m_id = newId;
}

void Precinct::setTotalPop(int newTotalPop)
{
    m_totalPop = newTotalPop;
}

void Precinct::setMajorPartyIndex(int newIndex)
{
	m_majorPartyIndex = newIndex;
}
/***** Getters *****/
string Precinct::getId()
{
    return m_id;
}

int Precinct::getTotalPop()
{
    return m_totalPop;
}

int Precinct::getMajorPartyIndex()
{
	return m_majorPartyIndex;
}
/**** Print ****/
// print(): prints all information about precinct
void Precinct::print()
{
    cout << endl << "Precinct ID: " << m_id << endl
	 << "Precinct Population: " << m_totalPop << endl
	 << "Precinct Party Percentages: " << endl
	 << "Democrat: " << (m_partyPercents[0] * 100) << "%" << endl
	 << "Green: " << (m_partyPercents[1] * 100) << "%"  << endl
	 << "Libertarian: " << (m_partyPercents[2] * 100) << "%"  << endl
	 << "Other Parties: " << (m_partyPercents[3] * 100) << "%"  << endl
	 << "Republican: " << (m_partyPercents[4] * 100) << "%"  << endl
	 << "Unaffiliated: " << (m_partyPercents[5] * 100) << "%"  << endl;
}

/**** Helpers ****/
// getPartyPop(): gets the population of a given party
// - Input: int partyIndex, the index of the specified party
// - Return: int, the population of the given party within the precinct
int Precinct::getPartyPop(int partyIndex)
{
    return (int)(m_partyPercents[partyIndex] * (double)m_totalPop);
}

// addPartyPercentage(): adds a percentage for a party and adds it to the
// back of m_partyPercents
// - Input: double percentage, the percentage of a party in a specific precinct
void Precinct::addPartyPercentage(double percentage)
{
    m_partyPercents.push_back(percentage);
}

// updateMajorParty(): iterates through available party percentages, and updates the majorPartyIndex to the highest
//	In case of no parties listed, will default to -1
void Precinct::updateMajorParty()
{
	if(m_partyPercents.size() == 0)
	{
		setMajorPartyIndex(-1);
	}
	else
	{
		int currentHighestIndex = -1;
		double currentHighestPercent = 0;

		vector<double>::iterator iter = m_partyPercents.begin();

		for(int i = 0;iter != m_partyPercents.end(); iter++, i++)
		{
			if(*iter > currentHighestPercent)
			{
				currentHighestIndex = i;
				currentHighestPercent = (*iter);
			}
		}
		setMajorPartyIndex(currentHighestIndex);
	}
}

#endif
