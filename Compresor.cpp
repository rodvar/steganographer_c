/************************************************************************
                        Compresor.h.cpp - Copyright rodrigo

Here you can write a license for your code, some comments or any other
information you want to have in your generated code. To to this simply
configure the "headings" directory in uml to point to a directory
where you have your heading files.

or you can just replace the contents of this file with your own.
If you want to do this, this file is located at

/usr/share/apps/umbrello/headings/heading.cpp

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
The original location of this file is /home/rodrigo/uml-generated-code/Compresor.cpp
**************************************************************************/

#include "Compresor.h"

// Constructors/Destructors
//
Compresor::Compresor ( ) {
    this->LONGVENTANA = 1023;
    this-> P = 10 ;//8
    this-> L= 4 ;//5
    this-> MAXMATCH = 15;
    this-> MINMATCH=  3;
}

Compresor::Compresor(unsigned short int bitsPosicion, unsigned short int bitsLongitud,unsigned short int minMatch){
    this->P = bitsPosicion;
    this->L = bitsLongitud;
    this->MINMATCH = minMatch;
    this->MAXMATCH = this->potenciaDos(bitsLongitud) - 1;
    this->LONGVENTANA = this->potenciaDos(bitsPosicion) - 1;
}

Compresor::~Compresor ( ) { }

//
// Methods
//

   /**
     * Funcion que comprime un string usando el metodo LZSS
     *
     * @return la salida comprimida
     * @param input cadena para comprimir
     */

string Compresor::comprimir(string input){
    char ventana[LONGVENTANA];
    string mM;//string con el minimoMatch buscado
    long int i;
    string::size_type matchPos ;
    unsigned int longMatch=0, actual;
    bool match = false;
    long int indice = -1;
    short int sig;
    //inicializo la ventana de memoria
    for (i=0; i<LONGVENTANA; i++)
        ventana[i]= NULL;
    long int longEntrada;
    longEntrada= input.length();
    //LOS PRIMEROS MINMATCH CARACTERES LOS EMITO DIRECTAMENTE
    for (i=0; i<MINMATCH; i++){
        actual = input[i];
        OperacionesBinarias::concatenarValorString(0);
        sig= OperacionesBinarias::siguienteValorCaracter((unsigned char*)&actual);
        while (sig !=-1){
            OperacionesBinarias::concatenarValorString(sig);
            sig= OperacionesBinarias::siguienteValorCaracter(NULL);
        }
    }

    //ACTUALIZO LA VENTANA
    actualizarVentana(MINMATCH -1, ventana, input);
    i=MINMATCH;
    while (i <= longEntrada - 1 ) {
        actual = input[i];
        if (i + MINMATCH <= longEntrada){
            //cadena con la que busco el minMatch
            mM = input.substr(i, MINMATCH);
            //funcion que me devuelve el indice de v donde comienza el match si hay
            // si no hay, devuelve npos
            matchPos= buscarMatch(mM, ventana);
        }
        else
            matchPos = string::npos;
        if (matchPos !=string::npos){
            match = true;
            indice = i;
            longMatch=MINMATCH;
            while (match && longMatch<MAXMATCH && (i+ longMatch < longEntrada)){
                if (matchPos + longMatch <= LONGVENTANA - 1 ) // SI ESTOY ADENTRO DE LA VENTANA
                    if (ventana[matchPos+longMatch] == input[i+longMatch]){
                        longMatch++;
                    }
                    else{
                        match=false; //termino el match
                        emitir(longMatch, matchPos);
                    }
                else{ //toy afuera de la ventana
                    if (input[indice] == input[i+ longMatch]){
                        longMatch++ ;
                        indice++ ;
                    }
                    else{
                        match=false;
                        emitir(longMatch, matchPos);
                    }
                }
            }
        }
        else { //no hay match
            match= false;
            OperacionesBinarias::concatenarValorString(0);
            sig= OperacionesBinarias::siguienteValorCaracter((unsigned char*)&actual);

            while (sig !=-1){
                OperacionesBinarias::concatenarValorString(sig);
                sig= OperacionesBinarias::siguienteValorCaracter(NULL);
            }
        }

        if (!match){
            if ( indice >= i){ //si hubo algun match q despues termino
                actualizarVentana(i+ longMatch -1, ventana, input);
                i+= longMatch;
            }
            else{ //realmente no hubo match
                actualizarVentana(i, ventana, input);
                i++;
            }
        }
        else{
            emitir(longMatch, matchPos); //si salio del while porque el match ya era maximo o estaba en la ultima pos de la entrada
            match= false;
            actualizarVentana( i + longMatch -1, ventana, input);
            i += longMatch;
            }
    }
    return OperacionesBinarias::concatenarValorString(-2); //devuelve la cadena completada con EOF
}

   /**
     * Funcion que descomprime un string que ha sido comprimido usando el metodo LZSS
     *
     * @return el string original (descomprimido)
     * @param input cadena comprimida
     */

string Compresor::descomprimir(string input){
    long int longEntrada = input.length(), incremento, i;
    long int fin;
    char ventana[LONGVENTANA];
    string buffertemp, ventanaTemp;
    char marca= buscarFin(input), caracter;
    unsigned char j;
    unsigned int  l, p, pAux;
    short int bit, sig;

    for (i=0; i<LONGVENTANA; i++)
        ventana[i]= NULL;

    OperacionesBinarias::siguienteValorString(&input);
    fin = (longEntrada -1)* BITS_BYTE + marca; // cant de bits a leer
    for (i =0; i < fin ; i+= incremento){
        bit= OperacionesBinarias::siguienteValorString(NULL);

        if (bit == 0){ // viene un caracter
            for (j=0; j < BITS_BYTE; j++){
                //voy acumulando los sig 8 bits
                OperacionesBinarias::concatenarValorString(OperacionesBinarias::siguienteValorString(NULL));
            }

            incremento= BITS_BYTE +1;
        }
        else{ //viene un patron
            //LOS PRIMEROS L BITS ME DICEN LA LONGITUD
            for (j=0; j< L; j++){
                OperacionesBinarias::decodificarBinario(OperacionesBinarias::siguienteValorString(NULL), L);
            }
            l = OperacionesBinarias::decodificarBinario(-1, L);

            //LOS SIGUIENTE P BITS ME DICEN LA POS
            for (j=0; j< P; j++){
                OperacionesBinarias::decodificarBinario(OperacionesBinarias::siguienteValorString(NULL), P);
            }
            p = OperacionesBinarias::decodificarBinario(-1, P);

            pAux = p;

            //EMITO EL MATCH CORRESPONDIENTE
            for (j= 0; j< l; j++){
                caracter = ventana[(int)pAux];
                sig = OperacionesBinarias::siguienteValorCaracter((unsigned char*)&caracter);
                //EMITO LOS BITS DEL CARACTER CORRESPONDIENTE
                while (sig !=-1){
                    OperacionesBinarias::concatenarValorString(sig);
                    sig= OperacionesBinarias::siguienteValorCaracter(NULL);
                }
                pAux++;
                if (pAux >= LONGVENTANA) //PARA NO SALIRME DE LA VENTANA
                    pAux = p;
            }
            incremento = P + L +1;
        }

        buffertemp= OperacionesBinarias::concatenarValorString(-3);//corto lo que tengo hasta ahora
        actualizarVentana(buffertemp.length()-1, ventana, buffertemp); //ACTUALIZO LA VENTANA
    }
    OperacionesBinarias::concatenarValorString(-2); // Reseteo el string estatico;
    return buffertemp;
}

// GETTERS
unsigned short int Compresor::getBitsLongitud(){
    return this->L;
}

unsigned short int Compresor::getBitsPosicion(){
    return this->P;
}

unsigned short int Compresor::getMaximoMatch(){
    return this->MAXMATCH;
}

unsigned short int Compresor::getMinimoMatch(){
    return this->MINMATCH;
}

unsigned short int Compresor::getLongitudVentana(){
    return this->LONGVENTANA;
}

// SETTERS
void Compresor::setBitsLongitud (const unsigned short int bitsLongitud){
    this->L = bitsLongitud;
    this->LONGVENTANA = this->potenciaDos(bitsLongitud) - 1;
}

void Compresor::setBitsPosicion (const unsigned short int bitsPosicion){
    this->P = bitsPosicion;
    this->MAXMATCH = this->potenciaDos(bitsPosicion) - 1;
}

void Compresor::setMinimoMatch (const unsigned short int minMatch){
    this->MINMATCH = minMatch;
}


void Compresor::actualizarVentana(long int actual , char* v, string input){
    short int i;

    for (i=LONGVENTANA -1; i>-1;i--){
        if (actual>=0){
            v[i]= input[actual];
            actual--;}
        else
            v[i]= 0x00;
    }
}


int Compresor::buscarMatch(string minMatch, char* v){
    string ventana;
    ventana.assign(v, LONGVENTANA);
    return ventana.find(minMatch, 0);
}


void Compresor::emitir (unsigned int  longitud, unsigned int  pos){
    char i;
    OperacionesBinarias::concatenarValorString(1);
    OperacionesBinarias::codificarBinario((unsigned int*)&longitud, L);
    for (i=0; i<L; i++){
            OperacionesBinarias::concatenarValorString(OperacionesBinarias::codificarBinario(NULL, L));
    }
    OperacionesBinarias::codificarBinario((unsigned int*)&pos, P);
    for (i=0; i< P ; i++){
            OperacionesBinarias::concatenarValorString(OperacionesBinarias::codificarBinario(NULL, P));
    }
}

char Compresor::buscarFin(string input){
    unsigned char ultimo;
    long int longEntrada = input.length();
    //en el ultimo byte esta la marca del fin (ej 00001000)
    ultimo = input[longEntrada -1];
    char i;
    bool marca = false;
    for (i=BITS_BYTE -1; i>-1; i--){
        marca = OperacionesBinarias::valorEnInverso(ultimo, i);
        if (marca)
            return i;
    }
    return -1;
}

unsigned short int Compresor::potenciaDos (const unsigned short int potencia){
    unsigned short int resultado = 2;
    for (unsigned int i = 0; i < potencia; i++){
        resultado *= 2;
    }
    return resultado;
}
