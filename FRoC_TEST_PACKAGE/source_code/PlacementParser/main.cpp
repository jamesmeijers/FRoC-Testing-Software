/* 
 * Project: Placement Parser Testing Sofware for FRoC
 * Author: James Meijers
 *
 * Created on May 9, 2017, 1:52 PM
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <valarray>
#include <sstream>
#include "hardware.h"

using namespace std;

vector <path> readPlacementFile(ifstream & file);

/*
 * This program takes 3 arguments
 * 1. the number of matches expected
 * 2. the meta-parser placement output file for the circuit
 * 3. the meta-parser placement output file for FRoC
 * It then parses the file, finds the number of circuits in each with
 * identical placement, and displays the number with either success if enough 
 * matches were found, or an error if not enough were found
 */

int main(int argc, char** argv) {
    
    //ensure enough arguments are given
    if(argc != 4){
        cout << "Too few/many arguments, need 3 " << endl;
        return 0;
    }
    
    stringstream temp(argv[1]);
    
    int numMatches;
    
    //get the number of expected matches
    temp >> numMatches;
    
    //open both the placement files
    ifstream circuit;
    circuit.open(argv[2]);
    ifstream FRoC;
    FRoC.open(argv[3]);
    
    vector <path> FRoCPaths;
    vector <path> circuitPaths;
    
    //read the files to extract path info
    circuitPaths = readPlacementFile(circuit);
    FRoCPaths = readPlacementFile(FRoC);
    
    int matches = 0;
    
    //compare the paths searching for matches
    for(unsigned i = 0; i < FRoCPaths.size(); i++){
        for(unsigned j = 0; j < circuitPaths.size(); j++){
            if(FRoCPaths[i] == circuitPaths[j]){
                matches++;
                cout << "FRoC path " << FRoCPaths[i].id << " matches circuit path " << circuitPaths[j].id << endl;
            }
        }
    }
    
    //print out results of the test
    if(matches >= numMatches){
        cout << matches << " matches found, enough" << endl;
        return 0;
    }
    else {
        cout << "error, " << matches << " matches found, not enough" << endl;
        int returnValue = numMatches - matches;
        return returnValue; //return the number of missing matches
    }



    return 0;
}

vector <path> readPlacementFile(ifstream & file){
    vector <path> inFile;
    string input;
    file.ignore(1000, '#');
    
    while (! file.fail() && ! file.eof()){
        //1 after first flip flop, 2 after second flip flop
        int stage = 0;
        path newPath;
        //path id is number coming directly after #
        file >> newPath.id;
        //clear out remainder of the line
        getline(file, input);
        while (! file.fail() && stage < 2){
            getline(file, input);
            stringstream in(input);
            //check if the new line refers to a register / FF
            if(in.peek() == 's'){
                //skip ahead to the next space
                getline(in, input, ' ');
                char a = in.peek();
                if(a == 'L'){
                    //element is a LUT
                    int X, Y, N;
                    
                    in.ignore(1000, 'X');
                    in >> X;
                    in.ignore(1000, 'Y');
                    in >> Y;
                    in.ignore(1000, 'N');
                    in >> N;
                    
                    LUT tempLUT;
                    tempLUT.X = X;
                    tempLUT.Y = Y;
                    tempLUT.N = N;
                    newPath.LUTPath.push_back(tempLUT);
                }
                else if(a == 'F'){
                    //element is a Flip FLop
                    int X, Y, N;
                    
                    in.ignore(1000, 'X');
                    in >> X;
                    in.ignore(1000, 'Y');
                    in >> Y;
                    in.ignore(1000, 'N');
                    in >> N;
                    
                    FF tempFF;
                    tempFF.X = X;
                    tempFF.Y = Y;
                    tempFF.N = N;
                    
                    
                    if(stage == 0){
                        newPath.startReg = tempFF;
                        stage = 1;
                    }
                    else if(stage == 1){
                        newPath.endReg = tempFF;
                        stage = 2;
                    }
                }
            }
        }
        //add the path
        inFile.push_back(newPath);
        //skip to the start of the next path
        file.ignore(1000, '#');
    }
    
    return inFile;
}