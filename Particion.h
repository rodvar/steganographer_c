/************************************************************************
                        Particion.h

**************************************************************************/


#ifndef PARTICION_H
#define PARTICION_H

#include "Informacion.h"

/**
  * Clase que representa un fragmento de un archivo de texto
  */

class Particion : public Informacion
{
private:
	unsigned int idImagen;
	unsigned int idTexto;
	unsigned int orden;
	unsigned int offsetImagen;
	string datos; // La cedena de datos de la particion

public:
	// Constructors/Destructors
	//
	/**
	 * Construye la particion
	 * @param id Es el id con el que se persistiria la particion
	 * @param tamano Es el tamano de los datos que contiene la particion
	 * @param idImagen Es el id de la imagen donde se aloja la particion
	 * @param orden Es el orden de la particion en el texto que la involucra
	 */
	Particion (string datos, unsigned int id, unsigned int tamano, unsigned int idImagen, unsigned int idTexto, unsigned int orden);

	/**
	 * Construye la particion
	 * @param id Es el id con el que se persistiria la particion
	 * @param tamano Es el tamano de los datos que contiene la particion
	 * @param idImagen Es el id de la imagen donde se aloja la particion
	 */
	Particion (string datos, unsigned int id, unsigned int tamano, unsigned int idImagen, unsigned int idTexto );

    /**
	 * Construye la particion. Queda como responsabilidad del llamador setear el idImagen
	 * donde la particion sera alocada.
	 * @param id Es el id con el que se persistiria la particion
	 * @param tamano Es el tamano de la particion
	 */
	Particion (string datos, unsigned int id, unsigned int tamano, unsigned int idTexto);

    Particion ();

	/**
	 * Destructor
	 */
	~Particion ( );


	/**
	 * Set the value of m_idImagen
	 * @param new_var the new value of m_idImagen
	 */
	void setIdImagen ( unsigned int new_var );

	/**
	 * Get the value of m_idImagen
	 * @return the value of m_idImagen
	 */
	unsigned int getIdImagen ( );

	/**
	 * Get the value of idTexto
	 * @return the value of idTexto
	 */
	unsigned int getIdTexto ( );

	/**
	 * Set the value of idTexto
	 * @param new_var the new value of idTexto
	 */
	void setIdTexto ( unsigned int new_var );

	/**
	 * Get the value of orden
	 * @return the value of orden
	 */
	unsigned int getOrden ( );

	/**
	 * Set the value of orden
	 * @param new_var the new value of orden
	 */
	void setOrden ( unsigned int new_var );

	/**
	 * Set the value of offset
	 * @param new_var the new value of offset
	 */
	void setOffsetImagen ( const unsigned int new_var );

	/**
	 * Get the value of offset
	 * @return the value of offset
	 */
	unsigned int getOffsetImagen ( );

	/**
	 * Permite setear los datos de la particion
	 */
	void setDatos(string datos);

	/**
	 * @return Los datos de la particion
	 */
    string getDatos();


    /**
     * Crea y devuelve una copia de la particion
     */
    Particion* copiar();

};

#endif // PARTICION_H
