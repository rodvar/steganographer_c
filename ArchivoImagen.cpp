/************************************************************************
                        ArchivoImagen.cpp
**************************************************************************/

#include "ArchivoImagen.h"
#include "Motor.h"

/**
 * Constructor de Archivo de Imagen
 */
ArchivoImagen::ArchivoImagen(const unsigned int id, unsigned int idDirectorio, const string nombre, const unsigned int tamano){
    this->id = id;
    this->idDirectorio = idDirectorio;
    this->nombre = nombre;
    this->tamano = tamano;
}

unsigned int ArchivoImagen::getIdDirectorio(){
    return this->idDirectorio;
}

void ArchivoImagen::setIdDirectorio(unsigned int idDirectorio){
    this->idDirectorio = idDirectorio;
}

string ArchivoImagen::getNombre(){
    return this->nombre;
}

void ArchivoImagen::setNombre(string nombre){
    this->nombre = nombre;
}

string ArchivoImagen::obtenerNombreCompleto(){
    string nombreCompleto= "";
    if(Motor::getInstancia()->getSoporte()->obtenerDirectorio(this->idDirectorio) != NULL)
        nombreCompleto.append(Motor::getInstancia()->getSoporte()->obtenerDirectorio(this->idDirectorio)->getRuta());
    nombreCompleto.append(this->nombre);
    return nombreCompleto;
}

// private:
// Se utiliza la libreria standart de C++ para calcular el tamaño del archivo
unsigned int ArchivoImagen::calcularTamano ( string nombreCompleto ){
    unsigned int tamano = 0;
    ifstream f;
    f.open(nombreCompleto.c_str(), ios_base::binary | ios_base::in); // TODO: concatenar con dir
    if (f.good() && !f.eof() && f.is_open()) {
        f.seekg(0, ios_base::beg);
        ifstream::pos_type begin_pos = f.tellg();
        f.seekg(0, ios_base::end);
        tamano = static_cast<int>(f.tellg() - begin_pos);
    }

    return tamano;
}

void ArchivoImagen::preProcesar(){
    // Por defecto no se hace nada
}
