#ifndef ARCHIVOREGISTROS_H_INCLUDED
#define ARCHIVOREGISTROS_H_INCLUDED

#include <fstream>
#include <string>
#include "Registro.h"
#include "ManagerFreeReg.h"

using namespace std;

/**
  * class ArchivoRegistros
  */

class ArchivoRegistros
{
private:
    fstream archivo;
    string nombre; // nombre del archivo
    int sizeReg; // tamano de cada registro fijo.
    Registro* registro;

public:

  // Constructors/Destructors

    ArchivoRegistros(string nombre, Registro* reg);

    ~ArchivoRegistros();

    /**
    *
    * @pre  :
    * @post : Se graba el registro en el archivo.
    * @param archivo donde se grabara el registro.
    * @param registro a grabar.
    */
    bool alta(Registro* &reg );

    /**
    *
    * @pre  :
    * @post : Se devuelve el registro pedido del archivo y devuelve TRUE
    *           Si no existe, devuelve FALSE.
    * @param registro a devolver.
    */
    bool getRegistro(Registro* &reg);
    /**
    *
    * @pre  : El registro debe existir.
    * @post : Marca el registro como borrado.
    */
    bool borrar(Registro* &reg);

    /**
    *
    * @pre  : El archivo fue abierto con exito en el constructor.
    * @post : Devulve el tamaño en bytes del archivo.
    */
    unsigned int sizeOfFile();

    /**
    * Obtiene la lista de registros libres, de este modo:
    * Busca el primer registro Libre, una vez encontrado,
    * obtiene los demas registros libres utilizando el encadenamiento
    * de registro libres, evitando de este modo leer todo el archivo.
    */
    void levantarListaRegLibres();

    /**
    * Actualiza el encadenamiento de registros libres en el archivo.
    */
    void actualizarEncadenamiento();

private:
    /**
     * Abre el archivo
     */
    bool abrir();

    /**
     * Cierra el archivo
     */
    void cerrar();

};

#endif // ArchivoRegistros_H_INCLUDED
