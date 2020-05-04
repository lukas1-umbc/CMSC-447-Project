// sc_vote_parser.cpp:
//  - Parser for south carolina precinct voter/election data (2016)
// 	- Input: raw csv of SC voter data for each precinct (may have some missing)
// 	- Output: Preprocessed/formatted file of south carolina prescinct voter data
//	- Dependent files:
//		- None
//
// Original Coder: David Ramsey
// Most Recent Change: 27 April 2020
//		- Finished output file creation.
//		- Party class now has m_sig member to flag if it is significant
//			- non-significant parties no longer destroyed
//		- Got rid of calls to test functions in main
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
	bool m_sig;			// If party is significant or not

public:
	// Construct - Destruct //
	Party()
	{
		m_id = " ";
		m_numVoters = 0;
		m_sig = true;
	}

	~Party()
	{
	}
};

// Precinct Class: Separate from Precinct class within algorithm.c, used to store raw information of each precinct.
class Precinct {
public:
	string m_id;			// Precinct id
	int m_numVoters;		// Number of REGISTERED voters
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
const string OUT_FILE_NAME = "sc_parsed_voter_data.txt";
ifstream g_inFile;
ofstream g_outFile;

// Parsing //
const double SIG_THRESHOLD = 0.12;
int g_numSigParties;
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
	g_numSigParties = (int)g_partyList.size();
	for(int i = 0; i < (int)g_partyList.size(); i++)
	{
		percent = (double)g_partyList[i]->m_numVoters / (double)total;
		if(percent < SIG_THRESHOLD)
		{
			g_partyList[i]->m_sig = false;
			g_numSigParties--;
		}
	}
}

// writeProcessedFile(): 
// requirements: - assumes input file has been fully parsed
// Misc: not required, but intended to be ran after non-significant parties have been removed 
void writeProcessedFile()
{
	// Init
	double percent;
	int voteTotal;
	Precinct* precinct;

	// Open output file
	g_outFile.open(OUT_FILE_NAME);

	// Number of congressional districts
	g_outFile << 7 << endl;

	// Number of sig parties line
	g_outFile << g_numSigParties << endl;
	
	// Sig party indication line
	for(int i = 0; i < (int)g_partyList.size(); i++)
	{
		if(g_partyList[i]->m_sig)
		{
			g_outFile << 1 << ",";
		}
		else
		{
			g_outFile << 0 << ",";
		}
	}
	g_outFile << endl;

	// Write party names
	for(int i = 0; i < (int)g_partyList.size(); i++)
	{
		g_outFile << g_partyList[i]->m_id << ", ";
	}
	g_outFile << endl;

	// Write precinct data header
	g_outFile << "# PrecinctID,RegisteredVoters,";
	for(int i = 0; i < (int)g_partyList.size(); i++)
	{
		g_outFile << g_partyList[i]->m_id << ",";
	}
	g_outFile << endl;

	// Write data for each precinct
	for(int i = 0; i < (int)g_precinctList.size(); i++)
	{
		// write precinct id and number of registered voters
		precinct = g_precinctList[i];
		g_outFile << precinct->m_id << ",";
	    g_outFile << precinct->m_numVoters << ",";
	
		// calculate total number of actual voters
		voteTotal = 0;
		for(int k = 0; k < (int)g_partyList.size(); k++)
		{
			voteTotal += precinct->m_partyNumbers[k];
		}

		// calculate and write affiliation ratio for each party
		for(int k = 0; k < (int)g_partyList.size(); k++)
		{
			if(g_partyList[k]->m_sig == true)
			{
				percent = (double)precinct->m_partyNumbers[k] / (double)voteTotal;
				g_outFile << percent << ",";
			}
			else
			{
				g_outFile << 0 << ",";
			}
		}
		g_outFile << endl;
	}

	// Close output file
	g_outFile.close();
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

	// Get rid of non-significant parties
	findSigParties();

	// Process information and write to formatted file
	writeProcessedFile();

	// Clean up
	g_inFile.close();
	clearAll();

	return 0;
}
