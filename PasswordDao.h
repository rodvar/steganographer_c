/************************************************************************
                        PasswordDao.h

**************************************************************************/


#ifndef PASSWORDDAO_H
#define PASSWORDDAO_H

#define PASSWORD "adm/c.dat"

#include <string>
#include <iostream>
#include <vector>       // stl vector header
#include "Dao.h"
#include "Campo.h"
#include "Registro.h"
#include "Password.h"
#include "Encriptador.h"
#include "OperacionesBinarias.h"
#include <fstream>


/**
  * class PasswordDao
  */

class PasswordDao : public Dao
{

public:

  // Constructors/Destructors
  //

  /**
   * Empty Destructor
   */
  ~PasswordDao ( );

  /**
   * Empty Constructor
   */
  PasswordDao ( );

   /**
   * Busca en el medio de almacenamiento la entidad recibida por parametro segun su
   * PK y la devuelve.
   * @param  entidad
   */

    bool get (Almacenable* entidad );

   /**
    * Busca en el medio de almacenamiento la entidad recibida por parametro
    * la devuelve. Si no existe, o si existe pero fue eliminada, devuelve false.
    * @param  entidad
    */
    bool get (Almacenable* entidad, bool &valido );

  /**
   * Da de alta en el medio de almacenamiento la entidad recibida por parametro.
   * @param  entidad
   */
    void alta (Almacenable* entidad );

  /**
   * Modifica la entidad ya existente sobreescribiendo sus datos con los pasados
   * en entidad
   * @param  entidad La entidad a modificar, debe tener seteado el PK
   */
    bool modificacion (Almacenable* entidad );

  /**
   * Da de baja en el medio de almacenamiento la entidad recibida por parametro.
   * @param  entidad
   */
  void baja (Almacenable* entidad );

    /**
    *
    * @post : Devuelve una lista de Almacenables.
    * @param  lista de Almacenables
    */
   void getAlmacenables (list<Almacenable*>* lista);

   /**
     * Procesa la estructura administrativa del archivo en el medio de almacenamiento
     * cargando la lista de libres asociada.
     */
    void levantarListaLibres(){
    }

    /**
     * Actualiza el encadenamiento de lista de libres en
     */
    void actualizarEncadenamiento(){
    }

    /**
     * Desencripta el archivo de variables asociado
     */
    void inicializarVariables();

    /**
     * Encripta el archivos variables asociado
     */
    void protegerVariables();

    /**
     * Devuelve el tamano del registro que administra.
     */
    int obtenerTamanoRegistro(){
        return 0;
    }

    /**
     * Devuelve el registro que administra.
     */
    Registro* getRegistro() {
        return NULL;
    };

};

#endif // PASSWORDDAO_H
