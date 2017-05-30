/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "hardware.h"

//luts are the same if their coordinates are the same
bool LUT:: operator==(LUT b){
    if(X == b.X && Y == b.Y && N == b.N && input == b.input) return true;
    else return false;
}

//flip flops are the same if their coordinates are the same
bool FF:: operator==(FF b){
    if(X == b.X && Y == b.Y && N == b.N) return true;
    else return false;
}

//paths are the same if they have all the same hardware in the same order
bool path:: operator==(path b){
    if(!(startReg == b.startReg)) return false;
    if(!(endReg == b.endReg)) return false;
    if(!(LUTPath.size() == b.LUTPath.size())) return false;
    
    
    for(unsigned i = 0; i < LUTPath.size(); i++){
        if(!(LUTPath[i] == b.LUTPath[i])) return false;
    }
    
    return true;
}