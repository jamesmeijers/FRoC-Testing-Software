/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "routing.h"

//pieces of hardware are equal if they have the same type and ID
bool hardware:: operator==(hardware b){
    if(b.typeID != typeID || b.ID != ID) return false;
    return true;
}

//Two paths are the same if they have the same hardware in the same order
bool path:: operator==(path b){
    if(b.hardwarePath.size() != hardwarePath.size()) return false;
    for(unsigned i = 0; i < hardwarePath.size(); i++){
        if(!(b.hardwarePath[i] == hardwarePath[i])) return false;
    }
    
    return true;
}