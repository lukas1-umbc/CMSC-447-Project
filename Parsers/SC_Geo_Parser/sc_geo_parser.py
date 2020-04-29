#sc_geo_parser.py file written by Lukas Mueller on 4/29/20.
#This program parses and modifies the geojson data for South 
#Carolina. It creates a list of neighbors for each precinct, 
#and adds a VTD attribute to the geojson data.

import json
import xlrd
import copy
import time
from fuzzywuzzy import process

#Use exact AND fuzzy matching to match up the keys in geojsonDict 
#with the ones in xlDict, and create a new dictionary with the 
#precincts from geojsonDict as keys and precinct does from xlDict as values.
def mergeDicts(geojsonDict, xlDict):

    mergedDict = {}

    #get what is directly the same between the two dictionaries
    for precinct in geojsonDict:
        if precinct in xlDict:
            mergedDict[precinct] = xlDict[precinct]

    #compare merged dict and geojson dict to see what's missing
    missingList = []
    for precinct in geojsonDict:
        if precinct not in mergedDict:
            missingList.append(precinct)
    
    xlList = [precinct for precinct in xlDict]
    #try to clean up with fuzzy matching
    for precinct in sorted(missingList):
    
        highest = process.extractOne(precinct, xlList)
        #need to see if the code is already assigned
        if xlDict[highest[0]] not in mergedDict.values():
            mergedDict[precinct] = xlDict[highest[0]]
        else:
            pass

    #compare merged dict and geojson dict to see what's missing (again)
    missingList = []
    for precinct in geojsonDict:
        if precinct not in mergedDict:
            missingList.append(precinct)

    print(str(len(missingList)) + " precincts in the geojson did not match with the xlsx.")

    return mergedDict

#See whether there are any precincts in the mergedDict that share
#precinct codes with another precinct. 
def checkForDuplicates(mergedDict):

    count = 0
    copiedPrecincts = copy.deepcopy(mergedDict)
    for precinct in mergedDict:
        for otherPrecinct in copiedPrecincts:
            if precinct != otherPrecinct and mergedDict[precinct] == copiedPrecincts[otherPrecinct]:
                count += 1
                print("Both " + precinct + " and " + otherPrecinct + " have code " + mergedDict[precinct])

    print("There are " + str(count) + " duplicates.")

#Add the precinct code attribute and set it for each feature
#using the mergedDict contents
def modifyGeo(mergedDict, filename):

    with open(filename) as geojsonFile:
        data = json.load(geojsonFile)

    for feature in data["features"]:
        precName = feature["properties"]["PNAME"].lower()
        #check that precinct is in mergedDict (~110 are not at the moment)
        if precName in mergedDict:
            feature["properties"]["VTD"] = mergedDict[precName]
        else:
            feature["properties"]["VTD"] = None

    #write the VTD information to the geojson file
    with open(filename, "w+") as f:
        json.dump(data, f, indent=2)
        
def parseSCPrecNbrs(filename):

    start = time.time()

    #precinct VTD as keys, list of coordinates as values
    precinctDict = {}

    with open(filename) as precinctFile:
        data = json.load(precinctFile)

    #populate precinctDict
    for feature in data["features"]:

        code = feature["properties"]["VTD"]
        #~110 precincts that could not be matched; they are null
        if code:
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
                    
        #print(precinct + ": " + str(precinctNbrs[precinct]))

    print("Neighbor parser took " + str(round((time.time() - start)/60, 2)) + " minutes")

    return precinctNbrs
    

def main():

    geoFilename = "2013_sc_precincts.geojson"

    geojsonDict = {}
    with open(geoFilename) as geojsonFile:
        data = json.load(geojsonFile)

    for feature in data["features"]:
        geojsonDict[feature["properties"]["PNAME"].lower()] = ""

    xlDict = {}
    wb = xlrd.open_workbook("Political_VTD_New.xlsx")
    sheet = wb.sheet_by_index(0)

    for i in range(sheet.nrows):
        xlDict[sheet.cell_value(i, 2).lower()] = sheet.cell_value(i, 1)
    
    mergedDict = mergeDicts(geojsonDict, xlDict)

    checkForDuplicates(mergedDict)

    modifyGeo(mergedDict, geoFilename)

    precinctNbrs = parseSCPrecNbrs(geoFilename)
    #write all the precinct + their neighbors to a txt file
    nbrFile = open("sc_precinct_neighbors.txt", "w+")
    for precinct in precinctNbrs:
        nbrFile.write(precinct + ": ")
        for nbr in precinctNbrs[precinct]:
            #don't need comma after last neighbor
            if nbr == precinctNbrs[precinct][len(precinctNbrs[precinct]) - 1]:
                nbrFile.write(nbr)
            else:
                nbrFile.write(nbr + ", ")
        nbrFile.write("\n")
    nbrFile.close()

main()