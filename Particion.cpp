/************************************************************************
                        Particion.h
**************************************************************************/

#include "Particion.h"

Particion::Particion (){
    this->id = 0;
    this->tamano = 0;
    this->idImagen = 0;
    this->idTexto = 0;
    this->orden =1;
    this->comprimida = false;
}

Particion::Particion (string datos, unsigned int id, unsigned int tamano, unsigned int idImagen, unsigned int idTexto){
    this->id = id;
    this->tamano = tamano;
    this->idImagen = idImagen;
    this->datos = datos;
    this->idTexto = idTexto;
    this->orden =1;
    this->comprimida = false;
}

Particion::Particion (string datos, unsigned int id, unsigned int tamano, unsigned int idImagen, unsigned int idTexto, unsigned int orden){
    this->id = id;
    this->tamano = tamano;
    this->idImagen = idImagen;
    this->datos = datos;
    this->idTexto = idTexto;
    this->orden =orden;
    this->comprimida = false;
}

Particion::Particion (string datos, unsigned int id, unsigned int tamano, unsigned int idTexto){
    this->id = id;
    this->tamano = tamano;
    this->idImagen = 0;
    this->idTexto = idTexto;
    this->datos = datos;
    this->orden =1;
    this->comprimida = false;
}

Particion::~Particion ( ){
    // TODO
}

void Particion::setIdImagen ( unsigned int new_var ) {
	this->idImagen = new_var;
}

unsigned int Particion::getIdTexto ( ){
    return this->idTexto;
}

unsigned int Particion::getIdImagen ( ) {
	return this->idImagen;
}

void Particion::setOffsetImagen ( const unsigned int new_var ) {
	this->offsetImagen = new_var;
}

unsigned int Particion::getOffsetImagen ( ) {
	return this->offsetImagen;
}

void Particion::setDatos(string datos){
    this->datos = datos;
    this->tamano = datos.size();
}

string Particion::getDatos(){
    return this->datos;
}

unsigned int Particion::getOrden ( ){
    return this->orden;
}

void Particion::setOrden ( unsigned int new_var ){
    this->orden = new_var;
}

void Particion::setIdTexto ( unsigned int new_var ){
    this->idTexto = new_var;
}

Particion* Particion::copiar(){
    Particion* particion = new Particion(this->datos, this->id, this->tamano,
                                    this->idImagen, this->idTexto, this->orden);
    particion->setOffsetImagen(this->offsetImagen);
    particion->setPersistido(this->esPersistido());
    particion->setComprimida(this->comprimida);
    return particion;
}
