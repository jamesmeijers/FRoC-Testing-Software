/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: jmeijers
 *
 * Created on May 23, 2017, 4:50 PM
 */

#include <cstdlib>
#include <iostream>
#include <valarray>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "LUTs.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

     if(argc != 3){
        cout << "Too few/many arguments, need 2 " << endl;
        return 0;
    }
    
    
    //Get the inputs and open the files
//
    ifstream simulationOut;
    simulationOut.open(argv[1]);
    if(!simulationOut.is_open()) return 1;
    
    ifstream constraints;
    constraints.open(argv[2]);
    if(!constraints.is_open()) return 1;

    int numLUTs;
    int numSinks;
    
    //get the number of sinks and intermediates
    simulationOut.ignore(1000, ':');
    simulationOut >> numSinks;
    simulationOut.ignore(1000, ':');
    simulationOut >> numLUTs;
    if(simulationOut.peek() == '\n') simulationOut.get();

    
    vector <LUT> LUTs;
    string general;
    
    while(!constraints.eof()){
        getline(constraints, general);
        if(general[0] == 'L'){ //"LUT"
            LUT newLUT;
            LUTs.push_back(newLUT);
        }
        else if(general[0] >= '0' && general[0] <= '9'){
            unsigned a;
            stringstream tempStream(general);
            tempStream >> a;
            LUTs[LUTs.size() - 1].inputs.push_back(a);
        }
    }
    
    wire* LUTWires = new wire[numLUTs];
    wire* sinkWires = new wire[numSinks];
    
    int Error = 0;
    bool multipleLUTchanges = false;
    bool failedToChange = false;
    int Reset = 0;
    string line;
    
    while(!simulationOut.eof()){
        getline(simulationOut, line); // Cycle line, don't care
        getline(simulationOut, line); // Error line
        stringstream errorLine(line);
        errorLine.ignore(100, ':');
        int tempError;
        errorLine >> tempError;
        //cout << tempError << endl;
        if(tempError != 0) {
            Error = 1;
            cout << "Error, fail flag has gone high" << endl;
        }
        getline(simulationOut, line); //Reset line
        stringstream resetLine(line);
        resetLine.ignore(100, ':');
        resetLine >> Reset;
        //cout << Reset << endl;
        getline(simulationOut, line); // Sinks:
        //cout << "sinks" << endl;
        for(int i = 0; i < numSinks; i++){
            getline(simulationOut, line);
            stringstream sinkLine(line);
            sinkWires[i].prevValue = sinkWires[i].currentValue;
            if(sinkLine.peek() == 'z') 
                sinkWires[i].currentValue = -1;
            else
                sinkLine >> sinkWires[i].currentValue;
            //cout << sinkWires[i].currentValue << endl;
            if(sinkWires[i].prevValue != sinkWires[i].currentValue && 
                    !sinkWires[i].highImpedence){
                sinkWires[i].cycleCount++;
                sinkWires[i].justChanged = true;
            }
            else 
                sinkWires[i].justChanged = false;
            if(sinkWires[i].currentValue != -1)
                sinkWires[i].highImpedence = false;
        }
        
        getline(simulationOut, line); //Intermediates:
        //cout << "Intermediates" << endl;
        for(int i = 0; i < numLUTs; i++){
            getline(simulationOut, line);
            stringstream LUTLine(line);
            LUTWires[i].prevValue = LUTWires[i].currentValue;
            if(LUTLine.peek() == 'z') 
                LUTWires[i].currentValue = -1;
            else
                LUTLine >> LUTWires[i].currentValue;
            //cout << LUTWires[i].currentValue << endl;
            if(LUTWires[i].prevValue != LUTWires[i].currentValue && 
                    !LUTWires[i].highImpedence){
                LUTWires[i].cycleCount++;
                LUTWires[i].justChanged = true;
            }
            else
                LUTWires[i].justChanged = false;
            if(LUTWires[i].currentValue != -1)
                LUTWires[i].highImpedence = false;
        }
        
        //get for multiple LUT changes
        if (!Reset) {
            for (int i = 0; i < LUTs.size(); i++) {
                int numChanges = 0;
                for (int j = 0; j < LUTs[i].inputs.size(); j++) {
                    if (LUTWires[LUTs[i].inputs[j]].justChanged)
                        numChanges++;
                }
                if (numChanges > 1) {
                    multipleLUTchanges = true;
                    cout << "Error, more than one LUT input changed in a single cycle" << endl;
                    cout << "\t LUT with input IDs: ";
                    for (int j = 0; j < LUTs[i].inputs.size(); j++) {
                        cout << LUTs[i].inputs[j] << " ";
                    }
                    cout << endl;
                }
            }
        }
        
    }
    
    for(int i = 0; i < numSinks; i++){
        if(sinkWires[i].cycleCount < 2 && !sinkWires[i].highImpedence){
            failedToChange = true;
            cout << "Error, sink with ID " << i << " failed to change value twice in simulation" << endl;
        }
    }
    for(int i = 0; i < numLUTs; i++){
        if(LUTWires[i].cycleCount < 2 && !LUTWires[i].highImpedence){
            failedToChange = true;
            cout << "Error, intermediate with ID " << i << " failed to change value twice in simulation" << endl;
        }
    }
    
    if(failedToChange && multipleLUTchanges && Error)
        return 7;
    if(failedToChange && multipleLUTchanges)
        return 6;
    if(failedToChange && Error)
        return 5;
    if(multipleLUTchanges && Error)
        return 4;
    if(multipleLUTchanges) 
        return 3;
    if(failedToChange)
        return 2;
    if(Error)
        return 1;
    
    
    return 0;
}

