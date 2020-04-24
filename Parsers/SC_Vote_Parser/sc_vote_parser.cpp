// sc_vote_parser.cpp:
//  - Parser for south carolina precinct voter/election data (2016)
// 	- Input: raw csv of SC voter data for each precinct (may have some missing)
// 	- Output: Preprocessed/formatted file of south carolina prescinct voter data
//	- Dependent files:
//		- None
//
// Original Coder: David Ramsey
// Most Recent Change: 24 April 2020
// 	- Fixed bug with getting/dumping precinct party voters info, it was a simple issue with the dump function.
// 	- Non-significant parties are now calculated and removed before writing output.
// 	- Skeleton of output function added.
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
const string g_OUT_FILE_NAME = "Processed_SC_Voter_Data.csv";
ifstream g_inFile;
ofstream g_outFile;

// Parsing //
const double SIG_THRESHOLD = 0.12;
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

// dumpPrecincts(): print information of each precinct in precinct list.
void dumpPrecincts()
{
	// Init
	Precinct* precinct;
	
	// Dump each precincts information
	cout << endl;
	for(int i = 0; i < (int)g_precinctList.size(); i++)
	{
		// print precinct
		precinct = g_precinctList[i];
		if(precinct != NULL)
		{
			// print id and numVoters
			cout << precinct->m_id << " ";
			cout << precinct->m_numVoters << " ";

			// print partyNumbers
			for(int k = 0; k < (int)g_partyList.size(); k++)
			{
				cout << precinct->m_partyNumbers[k] << " ";
			}
		}
		cout << endl;
	}
	cout << endl;
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
			// ignore leading comma
			getline(ss, ignore, ',');
			
			// get number of voters
			ss >> numField;
			newPrecinct->m_partyNumbers[i] = numField;
		
			// tally total voters to party
			g_partyList[i]->m_numVoters += numField;
		}

		// add precinct
		g_precinctList.push_back(newPrecinct);

	}

	// Clean
	newPrecinct = NULL;
}

// findSigParties(): finds significant parties, gets rid of non-significant parties.
// Assumes input file has been fully parsed
void findSigParties()
{
	// Find total num of voters
	int total = 0;
	for(int i = 0; i < (int)g_partyList.size(); i++)
	{
		total += g_partyList[i]->m_numVoters;
	}
	
	// Get rid of non-significant parties
	double percent;
	for(int i = 0; i < (int)g_partyList.size(); i++)
	{
		percent = (double)g_partyList[i]->m_numVoters / (double)total;
		if(percent < SIG_THRESHOLD)
		{
			delete g_partyList[i];
			g_partyList[i] = NULL;
		}
	}
}

// writeProcessedFile(): 
// requirements: - assumes input file has been fully parsed
// Misc: not required, but intended to be ran after non-significant parties have been removed 
void writeProcessedFile()
{
	
}

/******* Main ********/
int main() {

	// Init	
	string ignore;
	g_inFile.open(IN_FILE_NAME);
	
	// Parse headers
	getParties();
	getline(g_inFile, ignore);
	
	// Parse precinct info
	getPrecincts();

	// For Testing
	dumpParties();
	dumpPrecincts();

	// Get rid of non-significant parties
	findSigParties();

	// Process information and write to formatted file
	writeProcessedFile();

	// Clean up
	g_inFile.close();
	clearAll();

	return 0;
}
