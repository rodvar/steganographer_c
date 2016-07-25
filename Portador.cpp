/************************************************************************
                        Portador.h.cpp - Copyright rodrigo

Here you can write a license for your code, some comments or any other
information you want to have in your generated code. To to this simply
configure the "headings" directory in uml to point to a directory
where you have your heading files.

or you can just replace the contents of this file with your own.
If you want to do this, this file is located at

/usr/share/apps/umbrello/headings/heading.cpp

-->Code Generators searches for heading files based on the file extension
   i.e. it will look for a file name ending in ".h" to include in C++ header
   files, and for a file name ending in ".java" to include in all generated
   java code.
   If you name the file "heading.<extension>", Code Generator will always
   choose this file even if there are other files with the same extension in the
   directory. If you name the file something else, it must be the only one with that
   extension in the directory to guarantee that Code Generator will choose it.

you can use variables in your heading files which are replaced at generation
time. possible variables are : author, date, time, filename and filepath.
just write %variable_name%

This file was generated on sáb oct 11 2008 at 17:15:49
The original location of this file is /home/rodrigo/uml-generated-code/Portador.cpp
**************************************************************************/

#include "Portador.h"

void Portador::setBytesLibres(const unsigned int bytes){
    this->bytesLibres = bytes;
}

void Portador::setEspacioDisponible(const unsigned int espacio){
    this->espacioDisponible = espacio;
}

void Portador::setOffsetInicial(const unsigned int offset){
    this->offsetInicial = offset;
}

unsigned int Portador::getOffsetInicial(){
    return this->offsetInicial;
}

unsigned int Portador::getBytesLibres ( ){
    return this->bytesLibres;
}


unsigned int Portador::getTamano ( ){
    return this->tamano;
}

void Portador::setTamano (unsigned int tamano ) {
    this->tamano = tamano;
}

unsigned int Portador::getEspacioDisponible ( ){
    return this->espacioDisponible;
}

float Portador::calcularRendimiento(){
    return (this->espacioDisponible / this->tamano);
}

float Portador::calcularPorcentajeLibre(){
    return ((this->espacioDisponible != 0) ? (this->bytesLibres / this->espacioDisponible) : 0);
}


