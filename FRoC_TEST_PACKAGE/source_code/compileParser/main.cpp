/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: jmeijers
 *
 * Created on May 19, 2017, 10:55 AM
 */

#include <cstdlib>

using namespace std;
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <valarray>
#include <sstream>

/*
 * 
 */
#define WATCHED_CONSTRAINTS 73
int main(int argc, char** argv) {
    int constraintWarningIDs[WATCHED_CONSTRAINTS] = {170120, 170106, 170105, 143002, 43004, 143011, 
    143000, 143001, 176510, 170117, 13163, 13164, 12692, 13159, 143012, 12909, 12768,
    12929, 170114, 170104, 170115, 170109, 170110, 170108,  170102, 170103, 170099, 
    170063, 170078, 171008, 332040, 332011, 176224,  169258, 176206, 170122,  143003, 
    170121, 170111, 170096, 170098, 170095, 188000, 171156, 171102, 171103, 170113, 
    171120, 171116, 175019, 175020, 170082, 170082, 171165, 14505, 184033, 12869, 
    185012,  15553, 143008, 170116, 170090, 170091, 170093, 170092, 170100, 170101,
    170119, 11691, 11698, 11690, 170085, 170097};
    
    
    if(argc != 2){
        cout << "Too few/many arguments, need 1 " << endl;
        return 0;
    }
    
    
    
    
    ifstream compilation;
    compilation.open(argv[1]);
    string line;
    unsigned returnValue = 0;
    
    while(!compilation.eof()){
        getline(compilation, line);
        stringstream lineStream(line);
        if(line[0] == 'E'){ //Error
            cout << line << endl; //print the error
            returnValue = 1; //compilation failed
        }
        if(line[0] == 'C'){ //critical warning
            //cout << line << endl; //print critical warning
        }
        else { //Info or otherwise
            lineStream.ignore(1000, '('); //ignore to number
            int id;
            lineStream >> id;
            if (!lineStream.fail()) {
                int* begin = constraintWarningIDs;
                int* end = begin + WATCHED_CONSTRAINTS;
                if (find(begin, end, id) != end)
                    cout << line << endl; //print the info if it has to do with constraints
            }
        }
    }
    
    
    return returnValue;
}

