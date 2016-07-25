/************************************************************************
                        Texto.h

**************************************************************************/


#ifndef TEXTO_H
#define TEXTO_H

#include <string>
#include <list>
#include <sys/stat.h>
#include "Particion.h"

using namespace std;

/**
  * Clase que representa un Texto y su informacion administrativa. Como minimo un texto
  * esta compuesto por una particion, sin embargo puede estar fragmentado en N particiones.
  */

class Texto : public Informacion
{
private:
    list<Almacenable*> particiones; // Un texto tiene como minimo una unica particion
    unsigned int cantidadParticionesOriginales;
	string nombre; // Es el nombre completo incluyendo la ruta inicial que le dio el usuario

public:
	// Constructors/Destructors
    Texto (const string nombre, const unsigned int tamano);

    /**
	 * Construye el texto
	 * @param id Es el id con el que se guardaria el texto de persistirse
	 * @param nombre Es el nombre del texto (con su ruta completa)
	 * @param tamano Es el tamaño en bytes del texto
	 */
	Texto (const unsigned int id, const string nombre, const unsigned int tamano );

	/**
	 * Destruye el texto
	 */
	virtual ~Texto ();

	/**
	 * Get the list of Particiones objects held by m_particionesVector
	 * @return vector<Almacenable> list of Particiones objects held by
	 * m_particionesVector
	 */
	list<Almacenable*>* obtenerParticiones ( );

	/**
	 * Devuelve UNA COPIA de la lista de particiones del texto.
	 */
	list<Almacenable*> obtenerCopiaParticiones ( );

	/**
	 * Set the list of Particiones. Elimina la lista actual
	 * m_particionesVector
	 */
	void setParticionesList (list<Almacenable*> lista);


	/**
	 * @param  particion La particion a agregar a la lista
	 */
	void agregarParticion (Particion* particion );

	/**
	 * @param  particion La particion a agregar a la lista. Se agrega al comienzo de la lista.
	 */

	void agregarParticionInicio(Particion* particion);

	/**
	 * @return Particion
	 * @param  idParticion
	 */
	Particion* obtenerParticion (unsigned int idParticion );

	/**
	 * @return true si pudo hacerlo, false en caso contrario
	 * @param  idParticion
	 */
	bool eliminarParticion (unsigned int idParticion );

	/**
	 * La cantidad de particiones con la q fue creado el texto
	 */
	unsigned int getCantidadParticionesOriginal();

	/**
	 * permite setear la cantida de particiones origianles
	 */
	void setCantidadParticionesOriginal(const unsigned int cantidad);

	/**
	 * Devuelve TRUE si el texto tiene todas las particiones q tiene q tener
	 */
	bool esValido();

	/**
	 * Get the value of m_nombre
	 * @return the value of m_nombre
	 */
	string getNombre ( );

	/**
	 * Set the value of m_nombre
	 */
    void setNombre (string nombre );

	/**
	 * Get the value of m_nroParticiones
	 * @return the value of m_nroParticiones
	 */
	unsigned int getNroParticiones ( );

	/**
	 * Obtiene una lista con todas las particiones q aputen a ese id de imagen
	 */
	list<Almacenable*> obtenerParticiones(const unsigned int idImagen);

	/**
	 * Elimina todas las particiones que apunten al idImagen pasado por parametro
	 */
	void removerParticiones(const unsigned int idImagen);

	/**
	 * Devuelve true si el texto usa en alguna de sus particiones ese id pasado
	 */
	bool usaPortador(const unsigned int idImagen);

	/**
	 * Crea y devuelve una copia del texto
	 */
    Texto* copiar();

private:

    /**
     * Limpia correctamente la lista de particiones
     */
     void eliminarListaParticiones();

};

#endif // TEXTO_H
