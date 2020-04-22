// sc_vote_parser.cpp:
//  - Parser for south carolina precinct voter/election data (2016)
// 	- Input: raw csv of SC voter data for each precinct (may have some missing)
// 	- Output: Preprocessed/formatted file of south carolina prescinct voter data
//	- Dependent files:
//		- None
//
// Original Coder: David Ramsey
// Most Recent Change: 22 April 2020
// 	- Initial creation, get parties functions, get precincts function reads in precinct names and number of voters but having
// 	bugs getting party numbers, dump functions for testing, having trouble with dump precincts printing party numbers, party
// 	and precinct structures.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

// Pary Class: used to store information of each party
class Party {
public:
	string m_id;		// Party name/id
	int m_numVoters;	// Total number of people that voted for this party throughout the state
	
public:
	// Construct - Destruct //
	Party()
	{
		m_id = " ";
		m_numVoters = 0;
	}

	~Party()
	{
	}
};

// Precinct Class: Separate from Precinct class within algorithm.c, used to store raw information of each precinct.
class Precinct {
public:
	string m_id;			// Precinct id
	int m_numVoters;		// Number of *registered* voters
	int* m_partyNumbers;	// Number of people that voted for each party, won't add up to m_numVoters due to absentees
	
public:
	// Construct - Destruct //
	Precinct()
	{
		m_id = " ";
		m_numVoters = 0;
		m_partyNumbers = NULL;
	}

	~Precinct()
	{
		if(m_partyNumbers != NULL)
		{
			delete [] m_partyNumbers;
			m_partyNumbers = NULL;
		}
	}

};

/******* Constants - Globals ******/
// File I/O //
const string IN_FILE_NAME = "../../South_Carolina_2016_Precinct_Election_Data.csv";
string g_outFileName;
ifstream g_inFile;
ofstream g_outFile;

// Parsing //
vector<Party*> g_partyList;
vector<Precinct*> g_precinctList;

/******* Functions ********/
// UTILITY //
// clearAll(): free's all dynamically allocated memory // UNTESTED
void clearAll()
{
	// Free parties
	for(int i = 0; i < (int)g_partyList.size(); i++)
	{
		if(g_partyList[i] != NULL)
		{
			delete g_partyList[i];
			g_partyList[i] = NULL;
		}
	}
	
	// Free precincts
	for(int i = 0; i < (int)g_precinctList.size(); i++)
	{
		if(g_precinctList[i] != NULL)
		{
			delete g_precinctList[i];
			g_precinctList[i] = NULL;
		}
	}
}

// dumpParties(): prints information of each party in party list.
// Intended for testing purposes only
void dumpParties()
{
	// Init
	Party* party;	

	// Dump each parties information
	for(int i = 0; i < (int)g_partyList.size(); i++)
	{
		party = g_partyList[i];
		if(party != NULL)
		{
			cout << party->m_id << " " << party->m_numVoters << endl;
		
		}
	}
	// Clean
	party = NULL;
}

// dumpPrecincts(): print information of each precinct in precinct list. // UNFINISHED
void dumpPrecincts()
{
	// Init
	Precinct* precinct;
	
	// Dump each precincts information
	for(int i = 0; i < (int)g_precinctList.size(); i++)
	{
		precinct = g_precinctList[i];
		if(precinct != NULL)
		{
			// print id
			cout << precinct->m_id << " ";
			// print partyNumbers
			for(int k = 0; (int)g_partyList.size(); k++)
			{
				cout << precinct->m_partyNumbers[k] << " ";
			}
			// print numVoters
			cout << precinct->m_numVoters << endl;
		}
	}
}

// HELPERS //
// getParties(): parses the header of input file for parties, finds number of parties included in data and each parties id,
// 						instantiates party list.
// requirements: - Input file is assumed to be open
//				 - Input file is assumed to be on first line
void getParties()
{
	// Init
	stringstream ss;
	string partyLine;
	string partyName;
	Party* newParty;

	// Read in first line of input file
   	getline(g_inFile, partyLine);
	ss.str(partyLine);

	// Parse line, get all parties
	while(getline(ss, partyName, ','))
	{
		// if token is not empty, and is not whitespace or EOS (end of stream)
		if(partyName.size() > 0 && partyName[0] >= '0')
		{
			// create new party
			newParty = new Party;
			newParty->m_id = partyName;
			g_partyList.push_back(newParty);
		}
	}

	// Clean
	newParty = NULL;
}

// getPrecincts(): parses the input file for the precinct's and their information.
// requirements: - Input file is assumed to be open
//				 - Input file is assumed to be on third line or further.
void getPrecincts()
{
	// Init
	stringstream ss;
	string precinctLine;
	string field;
	int numField;
	string ignore;
	Precinct* newPrecinct;

	// Get precincts
	while(getline(g_inFile, precinctLine))
	{
		// create stream
		ss.str(precinctLine);
		
		// init new precinct
		newPrecinct = new Precinct;
		newPrecinct->m_partyNumbers = new int[(int)g_partyList.size()];

		// get precinct id and registerd voters
		getline(ss, field, ',');
		newPrecinct->m_id = field;
		ss >> newPrecinct->m_numVoters;
		
		// get num voters for each party
		for(int i = 0; i < (int)g_partyList.size(); i++)
		{
			getline(ss, ignore, ',');
			ss >> numField;
			newPrecinct->m_partyNumbers[i] = numField;
		}

		// add precinct
		g_precinctList.push_back(newPrecinct);

	}

	// Clean
	newPrecinct = NULL;
}

/******* Main ********/
int main() {
	
	string ignore;

	g_inFile.open(IN_FILE_NAME);
	
	getParties();
	dumpParties();
	
	// Dump header line
	getline(g_inFile, ignore);
	
	getPrecincts();
	dumpPrecincts();

	g_inFile.close();

	clearAll();

	return 0;
}
