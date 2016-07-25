/************************************************************************
                        ArchivoImagen.h.h - Copyright rodrigo

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

This file was generated on sáb oct 11 2008 at 17:17:25
The original location of this file is /home/rodrigo/uml-generated-code/ArchivoImagen.h
**************************************************************************/


#ifndef ARCHIVOIMAGEN_H
#define ARCHIVOIMAGEN_H
#include <Magick++.h>
#include <math.h>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include "Portador.h"
#include "OperacionesBinarias.h"




using namespace std;
using namespace Magick;

/**
  * Clase que representa un archivo de imagen
  */

/******************************* Abstract Class ****************************
ArchivoImagen does not have any pure virtual methods, but its author
  defined it as an abstract class, so you should not use it directly.
  Inherit from it instead and create only objects from the derived classes
*****************************************************************************/

class ArchivoImagen : public Portador
{
protected:
	unsigned int idDirectorio;
	string nombre;

public:

    /**
     * Constructor de Archivo de Imagen
     * @param id Es el identificador de este archivo en los archivos de registros
     * @param idDirectorio El id del directorio al cual pertenece el archivo
     * @param nombre Es el nombre del archivo sin su ruta (ej: "imagen.bmp")
     */
    ArchivoImagen(const unsigned int id, unsigned int idDirectorio, const string nombre, const unsigned int tamano);

    /**Destructor**/
    virtual ~ArchivoImagen(){
    }

    /**
     * El id del directorio donde se encuentra el archivo
     */
    unsigned int getIdDirectorio();

    /**
     * El id del directorio donde se encuentra el archivo
     */
    void setIdDirectorio(unsigned int idDirectorio);


    /**
     * El nombre del archivo incluyendo su extensión.
     */
    string getNombre();

	/**
	 * Set the value of m_nombre
	 */
    void setNombre (string nombre );

    /**
	 * @return bool True si es paosible, false en caso contrario
	 * @param  cadena Es la cadena de datos a ocultar en el portador
	 * @param  offset La cantidad de bytes a leer hasta encontrar el primero modificable
	 */
	virtual bool guardarDatos (string cadena, const unsigned int offset) = 0;


	/**
	 * Recupera la cantidad de bitsInformacion especificada. Completa con CEROS si no es multiplo de
	 * BITS_BYTE (8)
	 * @return string Los datos recuperados. Devuelve un string VACIO si no pudo hacerlo
	 * @param  offset La cantidad de bytes a leer hasta encontrar el primero que contiene los datos
	 * @param bitsInformacion Es la cantidad de bits a leer a partir del offset para conformar los datos
	 */
	virtual string recuperarDatos (const unsigned int offset, const unsigned int bitsDatos ) = 0;

    /**
     * Devuelve el nombre completo del archivo "RUTA/nombre.extension"
     */
    string obtenerNombreCompleto();

    /**
     * Realiza el preprocesamiento, de ser necesario, de la imagen para poder ser esteganografiada
     */
     virtual void preProcesar();

protected:

    /**
     * Calcula el tamaño del archivo
	 * @return unsigned int El tamaño del archivo
	 */
	virtual unsigned int calcularTamano ( string nombreCompleto );

};

#endif // ARCHIVOIMAGEN_H
