#include "ArchivoImagenGIF.h"

bool ArchivoImagenGIF::guardarDatos (string cadena, const unsigned int offset){
    fstream f;
    unsigned int offsetGifInicial = 0xD;//posicion D en decimal = 13
    unsigned int tamano = 3;//FFAAFF
    unsigned char buffer[3];
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned int offsetReal = ((offset*BITS_BYTE)/3)* 3;
    if (((offset*BITS_BYTE)%3) != 0)
        offsetReal++;
    char valor = 0;
    int i = 0;
    string palabra;
    string ruta = this->obtenerNombreCompleto();

    f.open(ruta.c_str(), ios::in | ios::out | ios::binary);
    if(f.good()) {
        OperacionesBinarias::siguienteValorString(&cadena);
        const int tamanoCadena = cadena.length() * BITS_BYTE;//cantidad de bits a ser escritos
        //for (int i=0; i<tamanoCadena; i= i+3){
        while ((valor != -1) && (i < tamanoCadena)){
            f.seekg(offsetGifInicial + offsetReal + i, ios::beg);
            //posiciono y tomo los valores de la 1º pos de la paleta seleccionada
            f.read((char*)buffer, tamano);
            red = buffer[0];
            green = buffer[1];
            blue = buffer[2];

            //Verificar si bit es 1 o 0 y contrastar con el nº de la paleta para ver si cambiar o dejarlo igual
            valor = OperacionesBinarias::siguienteValorString(NULL);
            if (valor >=0){
                if (((red%2) == 0) && (valor==1)){//valor par, pero pixel dice impar => hay q cambiar
                        red = red + 1;
                }else if (((red%2) == 1)  && (valor==0)){//valor impar, pero pixel dice par => hay q cambiar
                    red = red - 1;
                }
                valor = OperacionesBinarias::siguienteValorString(NULL);
                if (valor >=0){
                    if (((green%2) == 0) && (valor==1)){//valor par, pero pixel dice impar => hay q cambiar
                        green = green + 1;
                    }else if (((green%2) == 1)  && (valor==0)){//valor impar, pero pixel dice par => hay q cambiar
                        green = green - 1;
                    }
                    valor = OperacionesBinarias::siguienteValorString(NULL);
                    if (valor >=0){
                        if (((blue%2) == 0) && (valor==1)){//valor par, pero pixel dice impar => hay q cambiar
                            blue = blue + 1;
                        }else if (((blue%2) == 1)  && (valor==0)){//valor impar, pero pixel dice par => hay q cambiar
                            blue = blue - 1;
                        }
                    }
                }
            }
            palabra = "   ";
            palabra[0] = red;
            palabra[1] = green;
            palabra[2] = blue;
            f.seekp(offsetGifInicial + offsetReal + i, ios::beg);
            f.write(palabra.c_str(), palabra.length());
            i= i+3;
        }
        f.close();
        return true;
    }
    return false;
}

string ArchivoImagenGIF::recuperarDatos (const unsigned int offset, const unsigned int bitsInformacion ){
    fstream f;
    unsigned int offsetGifInicial = 0xD;//posicion D en decimal = 13
    unsigned int tamano = 3;//FFAAFF
    unsigned char buffer[3];
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    string palabra;
    bool salida = false;
    unsigned int contador = 0;
    unsigned int i = 0;
    unsigned int offsetReal = ((offset*BITS_BYTE)/3)* 3;
    if (((offset*BITS_BYTE)%3) != 0)
        offsetReal++;
    string ruta = this->obtenerNombreCompleto();

    f.open(ruta.c_str(), ios::in | ios::out | ios::binary);
    if(f.good()) {

        while (salida == false){//TODO: ver como hacer un corte no tan rustico!!!
            f.seekg(offsetGifInicial + offsetReal + i, ios::beg);
            //posiciono y tomo los valores de la 1º pos de la paleta seleccionada
            f.read((char*)buffer, tamano);
            red = buffer[0];
            green = buffer[1];
            blue = buffer[2];
            if (contador<bitsInformacion){
                if ((red%2) == 0){
                    OperacionesBinarias::concatenarValorString(0);
                }else
                    if ((red%2) == 1){
                        OperacionesBinarias::concatenarValorString(1);
                    }
                contador++;
            }else salida = true;
            if (contador<bitsInformacion){
                if ((green%2) == 0){
                    OperacionesBinarias::concatenarValorString(0);
                }else
                    if ((green%2) == 1){
                        OperacionesBinarias::concatenarValorString(1);
                    }
                contador++;
            }else salida = true;
            if (contador<bitsInformacion){
                if ((blue%2) == 0){
                    OperacionesBinarias::concatenarValorString(0);
                }else
                    if ((blue%2) == 1){
                        OperacionesBinarias::concatenarValorString(1);
                    }
                contador++;
            }else salida = true;
            i=i+3;
        }
        f.close();
    }
    return OperacionesBinarias::concatenarValorString(-1);
}

unsigned int ArchivoImagenGIF::calcularEspacioDisponible(){
    fstream f;
    string ruta = this->obtenerNombreCompleto();
    unsigned int posicionTamanoPaleta = (0xA);
    unsigned int tamano = 0;
    unsigned char buffer[1];
    unsigned char profundidadPorCanal;
    f.open(ruta.c_str(), ios::in | ios::out | ios::binary);
    if(f.good()) {
        f.seekg(posicionTamanoPaleta, ios::beg);
        f.read((char*)buffer, 1);
        profundidadPorCanal = buffer[0];
        if (OperacionesBinarias::valorEn(profundidadPorCanal, 0))
            tamano = tamano + 1;
        if (OperacionesBinarias::valorEn(profundidadPorCanal, 1))
            tamano = tamano + 2;
        if (OperacionesBinarias::valorEn(profundidadPorCanal, 2))
            tamano = tamano + 4;
    }else
        return 0;
    tamano = (pow (2,tamano+1))*3;
    return tamano;
}

void ArchivoImagenGIF::preProcesar(){
    this->espacioDisponible = calcularEspacioDisponible()/BITS_BYTE;
    if (this->espacioDisponible < 6){
        this->espacioDisponible = 0;
        }
    this->offsetInicial = OFF_GIF;
    this->bytesLibres = this->espacioDisponible;
}

//private
unsigned int ArchivoImagenGIF::calcularTamano(){

    return 0;
}
