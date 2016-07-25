/************************************************************************
                        Ocultador.h.cpp - Copyright rodrigo

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

This file was generated on dom oct 12 2008 at 16:58:06
The original location of this file is /home/rodrigo/uml-generated-code/Ocultador.cpp
**************************************************************************/

#include "Ocultador.h"

// Constructors/Destructors
//

Ocultador::Ocultador ( ) {
}

Ocultador::~Ocultador ( ) { }

//
// Methods
//

//Recibo particion + imagen y devuelvo true si pudo ocultar. False cualquier otra salida
bool Ocultador::ocultar(Informacion* informacion, Portador* archivo){
    Particion* particion;

    if (dynamic_cast<Particion*>(informacion) != NULL){
        particion = (Particion*) informacion;
        string cadena = particion->getDatos();
        unsigned int offset = particion->getOffsetImagen();
        if (archivo->guardarDatos(cadena,offset) != true)
            return false;
    }else
        if (dynamic_cast<Texto*>(informacion) != NULL){
            Texto* texto = (Texto*) informacion;
            list<Almacenable*>lista = texto->obtenerCopiaParticiones();
                if (lista.empty())
                    return false;
                list<Almacenable*>::iterator it = lista.begin();
                while (it != lista.end()){
                    particion = (Particion*)*it;
                    string cadena = particion->getDatos();
                    unsigned int offset = particion->getOffsetImagen();
                    if (archivo->guardarDatos(cadena,offset) != true)
                        return false;
                    it++;
                }
            lista.clear();
        }
    return true;
}

bool Ocultador::desocultar(Informacion* informacion, Portador* archivo){
    Particion* particion = dynamic_cast<Particion*>(informacion);

    if ( particion ){
        particion = (Particion*) informacion;
        particion->setDatos(archivo->recuperarDatos(particion->getOffsetImagen(),particion->getTamano()*BITS_BYTE));
    }else {
        Texto* texto = (Texto*) informacion;
        list<Almacenable*>lista = texto->obtenerCopiaParticiones();
        if (lista.empty())
            return false;
        list<Almacenable*>::iterator it = lista.begin();
        while (it != lista.end()){
            particion = (Particion*)*it;
            particion->setDatos(archivo->recuperarDatos(particion->getOffsetImagen(),particion->getTamano()*BITS_BYTE));
            it++;
        }
        lista.clear();
    }
    return true;
}

bool Ocultador::ocultar(map<Informacion*,Portador*> mapaInserciones){
    Informacion* informacion;
    Portador* portador;
    map<Informacion*,Portador*>::iterator it = mapaInserciones.begin();
    while (it != mapaInserciones.end()){
        informacion = (*it).first;
        portador = (*it).second;
        if (!this->ocultar(informacion, portador))
            return false;
        it++;
    }
    return true;
}

bool Ocultador::desOcultar(map<Informacion*,Portador*> mapaInserciones){
    Informacion* informacion;
    Portador* portador;
    map<Informacion*,Portador*>::iterator it = mapaInserciones.begin();
    while (it != mapaInserciones.end()){
        informacion = (*it).first;
        portador = (*it).second;
        if (!this->desocultar(informacion, portador))
            return false;
        it++;
    }
    return true;
}

