#include "RegVariable.h"

RegVariable::RegVariable(string nombre){
    this->nombre = nombre;
    this->size = this->nombre.length();
    this->libre = 0;
    this->estado = 'O';
    this->siguiente = 0;
}

RegVariable::~RegVariable(){

}

void RegVariable::setNombre(string n){
    if(n.size() <= this->nombre.size()){
        this->size = n.size();
        this->nombre = n;
        this->estado = 'O';
    }else{
        throw eRegLleno(1);
    }
}

void RegVariable::setEstado(bool estado){
    this->estado = estado;
}

unsigned short int RegVariable::getSize(){
    return this->size;
}

std::string RegVariable::getNombre(){
    return this->nombre;
}

char RegVariable::getEstado(){
    return this->estado;
}

string RegVariable::getCamposConcat(){
    string resultado = "O";
    resultado.append(OperacionesBinarias::shortAHexString(this->getSize()));
    resultado.append(OperacionesBinarias::shortAHexString(this->getLibre()));
    resultado.append(OperacionesBinarias::shortAHexString(this->getSiguiente()));
    resultado.append(this->getNombre());
    return resultado;
}

unsigned short int RegVariable::getSiguiente(){
    return this->siguiente;
}


void RegVariable::setSiguiente(unsigned short int siguiente){
    this->siguiente = siguiente;
}

unsigned short int RegVariable::getLibre(){
    return this->libre;
    }

void RegVariable::setLibre(unsigned short int libre){
    this->libre = libre;
    }

