/************************************************************************
                        Directorio.h.cpp - Copyright rodrigo

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
The original location of this file is /home/rodrigo/uml-generated-code/Directorio.cpp
**************************************************************************/

#include "Directorio.h"
#include "Motor.h"


// Constructors/Destructors
//

// Constructors/Destructors
//

Directorio::Directorio() {}


Directorio::Directorio (const unsigned int id, const string ruta)
{
    this->id = id;
    this->portadores.clear();
    this->espacioLibre = 0;
    this->ruta = ruta;
    if (ruta.find_last_of('/') != (ruta.size() - 1))
        this->ruta += "/";
}

Directorio::~Directorio ( )
{
    this->eliminarPortadores();
}

/**
 * Get the value of this->ruta
 * @return the value of this->ruta
 */
string Directorio::getRuta ( )
{
    return this->ruta;
}

/**
 * Set the value of this->portadores
 * @param new_var the new value of this->portadores
 */
void Directorio::setPortadores ( list<Almacenable*> new_var )
{
    this->eliminarPortadores();
    this->portadores = new_var;
}

/**
 * @return bool Devuelve falso si el portador ya existe en el directorio
 * @param  portador
 */
bool Directorio::agregarPortador (Portador* portador )
{
    bool agregado = false;
    if (!this->existePortador(portador->getId()))
    {
        this->portadores.push_back(portador);
        agregado = true;
    }
    return agregado;
}


/**
 * @return bool
 * @param  portador
 */
bool Directorio::borrarPortador (Portador* portador )
{
    list<Almacenable *>::iterator it = this->portadores.begin();
    bool encontrado = false;
    while ((!encontrado) && ( it != this->portadores.end()))
    {
        if ( ((Portador*)*it) == portador)
        {
            delete (*it);
            it = this->portadores.erase(it);
            encontrado = true;
        }
        else
            it++;
    }
    return encontrado;
}

/**
 * @return bool verdadero si existe, falso si no
 * @param  id es el id del portador a chequear
 */
bool Directorio::existePortador (const unsigned int idPortador )
{
    bool encontrado = false;
    if (this->obtenerPortador(idPortador))
        encontrado = true;
    return encontrado;
}

/**
 * @return Portador
 * @param  ID
 */
Portador* Directorio::obtenerPortador (const unsigned int id )
{
    list<Almacenable *>::iterator it = this->portadores.begin();
    Portador* portador = NULL;
    while ((!portador) && ( it != this->portadores.end()))
    {
        if ( ((Portador*)*it)->getId() == id)
        {
            portador = (Portador*)*it;
        }
        it++;
    }
    return portador;
}


list<Almacenable*>* Directorio::obtenerPortadores ( )
{
    return (&this->portadores);
}

list<Almacenable*> Directorio::obtenerCopiaPortadores ( )
{
    return (this->portadores);
}

list<Almacenable*> Directorio::obtenerCopiaPortadores ( const unsigned int tamano )
{
    list<Almacenable*> lista = this->obtenerListaEspacioLibreDescendente();
    list<Almacenable*> resultado;
    list<Almacenable *>::iterator it = lista.begin();
    Portador* portador;
    unsigned int acumulado = 0;
    unsigned int bytesLibres = 0;
    // unsigned int offset;
    while ((it != lista.end()) && (acumulado < tamano))
    {
        portador = (Portador*)*it;
        bytesLibres = portador->getBytesLibres();
        // Descarto los portadores que no tengan espacio
        if ( bytesLibres > 0)
        {
            if (bytesLibres >= tamano){
                acumulado = bytesLibres;
                resultado.push_back(*it);
            }
            //offset = Motor::getInstancia()->getProcesadorTextos()->calcularOffsetEscritura(portador->getId(), portador->getOffsetInicial());
            if (bytesLibres < tamano)
            {
                acumulado += bytesLibres;
                resultado.push_back(*it);
            }
        }
        it++;
    }
    return resultado;
}

//
unsigned int Directorio::obtenerCantidadPortadores ( )
{
    return this->portadores.size();
}

void Directorio::actualizarEspacioLibre()
{
    this->espacioLibre = 0;
    list<Almacenable*>::iterator it = this->portadores.begin();
    while ( it != this->portadores.end())
    {
        this->espacioLibre += (((Portador*)*it)->getBytesLibres());
        it++;
    }
}

unsigned long int Directorio::getEspacioLibre ( )
{
    return this->espacioLibre;
}

void Directorio::setEspacioLibre ( const unsigned long int espacioLibre )
{
    this->espacioLibre = espacioLibre;
}


float Directorio::calcularRendimiento()
{
    unsigned long int total = 0;
    unsigned long int disponible = 0;
    list<Almacenable *>::iterator it = this->portadores.begin();
    while ( it != this->portadores.end())
    {
        total += (((Portador*)*it)->getTamano());
        disponible += ((Portador*)*it)->getEspacioDisponible();
        it++;
    }
    if (total == 0)
    {
        total = 1;
        disponible = 0;
    }
    return 100*((float)disponible/total);
}


float Directorio::calcularUso()
{
    unsigned long int total = 0;
    unsigned long int libre = 0;
    list<Almacenable *>::iterator it = this->portadores.begin();
    while ( it != this->portadores.end())
    {
        total += (((Portador*)*it)->getEspacioDisponible());
        libre += ((Portador*)*it)->getBytesLibres();
        it++;
    }
    if (total == 0)
    {
        total = 1;
        libre = 1;
    }
    return 100*(1-(float)libre/total);
}

/* Elimina la lista de portadores del directorio*/
void Directorio::eliminarPortadores()
{
    list<Almacenable*>::iterator it = this->portadores.begin();
    while ( it != this->portadores.end() )
    {
        delete (*it);
        it = this->portadores.erase(it);
    }
}


bool ordenarEspacioLibreDescendente (Almacenable* res1, Almacenable* res2)
{
    if ((((Portador*)res1)->getBytesLibres()) > (((Portador*)res2)->getBytesLibres()))
        return true;
    else
        return false;
}

list<Almacenable*> Directorio::obtenerListaEspacioLibreDescendente()
{
    list<Almacenable*> resultado;
    resultado = this->obtenerCopiaPortadores();
    resultado.sort(ordenarEspacioLibreDescendente);
    return resultado;
}

void Directorio::setRuta(string ruta)
{
    this->ruta = ruta;
}
