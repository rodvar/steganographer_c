#ifndef ARCHIVOIMAGENPNG_H_INCLUDED
#define ARCHIVOIMAGENPNG_H_INCLUDED

#include "ArchivoImagen.h"

/**
 * Clase que representa un archivo de imagen en formato PNG. Encapsula toda la funcionalidad
 * clave para su manejo
 */
class ArchivoImagenPNG : public ArchivoImagen{

private:
    /**
     * Calcula y devuelve el offset inicial.
     */
    unsigned int calcOffsetInicial();

public:
    /**
     * Constructor de Archivo de Imagen PNG
     * @param id Es el identificador con el que se almacenara en los archivos de registros
     * @param idDirectorio Es el id del directorio al cual pertenece este archivo
     * @param nombre es el nombre del archivo (incluyendo extension)
     */
    ArchivoImagenPNG(const unsigned int id, unsigned int idDirectorio, const string nombre, const unsigned int tamano):
        ArchivoImagen(id, idDirectorio, nombre, tamano){
        if (tamano == 0)
            this->tamano = this->calcularTamano(nombre);
    }

    ~ArchivoImagenPNG(){ //destructor
    }

    /**
     * Reduce la calidad del PNG y rellena con bytes quedando del mismo tamaño.
     * Setea en espacioDisponible el espacio total q se puede utilizar en dicha imagen e inicialmente, bytesLibres con el mismo valor
     */
    void preProcesar();

    /**
	 * @return bool True si es paosible, false en caso contrario
	 * @param  cadena Es la cadena de datos a ocultar en el portador
	 * @param  offset La cantidad de bytes a leer hasta encontrar el primero modificable
	 */
	bool guardarDatos (string cadena, const unsigned int offset);


	/**
	 * Recupera la cantidad de bitsInformacion especificada. Completa con CEROS si no es multiplo de
	 * BITS_BYTE (8)
	 * @return string Los datos recuperados. Devuelve un string VACIO si no pudo hacerlo
	 * @param  offset La cantidad de bytes a leer hasta encontrar el primero que contiene los datos
	 * @param bitsInformacion Es la cantidad de bits a leer a partir del offset para conformar los datos
	 */
	string recuperarDatos (const unsigned int offset, const unsigned int bitsInformacion );



};

#endif // ARCHIVOIMAGENPNG_H_INCLUDED
