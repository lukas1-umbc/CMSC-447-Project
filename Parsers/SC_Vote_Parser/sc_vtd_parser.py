import xlrd
import copy
from fuzzywuzzy import process

def main():

    #get the VTDs from the excel file
    xlDict = {}
    wb = xlrd.open_workbook("Political_VTD_New.xlsx")
    sheet = wb.sheet_by_index(0)

    #populate xlDict - keys are precinct names, values are VTDs
    for i in range(sheet.nrows):
        xlDict[sheet.cell_value(i, 2).lower()] = sheet.cell_value(i, 1)
    
    #put the parsed voter data into a dictionary
    parsedDict = {}
    with open("sc_parsed_voter_data.txt", "r") as parsedData:
        lines = parsedData.readlines()
    
    #populate parsedDict - keys are precinct names, values are precinct data

    firstLines = [line for line in lines[0:5]]

    for line in lines[5:len(lines)]:
        line = line.strip().split(",")
        name = line[0].lower()
        info = line[1:len(line)]
        parsedDict[name] = info

    #merge the dictionaries together
    #keys are VTDs, values are precinct data
    mergedDict = {}

    #get what is directly the same between the two dictionaries
    missingList = []
    for name in parsedDict:
        if name in xlDict:
            mergedDict[xlDict[name]] = parsedDict[name]
        else:
            missingList.append(name)
    
    #try to clean up with fuzzy matching
    xlList = [precinct for precinct in xlDict]
    for precinct in sorted(missingList):
        highest = process.extractOne(precinct, xlList)
        #need to see if the code is already assigned
        if xlDict[highest[0]] not in mergedDict:
            mergedDict[xlDict[highest[0]]] = parsedDict[precinct]
            xlList.remove(highest[0])
        else:
            pass

    finalFile = open("sc_parsed_voter_with_VTD.txt", "w+")

    for line in firstLines:
        finalFile.write(line)
    for precinct in mergedDict:
        finalFile.write(precinct + "," + ",".join(mergedDict[precinct]) + "\n")
    



main()