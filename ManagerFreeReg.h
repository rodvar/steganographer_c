/************************************************************************
                        ManagerFreeReg.h

**************************************************************************/


#ifndef MANAGERFREEREG_H
#define MANAGERFREEREG_H

#include <string>
#include <list>
#include "Registro.h"

using namespace std;

/**
  * class ManagerFreeReg
  */

class ManagerFreeReg
{
private:
    //lista de posiciones de reg libres
    std::list<int> libres;

public:

  // Constructors/Destructors
  //

  /**
   * Empty Constructor
   */
  ManagerFreeReg ( );

  /**
   * Empty Destructor
   */
  virtual ~ManagerFreeReg ( );

  /**
   * Agrega una poscion a la lista de posiciones de registros libres
   * @param  pos Posicion de un Registro libre
   */
  void addLibre(int pos );


  /**
   * Devuelve, si existe, la posicion de un Registro libre. En ese caso,
   * ademas, lo saca de la lista.
   * Si no existe, devuelve -1
   */
  int getLibre();

  /**
   * Devuelve, si existe, la posicion del ultimo Registro libre.
   * Si no existe, devuelve -1
   */
  int getUltimo();

  /**
   * Devuelve la lista de Registros libres.
   */
  list<int> getLista();
};

#endif // MANAGERFREEREG_H
