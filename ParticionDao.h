/************************************************************************
                        ParticionDao.h

**************************************************************************/

#ifndef PARTICIONDAO_H
#define PARTICIONDAO_H

#include <string>
#include "Dao.h"
#include "Particion.h"
#include "ArchivoRegistros.h"
#include "Motor.h"

/**
  * class ParticionDao
  */

class ParticionDao : public Dao
{

private:

    ArchivoRegistros* managerFixedReg;

    Campo* sigLibre;
    Campo* campoId;
    Campo* campoIdOcultado;
    Campo* campoOrden;
    Campo* campoIdImagen;
    Campo* campoOffset;
    Campo* campoTamano;
    Campo* campoComprimida;
    Registro* reg;
    vector<Campo> campos;

public:

  // Constructors/Destructors
  //
  /**
   * Empty Constructor
   */
  ParticionDao ( );

  /**
   * Empty Destructor
   */
  virtual ~ParticionDao ( );

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
    void levantarListaLibres();

    /**
     * Actualiza el encadenamiento de lista de libres en
     */
    void actualizarEncadenamiento();

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
    int obtenerTamanoRegistro();

    /**
     * Devuelve el registro que administra.
     */
    Registro* getRegistro();

};

#endif // PARTICIONDAO_H
