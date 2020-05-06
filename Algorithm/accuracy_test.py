#Contributors: Lukas Mueller
#Test whether the districts are evenly distributed among the significant parties with a 5% margin of error.

mdParties = ["DEMOCRAT", "GREEN", "LIBERTARIAN", "OTHER", "REPUBLICAN"]

def main():
        
    with open("mdAlgOutput.txt", "r") as mdOutput:
        lines = mdOutput.readlines()
    
    results_dict = {}

    lines.pop(0)

    sigParties = []
    for line in lines:
        line = line.split(",")

        results_dict[line[0]] = mdParties[int(line[1])]
        sigParties.append(mdParties[int(line[1])])

    sigParties = set(sigParties)

    #for district in results_dict:
        #print("District " + district + " is " + results_dict[district])
    #print("The state has " + str(len(sigParties)) + " parties.")
    #print("The state has " + str(len(results_dict)) + " districts.")

    numDem = 0
    numGreen = 0
    numLibertarian = 0
    numOther = 0
    numRepublican = 0

    for district in results_dict:
        if results_dict[district] == "DEMOCRAT":
            numDem += 1
        elif results_dict[district] == "GREEN":
            numGreen += 1
        elif results_dict[district] == "LIBERTARIAN":
            numLibertarian += 1
        elif results_dict[district] == "OTHER":
            numOther += 1
        elif results_dict[district] == "REPUBLICAN":
            numRepublican += 1
    
    dem = (numDem/len(results_dict))*100
    green = (numGreen/len(results_dict))*100
    lib = (numLibertarian/len(results_dict))*100
    other = (numOther/len(results_dict))*100
    rep = (numRepublican/len(results_dict))*100

    finalList = [dem, green, lib, other, rep]

    #if the percentages don't add up to 100
    if sum(finalList) != 100:
        print("ERROR: PERCENTAGES OF EACH SIGNIFICANT PARTY ADD UP TO OVER 100: ")
        for i in range(0, len(finalList)):
            print(mdParties[i] + ": " + str(finalList[i]))
    
    #if there is not an even split within 5%
    correctPerc = 100/len(sigParties)
    for perc in finalList:
        if perc != 0:
            if perc == correctPerc:
                pass
            else:
                if perc != correctPerc + 5 or perc != correctPerc - 5:
                    print("ERROR: THERE IS NOT AN EVEN DISTRIBUTION OF THE DISTRICTS BETWEEN THE SIGNIFICANT PARTIES WITHIN A 5% TOLERANCE:")
                    print(mdParties[finalList.index(perc)] + ": " + str(perc))
        

        

main()
