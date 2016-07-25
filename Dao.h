/************************************************************************
                        Dao.h
**************************************************************************/


#ifndef DAO_H
#define DAO_H

#include "Almacenable.h"
#include "AdministradorRecursos.h"
#include <list>
#include <string>

/**
  * Clase que representa un Objeto de acceso a datos (Data Access Object). Resuelve el acceso
  * a los archivos de registros para su ABM.
  */

class Dao
{
public:

  /**
   * Busca en el medio de almacenamiento la entidad recibida por parametro segun su
   * PK y la devuelve.
   * @param  entidad
   */
  virtual bool get (Almacenable* entidad ) = 0;

   /**
    * Busca en el medio de almacenamiento la entidad recibida por parametro
    * la devuelve. Si no existe, o si existe pero fue eliminada, devuelve false.
    * @param  entidad
    */
    virtual bool get (Almacenable* entidad, bool &valido ) = 0;

  /**
   * Da de alta en el medio de almacenamiento la entidad recibida por parametro.
   * @param  entidad
   */
  virtual void alta (Almacenable* entidad ) = 0;

  /**
   * Modifica la entidad ya existente sobreescribiendo sus datos con los pasados
   * en entidad
   * @return TRUE si pudo hacerlo, false en caso contrario (no existe el reg)
   * @param  entidad La entidad a modificar, debe tener seteado el PK
   */
  virtual bool modificacion (Almacenable* entidad ) = 0;

  /**
   * Da de baja en el medio de almacenamiento la entidad recibida por parametro.
   * @param  entidad
   */
  virtual void baja (Almacenable* entidad ) = 0;

    /**
    *
    * @post : Devuelve una lista de Almacenables.
    * @param  lista de Almacenables
    */
   virtual void getAlmacenables (list<Almacenable*>* lista) = 0;

    /**
     * Devuelve el tamano del registro que administra.
     */
    virtual int obtenerTamanoRegistro() = 0;

    /**
     * Devuelve el registro que administra.
     */
    Registro* getRegistro();

    /**
     * Procesa la estructura administrativa del archivo en el medio de almacenamiento
     * cargando la lista de libres asociada.
     */
    virtual void levantarListaLibres() = 0;

    /**
     * Actualiza el encadenamiento de lista de libres en
     */
    virtual void actualizarEncadenamiento() = 0;

    /**
     * Desencripta el archivo de variables asociado
     */
    virtual void inicializarVariables() = 0;

    /**
     * Encripta el archivos variables asociado
     */
    virtual void protegerVariables() = 0;

    /**
    *
    * @post : Persiste la lista de Almacenables.
    * @param  lista de Almacenables
    */
    void setAlmacenables (list<Almacenable*> lista){
        if (!lista.empty()) {
            list<Almacenable*>::iterator it = lista.begin();
            while ( it != lista.end()){
                this->alta((Almacenable*)*it);
                it++;
            }
        }
    }
};

#endif // DAO_H
