#include "ArchivoImagenJPG.h"

bool ArchivoImagenJPG::guardarDatos (string cadena, const unsigned int offset){
// en el caso de jpg ese offset es relativo al offset donde empieza la informacion
// si offset = 0 , entonces el verdaderoOffset = offsetInicial + 0
// por ej  ?0000000000000000000  (el primer byte es el separador)
    string nombre = this->obtenerNombreCompleto();
    unsigned int verdaderoOffset = this->offsetInicial + offset;

    fstream f;
    f.open(nombre.c_str(), ios::in | ios::out | ios::binary);
    if(f.good()) {
        f.seekp(verdaderoOffset, ios::beg);
        f.write(cadena.c_str(), cadena.size());
        // setear marca
        //f.write(marca.c_str(), marca.size());
        f.close();
        return true;
    }
    return false;
}

string ArchivoImagenJPG::recuperarDatos (const unsigned int offset, const unsigned int bitsInformacion ){
    string nombre = this->obtenerNombreCompleto();
    char* buffer = new char[1];
    string cadena;
    //1- calcular el offset verdadero
    unsigned int verdaderoOffset = this->offsetInicial + offset;
    unsigned int bytesTotal = bitsInformacion/ BITS_BYTE;
    // 2- abrir el archivo y hacer un seek a esa posicion
    fstream f;
    f.open(nombre.c_str(), ios::in | ios::out | ios::binary);
    /*
    3- leer secuencialmente los bytes del archivo hasta haber leido
    (bitsInformacion/BITS_BYTE) bytes
    BITS_BYTE es un define = 8
     bits/byte
    */
    if(f.good()) {
        f.seekg(verdaderoOffset, ios::beg);
        for(unsigned int i=0; i<bytesTotal; i++) {
            if(!f.eof()) {
                f.read(buffer, sizeof(buffer[0]));
                cadena += buffer[0];
            }
        }
        f.close();
        delete[] buffer;
        return cadena;
    }
    delete[] buffer;
    return NULL;
}

unsigned int ArchivoImagenJPG::calcOffsetInicial() {
    unsigned int offset = 0;
    // si no fue calculada, obtenerla
    if(this->offsetInicial == 0) {
        // buscar de atras para adelante el caracter especial.
        string nombre = this->obtenerNombreCompleto();
        char* buffer = new char[1];
        string cadena;

        fstream f;
        f.open(nombre.c_str(), ios::in | ios::binary);
        if(f.good()) {
            f.seekg(0, ios::end);
            offset = f.tellg();
            int i = offset;
            while(i>0) {
                f.read(buffer, sizeof(buffer[0]));
                if(strcmp(buffer,"?")==0) {
                    offset=i;
                    i=0;
                }
                i--;
            }
            f.close();
        }
        delete[] buffer;
        this->offsetInicial = offset;
    }
    return this->offsetInicial;
}

void ArchivoImagenJPG::preProcesar()
{
    long tamanoOriginal = this->tamano;
    //Apertura del jpg original
    Image image;
    string nombre = this->obtenerNombreCompleto();
    image.read(nombre); //imagen original
    //Bajar la calidad
    unsigned int calidad = image.quality();
    if (calidad/2 > MIN_QUALITY){
        image.quality(calidad/2);
        //imagen con la calidad disminuida
        image.write(nombre);
        unsigned int tamano = this->calcularTamano(nombre);

        // rellenar la imagen manteniendo el tamano original
        unsigned int bytesRelleno = (tamanoOriginal - tamano);
        //se setea el espacio disponible y los bytes libres al momomento inicial
        this->espacioDisponible = bytesRelleno;
        this->bytesLibres = bytesRelleno;
        this->offsetInicial= tamano;

        char* relleno = new char[1];
        relleno[0] = '0';
        ofstream salida;
        salida.open (nombre.c_str(), ios::out | ios::app | ios::binary);
        if (salida.good()) {
            salida.seekp(0, ios_base::end);
            for(unsigned int i=0; i<bytesRelleno; i++)
                salida.write(reinterpret_cast<char *> (&relleno[0]), sizeof(relleno[0]));
            salida.close();
        }
        delete[] relleno;
    }
    else {
        this->espacioDisponible = 0;
        this->bytesLibres =0;
        this->offsetInicial= 0;
    }
}
