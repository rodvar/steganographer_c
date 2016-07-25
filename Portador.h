/************************************************************************
                        Portador.h.h - Copyright rodrigo

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
The original location of this file is /home/rodrigo/uml-generated-code/Portador.h
**************************************************************************/


#ifndef PORTADOR_H
#define PORTADOR_H

#include "Almacenable.h"
#include "Informacion.h"


/**
  * Clase abstracta que define el comportamiento de un Portador de informacion e implementa
  * metodos basicos para evaluar su rendimiento
  */

class Portador : public Almacenable
{
protected:
    unsigned int bytesLibres; // La cantidad de bytesLibres del total de espacio disponible
    unsigned int tamano; // en bytes
    unsigned int espacioDisponible; // es el espacio total que podemos utilizar
    unsigned int offsetInicial; // cuantos bytes hay q leer para llegar a donde esta la informacion

public:
    // Constructor
    Portador(){
        this->bytesLibres = 0;
        this->tamano = 0;
        this->espacioDisponible = 0;
        this->offsetInicial = 0;
    }

    // Destructor librado a clases hijas
    virtual ~Portador(){
    }

    /**
     * Setea el atributo bytesLibres con bytes
     */
    void setBytesLibres(const unsigned int bytes);

    /**
     * Setea el atributo espacioDisponible con espacio
     */
    void setEspacioDisponible(const unsigned int espacio);

    /**
     * Setea el offset inicial a la informacion en el archivo
     */
    void setOffsetInicial(const unsigned int offset);

    /**
     * Get the value of offsetInicial
     */
    unsigned int getOffsetInicial();

    /**
	 * Get the value of bytesLibres
	 * @return the value of bytesLibres
	 */
	unsigned int getBytesLibres ( );

	/**
     * Get the value of tamaño
     * @return the value of tamaño
     */
    unsigned int getTamano ( );

	/**
     * Sete el valor de tamano
     */
    void setTamano (unsigned int tamano );

	/**
     * Get the value of espacioDisponible
     * @return the value of espacioDisponible
     */
    unsigned int getEspacioDisponible ( );

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
	 * @return El porcentaje del archivo que es utilizable para guardar informacion del total
	 *         del tamaño del archivon
	 */
	float calcularRendimiento();

	/**
	 * @return El porcentaje del archivo que se encuentra libre del total disponible para
     *         guarar informacion
	 */
	float calcularPorcentajeLibre();

};

#endif // PORTADOR_H
