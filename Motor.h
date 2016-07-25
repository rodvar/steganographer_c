/************************************************************************
                        Motor.h
**************************************************************************/


#ifndef MOTOR_H
#define MOTOR_H

#include "ProcesadorTextos.h"
#include "Password.h"
#include "Soporte.h"
#include "Directorio.h"
#include "Texto.h"
#include "TextoDao.h"
#include "ImagenDao.h"
#include "DirectorioDao.h"
#include "PasswordDao.h"
#include "AdministradorRecursos.h"
#include "UI.h"
#include "Encriptador.h"
#include <list>
#include <pthread.h>
#include "Indexador.h"

#define PORCENTAJE_LIBRES 95

using namespace std;

/**
  * Representa el Motor del sistema. Dispone todos los elementos para el correcto funcionamiento
  * del sistema.
  */

class Motor
{
private:
    Soporte* soporte;
    AdministradorRecursos* administradorRecursos;
    ProcesadorTextos* procesadorTextos;
    Password* password;
    pthread_t hiloObservador;
    bool corriendo; // Permite saber si el motor esta activo

    /**
     * Empty Constructor
     */
    Motor ( ){
        this->corriendo = false;
        PasswordDao* passDao = new PasswordDao();
        AdministradorRecursos::verificarDirectorioAdministrativo();
        this->soporte = new Soporte();
        this->administradorRecursos = new AdministradorRecursos(this->soporte);
        this->procesadorTextos = new ProcesadorTextos();
        this->password = new Password(this->administradorRecursos->getEncriptador()->encriptarPassword("aGanar"));
        if (!passDao->get(this->password)){
            passDao->alta(this->password);
		}
        delete(passDao);

    }

    /**
     * Empty Destructor
     */
    inline ~Motor ( ){
        delete this->soporte;
        delete this->administradorRecursos;
        delete this->procesadorTextos;
        delete this->password;
    }

public:



    /**
     * Devuelve la instancia unica segun el patron de diseño Singleton
     */
    static Motor* getInstancia(){
        static Motor* motor = NULL;
        if (!motor)
            motor = new Motor();
        return motor;
    }

    /**
     *  Desabilita el motor limpiando los recursos que este estaba utilizando.
     *   El mismo queda inutilizable
     */
    inline static void limpiar(){
        delete (Motor::getInstancia());
    }


	/**
	 * Verifica que exista el directorio en el sistema de archivos del SO
	 * Si existe, crea una instancia de directorio y lo agrega al Soporte del sistema
	 * @param  ruta Es la ruta del directorio a agregar
	 */
	void addDirectory (string ruta );


	/**
	 * @param  ruta Es la ruta del directorio a remover del soporte del sistema
	 */
	void removeDirectory (string ruta );


	/**
	 * @param  nombre Es el nombre completo (ruta/nombre.extension) del archivo a
	 * ocultar
	 */
	void addFile (string nombre );


	/**
	 * @param  nombre Es el nombre del archivo a obtener
	 * @param  destino Es la ruta del destino donde se ubicara el archivo saliente.
	 */
	void getFile (string nombre, string destino );


	/**
	 * @param  nombre Es el nombre del archivo a remover
	 */
	void removeFile (string nombre );


	/**
     * Muestra por la UI el listado de archivos que oculta el sistema
	 */
	void showFiles ( );


	/**
     * Muestra por la UI el listado de directorios que se utilizan para ocultar.
	 */
	void showDirectories ( );

	/**
	 * Muestra la informacion detallada de un directorio (para uso interno hasta entrega)
	 */
	void showDirectoryInfo( unsigned int id );

	/**
	 * Muestra la informacion detallada de un texto (para uso interno hasta entrega)
	 */
	void showFileInfo( unsigned int id );

	/**
	 * Muestra la informacion detallada del estado del soporte (espacio disponible)
	 */
    void showMediumInfo();


	/**
     * Si la contraseña actual es correcta, cambia la contraseña y la guarda en el archivo
     * administrativo correspondiente
     * @param  oldPassword Es la contraseña actual
     * @param  newPassword Es la nueva contraseña
     */
	void changePassword (string oldPassword, string newPassword );


	/**
     * Descarga a Disco las caches de ser necesario, y cierra el sistema
     */
	void quit ( );


	/**
	 * Get the value of soporte
	 * @return the value of soporte
	 */
	Soporte* getSoporte ( )	 {
		return soporte;
	}

	/**
	 * Get the value of administradorRecursos
	 * @return the value of administradorRecursos
	 */
	AdministradorRecursos* getAdministradorRecursos ( )	 {
		return administradorRecursos;
	}

	/**
	 * Get the value of procesadorTextos
	 * @return the value of procesadorTextos
	 */
	ProcesadorTextos* getProcesadorTextos ( )	 {
		return procesadorTextos;
	}

    /**
     * Deja el motor listo para responder cualquier consulta durante la ejecucion del sistema
     * Ademas chequea incongruencias estaticas con respecto al ultimo uso del sistema, y
     * lanza el hilo Observador del Soporte para resolver incongruencias dinamicas.
     * Tambien se encarga de levantar el password del sistema.
    */
    void levantarMotor();

    /**
     * valida que el password pasado por parametro sea igual al actual del sistema.
     *
    */
    bool compararConPassActual(string passIngresado);

    /**
     * Permite saber si el motor esta trabajando
     */
    bool estaCorriendo();

private:

    /**
     * Recupera los directorios persistidos con sus respectivas imagenes.
     *
    */
    void cargarDirectoriosImagenes();


    /**
     * Recupera los textos persistidos con sus respectivas particiones.
     *
    */
    void cargarTextosParticiones();


};

#endif // MOTOR_H
