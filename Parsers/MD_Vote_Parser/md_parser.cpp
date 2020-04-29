// md_parser.cpp:
// 	- md_parser will create a uniform data file for the Swing State algorithm.
// 	- Input: Unprocessed data files
// 	- Output: File containing processed data required by algorithm.cpp
//	- Dependent files: 
//		- 2018_eligible_voters_by_precinct.csv - contains the actual voter data
//		- md_county_codes.csv - contains the county codes
//
// Original Coder: Lauren Mundy
// Most Recent Change: 22 April 2020
//		- added loadCountyCodes, fixed splitString function, completed full file read in for precinct voting data, added loadPrecinctPop
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>

using namespace std;

// File I/O
ifstream g_inFile;
ofstream g_outFile;

// Constants
const string IN_DATA_FILE_NAME = "2018_eligible_voters_by_precinct.csv";
const string IN_CODE_FILE_NAME = "md_county_codes.csv";
const string OUT_PARSED_FILE_NAME = "md_parsed_data.txt";
const string INTERIM_FILE_NAME = "md_interim_data.txt";

// Data
map<string, string> g_CountyCodes;

// splitStrings(): Splits each line from the file into tokens
// - Inputs: string str, the string to be splitString
//           string* tokens, a given array that will be filled with the tokens
void splitString(string str, string* tokens)
{
	string delimiter = ",";
	string temp_str = "";
	
	size_t pos = 0;
	
	int counter = 0;
	
	while ((pos = str.find(delimiter)) != std::string::npos)
	{
		temp_str = str.substr(0, pos);
		tokens[counter] = temp_str;
		str.erase(0, pos + delimiter.length());
		counter++;
	}
	
	tokens[counter] = str;
}

// loadCountyCodes(): Reads in the county names, and codes, and then
//                    loads them into a map for easy access
void loadCountyCodes()
{
	string data = "";
	string line[2];
	
	// Open the data file
	g_inFile.open(IN_CODE_FILE_NAME);
	if(g_inFile.is_open())
	{
		// Read in the first line of the file and discard it
		// The first line just has the column headers
		getline(g_inFile, data);
		
		while(getline(g_inFile, data))
		{
			// Split the data into the names, and codes
			// then add them to the map
			splitString(data, line);
			g_CountyCodes.insert(pair<string, string>(line[0], line[1]));
		}
			
		g_inFile.close();
	}
}


// loadPrecinctData(): Loads in the precinct data from the given file, and processes it for the 
void loadPrecinctData()
{
	/******************************* Notes **************************************
	Each party will have a number associate with it in the precinct object,
	this will be determined by how they are laid out in the data file.
	They are as follows:
	Democrat = 0, Green = 1, Libertarian = 2, Other Parties = 3, Republican = 4, Unaffiliated = 5
	
	Precinct IDs are added to county codes in the following form:
    Allegany county code = 24001, precinct 1 id = 001-000 => 24001001-000
	****************************************************************************/
	
	int counter = 0;
	int totalVoterCount = 0;
	int voterCount[6];
	
	string precinctID = "";
	string data = "";
	string line[10];
	
	//Precinct* newPrecinct;
	
	// Load in the county codes for precinct IDs
	loadCountyCodes();
	
	// Open the file to be written to
	g_outFile.open(INTERIM_FILE_NAME);
	
	// Give the file headers for each column
	g_outFile << "PrecinctID,PrecinctPop,DemocratP,GreenP,LibertarianP,OtherPartiesP,RepublicanP,UnaffiliatedP\n";

	// Open the file to be read from
	g_inFile.open(IN_DATA_FILE_NAME);
	if(g_inFile.is_open())
	{
		// Read in the first line of the file and discard it
		// The first line just has the column headers
		getline(g_inFile, data);
		
		// Start reading in valid data
		while(getline(g_inFile, data))
		{
			splitString(data, line);
			
			// Some of the data entries are empty. They are signafied
			// by the phrase "Unable to Determine" in the precinct id line
			if(line[3] != "Unable to Determine")
			{
				// If this is the first loop, get initial pricinct id and create new precinct object
				if(precinctID == "")
				{
					//newPrecinct = new Precinct();
					precinctID = line[3];
				}

				// Save voter count arranged by the party number formatted listed above
				voterCount[counter] = stoi(line[9]);
				counter++;
				
				// If all the data is read in for a precinct (6 lines of data), then complete
				// the precinct and add it to the precinct list
				if(counter == 6)
				{	
					// This segment is needed (I wasn't able to just append the county code because
					// there was some weird formatting) to clean up the county code, and then
					// get it formatted properly
					string temp = "";
					temp.append(to_string(stoi(g_CountyCodes.find(line[0])->second)));
					temp.append(line[3]);
					//newPrecinct->setId(temp);
					g_outFile << temp << ",";
					
					// Get the total amount of voters to find percentage of each party
					for(int i = 0; i < 6; i++)
					{
						totalVoterCount += voterCount[i];
					}
					
					g_outFile << totalVoterCount << ",";
					
					// Add percentage for each party to m_partyPercents
					for(int i = 0; i < 6; i++)
					{
						if((double)totalVoterCount != 0)
						{
							g_outFile << voterCount[i]/(double)totalVoterCount;
							
							if(i != 5)
								g_outFile << ",";
						}
						else
						{
							g_outFile << 0;
							
							if(i != 5)
								g_outFile << ",";
						}
					}
						
					// Add the precinct to g_Precincts, then reset data and create new precinct
					//g_Precincts.push_back(newPrecinct);
					
					g_outFile << "\n";
					
					//newPrecinct = new Precinct();
					totalVoterCount = 0;
					counter = 0;

				}
			}
		}
		g_inFile.close();
	}
	g_outFile.close();
}

// parsePrecinctData(): Takes the interim data and generates data usable by the algorithm
void parsePrecinctData()
{
	
	int totalParties = 0;
	int whichParties[5];
	
	double democratStatePercent = 0;
	double greenStatePercent = 0;
	double libertarianStatePercent = 0;
	double otherPartyStatePercent = 0;
	double republicanStatePercent = 0;
	double unaffiliatedStatePercent = 0;
	
	string data = "";
	string line[10];
	
	vector<string> originalData;
	vector<double> democrat;
	vector<double> green;
	vector<double> libertarian;
	vector<double> otherParty;
	vector<double> republican;
	vector<double> unaffiliated;
	
	// Open the data file
	g_inFile.open(INTERIM_FILE_NAME);
	if(g_inFile.is_open())
	{
		// Read in the first line of the file and discard it
		// The first line just has the column headers
		getline(g_inFile, data);
		
		while(getline(g_inFile, data))
		{
			originalData.push_back(data);
			
			// Split the data into the names, and codes
			// then add them to the map
			splitString(data, line);
			
			democrat.push_back(stod(line[2]));
			green.push_back(stod(line[3]));
			libertarian.push_back(stod(line[4]));
			otherParty.push_back(stod(line[5]));
			republican.push_back(stod(line[6]));
			unaffiliated.push_back(stod(line[7]));
		}
			
		g_inFile.close();
	}
	
	
	// Get the average percentage for each party accross the state
	for(int i = 0; i < (int) democrat.size(); i++)
	{
		democratStatePercent += democrat[i];			
		greenStatePercent += green[i];
		libertarianStatePercent += libertarian[i];
		otherPartyStatePercent += otherParty[i];
		republicanStatePercent += republican[i];
		unaffiliatedStatePercent += unaffiliated[i];
	}

	democratStatePercent = democratStatePercent/((int)democrat.size());
	greenStatePercent = greenStatePercent/((int)green.size());
	libertarianStatePercent = libertarianStatePercent/((int)libertarian.size());
	otherPartyStatePercent = otherPartyStatePercent/((int)otherParty.size());
	republicanStatePercent = republicanStatePercent/((int)republican.size());
	unaffiliatedStatePercent = unaffiliatedStatePercent/((int)unaffiliated.size());
	
	// Find the total amount of parties the algorithm will use, and which ones they are
	if(democratStatePercent > 0.12)
	{
		totalParties++;
		whichParties[0] = 1;
	}
	else
		whichParties[0] = 0;
	
	if(greenStatePercent > 0.12)
	{
		totalParties++;
		whichParties[1] = 1;
	}
	else
		whichParties[1] = 0;
	
	if(libertarianStatePercent > 0.12)
	{
		totalParties++;
		whichParties[2] = 1;
	}
	else
		whichParties[2] = 0;
	
	if(otherPartyStatePercent > 0.12)
	{
		totalParties++;
		whichParties[3] = 1;
	}
	else
		whichParties[3] = 0;
	
	if(republicanStatePercent > 0.12)
	{
		totalParties++;
		whichParties[4] = 1;
	}
	else
		whichParties[4] = 0;
	// Open the file for the final parsed data
	g_outFile.open(OUT_PARSED_FILE_NAME);
	if(g_outFile.is_open())
	{
		// Print the total number of districts in md
		g_outFile << "8\n";
		
		// Print the total number of parties being considered by the algorithm
		g_outFile << totalParties << "\n";
		
		// Print which ones they are
		for(int i = 0; i < 5; i++)
		{
			g_outFile << whichParties[i] << ",";
		}
		
		g_outFile << "\nDemocrat,Green,Libertarian,OtherParties,Republican";
		
		// This is just the layout of each row of data
		g_outFile << "\n#PrecinctID,PrecinctPop,DemocratP,GreenP,LibertarianP,OtherPartiesP,RepublicanP\n";
		
		
		// Output data to file
		for(int i = 0; i < (int) originalData.size(); i++)
		{
			splitString(originalData[i], line);
			
			line[0].erase(line[0].begin() + 5);
			
			g_outFile << line[0] << "," << line[1] << ",";
			
			if(democratStatePercent >= 0.12)
				g_outFile << line[2] << ",";
			else
				g_outFile << 0 << ",";
			
			if(greenStatePercent >= 0.12)
				g_outFile << line[3] << ",";
			else
				g_outFile << 0 << ",";
			
			if(libertarianStatePercent >= 0.12)
				g_outFile << line[4] << ",";
			else
				g_outFile << 0 << ",";
			
			if(otherPartyStatePercent >= 0.12)
				g_outFile << line[5] << ",";
			else
				g_outFile << 0 << ",";
			
			if(republicanStatePercent >= 0.12)
				g_outFile << line[6] << ",";
			else
				g_outFile << 0 << ",";
			// Unaffiliated is being left out since it isn't being considered
			
			g_outFile << "\n";
		}
		g_outFile.close();
	}
}

/**** MAIN ****/
int main() {
	loadPrecinctData();
	parsePrecinctData();
	
    return 0;
}