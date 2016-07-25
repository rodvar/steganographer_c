/************************************************************************
                        Texto.cpp

**************************************************************************/

#include "Texto.h"

// Constructors/Destructors
//
Texto::Texto (const string nombre, const unsigned int tamano) {
    this->id = 0;
    this->nombre = nombre;
    this->tamano = tamano;
    this->comprimida = false;
    this->particiones.clear();
}

Texto::Texto (const unsigned int id, const string nombre, const unsigned int tamano ) {
    this->id = id;
    this->nombre = nombre;
    this->tamano = tamano;
    this->comprimida = false;
    this->particiones.clear();
}

Texto::~Texto ( ) {
    this->eliminarListaParticiones();
}

string Texto::getNombre ( ) {
	return this->nombre;
}

void Texto::setNombre (string nombre ) {
	this->nombre = nombre;
}

unsigned int Texto::getNroParticiones ( ) {
	return this->particiones.size();
}

unsigned int Texto::getCantidadParticionesOriginal(){
    return this->cantidadParticionesOriginales;
}

void Texto::setCantidadParticionesOriginal(unsigned int cantidad){
    this->cantidadParticionesOriginales = cantidad;
}

bool Texto::esValido(){
    return (this->getNroParticiones() == this->getCantidadParticionesOriginal());
}

list<Almacenable *>* Texto::obtenerParticiones ( ) {
	return (&this->particiones);
}

list<Almacenable*> Texto::obtenerCopiaParticiones ( ){
    return this->particiones;
}

void Texto::setParticionesList (list<Almacenable*> lista){
    this->eliminarListaParticiones();
    this->particiones = lista;
}


void Texto::agregarParticion (Particion* particion ) {
    this->particiones.push_back(particion);
}

void Texto ::agregarParticionInicio(Particion* particion){
    this->particiones.push_front(particion);
    }

bool Texto::eliminarParticion (unsigned int idParticion ){
    list<Almacenable *>::iterator it = this->particiones.begin();
    bool encontrado = false;
    while ((!encontrado) && (it != this->particiones.end())){
        if (((Particion*)*it)->getId() == idParticion){
            delete (*it);
            it = this->particiones.erase(it);
            encontrado = true;
        }
        else
            it++;
     }
     return encontrado;
}


Particion* Texto::obtenerParticion (unsigned int idParticion ) {
    list<Almacenable *>::iterator it = this->particiones.begin();
    Almacenable* encontrado = NULL;
    while ((!encontrado) && (it != this->particiones.end())){
        if (((Particion*)*it)->getId() == idParticion)
            encontrado = *it;
        it++;
     }
     return (Particion*)encontrado;
}

// private:
void Texto::eliminarListaParticiones(){
    list<Almacenable *>::iterator it = this->particiones.begin();
    while ( it != this->particiones.end()){
        delete (*it);
        it = this->particiones.erase(it);
     }
}

bool Texto::usaPortador(unsigned int idImagen){
    bool usa = false;
    list<Almacenable*>::iterator it = this->particiones.begin();
    while ((!usa) && (it != this->particiones.end())){
        if (((Particion*)*it)->getIdImagen() == idImagen)
            usa = true;
        else
            it++;
    }
    return usa;
}

void Texto::removerParticiones(unsigned int idImagen){
    list<Almacenable *>::iterator it = this->particiones.begin();
    while ( it != this->particiones.end()){
        if (((Particion*)*it)->getIdImagen() == idImagen){
            delete (*it);
            it = this->particiones.erase(it);
        }
        else
            it++;
     }
}


list<Almacenable*> Texto::obtenerParticiones(const unsigned int idImagen){
    list<Almacenable*>::iterator it = this->particiones.begin();
    list<Almacenable*> resultado;
    while ( it != this->particiones.end()){
        if (((Particion*)*it)->getIdImagen() == idImagen)
            resultado.push_back(*it);
        it++;
    }
    return resultado;
}

/**
 * Crea y devuelve una copia del texto
 */
Texto* Texto::copiar(){
    Texto* texto = new Texto(this->id, this->nombre, this->tamano);
    list<Almacenable*> particiones = this->obtenerCopiaParticiones();
    list<Almacenable*>::iterator it = particiones.begin();
    while (it != particiones.end()){
        *it = ((Particion*)*it)->copiar();
        it++;
    }
    texto->setParticionesList(particiones);
    texto->setPersistido(this->esPersistido());
    texto->setComprimida(this->comprimida);
    return texto;
}
