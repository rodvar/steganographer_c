#include "ArchivoImagenBMP.h"

bool ArchivoImagenBMP::guardarDatos (string cadena, const unsigned int offset){
    bool estado, lsbMitad;
    unsigned short int bpp; // bits per pixel
    unsigned short int bitsUtilesPx, correccion;
    short int valor;
    unsigned int fila, columna, largo, ancho, tamanoFila;
    string nombreArchivo = this->obtenerNombreCompleto();

    // Abrimos el imagen BMP
    BMP imagen;
    RGBApixel pixel;
    estado = imagen.ReadFromFile(nombreArchivo.c_str());

    if (estado){
        bpp = imagen.TellBitDepth();
        largo = imagen.TellHeight();
        ancho = imagen.TellWidth();

        lsbMitad = (bpp == (2*BITS_BYTE));

        bitsUtilesPx = (bpp == (4*BITS_BYTE)) ? 4 : 3;
        tamanoFila = bitsUtilesPx * ancho; // Real en bits
        fila = (offset * BITS_BYTE) / tamanoFila;
        columna = ((offset*BITS_BYTE - fila*tamanoFila) / (bitsUtilesPx));

        correccion = ((offset*BITS_BYTE - fila*tamanoFila) % (bitsUtilesPx));
        if (correccion != 0)
           columna++;
        if (columna >= ancho){
            columna = 0;
            fila++;
        }

        // Se resuelve modificando los pixels de la paleta (3 bits por pixel con LSB)
        if (bpp == BITS_BYTE) { //8 - offset values= 0,1,...,255
            fila = (offset*BITS_BYTE)/bitsUtilesPx;
            if (((offset*BITS_BYTE) % bitsUtilesPx) != 0)
                fila++;
            OperacionesBinarias::siguienteValorString(&cadena);
            valor = 0;
            // Esta parte esta en testeo
            while (valor >= 0){
                pixel = imagen.GetColor(fila);

                valor = OperacionesBinarias::siguienteValorString(NULL);
                if (valor >= 0){
                    OperacionesBinarias::setearValorLSB(&pixel.Red, valor, lsbMitad);
                    valor = OperacionesBinarias::siguienteValorString(NULL);
                    if (valor >= 0){
                        OperacionesBinarias::setearValorLSB(&pixel.Green, valor, lsbMitad);
                        valor = OperacionesBinarias::siguienteValorString(NULL);
                        if (valor >= 0){
                            OperacionesBinarias::setearValorLSB(&pixel.Blue, valor, lsbMitad);
                        }
                    }
                }
                imagen.SetColor(fila, pixel);
                if (++fila == 256)
                    valor = -1;
            }
        }
        else if (bpp >= (2*BITS_BYTE)){ //16, 24 y 32
            OperacionesBinarias::siguienteValorString(&cadena);
            valor = 0;
            // Esta parte esta en testeo
            while (valor >= 0){
                pixel = imagen.GetPixel(columna, fila);

                // Se esteganografia por el metodo LSB en el orden: r, g, b y a (si 32 bpp)
                valor = OperacionesBinarias::siguienteValorString(NULL);
                if (valor >= 0){
                    OperacionesBinarias::setearValorLSB(&pixel.Red, valor, lsbMitad);
                    valor = OperacionesBinarias::siguienteValorString(NULL);
                    if (valor >= 0){
                        OperacionesBinarias::setearValorLSB(&pixel.Green, valor, lsbMitad);
                        valor = OperacionesBinarias::siguienteValorString(NULL);
                        if (valor >= 0){
                            OperacionesBinarias::setearValorLSB(&pixel.Blue, valor, lsbMitad);
                            if (bpp == 32){
                                valor = OperacionesBinarias::siguienteValorString(NULL);
                                if (valor >= 0){
                                    OperacionesBinarias::setearValorLSB(&pixel.Alpha, valor, lsbMitad);
                                }
                            }
                        }
                    }
                }
                // Seteo el pixel y avanzo el recorrido de la imagen
                imagen.SetPixel(columna++, fila, pixel);
                if (columna == ancho){
                    columna = 0;
                    fila++;
                    if (fila == largo)
                        valor = -1;
                }
            }//while
        }//16,24 y 32
        estado = imagen.WriteToFile(nombreArchivo.c_str());
    }
    return estado;
}

string ArchivoImagenBMP::recuperarDatos (const unsigned int offset, const unsigned int bitsInformacion ){
    bool estado = false;
    bool valor;
    unsigned short int bpp; // bits per pixel
    unsigned short int lsb, bitsUtilesPx, correccion;
    unsigned int fila, columna, largo, ancho, tamanoFila;
    unsigned int bitsLeidos;

    // Abrimos el archivo imagen BMP
    BMP imagen;
    RGBApixel pixel;
    estado = imagen.ReadFromFile(this->obtenerNombreCompleto().c_str());
    if (estado){
        bpp = imagen.TellBitDepth();
        largo = imagen.TellHeight();
        ancho = imagen.TellWidth();

        bitsLeidos = 0;
        lsb = (bpp == (2*BITS_BYTE)) ? 4 : 0;

        bitsUtilesPx = (bpp == (4*BITS_BYTE)) ? 4 : 3;
        tamanoFila = bitsUtilesPx * ancho; // Real en bits
        fila = (offset * BITS_BYTE) / tamanoFila;
        columna = ((offset*BITS_BYTE - fila*tamanoFila) / (bitsUtilesPx));

        correccion = ((offset*BITS_BYTE - fila*tamanoFila) % (bitsUtilesPx));
        if (correccion != 0)
           columna++;
        if (columna >= ancho){
            columna = 0;
            fila++;
        }

        if (bpp == BITS_BYTE) {
            fila = (offset*BITS_BYTE)/bitsUtilesPx;
            if (((offset*BITS_BYTE) % bitsUtilesPx) != 0)
                fila++;
            while (bitsLeidos < bitsInformacion){
                pixel = imagen.GetColor(fila);

                valor = OperacionesBinarias::valorEn(pixel.Red,lsb);
                OperacionesBinarias::concatenarValorString(valor);
                if (++bitsLeidos < bitsInformacion){
                    valor = OperacionesBinarias::valorEn(pixel.Green,lsb);
                    OperacionesBinarias::concatenarValorString(valor);
                    if (++bitsLeidos < bitsInformacion){
                        valor = OperacionesBinarias::valorEn(pixel.Blue,lsb);
                        OperacionesBinarias::concatenarValorString(valor);
                        bitsLeidos++;
                    }
                }
                imagen.SetColor(fila, pixel);
                if (++fila == 256)
                    bitsLeidos = bitsInformacion;
            }// while
        }
        else if (bpp >= (2*BITS_BYTE)){ // 16, 24 y 32
            while (bitsLeidos < bitsInformacion){
                pixel = imagen.GetPixel(columna, fila);

                valor = OperacionesBinarias::valorEn(pixel.Red,lsb);
                OperacionesBinarias::concatenarValorString(valor);
                if (++bitsLeidos < bitsInformacion){
                    valor = OperacionesBinarias::valorEn(pixel.Green,lsb);
                    OperacionesBinarias::concatenarValorString(valor);
                    if (++bitsLeidos < bitsInformacion){
                        valor = OperacionesBinarias::valorEn(pixel.Blue,lsb);
                        OperacionesBinarias::concatenarValorString(valor);
                        if (++bitsLeidos < bitsInformacion){
                            if (bpp == 32){
                                OperacionesBinarias::concatenarValorString(OperacionesBinarias::valorEn(pixel.Alpha,lsb));
                                bitsLeidos++;
                            }
                        }
                    }
                }
                if (++columna == ancho){
                    columna = 0;
                    fila++;
                    if (fila == largo)
                        bitsLeidos = bitsInformacion;
                }
            }// while
        }//16,24 y 32
    }
    return OperacionesBinarias::concatenarValorString(-1);
}

void ArchivoImagenBMP::preProcesar(){
    BMP imagen;
    imagen.ReadFromFile(this->obtenerNombreCompleto().c_str());
    unsigned short int bpp = imagen.TellBitDepth();
    unsigned short int largo = imagen.TellHeight();
    unsigned short int ancho = imagen.TellWidth();

    if (bpp == BITS_BYTE)
        this->espacioDisponible = (3*256)/BITS_BYTE;
    else if (bpp == 4*BITS_BYTE)
        this->espacioDisponible = (4*largo*ancho)/BITS_BYTE;
    else
        this->espacioDisponible = (3*largo*ancho)/BITS_BYTE;
    this->bytesLibres = this->espacioDisponible;
}


// private:
