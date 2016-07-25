/************************************************************************
                        Soporte.h.h - Copyright rodrigo

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
The original location of this file is /home/rodrigo/uml-generated-code/Soporte.h
**************************************************************************/


#ifndef SOPORTE_H
#define SOPORTE_H

#include <string>
#include <list>

class Directorio;
class Almacenable;
class Portador;

using namespace std;


/**
  * Clase que representa el Soporte de directorios del sistema, donde se almacenan los textos
  * ocultos por el mismo.
  */

class Soporte
{
private:
	list<Almacenable*> directorios;
	unsigned long espacioLibre;
	pthread_mutex_t directoriosMutex; // Exclusion para threads para directorios

public:

	// Constructors/Destructors
	//
	/**
	 * Empty Constructor
	 */
	Soporte ( );

	/**
	 * Empty Destructor
	 */
	virtual ~Soporte ( );

	/**
	 * Agrega el directorio al soporte
	 * @param  directorio El directorio a agregar
	 */
    void agregarDirectorio (Directorio* directorio );


	/**
	 * Elimina el directorio del soporte
	 * @param  directorio El directorio a eliminar
	 */
	void borrarDirectorio (const string ruta );

	/**
	 * Elimina el directorio del soporte
	 * @param  directorio El directorio a eliminar
	 */
	void borrarDirectorio (const unsigned int id );


	/**
	 * @return bool TRUE si existe el directorio en el soporte, false en caso contrario
	 * @param  ruta la ruta del directorio a verififcar
	 */
	bool existeDirectorio (string ruta );


	/**
	 * @return Directorio
	 * @param  ID_Directorio
	 */
	Directorio* obtenerDirectorio (const unsigned int ID_Directorio );

	Directorio* obtenerDirectorio (const string ruta );

	/**
     * @return Recorre todos los directorios en busca del portador cuyo id es el pasado
     *         por parametro
     */
    Portador* obtenerPortador (const unsigned int id ) ;


	/**
	 * @return unsigned int
	 */
	unsigned int obtenerCantidadDirectorios ( );

    /**
	 * @return La Lista de directorios del soporte
	 */
	list<Almacenable*>* obtenerDirectorios ( );

	/**
	 * @return UNA COPIA de la lista de directorios del soporte
	 * Sus datos NO estan copiados, solo la lista.
	 */
	list<Almacenable*> obtenerCopiaDirectorios ( );


	/**
	 * @return Devuelve todos los directorios cuyo espacioLibre sea mayor o igual al tamaño
	 * pasado por parametro, ordenados en forma DESCENDENTE por tamaño
	 */
	list<Almacenable*> obtenerCopiaDirectorios ( const unsigned int tamano );

	/**
	 * @return Ordena la lista en forma DESCENDENTE por tamaño, devolviendo los directorios
	 * necesarios para que cubra el tamano pasado por parametro
	 */
	list<Almacenable*> obtenerCopiaDirectoriosParaCompletar( const unsigned int tamano );


	/**
	 * Set the value of directorios. Elimina la lista actual!
	 * @param new_var the new value of directorios
	 */
	void setDirectorios ( list<Almacenable*> new_var );


	/**
	 * Chequea los Directorios del soporte actualizando el atributo de espacio libre
	 */
    void actualizarEspacioLibre();

	/**
	 * @return Devuelve el espacio libre total del soporte
	 */
	unsigned long int getEspacioLibre ( );

	/**
	 * Setea el espacio libre total del soporte
	 */
	void setEspacioLibre ( const unsigned long int espacioLibre );

	/**
	 * Incrementa el espacio libre en la cantidad pasada por parametro.
	 */
	void agregarEspacioLibre (const unsigned long int masEspacioLibre );

    /**
	 * Decrementa el espacio libre en la cantidad pasada por parametro.
	 */
	void reducirEspacioLibre (const unsigned long int menosEspacioLibre);

	/**
	 * @return Devuelve el directorio que contiene a la imagen cuyo id es el pasado por parametro.
	 */
	Directorio* obtenerDirectorioContenedor( const unsigned int idImagen );


private:

    /**
     * Limpia la lista de portadores del directorio
     */
     void eliminarDirectorios();

     /**
      * Crea una lista de directorios ordenada por EspacioLibre DESCENDENTE a partir de la
      * lista de directorios del soporte
      */
     list<Almacenable*> obtenerListaEspacioLibreDescendente();



};

#endif // SOPORTE_H
