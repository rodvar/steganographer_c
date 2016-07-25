/************************************************************************
                        Segmentador.h.cpp - Copyright rodrigo

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
The original location of this file is /home/rodrigo/uml-generated-code/Segmentador.cpp
**************************************************************************/

#include "Segmentador.h"
#include "Motor.h"

// Constructors/Destructors
//

Segmentador::Segmentador ( ) {
}

Segmentador::~Segmentador ( ) { }


bool Segmentador::particionar (Texto* texto, unsigned short int cantParticiones){
    bool particionado = false;
    list<Almacenable*> particiones;
    if ((texto->getNroParticiones() == 1) && (cantParticiones > 1)){
        particiones = this->particionar((Particion*)texto->obtenerCopiaParticiones().front(), cantParticiones);
        if (!particiones.empty()){
            texto->setParticionesList(particiones);
            particionado = true;
        }
        particiones.clear();
    }
    return particionado;
}

bool Segmentador::particionar (Texto* texto, list<Almacenable*> recursos){
    bool particionado = false;
    list<Almacenable*> particiones;
    if((texto->getNroParticiones() == 1)&& (recursos.size() >= 1)){
        particiones = this->particionar((Particion*)texto->obtenerCopiaParticiones().front(), recursos);
        if (!particiones.empty()){
                texto->setParticionesList(particiones);
                particionado = true;
            }
            particiones.clear();
        }

return particionado;
}


/**
 * Toma la lista de particiones del texto, y las junta en una unica particion recomponiendo
 * asi el texto original.
 * @param texto Es el texto a recomponer
 * @return true si pudo hacerlo, false si la cantidad de segmentos actual de texto es invalida
 */
bool Segmentador::desParticionar (Texto* texto){
    bool particionado = false;
    Particion* particion;
    unsigned short int cantidadParticiones = texto->getNroParticiones();
    if ((cantidadParticiones > 1) /*&& (cantidadParticiones < this->maxParticiones)*/){
        list<Almacenable*> listaParticiones = texto->obtenerCopiaParticiones();
        particion = this->desParticionar(listaParticiones);
        if (particion){
            texto->agregarParticionInicio(particion);
            particionado = true;
        }
    }
    return particionado;
}


//private

list<Almacenable*> Segmentador::particionar (Particion* particion, unsigned short int cantParticiones){
    //checkeo para no particionar mas de lo definido como maxima cant de particiones
    list<Almacenable*> listaParticiones;
    //if (cantParticiones <= this->maxParticiones){
        Particion* Pi;
        string datos = particion->getDatos();
        unsigned int tamano = datos.size() / cantParticiones;
        unsigned int resto = datos.size() % cantParticiones;

        string di;
        int indice =0;
        unsigned int id, i;

        for (i = 1; i < cantParticiones ; i ++){
            di=datos.substr(indice, tamano);
            indice += tamano;
            id = Motor::getInstancia()->getAdministradorRecursos()->getIndexador(PARTICIONES)->proximoId();
            Pi = new Particion(di, id, di.size(), particion->getIdImagen(), particion->getIdTexto(), i);
            listaParticiones.push_back(Pi);
        }
        // el ultimo elemento del vector puede quedar mas grande que los demas si size mod cantParticiones != 0
        di=datos.substr(indice, datos.size()+ resto);
        id = Motor::getInstancia()->getAdministradorRecursos()->getIndexador(PARTICIONES)->proximoId();
        Pi = new Particion(di, id, di.size(), particion->getIdImagen(),particion->getIdTexto(), ++i);
        listaParticiones.push_back(Pi);
    //}
    return listaParticiones;

}

list<Almacenable*> Segmentador::particionar (Particion* particion,list<Almacenable*> recursos){
    //checkeo para no particionar mas de lo definido como maxima cant de particiones
    list<Almacenable*> listaParticiones;
    int indice =0;
    string datoi;
    unsigned int id;

    //if (recursos.size() <= this->maxParticiones){
        unsigned int i = 1;
        list<Almacenable*>::iterator itRecursos = recursos.begin();
        string datos = particion->getDatos();
        unsigned int tamanioRestante = datos.size();
        while(itRecursos!=recursos.end()){
            Portador* recurso = (Portador*)*itRecursos;
            Particion* Pi;
            unsigned int tamanio = recurso->getBytesLibres();//datos.size() / cantParticiones;
            if(tamanio>tamanioRestante){
                tamanio = tamanioRestante;
            }

            datoi=datos.substr(indice, tamanio);
            indice += tamanio;
            id = 0; //Motor::getInstancia()->getAdministradorRecursos()->getIndexador(PARTICIONES)->proximoId();
            Pi = new Particion(datoi, id, datoi.size(), recurso->getId(), particion->getIdTexto(), i);
            listaParticiones.push_back(Pi);
            tamanioRestante-=tamanio;
            itRecursos++;
            i ++;
        }
    //}
    return listaParticiones;
}



Particion* Segmentador::desParticionar (list<Almacenable*> listaParticiones){
    list<Almacenable*>::iterator it= listaParticiones.begin();
    Particion* salida = new Particion();
    Particion* particion = NULL;
    string datos="";

    while (it!= listaParticiones.end()){
        particion = (Particion*)*it;
        datos += particion->getDatos();
        it = listaParticiones.erase(it);
    }
    salida->setDatos(datos);
    return salida;
}
