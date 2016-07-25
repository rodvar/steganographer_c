#include "ManagerFreeRegVars.h"

/**
* Empty Constructor
*/
ManagerFreeRegVars::ManagerFreeRegVars ( ){

}

/**
* Empty Destructor
*/
ManagerFreeRegVars::~ManagerFreeRegVars ( ){
    this->eliminarListaLibres();
}

list<ManagerFreeRegVars::RegLibre*> ManagerFreeRegVars::getLista(){
    return this->listaLibres;
}

vector<short int> ManagerFreeRegVars::obtenerVariablesHermanos(unsigned short int offset){
    vector<short int> hermanos(2);
    list<ManagerFreeRegVars::RegLibre*>::iterator it = this->listaLibres.begin();
    RegLibre* anterior = NULL;
    RegLibre* siguiente = NULL;

    bool encontrado = false;
    while ((!encontrado) && ( it != this->listaLibres.end())){
        if ( ((RegLibre*)*it)->offset == offset){
            encontrado = true;
            if (it != this->listaLibres.begin()){
                anterior = *(--it);
                it++;
            }
            if (it != this->listaLibres.end())
                siguiente = *(++it);
        }
        else
            it++;
    }

    hermanos[0] = -1;
    hermanos[1] = -1;
    if (encontrado){
        if (anterior)
            hermanos[0] = anterior->offset;
        if (siguiente)
            hermanos[1] = siguiente->offset;
    }
    return hermanos;
}

void ManagerFreeRegVars::insertarOrdenadoEnListaVariables(unsigned short int offset, unsigned short int size){
    RegLibre* registro = new RegLibre();
    registro->offset = offset;
    registro->size = size;
        bool encontrado = false;
    list<RegLibre*>::iterator it = this->listaLibres.begin();
    if(this->listaLibres.empty()){
        this->listaLibres.insert(it, registro);
        }
    else{
        while ((!encontrado) && (it != this->listaLibres.end())){
            if (((RegLibre*)*it)->offset > offset){
                this->listaLibres.insert(it, registro);
                encontrado = true;
                }

            else
                it++;
        }
        if(it == this->listaLibres.end() && !encontrado)
            this->listaLibres.push_back(registro);
    }
}

/**
*
* @pre  :
* @post : Devuelve la primera posicion donde entre el registro variable que se quiera guardar, sino devuelve -1
* @return int
*/
short int ManagerFreeRegVars::obtenerVariablesPrimerLibre(unsigned short int size){
    short int offset = -1;
    list<RegLibre*>::iterator it = this->listaLibres.begin();
    while ((offset == -1) && ( it != this->listaLibres.end())){
        if ( ((RegLibre*)*it)->size >= size){
                offset = ((RegLibre*)*it)->offset;
                delete *it;
                it = this->listaLibres.erase(it);
        }
        else
            it++;
     }

    return offset;
}



// private methods
void ManagerFreeRegVars::eliminarListaLibres(){

        list<RegLibre*>::iterator it = this->listaLibres.begin();
        while (it != this->listaLibres.end()){
            delete *it;
            it = this->listaLibres.erase(it);
        }
}
