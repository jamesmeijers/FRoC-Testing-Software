/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: jmeijers
 *
 * Created on May 19, 2017, 1:46 PM
 */
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <valarray>
#include <sstream>
#include <cstdlib>

using namespace std;


/*
 * 
 */
int main(int argc, char** argv) {

    if(argc != 2){
        cout << "Too few/many arguments, need 1 " << endl;
        return 0;
    }
    
    int numPaths;
    string temp;
    string temp2;
    
    ifstream summaryFile;
    summaryFile.open(argv[1]);
    
    while(!summaryFile.eof()){
        temp2 = temp;
        getline(summaryFile, temp);
    }
    
    stringstream lastLine(temp2);
    
    int tempInt;
    
    while(!lastLine.fail()){
        numPaths = tempInt;
        
        lastLine >> tempInt;
        
    }
    cout << numPaths << " Paths are Tested in FRoC Project" << endl;
    return numPaths;
    
    return 0;
}

