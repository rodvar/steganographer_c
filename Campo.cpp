#include "Campo.h"


/**
* Campo: Constructor de Campo sin parametros.
*/
Campo::Campo(){
    }

/**
* Campo: Constructor de Campo que recibe un nombre de tipo string el cual sirve para
* dejarlo en un estado valido para operar con el.
* Inicialmente el valor asignado al campo es 0 (cero).
* @param  nombre
*/
Campo::Campo(std::string nombre){
    this->nombre = nombre;
    this->valor = 0;
    }

/**
* getValor: Devuelve el valor que guarda el campo de tipo unsigned int
* Inicialmente el valor asignado al campo es 0 (cero).
* @return unsigned int
*/
unsigned int Campo::getValor(){
    return this->valor;
    }

/**
* setValor: Setea el valor que guarda el campo de tipo unsigned int
* El valor debe ser unsigned int
* @return void
* @param unsigned int
*/
void Campo::setValor(unsigned int valor){
    this->valor = valor;
    }
/**
* getNombre: Devuelve el nombre del campo de tipo std::string
* @return std::string
*/
std::string Campo::getNombre(){
    return this->nombre;
    }
/**
* ~Campo: Destructor de la clase Campo.
*/
Campo::~Campo(){}
