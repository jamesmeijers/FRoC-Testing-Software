/* 
 * Project: Device Under Test wrapper creator for FRoC output files
 * Author: James Meijers
 *
 * Created on May 16, 2017, 3:48 PM
 */

#include <cstdlib>
#include <iostream>
#include <valarray>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#define CYCLES_TO_RUN 10000

using namespace std;

/*
 * This program takes two inputs
 * 1. A path the a FRoC top module file
 * 2. A path to the corresponding controller module file
 * It produces four outputs
 * 1. A DUT wrapper for the top file to access internal signals in simulation
 * 2. A constants file used for simulation
 * 3. A constraints file that tells the simulation parser what signals enter the same LUTs
 * 4. A labels file that can convert from numerical iterators to signal names
 * The resulting files can be combined with other pre-canned files to run simulation
 */
int main(int argc, char** argv) {
    
    //declare and open the file streams
    ifstream FRoC;
    ifstream controller;
    ofstream DUT;
    ofstream constants;
    ofstream constraints;
    ofstream labels;
    
    if(argc != 7){
        cout << "Error, require 6 input arguments" << endl;
        return -1;
    }
    
    
    
    /*FRoC.open("FRoC.v");
    controller.open("controller.v");
    DUT.open("DUT.sv");
    constants.open("constants.sv");
    constraints.open("constraints.txt");
    labels.open("labels.txt");
    */
    
    FRoC.open(argv[1]);
    controller.open(argv[2]);
    DUT.open(argv[3]);
    constants.open(argv[4]);
    constraints.open(argv[5]);
    labels.open(argv[6]);
    
    //Lists of the sinks and intermediates, strings for their names
    vector <string> sinks;
    vector <string> intermediates;
    
    //general use string for parsing
    string current;
    
    //The state number at which any number equal or greater than it represents a reset state
    int resetValue;
    //The length of the state bus
    int stateLength;
    bool resetValueFound = false;
    
    //get the reset value and state length from the controller file
    while (!resetValueFound) {
        controller.ignore(1000, 'R'); //looking for R in "Reset_phase_0"
        getline(controller, current, ' ');
        if (current[0] == 'e' && current[1] == 's' && current[11] == '0') {
            controller.ignore(3, ' '); //ignore equal sign
            controller >> stateLength; //get the x value of x'dy
            controller.ignore(10, 'd'); //ignore the d in x'dy
            controller >> resetValue;  //get the y of x'dy
            resetValueFound = true;
            //reset value now holds the lowest state number at which a reset occurs
            //any state values equal or higher than this number are reset phases
            //(or the done phase)
        }
    }

    
    
    
    
    // <editor-fold>
    
    //parsing FRoC file to get signal names for sinks and intermediates
    
    bool done = false;
    
    FRoC.ignore(1000, '/');
    FRoC.ignore(1000, '/');
    FRoC.ignore(10000, '/'); //at //Sinks//
    FRoC.ignore(1000, '\n');
    while (FRoC.peek() == ' ') FRoC.get(); //clear whitespace
    getline(FRoC, current, '\n'); //get line with sinks on it, held in current
    replace(current.begin(), current.end(), ',', ' '); //replace all commas and semicolons with spaces
    replace(current.begin(), current.end(), ';', ' ');
    stringstream sinkStream(current);
    while (!sinkStream.fail() && !sinkStream.eof()){
        getline(sinkStream, current, ' ');
        sinks.push_back(current); //add the found sink to the list
        while (sinkStream.peek() == ' ' && !sinkStream.eof()) sinkStream.get(); //remove whitespace
    }
    
    FRoC.ignore(1000, '\n'); //ignore //Sources line
    FRoC.ignore(1000, '\n'); //ignore line listing sources
    FRoC.ignore(1000, '\n'); //ignore //intermediate signals line
    FRoC.ignore(1000, ' '); //ignore "wire"
    
    getline(FRoC, current, '\n'); //get line with sources on it, held in current
    replace(current.begin(), current.end(), ',', ' '); //replace all commas and semicolons with spaces
    replace(current.begin(), current.end(), ';', ' ');
    stringstream LUTstream(current);
    while (!LUTstream.fail() && !LUTstream.eof()){
        getline(LUTstream, current, ' ');
        intermediates.push_back(current); //add the values found to the list
        while (LUTstream.peek() == ' ' && !LUTstream.eof()) LUTstream.get();
    }
    
    FRoC.ignore(1000, ' '); //ignore "wire"
    
    if (FRoC.peek() == 'P') { //ensure that cout signals exist
        getline(FRoC, current, '\n'); //get line with sources cout on it, held in current
        replace(current.begin(), current.end(), ',', ' '); //replace all commas and semicolons with spaces
        replace(current.begin(), current.end(), ';', ' ');
        stringstream coutStream(current);
        while (!coutStream.fail() && !coutStream.eof()) {
            getline(coutStream, current, ' ');
            intermediates.push_back(current);
            while (coutStream.peek() == ' ' && !coutStream.eof()) coutStream.get();
        }
    }
    //output labels
    labels << "This file contains a list of signals tracked by the simulation platform" << endl;
    labels << "What follows is a list of reference numbers followed by the name of the wire they relate to" << endl;
    labels << "sinks:" << endl;
    //list all the sinks
    for(unsigned i = 0; i < sinks.size(); i++){
        labels << i << " : " << sinks[i] << endl;
    }
    labels << "intermediates:" << endl;
    //list all the intermediates
    for(unsigned i = 0; i < intermediates.size(); i++){
        labels << i << " : " << intermediates[i] << endl;
    }
    
    
    //Write DUT File
    DUT << "module DUT(input CLK, input reset, input start_test, output error, output fuck, signal_bus_if bus);";
    DUT << endl << endl;
    DUT << "\ttop FRoC(CLK, reset, start_test, error, fuck );";
    DUT << endl << endl;
    DUT << "\talways@(posedge CLK) begin";
    DUT << endl; 
    DUT << "\t\tbus.state = DUT.FRoC.control0.state;";
    DUT << endl << endl;
    for(unsigned i = 0; i < sinks.size(); i++){
        DUT << "\t\tbus.sinks[" << i << "] = DUT.FRoC." << sinks[i] << ';' << endl;
    }
    DUT << endl;
    for(unsigned i = 0; i < intermediates.size(); i++){
        DUT << "\t\tbus.intermediates[" << i << "] = DUT.FRoC." << intermediates[i] << ';' << endl;
    }
    
    DUT << endl;
    DUT << "\tend" << endl << endl;
    DUT << "endmodule" << endl;
    
    cout << "DUT made" << endl;
    
    //Write Constants file
    constants << "`define MAX_CYCLE " << CYCLES_TO_RUN << endl;
    constants << "`define NUMBER_OF_INTERMEDIATES " << intermediates.size() << endl;
    constants << "`define NUMBER_OF_SINKS " << sinks.size() << endl;
    constants << "`define MIN_RESET_VALUE " << resetValue << endl;
    constants << "`define STATE_LENGTH " << stateLength << endl;
    constants << "`define OUT_FILE_LOCATION " << "\"~/Documents\"" << endl;
    
    cout << "constants made " << endl;
    
    // </editor-fold>
    
    //Verilog files are made, now need to make path constraints
    
    while (!FRoC.eof()) {
        FRoC.ignore(1000, 'c'); //skip to the next c
        getline(FRoC, current, '\n'); //get the next word
        if (current[0] == 'y' && current[1] == 'c' && current[11] == 'c') { //if it is the declaration of a LUT module
            constraints << "LUT" << endl;
            getline(FRoC, current, ';'); //get the entire module declaration in current
            stringstream LUTstream(current);
            LUTstream.ignore(1000, '.');
            while (!LUTstream.eof()) {
                
                if (LUTstream.peek() == 'd') {
                    LUTstream.ignore(1000, '(');
                    getline(LUTstream, current, ')'); //get the name of the input to the LUT
                    //remove trailing whitespace
                    while(current[(current.size() - 1)] == ' ') current = current.substr(0, current.size()-1);
                    
                    vector<string>::iterator iter = find(intermediates.begin(), intermediates.end(), current);
                    if(iter != intermediates.end()){
                        constraints << iter - intermediates.begin() << endl;
                    }
                } else if (LUTstream.peek() == 'c') {
                    LUTstream.get();
                    if (LUTstream.peek() == 'i') { //cin rather than cout or combout
                        LUTstream.ignore(1000, '(');
                        getline(LUTstream, current, ')'); //get the name of the input to the LUT
                        //remove trailing whitespace
                        while(current[(current.size() - 1)] == ' ') current = current.substr(0, current.size()-1);
                        
                        vector<string>::iterator iter = find(intermediates.begin(), intermediates.end(), current);
                        if (iter != intermediates.end()) {
                            constraints << iter - intermediates.begin() << endl;
                        }
                    }
                }
                LUTstream.ignore(1000, '.');
            }
            constraints << endl;
        }
    }
    
    
    cout << "constraints made" << endl;
    
    
    
    
    
    
    
    return 0;
}

