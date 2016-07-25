/************************************************************************
                        Directorio.h.h - Copyright rodrigo

Here you can write a license for your code, some comments or any other
information you want to have in your generated code. To to this simply
configure the "headings" directory in uml to point to a directory
where you have your heading files.

or you can just replace the contents of this file with your own.
If you want to do this, this file is located at

/usr/share/apps/umbrello/headings/heading.h

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
The original location of this file is /home/rodrigo/uml-generated-code/Directorio.h
**************************************************************************/


#ifndef DIRECTORIO_H
#define DIRECTORIO_H
#include "Portador.h"
#include "ArchivoImagenBMP.h"
#include "ArchivoImagenGIF.h"
#include "ArchivoImagenJPG.h"
#include "ArchivoImagenPNG.h"
#include <dirent.h>
#include <list>
#include <string>
#include <sys/stat.h>



using namespace std;

/**
  * Clase que representa un directorio de imagenes del sistema.
  */

class Directorio : public Almacenable
{
private:
	string ruta;
	unsigned long int espacioLibre; // Es el espacio disponible en todo el directorio
	list<Almacenable*> portadores;

public:

	// Constructors/Destructors
	//
	/**
	 * Constructor de directorio
	 * @param ruta es la ruta del directorio en el sistema operativo.
	 */
	Directorio (const unsigned int id, const string ruta);

    /**
	 * Constructor de directorio
	 */
	Directorio ();

	/**
	 * Empty Destructor
	 */
	virtual ~Directorio ( );

	/**
	 * @return bool Devuelve falso si el portador ya existe en el directorio
	 * @param  portador Es el portador a agregar
	 */
	bool agregarPortador (Portador* portador );


	/**
	 * @return bool Devuelve falso si el portador no existe en el directorio
	 * @param  portador Es el portador a eliminar
	 */
	bool borrarPortador (Portador* portador );


	/**
	 * @return bool verdadero si existe, falso si no
	 * @param  id es el id del portador a chequear
	 */
	bool existePortador (const unsigned int idPortador );


	/**
	 * @return Portador Devuelve el portador correspondiente a id. NULL si no lo encuentra
	 * @param  id el id del portador
	 */
	Portador* obtenerPortador (const unsigned int id );

    /**
	 * @return La coleccion de portadores del directorio
	 */
	list<Almacenable*>* obtenerPortadores ( );

	/**
	 * @return UNA COPIA de La coleccion de portadores
	 * Sus datos NO estan copiados, solo la lista
	 */
	list<Almacenable*> obtenerCopiaPortadores ( );

	/**
	 * @return Devuelve en el mejor caso un unico portador segun algoritmo de BEST_FIT
	 *         Si no es posible, devuelve la cantidad de portadores necesarios para cubrir
     *         el tamano pasado por parametro , ordenados en forma DESCENDENTE por tamaño
	 */
	list<Almacenable*> obtenerCopiaPortadores ( const unsigned int tamano );


	/**
	 * @return unsigned int la cantidad de portadores que posee este directorio
	 */
	unsigned int obtenerCantidadPortadores ( );

	/**
	 * Chequea los Portadores del directorio actualizando el atributo de espacio libre
	 */
    void actualizarEspacioLibre();

	/**
	 * @return Devuelve el espacio libre total del directorio
	 */
	unsigned long int getEspacioLibre ( );

	/**
	 * Setea el espacio libre total del directorio
	 */
	void setEspacioLibre ( const unsigned long int espacioLibre );

	/**
	 * Get the value of m_ruta
	 * @return the value of m_ruta
	 */
	string getRuta ( );

	/**
	 * Permite cambiar la ruta
	 */
    void setRuta(string ruta);

	/**
	 * Set the value of m_portadores (Destruye la lista actual y graba esta copia
	 * @param new_var the new value of m_portadores
	 */
	void setPortadores ( list<Almacenable*> new_var );

	/**
	 * Calcula el rendimiento en base al espacio disponible para guardar informacion
	 * y el tamaño total del directorio (donde cada atributo es la suma de los atributos
	 * de sus portadores
     * @return ESPACIO_DISPONIBLE/ESPACIO_TOTAL
     */
     float calcularRendimiento();

    /**
	 * Calcula el rendimiento en base al espacio disponible para guardar informacion
	 * y el tamaño total del directorio (donde cada atributo es la suma de los atributos
	 * de sus portadores
     * @return ESPACIO_DISPONIBLE/ESPACIO_TOTAL
     */
     float calcularUso();


private:
    /**
     * Limpia la lista de portadores del directorio
     */
     void eliminarPortadores();

     /**
      * Crea una lista de portadores ordenada por EspacioLibre DESCENDENTE a partir de la
      * lista de portadores del directorio
      */
     list<Almacenable*> obtenerListaEspacioLibreDescendente();

};

#endif // DIRECTORIO_H
