/************************************************************************
                        ProcesadorTextos.h.h - Copyright rodrigo

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

This file was generated on sáb oct 11 2008 at 17:21:24
The original location of this file is /home/rodrigo/uml-generated-code/ProcesadorTextos.h
**************************************************************************/


#ifndef PROCESADORTEXTOS_H
#define PROCESADORTEXTOS_H

#include <string>
#include <list>
#include <fstream>

#include "Texto.h"

using namespace std;

/**
  * class ProcesadorTextos
  */

class ProcesadorTextos
{
private:
    list<Almacenable*> textoslist;

public:
	/**
	 * Empty Constructor
	 */
	ProcesadorTextos ( );

	/**
	 * Empty Destructor
	 */
	virtual ~ProcesadorTextos ( );

	/**
     * Crea un Texto con el nombre  pasado por parametro y lo agrega a la lista de
     * Texto si no existia previamente.
     * Si ya existe, devuelve False.
     * @return bool
     * @param  nombre Agrega un archivo a la lista de archivos
     */
	void addTextos ( Texto* add_object );

	/**
	 * Levanta un texto del FS y lo devuelve
	 * @return El texto agregado. NULL si no pudo crearlo
	 */
	Texto* crearTexto (string nombre );

	/**
	 * Elimina de la lista de Textos, el Texto correspondiente al idTexto pasado por
	 * parametro, si lo encuentra.
	 *
	 * @return bool Si no lo encuentra devuelve False.
	 * @param  idTexto El id del texto a eliminar
	 */
	bool removerTexto (unsigned int idTexto );

	/**
	 * Elimina de la lista de Textos, el Texto correspondiente al nombre pasado por
	 * parametro, si lo encuentra.
	 * Si no lo encuentra devuelve False.
	 * @return bool Si no lo encuentra devuelve False.
	 * @param  nombre El nombre completo del texto a eliminar (/ruta/nombre.extension)
	 */
	bool removerTexto (string nombre );


	/**
	 * Devuelve el Texto correspondiente al idTexto pasado por parametro si lo
	 * encuentra.
	 * @return Texto Obtiene el Texto pedido
	 * @param  idTexto El id deltexto a obtener
	 */
	Texto* obtenerTexto (unsigned int idTexto );

	/**
	 * Devuelve el Texto correspondiente al nombre pasado por parametro si lo
	 * encuentra.
	 * @return Texto Obtiene el Texto pedido
	 * @param  nombre El nombre deltexto a obtener
	 */
	Texto* obtenerTexto (string nombre );

	/**
	 * @return Calcula el offset donde es posible comenzar a escribir en la imagen de id
	 *         pasado por parametro.
	 */
	unsigned int calcularOffsetEscritura(const unsigned int idImagen, const unsigned int offsetInicial);

    /**
     * Carga la lista de textos consultando la estructura administrativa para su trabajo en
     * memoria mientras el sistema esta en ejecucion
     */
    void cargarListaTextos ( );

    /**
    *Verifica si ya existe cargado un texto con el nombre pasado.
    *@return true si ya existe, false si no.
    *@param  nombre del texto que deseo verificar.
    */
    bool existeTexto (string nombre);

    /**
    *Devuelve la cantidad de textos existentes.
    **/

    unsigned int obtenerCantidadTextos();

    /**
    *Devuelve los Textos del procesador.
    **/
    list<Almacenable*>* obtenerTextos();

    /**
    *Reemplaza la lista de textos por la pasada por parametro.
    **/
    void setTextos ( list<Almacenable*> new_var );

    /**
    *Devuelve los Textos del procesador.
    **/
    list<Almacenable*> obtenerCopiaTextos();

    /**
     * Devuelve una lista con todos los textos q tengan alguna particion cuyo
     * idImagen sea el pasado por parametro
     */
    list<Almacenable*> obtenerTextos(const unsigned int idImagen);

    /**
     * Graba el contenido del texto en la ruta pasada por parametro
     */
    bool grabar(Texto* texto, const string ruta);


private:
	/**
	 * Devuelve el size del archivo identificado por el file descriptor.
	 * @return int
	 * @param  texto File descriptor
	 */
	unsigned int calcularTamano (Texto* texto );

	/**
	 * Para cada texto, agrega las particiones correspondientes al idImagen ordenadas
	 * por offset ASCENDENTE
	 */
	list<Almacenable*> generarListaParticionesOrdenadaOffset(const unsigned int idImagen);

	/**
	 * Elimina correctamente la lista de textos
	 */
	void eliminarListaTextos();


};

#endif // PROCESADORTEXTOS_H
