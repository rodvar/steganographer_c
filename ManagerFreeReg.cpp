/************************************************************************
                        ManagerFreeReg.cpp

**************************************************************************/

#include "ManagerFreeReg.h"

// Constructors/Destructors
//

ManagerFreeReg::ManagerFreeReg ( ) {
}

ManagerFreeReg::~ManagerFreeReg ( ) { }

//
// Methods
//

void ManagerFreeReg::addLibre(int pos) {
    this->libres.push_back(pos);
}

int ManagerFreeReg::getLibre() {
    int pos;
    this->libres.sort();
    if(this->libres.size()==0) return -1;
    pos = this->libres.front();
    this->libres.pop_front();
    return pos;
}


int ManagerFreeReg::getUltimo() {
    int pos;
    this->libres.sort();
    if(this->libres.size()==0) return -1;
    pos = this->libres.back();
    return pos;
}

list<int> ManagerFreeReg::getLista(){
    this->libres.sort();
    return this->libres;
}
