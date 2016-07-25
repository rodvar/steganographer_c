/************************************************************************
                        Ocultador.h.h - Copyright rodrigo

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

This file was generated on dom oct 12 2008 at 16:58:06
The original location of this file is /home/rodrigo/uml-generated-code/Ocultador.h
**************************************************************************/


#ifndef OCULTADOR_H
#define OCULTADOR_H

#include <string>
#include <list>
#include "Particion.h"
#include "Texto.h"
#include "ArchivoImagen.h"

/**
  * Clase que representa un ocultador el cual permite ocutlar/desocultar informacion segun
  * los principios de la ciencia Esteganografia.
  */

class Ocultador
{
public:

	// Constructors/Destructors
	//
	/**
	 * Empty Constructor
	 */
	Ocultador ( );

	/**
	 * Empty Destructor
	 */
	virtual ~Ocultador ( );

    /**
	 * @return bool True si es paosible, false en caso contrario
	 * @param  particion Es el obejeto Particion quien incluye el offset y cadena a ocultar
	 * @param  portador El el objeto portador que "porta" a la imagen donde se ocultaran los datos contenidos en "cadena"
	 */
    bool ocultar(Informacion* info, Portador* archivo);

    /**
	 * @return bool True si es paosible, false en caso contrario
	 * @param  particion Es el obejeto Particion que se grabara el mensaje desocultado
	 * @param  portador El el objeto portador que "porta" offset y tamano a desocultar
	 */
    bool desocultar(Informacion* info, Portador* archivo);

    /**
     * Oculta cada clave del mapa en su valor
     */
    bool ocultar(map<Informacion*,Portador*> mapaInserciones);

    /**
     * Recupera cada clave del mapa de su valor
     */
    bool desOcultar(map<Informacion*,Portador*> mapaInserciones);

};

#endif // OCULTADOR_H
