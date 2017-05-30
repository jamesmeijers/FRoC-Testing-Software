/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LUTs.h
 * Author: jmeijers
 *
 * Created on May 23, 2017, 4:56 PM
 */

#ifndef LUTS_H
#define LUTS_H

#include <vector>


class LUT{
public:
    std:: vector <unsigned> inputs;
};

class wire{
public:
    int currentValue = -1;
    int prevValue = -1;
    int cycleCount = 0;
    bool highImpedence = true;
    bool justChanged = false;
};

#endif /* LUTS_H */

