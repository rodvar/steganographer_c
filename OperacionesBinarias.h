#ifndef OPERACIONESBINARIAS_H_INCLUDED
#define OPERACIONESBINARIAS_H_INCLUDED

#define MASK 0x01       // 0000 0001
#define MASK_NULA 0x00  // 0000 0000
#define MASK_EOF  0x80  // 1000 0000
#ifndef BITS_BYTE
#define BITS_BYTE 8
#endif

#include <string>
#include <sstream>
#include <math.h>
using namespace std;

/**
 OperacionesBinarias encapsula todas las operaciones posibles a realizar sobre la minima
 unidad de informacion manejable en memoria: El Byte (8 bits)
*/
class OperacionesBinarias{

public:
    /**
     * Procesa la cadena bit a bit guardando el estado en la posicion correspondiente
     * de un unsigned int. El algoritmo es independiente de la arquitectura del a computadora
     * que corre el programa
     * @return La tira de bits de la cadena interpretada como un unsigned int
     * @param cadena Es la cadena a procesar. La misma debe tener tantos 4B
     */
    static int convertirStringInt(const string cadena){
        register int retorno = 0;
        int mascara = 1;
        unsigned char caracter = 0;
        unsigned short int posicion = 0;
        const unsigned short int longitud = sizeof(unsigned int) * BITS_BYTE;
        bool valor;

        for (unsigned int i=0; i < cadena.size(); i++){
            caracter = cadena[i];
            for (unsigned int j =0; j < BITS_BYTE; j++){
                valor = valorEnInverso(caracter, j);
                if (valor)
                    retorno = retorno | (mascara << ((longitud-1) - posicion));
                else
                    retorno = retorno & ~(mascara << ((longitud-1) - posicion));
                posicion++;
            }
        }

        return retorno;
    }

    /**
     * Procesa un entero bit a bit formando una cadena de caracteres de 8 bits . La salida tiene 4B
     * @return Una cadena de 4 Bytes
     * @param entero Es el entero a convertir
     */
    static string convertirIntAString(const int entero){
        string retorno = " ";
        unsigned char byte = MASK_NULA;
        int mascara = 1;
        unsigned short int posicion = 0;
        unsigned short int posicionByte;
        const unsigned short int longitud = sizeof(unsigned int)  * BITS_BYTE;
        bool valor;

        for (int i = 0; i < longitud; i++){
            posicionByte = i % BITS_BYTE;
            if ((i > 0) && (posicionByte == 0)){
                retorno [posicion++] = byte;
                byte = MASK_NULA;
                retorno.append(" ");
            }
            valor = ((mascara << ((longitud-1) - i)) & entero);
            OperacionesBinarias::setearValorEnInverso(&byte,posicionByte ,valor);
        }
        retorno [posicion] = byte;

        return retorno;
    }

    /**
     * Procesa un entero bit a bit formando una cadena de caracteres de 8 bits de acuerdo a la arquitectura
     * de la computadora que corre el programa. Ej:  Si la arquitectura soporta 64 bits como doble palabra,
     * el string de salida tendra 8 bytes
     * @return Una cadena de tantos caracteres como bytes use la computadora para representar
     *         el unsigned long int
     * @param entero Es el entero a convertir
     */
    static string convertirLongAString(const unsigned long int entero){
        string retorno = " ";
        unsigned char byte = MASK_NULA;
        unsigned long int mascara = 1;
        unsigned short int posicion = 0;
        unsigned short int posicionByte;
        const unsigned short int longitud = OperacionesBinarias::longitudDoblePalabra();
        bool valor;

        for (int i = 0; i < longitud; i++){
            posicionByte = i % BITS_BYTE;
            if ((i > 0) && (posicionByte == 0)){
                retorno [posicion++] = byte;
                byte = MASK_NULA;
                retorno.append(" ");
            }
            valor = ((mascara << ((longitud-1) - i)) & entero);
            OperacionesBinarias::setearValorEnInverso(&byte,posicionByte ,valor);
        }
        retorno [posicion] = byte;

        return retorno;
    }

    /**
     * Procesa la cadena bit a bit guardando el estado en la posicion correspondiente
     * de un unsigned long int. El algoritmo es independiente de la arquitectura del a computadora
     * que corre el programa
     * @return La tira de bits de la cadena interpretada como un unsigned long int
     * @param cadena Es la cadena a procesar. La misma debe tener tantos bytes como
     *        bytes use la arquitectura de la computadora que corre el sistema, para representar
     *        un "unsigned long int".
     */
    static unsigned long int convertirString(const string cadena){
        unsigned long int retorno = 0;
        unsigned long int mascara = 1;
        unsigned char caracter = 0;
        unsigned short int posicion = 0;
        unsigned short int longitud = longitudDoblePalabra();
        bool valor;

        for (unsigned int i=0; i < cadena.size(); i++){
            caracter = cadena[i];
            for (int j =0; j < BITS_BYTE; j++){
                valor = valorEnInverso(caracter, j);
                if (valor)
                    retorno = retorno | (mascara << ((longitud-1) - posicion));
                else
                    retorno = retorno & ~(mascara << ((longitud-1) - posicion));
                posicion++;
            }
        }

        return retorno;
    }

    /**
     * Permite saber para la computadora en la que se corre el sistema, la cantidad de bits
     * que se estan utilizando para representar una doble palabra
     */
    static unsigned short int longitudDoblePalabra(){
        static unsigned short int retorno = 0;
        if (!retorno){
            switch (sizeof(unsigned long int)){
                    case 2: //Computadora vieja..
                        retorno = 16;
                        break;
                    case 4:
                        retorno = 32;
                        break;
                    case 8:
                        retorno = 64;
                        break;
                    default:
                        retorno = 32;
                        break;
            };
        }
        return retorno;
    }

    /**
     * @return Devuelve el valor del bit en posicion (TRUE = 1 , FALSE = 0)
     * @param byte El byte del cual obtener el valor del bit en posicion
     * @param posicion La posicion del bit. Valores posibles de 0 a 7 (de menos a mas significativo)
     */
    static bool valorEn(const unsigned char byte, const unsigned short int posicion){
        return ((MASK << (posicion)) & byte);
    }

    /**
     * @return Devuelve el valor del bit en posicion (TRUE = 1 , FALSE = 0) contando los bits en orden inverso
     * @param byte El byte del cual obtener el valor del bit en posicion
     * @param posicion La posicion del bit. Valores posibles de 0 a 7 (de mas a menos significativo)
     */
    static bool valorEnInverso(const unsigned char byte, const unsigned short int posicion){
        return ((MASK << ((BITS_BYTE-1) - posicion)) & byte);
    }

    /**
     * @return Devuelve el valor del bit en posicion (TRUE = 1 , FALSE = 0) contando los bits en orden inverso
     * @param byte El byte del cual obtener el valor del bit en posicion
     * @param posicion La posicion del bit. Valores posibles de 0 a 7 (de mas a menos significativo)
     */
    static bool valorEnInversoPalabra(const unsigned int palabra, const unsigned short int posicion){
        register unsigned int mask = 1;
        return ((mask << ((sizeof(unsigned int)*BITS_BYTE-1) - posicion)) & palabra);
    }

    /**
     * Setea el valor en el LSB del byte
     * @param byte El byte a modificar
     * @param valor true para setear 1, false para 0
     * @param mitad Si se debe interpretar el LSB desde la mitad del byte es true, sino false
     */
    static void setearValorLSB(unsigned char* byte, bool valor, bool mitad){
        unsigned short int shift = mitad ? 4 : 0;
        if (valor)
            *byte = (*byte | (MASK << shift));
        else
            *byte = (*byte & (~(MASK << shift)));
    }

    /**
     * Setea en el byte, el bit de la posicion pasada por parametro en el valor correspondiente.
     * @param byte El byte a modificar
     * @param posicion 0 -> mas significativa, 7 -> menos significativa
     * @param valor true para setear 1, false para 0
     */
    static void setearValorEnInverso(unsigned char* byte, const unsigned short int posicion, bool valor){
        if (valor)
            *byte = *byte | (MASK << ((BITS_BYTE-1) - posicion));
        else
            *byte = *byte & ~(MASK << ((BITS_BYTE-1) - posicion));
    }

    /**
     * Setea en el palabra, el bit de la posicion pasada por parametro en el valor correspondiente.
     * @param palabra El palabra a modificar
     * @param posicion 0 -> mas significativa, 31 -> menos significativa
     * @param valor true para setear 1, false para 0
     */
    static void setearValorEnInversoPalabra(unsigned int* palabra, const unsigned short int posicion, bool valor){
        register unsigned int mask = 1;
        if (valor)
            *palabra = *palabra | (mask << ((sizeof(unsigned int)*BITS_BYTE-1) - posicion));
        else
            *palabra = *palabra & ~(mask << ((sizeof(unsigned int)*BITS_BYTE-1) - posicion));
    }

    /**
     * Dado un octeto de bits (byte), devuelve el valor de su bit mas significativo.
     * Sucesivas llamadas a la funcion con el parametro byte=NULL devolveran los siguientes
     * bits hasta terminar de recorrerlo. Pasar un nuevo byte reinicializa el proceso
     * @return Devuelve el valor del bit en la posicion mas significativa, siguiendo hasta
     *         la menos significativa. Devuelve -1 si termino de recorrer el byte
     * @param byte El byte a recorrer
     */
    static short int siguienteValorCaracter(unsigned char* byte){
        static unsigned char octeto;
        static short int posicion;
        register short int retorno = 0;
        if (byte){
            octeto = *byte;
            retorno = valorEn(octeto, (BITS_BYTE-1));
            posicion = (BITS_BYTE-2);
        }
        else {
            if (posicion >= 0)
                retorno = valorEn(octeto, posicion--);
            else
                retorno = -1;
        }
        return retorno;
    }

    /**
     * Dada una cadena de bytes, trabaja uno a uno sus bytes. En la primer llamada pasando el string
     * como parametro se obtiene como respuesta 0 si todo esta bien
     * Sucesivas llamadas a la funcion con el parametro cadena=NULL devolveran los bits de la cadena
     * del mas al menos significativo hasta terminar de recorrerlo. Pasar una nueva cadena reinicializa
     *  el proceso
     * @return Devuelve el valor del bit en la posicion mas significativa, siguiendo hasta
     *         la menos significativa. Devuelve -1 si termino de recorrer la cadena
     * @param cadena La cadena a recorrer
     */
    static short int siguienteValorString(string* cadena){
        static const char* caracteres;
        static unsigned char octeto;
        static unsigned int posicion;
        static unsigned int tamano; // en bits
        register short int retorno = 0;
        if (cadena){
            caracteres = (*cadena).c_str();
            tamano = (*cadena).size() * BITS_BYTE;
            retorno = 0;
            posicion = 0;
        }
        else {
            if (posicion < tamano){
                if ((posicion % BITS_BYTE) == 0)
                    octeto = caracteres[posicion / BITS_BYTE];
                retorno = valorEnInverso(octeto, (posicion - ((posicion/BITS_BYTE)*BITS_BYTE)));
                posicion++;
            }
            else
                retorno = -1;
        }
        return retorno;
    }

    /**
     * Permite manejar un string como una cadena de bits, agregando los valores pasados por parametro
     * Si se quiere finalizar el proceso, valor = -1 devuelve la cadena al momento de la llamada.
     * Si se quiere agregar un valor 1 q marque el fin de la informacion, se debe pasar -2 para finalizar
     *  el proceso
     * @return Devuelve el string con caracteres acumulados hasta el momento de la llamada. Si no cierra
     *         a multiplo de 8 completa con 0.
     * @param valor es el valor de bit a agregar a la cadena de bits. -1 devuelve la cadena actual y
     *        reinicializa el proceso
     */
    static string concatenarValorString(const short int valor){
        static unsigned char octeto = MASK_NULA;
        static unsigned int posicion = 0;
        static unsigned int ultimaPosicion = 0;
        static string cadena = " ";
        string retorno;
        if (valor < 0){
            if (valor == -3){
                if (posicion != 0)
                    cadena[ultimaPosicion] = octeto;
            }
            else if (valor == -2){ // con EOF
                if (posicion != BITS_BYTE){
                    setearValorEnInverso(&octeto, posicion++, 1);
                    cadena[ultimaPosicion] = octeto;
                }
                else{
                    cadena[ultimaPosicion++] = octeto;
                    cadena.append(" ");
                    octeto = MASK_EOF;
                    cadena[ultimaPosicion] = octeto;
                }
            }
            else if (valor == -1)  // sin EOF
                if (posicion != 0)
                    cadena[ultimaPosicion] = octeto;

            retorno = cadena;
            if (valor != -3){ // No reinicializar si no es necesario
                cadena.clear();
                octeto = MASK_NULA;
                cadena.append(" ");
                posicion = 0;
                ultimaPosicion = 0;
            }
        }
        else {
            if (posicion == BITS_BYTE){
                cadena[ultimaPosicion] = octeto;
                cadena.append(" ");
                ultimaPosicion++;
                octeto = MASK_NULA;
                posicion = 0;
            }
            setearValorEnInverso(&octeto, posicion++, valor);
            retorno = cadena;
        }
        return retorno;
    }

    /**
     * Expresa el numero en formato hexadecimal, y luego lo formatea en un string de
     * de 4 caracteres de longitud ("0000" a "FFFF")
     * @return El numero pasado a hexa en un string de 4 caracteres
     * @param numero el numero a convertir
     */
    static string shortAHexString(unsigned short int numero){
        ostringstream osstream;
        string base = "0123456789ABCDEF";

        for (int i= 2*sizeof(unsigned short int) - 1; i>=0; i--) {
             osstream << base[((numero >> i*4) & 0xF)];
        }

        return osstream.str();
    }

    /**
     * Interpreta un string de 4 caracteres como un numero hexadecimal y lo pasa a
     * unsigned short
     * @return El entero corto que representa el hexString
     * @param hexString Los 4 caracteres que representan un numero hexadecimal
     */
    static unsigned short int hexStringAShort(string hexString){
        unsigned short int numero = 0;
        string base = "0123456789ABCDEF";
        unsigned short int size = hexString.size();

        for (unsigned int i=0; i < size; i ++){
            numero += base.find_first_of(hexString[i],0) * pow(16, (size-1) - i);
        }

        return numero;
    }

    /**
     * Dado un valor decimal, sucesivas llamadas con este en NULL devolveran su codificacion
     * binaria en N bits.
     * @return La codificacion binaria del decimal pasado por parametro, en N bits
     * @param decimal El decimal a codificar
     * @param N la cantidad de bits a utilizar (max = 32)
     */
    static bool codificarBinario(unsigned int* decimal, unsigned short int N){
        static unsigned int numero;
        static short int posicion;
        register bool retorno = false;

        if (decimal){
            numero = *decimal;
            posicion = sizeof(unsigned int)*BITS_BYTE - N;
        }
        else{
            retorno = valorEnInversoPalabra(numero,posicion++);
        }
        return retorno;
    }

    /**
     * Concatena los valores pasados por parametro hasta recibir un valor=-1, donde devuelve
     * el valor decimal correspondiente
     * @return El numero decimal correspondiente a la codificacion binaria concatenada
     * @param valor El valor del bit
     * @param N la cantidad de bits q codifican el binario del decimal a obtener (max = 32)
     */
    static unsigned int decodificarBinario(short int valor, unsigned short int N){
        static unsigned int numero = MASK_NULA;
        static short int posicion;
        static bool primeraVez = true;
        register unsigned int retorno = 0;

        if (valor == -1){
            retorno = numero;
            primeraVez = true;
            numero = MASK_NULA;
        }
        else{
            if (primeraVez){
                posicion = sizeof(unsigned int)*BITS_BYTE - N;
                primeraVez = false;
            }
            setearValorEnInversoPalabra(&numero,posicion++,valor);
        }
        return retorno;
    }

    /**
     * Para arquitecturas de 64 bits, concatena a y b (ambos de 32 bits) dando
     * como resultado los 64 bits :  ab
     */
    static unsigned long int fusionar (const int a, const int b){
        register unsigned long int fusion = 0;
        register unsigned long int mask = a;
        fusion |= (mask << (sizeof(unsigned int)*BITS_BYTE));
        mask = 1;
        for (short int i = (sizeof(unsigned int)*BITS_BYTE - 1); i >= 0; i--){
            if ((mask << i) & b)
                fusion |= (mask << i);
            else
                fusion &= (~(mask << i));
        }
        return fusion;
    }

    /**
     * Para arquitecturas de 64 bits, separa un long de 64 bits en 2 ints de
     * 32 bits cada uno
     */
    static void desFusionar (const unsigned long int numero, int* a, int* b){
        *b = 0;
        *a = 0;
        *b |= numero;
        *a |= (numero >> (sizeof(unsigned int)*BITS_BYTE));
    }

    /**
     * @return El AND logico entre 2 octetos
     * @param byte, byte2: Los bytes a evaluar
     */
    static unsigned char AND(const unsigned char byte, const unsigned char byte2){
        return (byte & byte2);
    }

    /**
     * @return El OR logico entre 2 octetos
     * @param byte, byte2: Los bytes a evaluar
     */
    static unsigned char OR(const unsigned char byte, const unsigned char byte2){
        return (byte | byte2);
    }

    /**
     * @return El XOR logico entre 2 octetos
     * @param byte, byte2: Los bytes a evaluar
     */
    static unsigned char XOR(const unsigned char byte, const unsigned char byte2){
        return (byte ^ byte2);
    }

    /**
     * @return El NOR logico del byte
     * @param byte el byte a negar
     */
    static unsigned char NOT(const unsigned char byte){
        return (~byte);
    }
};

#endif // OPERACIONESBINARIAS_H_INCLUDED
