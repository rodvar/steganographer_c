/************************************************************************
                        AdministradorRecursos.h.h - Copyright rodrigo

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
The original location of this file is /home/rodrigo/uml-generated-code/AdministradorRecursos.h
**************************************************************************/


#ifndef ADMINISTRADORRECURSOS_H
#define ADMINISTRADORRECURSOS_H

#define NOMBRE "n"
#define NOMBRE_SIZE 1
/* Definicion de codigos para los archivos administrativos del sistema*/
#define OCULTADOS          "adm/o.dat"
#define PARTICIONES        "adm/p.dat"
#define DIRECTORIOS        "adm/d.dat"
#define IMAGENES           "adm/i.dat"
#define NOMBRE_OCULTADOS   "adm/on.dat"
#define NOMBRE_DIRECTORIOS "adm/dn.dat"
#define NOMBRE_IMAGENES    "adm/in.dat"

#define TIMEOUT 1

#include <string>
#include <map>
#include <sys/errno.h>
#include <sys/time.h>
#include "Ocultador.h"
#include "Segmentador.h"
#include "Encriptador.h"
#include "Compresor.h"
#include "Indexador.h"
#include "Soporte.h"
#include "Directorio.h"
#include "Directorio.h"
#include "Texto.h"
#include "UI.h"
using namespace std;


/**
  * El administrador de recursos se encarga de mantener alto el rendimiento del soporte
  * asi como de permitir su correcta modificacion.
  */

class AdministradorRecursos
{
private:
    Soporte* soporte;
	Ocultador* ocultador;
	Encriptador* encriptador;
	Segmentador* segmentador;
	Compresor* compresor;
	map<string, Indexador*> indexadores;

public:

	// Constructors/Destructors
	//
	/**
	 * Constructor
	 * @param soporte Es el soporte a administrar
	 */
	AdministradorRecursos (Soporte* soporte);

	/**
	 * Empty Destructor
	 */
	~AdministradorRecursos ( );

	/**
     *  Analiza el Soporte en busca de los mejores recursos para esteganografiar un texto
     *  Si es necesario, a partir de un texto genera tantas particiones como sean necesarias
     * para que entre en los recursos disponibles.
     *  Actualiza la estructura administrativa luego de grabar la informacion
	 * @return Error Codigo de error . OK si todo estuvo bien
	 * @param  texto Es el texto a ocultar
	 */
	UI::Error esteganografiar (Texto* texto );


	/**
	 * Este metodo no se encarga de persistir el texto recuperado
	 * @param  texto Es el texto donde completar la referencia al archivo luego de
	 * obtenerlo
	 */
	UI::Error desEsteganografiar (Texto* texto );

	/**
	 * Recupera el texto y lo persiste con el nombre completo pasado por parametro
	 * @return Error . OK si todo salio bien
	 * @param  se pasa el texto y la ruta donde se desea guardar el documento desEsteganografiado.
	 */
	UI::Error desEsteganografiar(Texto* texto,string nombreFinal);

    /**
     * Actualiza el estado del soporte si este cambio. Realiza los cambios para mantener
     * su maximo rendimiento
     */
    void actualizar();

	/**
	 * Get the value of ocultador
	 * @return the value of ocultador
	 */
	Ocultador* getOcultador ( );

	/**
	 * Get the value of encriptador
	 * @return the value of encriptador
	 */
	Encriptador* getEncriptador ( );

	/**
	 * Get the value of segmentador
	 * @return the value of segmentador
	 */
	Segmentador* getSegmentador ( );


	/**
	 * Get the value of compresor
	 * @return the value of compresor
	 */
	Compresor* getCompresor ( );

	/**
	 *
	 * @return Devuelve el indexador para el archivo de nombre pasado por parametro
	 * NULL si no existe
	 * @param nombre del archivo cuyo indexador se quiere obtener
	 */
	Indexador* getIndexador (string nombre);

	/**
	 * Valida que el directorio exista. Si existe crea el directorio y lo devuelve. Sino
	 * devuelve NULL
     */
	 Directorio* crearDirectorio(string ruta);

    /**
     * Para todos los indexadores, levanta la lista de libres de los archivos
     * que indexan a memoria.
     */
    void indexarLibres();

    /**
     * Prepara la estructura administrativa para ser accedida
     */
    void inicializar();

    /**
     * Para todos los indexadores, protege adecuadamente la informacion de sus
     * archivos de registros variables
     */
    void liberar();

    /**
    * Mediante system calls obtiene el estado actual de los directorios y los compara con el estado
    * actual de los directorios administrativos. Ante alguna incongruencia realiza el alta y baja
    * correspondiente en la estructura administrativa.
    */
    void analizarIncongruencias();

    /**
     * Cada TIMEOUT segundos realiza un analisis de incongruencias sobre el soporte.
     */
    static void* observarSoporte (void* objeto);

        /**
         * Definicion de tipos de analisis.
         */
          enum TipoAnalisis{
            ANALISIS_INICIAL,
            ANALISIS_INCONGRUENCIAS
        };


	/**
	* Analiza el contenido del directorio
	* y carga el list con las imagenes segun el tipo.
	*/
    bool analisisInicial (Directorio* &directorio,TipoAnalisis tipo);

    /**
     * Reorganizar archivos
     */
    void reorganizarArchivos(double porcentage);

    /**
     * actualizar encadenamiento de reg libres
     */
    void actualizarEncadenamiento();

    /**
     * Chequea por la existencia del directorio administrativo del sistema.
     * Si no existe lo crea.
     * @return true si todo salio bien, false en caso de q nos quedemos sin dir adm.
     */
    static bool verificarDirectorioAdministrativo();

    /**
     * Elimina las particiones q aputen a ese idImagen tanto de mem como disco
     */
    void limpiarInfoAdministrativa(const unsigned int idImagen);

private:

    /***************************************
    Esta funcion parsea el nombre
    del archivo y me devuelve la extension
    ***************************************/
     string getTipoArchivo(string nombre);

    /**
	 * Analiza el texto a almacenar y el estado actual del soporte. En base a estos parametros,
	 * decide cual es la mejor forma de trabajar el texto para hacerlo en el soporte en forma
	 * eficiente. Implementa logica de decision intentando que el texto entre en una unica imagen.
	 * Si esto no es posible, lo particiona devolviendo tantos portadores como sean necesarios.
	 * @return La lista de portadores necesarios para ocultar la informacion. Si la lista esta vacia
	 *         significa que no es posible ocultar el texto con los recursos disponibles.
	 * @param  texto Es el texto a analizar
	 */
	list<Almacenable*> obtenerRecursos (Texto* texto );

	/**
	 * Realiza las consultas necesarias al soporte y calcula el offset donde se puede
	 * escribir en la imagen de id pasado por parametro
	 */
	unsigned int calcularOffsetEscritura(Portador* portador);

    /**
     * Elimina correctamente la lista de indexadores
     */
    void eliminarIndexadores();
};

#endif // ADMINISTRADORRECURSOS_H
