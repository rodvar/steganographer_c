#ifndef ARCHIVOIMAGENBMP_H_INCLUDED
#define ARCHIVOIMAGENBMP_H_INCLUDED

#include "ArchivoImagen.h"
#include "EasyBMP.h"

/**
 * Clase que representa un archivo de imagen en formato BMP. Encapsula toda la funcionalidad
 * clave para su manejo
 */
class ArchivoImagenBMP : public ArchivoImagen{
private:

public:
    /**
     * Constructor de Archivo de Imagen BMP
     * @param id Es el identificador con el que se almacenara en los archivos de registros
     * @param idDirectorio Es el id del directorio al cual pertenece este archivo
     * @param nombre es el nombre del archivo (incluyendo extension)
     * @param tamano Es el tamaño del archivo
     */
    ArchivoImagenBMP(const unsigned int id, unsigned int idDirectorio, const string nombre, const unsigned int tamano)
    : ArchivoImagen(id, idDirectorio, nombre, tamano){
        if (tamano == 0)
            this-> tamano = this->calcularTamano(nombre);
    }

    ~ArchivoImagenBMP(){

    }

    /**
     * El analisis de si la cadena entra en la imagen debe ser realizado antes de llamar a
     * este metodo
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
	string recuperarDatos (const unsigned int offset, const unsigned int bitsDatos );

	/**
	 * Pre procesa el archivo BMP para saber el espacio que tiene para guardar info
	 */
	void preProcesar();
};

#endif // ARCHIVOIMAGENBMP_H_INCLUDED
