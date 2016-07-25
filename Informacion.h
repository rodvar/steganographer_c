#ifndef INFORMACION_H_INCLUDED
#define INFORMACION_H_INCLUDED

#include "Almacenable.h"
#include <string>
#include <fstream>

using namespace std;

/**
 * Clase que representa lo que es la informacion para el sistema.
 */
class Informacion : public Almacenable{
    protected:
        unsigned int tamano;
        bool comprimida;

    public:
        // Constructor
        Informacion(){
            this->tamano = 0;
            this->comprimida = false;
        }

        // Destructor librado a clases hijas
        virtual ~Informacion(){
        }

        /**
         * Devuelve el tamaño de la informacion
         */
        virtual unsigned int getTamano(){
            return this->tamano;
        };

        /**
         * Setea el tamaño de la informacion
         */
        virtual void setTamano(unsigned int tamano){
            this->tamano = tamano;
        };

        /**
         * Permite setear la bandera q indica si la particion fue o no comprimida
         */
        void setComprimida (bool comprimida){
            this->comprimida = comprimida;
        }

        /**
         * Permite saber si la particion esta comprimida
         */
        bool estaComprimida(){
            return this->comprimida;
        }

};

#endif // INFORMACION_H_INCLUDED
