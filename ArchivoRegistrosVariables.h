#ifndef ARCHIVOREGISTROSVARIABLES_H_INCLUDED
#define ARCHIVOREGISTROSVARIABLES_H_INCLUDED
#include "RegVariable.h"
#include <list>
#include <fstream>
#include <vector>

using namespace std;

/**
  * class ArchivoRegistrosVariables es un archivo stream el cual soporta un manejo ordenado
  * de registros de tamaño variable
  */



class ArchivoRegistrosVariables
{

private:
    string nombre;

public:

  // Constructors/Destructors
  //
  /**
   * Empty Constructor
   */
  ArchivoRegistrosVariables (string nombre);

  /**
   * Empty Destructor
   */
  ~ArchivoRegistrosVariables ( );

    /**
    *
    * Se graba el valor en el archivo, tratando de reutilizar espacios libres. El metodo se encarga de liberar memoria.
    * @post : se elimina el registro
    * @return Offset del valor o Codigo de Error
    * @param valor a grabar.
    */
    int alta(RegVariable* registro);

    /**
    * Crea el registro y llama al alta dfe registro
    * @pre  :
    * @post : Se graba el valor en el archivo, tratando de reutilizar espacios libres. El metodo se encarga de liberar memoria.
    * @return Offset del valor o Codigo de Error
    * @param valor a grabar.
    */
    int alta(string dato);

    /**
    *
    * @pre  :
    * @post :
    * @return String con el nombre del archivo.
    */
    string getNombre();

    /**
    *
    * @pre  :
    * @post : Levanta una lista con los punteros y tamanio de cada registro.
    * @return void
    */
    void levantarListaRegLibres();

    /**
    *
    * @pre  : El registro debe estar en el archivo
    * @post : Da de baja un registro en el archivo a partir de una posicion, en caso de error devuelve -1;
    * @return bool
    */
    int baja(unsigned short int offsetRegistro);

    /**
     * A partir del offset pasado por parametro, actualiza la lista de libres en disco
     * en base a la lista de libres en memoria, encadenando los registros hermanos al del offset
     * @return TRUE si todo OK, false en caso contrario
     */
    bool actualizarListaEnDisco(unsigned short int offsetRegistro);


    /**
     * Recorre toda la lista de libes bajandola a disco.
     * @return TRUE si todo OK, false en caso contrario
     */
    bool actualizarListaEnDisco();
    /**
     * A partir del offset pasado por parametro, actualiza la lista de libres en disco
     * en base a la lista de libres en memoria, encadenando los registros anterior y siguiente, se usa mayormente para cuando se da de baja un registro LIBRE.
     * @return TRUE si todo OK, false en caso contrario
     */
    void encadenarAnteriorSiguiente(unsigned short int offsetRegistro);

    /**
     * Obtiene el registro del offset pasado por parametro
     */
    string getRegistro(unsigned short int offset);

    /**
     * Prepara el archivo para trabajarse
     */
    void cargarArchivoEncriptado();

    /**
     * Encripta el archivo y lo guarda
     */
    void guardarArchivoEncriptado();
};



#endif // MANAGERREGVARS_H_INCLUDED
