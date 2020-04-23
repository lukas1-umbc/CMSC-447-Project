#md_geo_parser.py file written by Lukas Mueller on 4/19/20.
#This program parses the geojson files for the necessary data.

import json
import csv
import copy
import time

#Gets the county codes for each county in Maryland and 
#stores them as key, value pairs in a dictionary
def parseMDCountyCodes(filename):

    countyCodes = {}

    with open(filename) as precinctFile:
        data = json.load(precinctFile)

    #populate the dictionary
    for feature in data["features"]:
        line = feature["properties"]["NAME"] + ", " + feature["properties"]["COUNTY"]
        line = line.split("Precinct")
        countyCodes[line[0].strip()] = line[1].split()[1]
    
    precinctFile.close()

    return countyCodes

#get list of neighboring precincts for each precinct
#two precincts are neighbors if they share at least one coordinate
def parseMDPrecNbrs(filename):

    start = time.time()

    #precinct VTD as keys, list of coordinates as values
    precinctDict = {}

    with open(filename) as precinctFile:
        data = json.load(precinctFile)

    #populate precinctDict
    for feature in data["features"]:

        code = feature["properties"]["VTD"]
        coordList = feature["geometry"]["coordinates"][0]
        precinctDict[code] = coordList

    precinctFile.close()

    #deep copy of precinctDict for comparisons
    otherPrecincts = copy.deepcopy(precinctDict)
    #precinct VTD as keys, list of neighbor precincts as values
    precinctNbrs = {}

    for precinct in precinctDict:
        
        precinctCoords = precinctDict[precinct]

        precinctNbrs[precinct] = []

        #compare every precinct to every other precinct
        for potentialNbr in otherPrecincts:

            #don't compare precinct to self
            if precinct != potentialNbr:

                nbrCoords = otherPrecincts[potentialNbr]

                #check for coordinate matches
                for coord in precinctCoords:

                    #precinct and potentialNbr share a coordinate, so they are neighbors
                    if coord in nbrCoords:

                        precinctNbrs[precinct].append(potentialNbr)
                        break

    print("Neighbor parser took " + str(round((time.time() - start)/60, 2)) + " minutes")

    return precinctNbrs

def main():

    filename = "md_precinct2010.geojson"

    countyCodes = parseMDCountyCodes(filename)
    #write all the counties + their codes to a csv
    countyFile = open("md_county_codes.csv", "w+", newline='')

    writer = csv.writer(countyFile)
    writer.writerow(["County", "Code"])
    
    for county in countyCodes:
        writer.writerow([county, countyCodes[county]])
    countyFile.close()

    precinctNbrs = parseMDPrecNbrs(filename)
    #write all the precinct + their neighbors to a txt file
    nbrFile = open("md_precinct_neighbors.txt", "w+")
    for precinct in precinctNbrs:
        nbrFile.write(precinct + ": " + str(precinctNbrs[precinct]) + "\n")
    nbrFile.close()

main()
