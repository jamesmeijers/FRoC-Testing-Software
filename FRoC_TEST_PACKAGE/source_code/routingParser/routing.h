/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   routing.h
 * Author: jmeijers
 *
 * Created on May 9, 2017, 4:20 PM
 */

#ifndef ROUTING_H
#define ROUTING_H

#include <string>
#include <vector>

class hardware {
public:
    //0 = LE_BUFFER
    //1 = LOCAL_INTERCONNECT
    //2 = LOCAL_LINE
    //3 = R
    //4 = C
    int typeID;
    std:: string ID;
    bool operator==(hardware b);
};


class path{
public:
    std:: vector <hardware> hardwarePath;
    bool matchFound;
    unsigned id;
    
    bool operator==(path b);
    
};


#endif /* ROUTING_H */

