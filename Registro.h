#ifndef REGISTRO_H_INCLUDED
#define REGISTRO_H_INCLUDED

#include <vector>
#include "Campo.h"


class Registro{

    /*------------------------ATRIBUTOS/METODOS PRIVADOS ---------------------*/
    private:
            std::vector<Campo> campos;//VECTOR DINAMICO DE CAMPOS

            int numCampos;//CANTIDAD DE CAMPOS DENTRO DEL REGISTRO
            char estado;//DEFINE UN ESTADO PARA EL REGISTRO (VALIDO,INVALIDO)

            /**
            @pre  : El campo debe ser valido.
            @post : Agrega un campo al registro. */
            void agregarCampo(Campo* campo);
    /*  --------------------------------------------------------------------- */



    /*--------------------------- METODOS PUBLICOS ---------------------------*/
    public:
            /**
            @pre  : Vector de campos valido y con al menos un campo valido.
            @post : Constructor que inicializar a partir de la lista de campos un registro operable. */
            Registro(std::vector<Campo> campo);

            /**
            @post : Cambia el estado del registro en libre ('L') u ocupado ('O'). */
            void cambiarEstado(char estado);

            /**
            @pre  : El nombre del campo debe ser valido. (UPCASE)
            @post : Devuelve un campo del registro con el nombre establecido .*/
            Campo getCampo(std::string nombre);

            /**
            @pre  : El numero de campo debe ser valido. (< getNumCampos())
            @post : Devuelve un campo del registro correspondiente al indice pedido .*/
            Campo getCampo(int indice);

            unsigned int getNumCampos();

            std::vector<int> getCampos();

            /**
            * Devuelve el tamaño en bytes del registro (datos), mas el tamaño del campo Estado.
            */
            unsigned int getSizeCampos();

            /**
            * @post : Devuelve el estado del registro
            */
            char getEstado();

            /**
            @pre  : El nombre del campo debe ser valido. (UPCASE)
            @post : Setea el valor del campo */
            void setValorCampo(std::string nombre, int valor);

            /**
            Destructor por defecto..
            */
            ~Registro();
    };

#endif // REGISTRO_H_INCLUDED
