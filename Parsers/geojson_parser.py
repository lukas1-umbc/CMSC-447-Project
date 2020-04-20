#maryland_geo_parser.py file written by Lukas Mueller on 4/19/20.
#This program parses the geojson files for the necessary data.

import json
import csv

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

#Gets the neighboring precincts of each precinct in Maryland
#and stores them as key, value pairs in a dictionary
def parseMDPrecNbrs(filename):

    precNbrs = {}

    with open(filename) as precinctFile:
        data = json.load(precinctFile)

    #populate the dictionary
    for feature in data["features"]:
        precNbrs[feature["properties"]["VTD"]] = feature["properties"]["NEIGHBORS"]
        
    return precNbrs

def main():

    #write all the counties + their codes to a csv
    countyFile = open("md_county_codes.csv", "w+", newline='')
    countyCodes = parseMDCountyCodes("md_precinct2010.geojson")

    writer = csv.writer(countyFile)
    writer.writerow(["County", "Code"])
    
    for county in countyCodes:
        writer.writerow([county, countyCodes[county]])
    countyFile.close()

    #write all precincts + their neighbors to a file
    nbrFile = open("md_precinct_neighbors.txt", "w+")
    precNbrs = parseMDPrecNbrs("md_precinct2010.geojson")
    for precinct in precNbrs:
        #6 precincts do not have neighbor lists
        if precNbrs[precinct]:
            nbrFile.write(precinct + ": " + precNbrs[precinct] + "\n")
        else:
            nbrFile.write(precinct + "\n")
    nbrFile.close()

main()