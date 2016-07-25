#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

#define MAX_BUFFER 64

#include <iostream>
#include <string>
#include <queue>
using namespace std;

class UI{
    protected:

    // Constructor
       UI();


    private:

       static UI* _instancia;

        /**
         * Parsea la instruccion pasada por parametro y la ejecuta. Devuelve false si la
         * instruccion no es una instruccion valida para el sistema
         */
        bool parsearInstruccion(string instruccion);

        /**
         *Parsea parametros
         */
        void obtenerParametros(string linea,string &parametroUno,string &parametroDos);


        // Destructor
        inline ~UI(){

        };

    public:
       /**
         * Devuelve la instancia unica segun el patron de diseño Singleton
         */
        static UI* getInstancia();

        /**
         * Espera las instrucciones del usuario, y las ejecuta hasta la instruccion "quit".
         */
        void escucharInstrucciones();


        /**
         * Pide al usuario el password del sistema hasta que se ingrese el correcto.
         */
        void perdirPassword();

        /**
         *  Desabilita el motor limpiando los recursos que este estaba utilizando.
         *   El mismo queda inutilizable
         */
        inline static void limpiar(){
            delete (UI::getInstancia());
        }

        /**
         * Imprime por pantalla el mensaje acorde al codigo de error recibido por parámetro
         */
        void imprimir ( int error );

        /**
         * Imprime por pantalla el mensaje pasado por parametro
         */
        void imprimir ( string mensaje );

        /**
         * Recorre la lista imprimiendo su contenido
         */
        void imprimir ( queue<string> elementos );


        /**
         * Definicion de errores posibles
         */
          enum Error{
            OK,
            RECURSOS_INSUFICIENTES,
            DIRECTORIO_INVALIDO,
            DIRECTORIO_YAEXISTE,
            DIRECTORIO_NO_EXISTE,
            FALTA_PORTADOR,
            FALLO_OCULTAR,
            FALLO_DESOCULTAR,
            FALLO_ACTUALIZARREGS,
            FALLO_CREARHILOOBS,
            TEXTO_YAEXISTE,
            TEXTO_NO_EXISTE,
            TEXTO_INVALIDO,
            TEXTO_VACIO,
            PASSWORD_INVALIDO,
            PASSWORD_LARGO_INCORRECTO,
            PASSWORD_MAX_INTENTOS_SUPERADOS,
            PERDIDA_RECURSOS,
            ERROR_INICIAR_SISTEMA,
            SENTENCIA_MAL_FORMADA,
            SIN_DIRECTORIOS,
            SIN_ARCHIVOS
        };


        /**
         * Definicion de parseos posibles
         */
          enum TipoParseo{
            INSTRUCCION_PARAMETROS,
            PARAMETRO_UNO_DOS
        };

     /**
     * Imprime por pantalla el mensaje acorde al codigo de error recibido por parámetro
     */
    void imprimir ( Error error );

    private:
    /**
    *Parsea parametros
    */
    bool obtenerParametros(string linea,string &parserUno,string &parserDos,TipoParseo tipo);
};

#endif // UI_H_INCLUDED
