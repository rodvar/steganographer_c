#ifndef ARCHIVOIMAGENGIF_H_INCLUDED
#define ARCHIVOIMAGENGIF_H_INCLUDED

#define OFF_GIF 0

#include "ArchivoImagen.h"

/**
 * Clase que representa un archivo de imagen en formato GIF. Encapsula toda la funcionalidad
 * clave para su manejo
 */
class ArchivoImagenGIF : public ArchivoImagen{
private:

public:
    /**
     * Constructor de Archivo de Imagen GIF
     * @param id Es el identificador con el que se almacenara en los archivos de registros
     * @param idDirectorio Es el id del directorio al cual pertenece este archivo
     * @param nombre es el nombre del archivo (incluyendo extension)
     */
    ArchivoImagenGIF(const unsigned int id, unsigned int idDirectorio, const string nombre, const unsigned int tamano)
    : ArchivoImagen(id, idDirectorio, nombre, tamano){
        if (tamano == 0)
            this->tamano = this->calcularTamano();
    }

    ~ArchivoImagenGIF(){

    }

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

    /**
	 * Calcula y asigna el espacio que se puedo utilizar en el gif para guardar infomarcion
	 * Se descartan los gif cuya paleta sea menor a 16 bits
	 */
	void preProcesar();

private:
    /**
     * Calcula el tamaño disponible en bits de un determinado gif
	 * @return unsigned int El tamaño en bits disponibles
	 */
	unsigned int calcularEspacioDisponible ( ) ;

    /**
     * Calcula el tamaño del archivo
	 * @return unsigned int El tamaño del archivo
	 */
	unsigned int calcularTamano ( ) ;

};

#endif // ARCHIVOIMAGENGIF_H_INCLUDED
