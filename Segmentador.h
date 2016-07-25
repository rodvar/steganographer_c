/************************************************************************
                        Segmentador.h.h - Copyright rodrigo

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
The original location of this file is /home/rodrigo/uml-generated-code/Segmentador.h
**************************************************************************/


#ifndef SEGMENTADOR_H
#define SEGMENTADOR_H
#include <list>
#include <vector>
#include <string>
#include "Texto.h"
#include "Portador.h"



using namespace std;
/**
  * Clase que representa un sergmentador el cual permite segmentar/unir informacion
  *
  */

class Segmentador
{
private:
    static const unsigned short int maxParticiones = 10; //VER

public:
	/**
	 * Empty Constructor
	 */
	Segmentador ( );

	/**
	 * Empty Destructor
	 */
	~Segmentador ( );

    /**
     * Segmenta en "cantParticiones" particiones un texto pasado por parametro
     * @param texto : texto origen
     * @param cantParticiones: cantidad de particiones a realizar
     * @return true si pudo hacerlo, false si la cantidad de particiones es invalida
     */
    bool particionar (Texto* texto, unsigned short int cantParticiones);

    /**
     * Segmenta en "cantParticiones" particiones un texto pasado por parametro
     * @param texto : texto origen
     * @param recursos: Recursos disponibles para guardar la informacion de cada particion.
     *                  Se genera una particion por recurso disponible.
     * @return true si pudo hacerlo, false si la cantidad de particiones es invalida
     */
    bool particionar (Texto* texto, list<Almacenable*> recursos);

    /**
     * Toma la lista de particiones del texto, y las junta en una unica particion recomponiendo
     * asi el texto original.
     * @param texto Es el texto a recomponer
     * @return true si pudo hacerlo, false si la cantidad de segmentos actual de texto es invalida
     */
    bool desParticionar (Texto* texto);

private:

    /**
	 * Particiona una particion en la cantidad pedida por parametro. Si tiene exito, la particion
	 * original queda inutilizable.
	 * @param particion : particion origen
	 * @param cantidad: cantidad de particiones a realizar
	 * @return Lista de Particiones (la ultima particion puede tener mayor tamaño si no era posible
	 * que todas queden del mismo tamaño)
	 * IMPORTANTE: si cantParticiones > maxParticiones devuelve la lista vacia
	 */
	list<Almacenable*> particionar (Particion* particion, unsigned short int cantParticiones);

	/**
	 * Particiona una particion en la cantidad pedida por parametro. Si tiene exito, la particion
	 * original queda inutilizable.
	 * @param particion : particion origen
	 * @param recursos: son los recursos con los que disponemos para guardar la informacion de cada particion.
	 *        La obtendra una particion por cada recuso disponible.
	 * @return Lista de Particiones (la ultima particion puede tener mayor tamaño si no era posible
	 * que todas queden del mismo tamaño)
	 * IMPORTANTE: si cantParticiones > maxParticiones devuelve la lista vacia
	 */
	list<Almacenable*> particionar (Particion* particion,list<Almacenable*> recursos);


	/**
	 * A partir de la lista de particiones, devuelve una sola que es la reconstruccion
	 * del particionado. Se eliminan las particiones q componen la lista excepto la primera q es la devuelta
	 * @param listaParticiones : Lista de particiones
	 * @return una particion con la concatenacion de todos los datos que tenia cada particion de la lista */
	Particion* desParticionar (list<Almacenable*> listaParticiones);
};

#endif // SEGMENTADOR_H
