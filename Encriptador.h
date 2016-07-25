#ifndef ENCRIPTADOR_H
#define ENCRIPTADOR_H
#include <vector>
#include <stdio.h>
#include <math.h>
#include <string>
#include "OperacionesBinarias.h"

using namespace std;

        /** OPERACIONES DE CORRIMIENTO DE BITS */
#define ROTL(x,y) (((x)<<(y&(w-1))) | ((x)>>(w-(y&(w-1)))))
#define ROTR(x,y) (((x)>>(y&(w-1))) | ((x)<<(w-(y&(w-1)))))

#define Pw16     0xb7e1     /* constantes magicas   */
#define Qw16     0x9e37     /*para w=16             */


#define Pw32     0xb7e15163     /* constantes magicas   */
#define Qw32     0x9e3779b9     /*para w=32             */

#define Pw64     0xb7e151628aed2a6b     /* constantes magicas   */
#define Qw64     0x9e3779b97f4a7c15     /*para w=64             */

#define MAXUINT  2147483647    /* maximo valor magico */

        /** PARAMETROS VARIABLES DEL RC5: W, R, B
        *** TODOS CON VALORES POSIBLES PERTENECIONTES AL INTERVALO [0...255]*/
        /** SE SUGIERE VALORES PARA W DE 16, 32, 64 */

const char basura= '?'; // caracter basura con el q se completa el bloque q queda mas chico de lo q deberia
const int relleno=  MAXUINT -1; //  basura con el q se completa el bloque q queda mas chico de lo q deberia para encriptar vectores

//#define w   32             /* tamaño de la palabra en bits      */
//#define w   64             /* tamaño de la palabra en bits      */

//typedef unsigned /*long*/ int WORD; //para tamaño de palabra = 32
typedef unsigned long int WORD; //para tamaño de palabra = 32
class Encriptador
{
    public:
        /** Constructores
         */
        Encriptador();
        /** Constructor ara poder setear parametros variables *
         * @param r : cantidad de iteraciones a realizar
         * @param b : cantidad de bits de la clave
         */
        Encriptador(const unsigned char r,const unsigned char b);

        /**Destructor por defecto */
        ~Encriptador();

        /**
        * Funcion que encripta un string.
        * @return string encriptado con longitud multiplo de 2*la cantidad de bytes use la arquitectura de la computadora
        * que corre el sistema, para representar un "unsigned long int".
        * @param cadena Es la cadena a procesar.
        */
        string encriptarString(std::string input);

        /**
        * Funcion que desencripta un string.
        * @return string desencriptado (sacandole la basura que se pudo haber agregado al encr¡tar para completar)
        * @param cadena Es la cadena a procesar.
        */
        string desencriptarString(std::string input);

        /** HASHING DE PASSWORD **/

        /**
        * Devuelve un vector de 2 pos de unsigned long int
        * @pre : la password debe tener longitud >= 6
        * @return en la primera devuelve el leftHash de la clave
        * en la segunda, el rightHassh
        * @param pass password a Hashear
        */

        std::vector<unsigned long int> encriptarPassword(std::string pass);

        /** Funcion que dado el size de un vector de ints devuelve el size
            que tendria el vector encriptado mediante encriptarVector */
        unsigned int getTamanoVEncriptado(unsigned int longitud);


        unsigned int getTamanoSEncriptado(unsigned int size);

        /** Funcion que encripta un vector de ints independientemente de la
            arquitectura de la maquina */
        vector<int>  encriptarVector(vector<int> input);

        /** Funcion que desencripta un vector de ints (quitando la basura q pudo haber puesto el encriptador */
        vector<int> desencriptarVector(vector<int> input);


    private:
        /** TODO LO REFERIDO AL ENCRIPTADOR */
        unsigned short int tr;

        std::vector<WORD> S; //vector utilizado en el proceso de encriptacion/desencriptacion de tamaño t=2*(r+1)

        unsigned char rounds; //numero de rounds = iteraciones totales
        unsigned char bK; //numero de bytes de la clave secrta
        unsigned char w; // long de doble palabra


        /*** funcion que setea el vector S usado en la encriptacion/desencriptacion
           */
        void setearS();


        /** funcion interna para la encriptacion/desencriptacion:
        *** toma el string de entrada y lo corta tomando los bytes necsarios
        *** para formar un subbloque de 32 bits
        *** el vector resultante rellena con basura al final de un subbloque q no llegue a la long minima
        *** y si la cant de subbloques es impar agrega un subbloque final con cadena nula
        *** esto es porque la funcion que encripta toma de a 2 subbloques
        *** @param cadena el string originak*/
        vector<string> vStringSemiBloques(string cadena);



        /** funcion para encriptar utilizando el algoritmo en RC5:
        *** @param input: recibe la entrada en un bloque de 2 WORDS
        *** @param output devuelve el texto cifrado en un bloque de 2 WORDS **/
        void encriptar(WORD *input, WORD *ouput);

        void desencriptar(WORD *input, WORD *ouput);




        /** TODO LO REFERIDO AL HASH DE LA PASWORD */
        unsigned long int rightHash(std::string src); //funcion que calcula el hash derecho
        unsigned long int leftHash(std::string src);  //funcion que calcula el hash izq

};

#endif // ENCRIPTADOR_H
