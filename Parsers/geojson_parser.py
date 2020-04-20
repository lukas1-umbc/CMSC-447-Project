#maryland_geo_parser.py file written by Lukas Mueller on 4/19/20.
#This program parses the geojson files for the necessary data.

import json

#Gets the county codes for each county in Maryland and 
#stores them as key, value pairs in a dictionary
def parseMDCountyCodes(filename):

    countyCodes = {}

    with open(filename) as precinct_file:
        data = json.load(precinct_file)

    for feature in data["features"]:
        line = feature["properties"]["NAME"] + ", " + feature["properties"]["COUNTY"]
        line = line.split("Precinct")
        countyCodes[line[0].strip()] = line[1].split()[1]
    
    precinct_file.close()

    return countyCodes


def main():

    #write all the counties + their codes to a csv
    county_file = open("md_county_codes.csv", "w+")
    countyCodes = parseMDCountyCodes("precinct2010.geojson")
    for county in countyCodes:
        county_file.write(county + ", " + countyCodes[county] + "\n")
    county_file.close()

main()