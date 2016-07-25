/************************************************************************
                        Soporte.h.cpp - Copyright rodrigo

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
The original location of this file is /home/rodrigo/uml-generated-code/Soporte.cpp
**************************************************************************/

#include "Soporte.h"
#include "Directorio.h"

// Constructor // Destructuctor
Soporte::Soporte ( ){
    this->directorios.clear();
    this->espacioLibre = 0;
}

Soporte::~Soporte ( ){
    this->eliminarDirectorios();
}


void Soporte::agregarDirectorio (Directorio* directorio ) {
    this->directorios.push_back(directorio);
}

// TODO: Analizar la mejora de mantener un orden y hacer busqueda binaria!!!
void Soporte::borrarDirectorio (const string ruta ) {
    Directorio* dir;
    list<Almacenable*>::iterator it = this->directorios.begin();
    bool encontrado = false;
    while ((!encontrado) && ( it != this->directorios.end())){
        if ( ((Directorio*)*it)->getRuta() == ruta) {
            dir = (Directorio*)*it;
            delete dir;
            it = this->directorios.erase(it);
            encontrado = true;
        }
        else
            it++;
     }
}

/**
 * Elimina el directorio del soporte
 * @param  directorio El directorio a eliminar
 */
void Soporte::borrarDirectorio (const unsigned int id ){
    list<Almacenable *>::iterator it = this->directorios.begin();
    bool encontrado = false;
    while ((!encontrado) && ( it != this->directorios.end())){
        if ( ((Directorio*)*it)->getId() == id) {
            delete (*it);
            it = this->directorios.erase(it);
            encontrado = true;
        }
        else
            it++;
     }
}

/**
 * @return bool TRUE si existe el directorio en el soporte, false en caso cotnrario
 * @param  ruta La ruta del directorio a verificar
 */
bool Soporte::existeDirectorio (string ruta ) {
    Directorio* dir = NULL;
    bool existe = false;
    list<Almacenable *>::iterator it =  this->directorios.begin();
    while ( (!existe) && (it != this->directorios.end()) ){
        dir = (Directorio*)*it;
        if (ruta.compare(dir->getRuta()) == 0){
            existe = true;
            }

        it++;
     }

    return existe;
}


/**
 * @return Directorio
 * @param  ID_Directorio
 */
Directorio* Soporte::obtenerDirectorio (const unsigned int id ) {
    pthread_mutex_lock( &this->directoriosMutex );
    list<Almacenable *>::iterator it = this->directorios.begin();
    Directorio* directorio = NULL;
    while ((!directorio) && ( it != this->directorios.end())){
        if ( ((Directorio*)*it)->getId() == id) {
            directorio = (Directorio*)*it;
        }
        it++;
     }
     pthread_mutex_unlock( &this->directoriosMutex );
     return directorio;
}

Directorio* Soporte::obtenerDirectorio(const string ruta ) {
    pthread_mutex_lock( &this->directoriosMutex );
    list<Almacenable *>::iterator it = this->directorios.begin();
    Directorio* dir= NULL;
    while ((!dir) && ( it != this->directorios.end())){
        if ( ((Directorio*)*it)->getRuta() == ruta) {
            dir = (Directorio*)*it;
        }
        it++;
     }
     pthread_mutex_unlock( &this->directoriosMutex );
     return dir;
}

Portador* Soporte::obtenerPortador (const unsigned int id ) {
    pthread_mutex_lock( &this->directoriosMutex );
    list<Almacenable*>::iterator it = this->directorios.begin();
    Portador* portador = NULL;
    Directorio* directorio;
    while ((!portador) && ( it != this->directorios.end())){
        directorio = (Directorio*)*it;
        portador = directorio->obtenerPortador(id);
        it++;
     }
     pthread_mutex_unlock( &this->directoriosMutex );
     return portador;
}



unsigned int Soporte::obtenerCantidadDirectorios ( ) {
    pthread_mutex_lock( &this->directoriosMutex );
    pthread_mutex_unlock( &this->directoriosMutex );
    return this->directorios.size();
}

list<Almacenable*>* Soporte::obtenerDirectorios ( ){
    pthread_mutex_lock( &this->directoriosMutex );
    pthread_mutex_unlock( &this->directoriosMutex );
    return (&this->directorios);
}

list<Almacenable*> Soporte::obtenerCopiaDirectorios ( ) {
    pthread_mutex_lock( &this->directoriosMutex );
    pthread_mutex_unlock( &this->directoriosMutex );
    return this->directorios;
}

list<Almacenable*> Soporte::obtenerCopiaDirectorios ( const unsigned int tamano ){
    pthread_mutex_lock( &this->directoriosMutex );
    list<Almacenable *>::iterator it = this->directorios.begin();
    list<Almacenable *>::iterator itResultado;
    list<Almacenable*> resultado;
    Directorio* directorio;
    bool insertado;
    while ( it != this->directorios.end()){ // insercion ordenada en lista
        directorio = (Directorio*)*it;
        if (directorio->getEspacioLibre() >= tamano){
            if (resultado.empty())
                resultado.push_front(directorio);
            else{
                itResultado = resultado.begin();
                insertado = false;
                while ((!insertado) && (itResultado != resultado.end())){
                    if (((Directorio*)*itResultado)->getEspacioLibre() < directorio->getEspacioLibre()){
                        resultado.insert(itResultado,directorio);
                        insertado = true;
                    }
                    itResultado++;
                }
                if (!insertado)
                    resultado.push_back(directorio);
            }
        }
        it++;
    }
    pthread_mutex_unlock( &this->directoriosMutex );
    return resultado;
}

list<Almacenable*> Soporte::obtenerCopiaDirectoriosParaCompletar( const unsigned int tamano ){
    pthread_mutex_lock( &this->directoriosMutex );
    list<Almacenable*> resultado;
    list<Almacenable*> lista = this->obtenerListaEspacioLibreDescendente();
    list<Almacenable *>::iterator it = lista.begin();
    unsigned long acumulado = 0;

    while ((it != lista.end()) && (acumulado < tamano)){
        acumulado += ((Directorio*)*it)->getEspacioLibre();
        resultado.push_back(*it);
        it++;
    }
    pthread_mutex_unlock( &this->directoriosMutex );
    return resultado;
}


Directorio* Soporte::obtenerDirectorioContenedor( const unsigned int idImagen ){
    pthread_mutex_lock( &this->directoriosMutex );
    list<Almacenable *>::iterator it = this->directorios.begin();
    Directorio* dir = NULL;

    while ((it != this->directorios.end()) && (!dir)){
        if (((Directorio*)*it)->existePortador(idImagen)){
            dir = ((Directorio*)*it);
            }
        it++;
    }
    pthread_mutex_unlock( &this->directoriosMutex );
    return dir;
}

void Soporte::actualizarEspacioLibre(){
    pthread_mutex_lock( &this->directoriosMutex );
    this->espacioLibre = 0;
    list<Almacenable *>::iterator it = this->directorios.begin();
    while ( it != this->directorios.end()){
       this->espacioLibre += (((Directorio*)*it)->getEspacioLibre());
       it++;
    }
    pthread_mutex_unlock( &this->directoriosMutex );
}

unsigned long int Soporte::getEspacioLibre ( ){
    return this->espacioLibre;
}

void Soporte::setDirectorios ( list<Almacenable*> new_var ){
    pthread_mutex_lock( &this->directoriosMutex );
    this->eliminarDirectorios();
    this->directorios = new_var;
    pthread_mutex_unlock( &this->directoriosMutex );
}

void Soporte::setEspacioLibre ( const unsigned long int espacioLibre ){
    this->espacioLibre = espacioLibre;
}

void Soporte::agregarEspacioLibre (const unsigned long int masEspacioLibre ){
    this->espacioLibre+= masEspacioLibre;
}

void Soporte::reducirEspacioLibre (const unsigned long int menosEspacioLibre ){
    this->espacioLibre-= menosEspacioLibre;
}

//private
 /* Elimina la lista de directorios del soporte*/
 void Soporte::eliminarDirectorios(){
    list<Almacenable *>::iterator it = this->directorios.begin();
    while ( it != this->directorios.end() ){
        delete (*it);
        it = this->directorios.erase(it);
     }
 }

bool ordenarEspacioLibre (Almacenable* res1, Almacenable* res2){
    if((((Directorio*)res1)->getEspacioLibre()) < (((Directorio*)res2)->getEspacioLibre()))
        return true;
        else
            return false;
}

list<Almacenable*> Soporte::obtenerListaEspacioLibreDescendente(){
    list<Almacenable*> resultado;
    resultado = this->directorios;
    resultado.sort(ordenarEspacioLibre);
    return resultado;
}




