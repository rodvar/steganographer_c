#ifndef MANAGERFREEREGVARS_H_INCLUDED
#define MANAGERFREEREGVARS_H_INCLUDED

#include <string>
#include <list>
#include <vector>

using namespace std;

/**
  * class ManagerFreeReg
  */

class ManagerFreeRegVars
{
public:

 /*Definicion de listas de registros variables*/
	struct RegLibre{
        unsigned short int offset;
        unsigned short int size;
};
private:

    list<RegLibre*> listaLibres;

public:

  // Constructors/Destructors
  //

  /**
   * Empty Constructor
   */
  ManagerFreeRegVars ( );

  /**
   * Empty Destructor
   */
  ~ManagerFreeRegVars ( );

  /**
   * Devuelve la lista de variables corespondiente
   */
  list<RegLibre*> getLista();

  /**
     * Inserta ordenado por offset en la lista de registros libres una nuevo elemento libre.
     * @param offset Es el offset del nuevo registro libre
     * @param size es el size de ese registro libre
     */
    void insertarOrdenadoEnListaVariables(unsigned short int offset, unsigned short int size);

     /**
     * Recorre la lista de elementos libres busacndo hermanos a un determinado elemento
     * @param el offset del registro a buscar en la lista
     * @return Devuelve un vector cuya primera posicion tiene el offset del anterior, y la
     *         segunda la del siguiente. Si no tiene alguno de los 2, habra un valor "-1"
     */
    vector<short int> obtenerVariablesHermanos(unsigned short int offset);


    /**
    * Recorre la lista utilizando algoritmo de FIRST FIT
    * @param size El tamaño del registro a verificar en la lista
    * @return la primera posicion donde entre el registro variable que se quiera guardar, sino devuelve -1
    */
    short int obtenerVariablesPrimerLibre(unsigned short int size);

    /**
     * Elimina correctamente la lista de variables
     */
    void eliminarListaLibres();
};


#endif // MANAGERFREEREGVARS_H_INCLUDED
