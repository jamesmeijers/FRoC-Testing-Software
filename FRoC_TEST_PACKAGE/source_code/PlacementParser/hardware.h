/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   hardware.h
 * Author: jmeijers
 *
 * Created on May 9, 2017, 1:55 PM
 */

#ifndef HARDWARE_H
#define HARDWARE_H

#include <vector>


class LUT {
public:
    //location of LUT
   int X;
   int Y;
   int N;
   
   bool operator==(LUT b);
};

class FF {
public:
    //Location of Flip Flop
    int X;
    int Y;
    int N;
    
    bool operator==(FF b);
};

class path {
public:
    //ID of path as appears in meta-parser output
    unsigned id;
    
    //start and end registers of the path
    FF startReg;
    FF endReg;
    
    //Vector of all LUTs in the path
    std:: vector <LUT> LUTPath;
    
    //true if path b has the same LUTs and FF in the same order
    bool operator==(path b);
};


#endif /* HARDWARE_H */

