/* 
 * Project: Routing Parser Testing Sofware for FRoC
 * Author: James Meijers
 *
 * Created on May 9, 2017, 4:04 PM
 */

#include <cstdlib>
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <valarray>
#include <sstream>
#include "routing.h"

using namespace std;

vector <path> readRoutingFile(ifstream & file);

/*
 * This program takes 3 arguments
 * 1. the number of matches expected
 * 2. the meta-parser routing output file for the circuit
 * 3. the meta-parser routing output file for FRoC
 * It then parses the file, finds the number of circuits in each with
 * identical routing, and displays the number with either success if enough 
 * matches were found, or an error if not enough were found
 */

int main(int argc, char** argv) {
    //ensure enough arguments are given
    if(argc != 4){
        cout << "Too few/many arguments, need 3 " << endl;
        return 0;
    }
    
    
    //Get the inputs and open the files
    stringstream temp(argv[1]);
    
    int numMatches;
    
    temp >> numMatches;
    
    ifstream circuit;
    circuit.open(argv[2]);
    ifstream FRoC;
    FRoC.open(argv[3]);
    
    vector <path> FRoCPaths;
    vector <path> circuitPaths;
    
    //parse the files to get the data
    circuitPaths = readRoutingFile(circuit);
    FRoCPaths = readRoutingFile(FRoC);
    
    int matches = 0;
    
    //Check for matches
    for(unsigned i = 0; i < FRoCPaths.size(); i++){
        for(unsigned j = 0; j < circuitPaths.size(); j++){
            if(FRoCPaths[i] == circuitPaths[j]){
                if(!FRoCPaths[i].matchFound && !circuitPaths[j].matchFound){
                    matches++;
                    FRoCPaths[i].matchFound = true;
                    circuitPaths[j].matchFound = true;
                }
                cout << "FRoC path " << FRoCPaths[i].id << " matches circuit path " << circuitPaths[j].id << endl;
            }
        }
    }
    
    //report results
    if(matches >= numMatches){
        cout << matches << " matches found, enough" << endl;
    }
    else {
        cout << "error, " << matches << " matches found, not enough" << endl;
        int returnValue = numMatches - matches;
        return returnValue; //return the number of missing matches
    }

    return 0;
}






vector <path> readRoutingFile(ifstream & file){
    //always starts with one, so this is fine
    path currentPath;
    currentPath.id = 1;
    currentPath.matchFound = false;
    int pathNumber = 1;
    //final result
    vector <path> pathsInFile;

    //read to the end of the file
    while (!file.eof()) {
        string line;
        //get the next line to analyze
        getline(file, line);

        //if the line starts with a * it is the beginning or end of a route
        if (line[0] == '*') {
            stringstream linestream(line);
            linestream.ignore(1000, 'H');
            int tempPathNumber;
            linestream >> tempPathNumber;

            //check if new route is the beginning of a new path
            //if so, add the current path to the list and start a new one
            if (tempPathNumber != pathNumber) {
                pathsInFile.push_back(currentPath);
                currentPath.hardwarePath.clear();
                pathNumber = tempPathNumber;
                currentPath.id = pathNumber;
                currentPath.matchFound = false;
            }

        } else if (line[0] == 'L' || line[1] == '4') { //check if line is some sort of hardware
            int typeID = 0;
            if (line[0] == 'L') {
                if (line[1] == 'E') { //LE_BUFFER
                    typeID = 0;
                } else if (line[1] == 'O') {
                    if (line[6] == 'I') { //LOCAL_INTERCONNECT
                        typeID = 1;
                    } else if (line[6] == 'L') { //LOCAL_LINE
                        typeID = 2;
                    }
                }
            } else if (line[1] == '4') {

                if (line[0] == 'C') { //C4
                    typeID = 4;
                } else if (line[0] == 'R') { //R4
                    typeID = 3;
                }
            }
            
            //get hardware ID string
            stringstream linestream(line);
            linestream.ignore(1000, ':');
            getline(linestream, line);
            
            //add info to the currentpath hardware vector
            hardware tempHardware;
            tempHardware.ID = line;
            tempHardware.typeID = typeID;
            currentPath.hardwarePath.push_back(tempHardware);
        }
    }
    //add the final path to the list
    pathsInFile.push_back(currentPath);
    return pathsInFile;
}











