/************************************************************************
                        Indexador.h

**************************************************************************/


#ifndef INDEXADOR_H
#define INDEXADOR_H

#include "ManagerFreeReg.h"
#include "ManagerFreeRegVars.h"
#include <string>

class Dao;
using namespace std;


/**
  * Clase que representa un indexador el cual permite generar eficientemente indices en memoria
  * sobre la estructura administrativa de archivos dada
  */

class Indexador
{
private:
    bool variablesAsociado;
    string nombreArchivo;
    //unsigned short int siguienteId;
    unsigned short int maximoId;
    ManagerFreeReg* managerFijos;
    ManagerFreeRegVars* managerVariables;
    Dao* dao;

public:

	// Constructors/Destructors
	//
	/**
	 * Constructor
	 * @param nombreArchivo Es el nombre del archivo a indexar
	 */
	Indexador (string nombreArchivo, bool variablesAsociado);

	/**
	 * Empty Destructor
	 */
	~Indexador ( );

	/**
	 * Devuelve el manejador para la lista de libres de Archivos de registros de long Fija
	 */
    ManagerFreeReg* getManagerLibresFijos();

     /**
      * Devuelve la lista de registros libres para archivos de registros de long. variable
      */
    ManagerFreeRegVars* getManagerLibresVariables();

    /**
     * Devuelve el nombre del archivo que indexa este indexador
     */
	string getNombre();

	/**
	 * @return el nombre del archivo de registros variable asociado. Si no tiene archivo
	 * variable devuelve un string vacio
	 */
    string getNombreVariableAsociado();

    /**
     * Reorganiza el archivo de registrosVariables quitando los espacios libres y eliminando
     * la lista de libres
      */
	bool reorganizarArchivoVariablesAsociado();

    /**
     * Devuelve el proximo Id del archivo
     */
    unsigned short int proximoId();

    /**
     * Setea el maximo Id del archivo
     */
    void setMaximoId(unsigned short int id);

    /**
     * Setea el maximo Id del archivo
     */
    void setSiguienteId(unsigned short int id);

    /**
     * Para sus archivos asociados, levanta a memoria la informacion de los
     * registros libres en los managers correspondientes
     */
    void indexarLibres();

    /**
     * Prepara los archivos para ser accedidos
     */
    void inicializar();

    /**
     * Para su archivo de registros variables asociado, guarda y protege adecuadamente
     * su informacion.
     */
    void liberar();

    /**
    * Reorganizar el archivo si el porcentage del espacio ocupado por los ultimos registros
    * libres consecutivos es igual al poorcentage recibido por parametro.
    * En ese caso, reorganizar el archivo;
    *
    */
    bool reorganizarArchivoRegistro(double porcentage);

    /**
    * Actualizar encadenamiento de registros libres.
    */
    void actualizarEncadenamientos();

private:
    /**
     * Construye la Dao correspondiente al indexador segun su nombre
     */
    void crearDaoAsociada();

    /**
     * Destruye la dao correspondiente
     */
    void liberarDao();

};

#endif // INDEXADOR_H
