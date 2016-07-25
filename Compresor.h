/************************************************************************
                        Compresor.h.h - Copyright rodrigo

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
The original location of this file is /home/rodrigo/uml-generated-code/Compresor.h
**************************************************************************/
#ifndef COMPRESOR_H
#define COMPRESOR_H
#include "OperacionesBinarias.h"
#include <vector>

#define MAX_COMPRIMIBLE 153600

/*
#define LONGVENTANA 255 //antes = 64    2°P - 1
#define P 8 //CANT DE BITS PARA REPRESENTAR LA POS (antes = 6)
#define L 5 // CANT DE BITS PARA REPRESENTAR LA LONG DEL MATCH (antes = 3)
#define MAXMATCH 31// MAXIMO MATCH MATCH (antes = 7)  2°L - 1
#define MINMATCH 3// MINIMO MATCH MATCH (antes = 7) en bytes
*/

using namespace std;

/**
  * Clase que representa un compresor el cual permite comprimir/descomprimir informacion
  * Actualmente implementa el algoritmo LZSS
  */

class Compresor
{
private:
    unsigned short int LONGVENTANA;
    unsigned short int P;
    unsigned short int L;
    unsigned short int MAXMATCH;
    unsigned short int MINMATCH;

public:

	// Constructors/Destructors
	//
	/**
	 * Constructor del compresor
	 * @param bitsPosicion La cantidad de bits que se quieren usar para
	 * @param bitsLongitud
	 * @param minMatch La cantidad de
	 */
	Compresor(const unsigned short int bitsPosicion, const unsigned short int bitsLongitud,const unsigned short int minMatch);

    /**
     * Constructor vacio. Toma valores por defecto para sus atributos.
     * (L = 5, P =8, MIN_MATCH = 3)
     */
    Compresor();

	/**
	 * Empty Destructor
	 */
	 ~Compresor ( );

    // GETTERS
    unsigned short int getBitsLongitud();
    unsigned short int getBitsPosicion();
    unsigned short int getMaximoMatch();
    unsigned short int getMinimoMatch();
    unsigned short int getLongitudVentana();
    // SETTERS
    void setBitsLongitud (const unsigned short int bitsLongitud);
    void setBitsPosicion (const unsigned short int bitsPosicion);
    void setMinimoMatch (const unsigned short int minMatch);

   /**
     * @return la salida comprimida por el metodo LZSS
     * @param input cadena para comprimir
     */

    string comprimir(string input);

   /**
     * @return la salida descomprimida
     * @param input comprimida que se quiere recuperar
     */

    string descomprimir(string input);

private:

    /**
     * Metodo que actualiza la ventana de memoria dejando  en su ultima pos el caracter actual que se estaba procesando
     * y anteriormente todo lo que habia antes de ese caracter
     * @param actual indice del caracter actual de la entrada
     * @param v Ventana de memoria a actualizar
     * @param input cadena que se esta procesando     */
    void actualizarVentana(long int actual , char* v, string input);

    /**
     * Funcion que se encarga de buscar el primer minMatch en la ventana
     * @return el indice de la ventana donde comienza el match
     * @param minMatch string con el minimo math que se esta buscando (de longitud MINMATCH)
     * @param v ventana de memoria
     */
    int buscarMatch(string minMatch, char* v);

    /**
     * Funcion que emite la codificacion binaria para la long, pos tomando en cuenta
     * la cant de bits que se definio para representar a cada uno
     * @return la salida comprimida
     * @param input cadena para comprimir
     */
    void emitir (unsigned int longitud, unsigned int pos);

    /**
     * Funcion que busca el indice del ultimo byte donde comienza la "basura" (o relleno) que agrego el compresor
     * @return el indice (tomando como 0 la pos mas significativa)
     * @param input salida del compresor
     */
    char buscarFin(string input);

    // Se hace esta rusticidad porque pow es muy lento
    unsigned short int potenciaDos (const unsigned short int potencia);

};

#endif // COMPRESOR_H
