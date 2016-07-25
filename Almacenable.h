#ifndef ALMACENABLE_H_INCLUDED
#define ALMACENABLE_H_INCLUDED

#include "OperacionesBinarias.h"

/**
 * Clase que representa una entidad almacenable en memoria secundaria
 */
class Almacenable{
    private:
        bool persistido; // TRUE si el Almacenable fue persistido.

    protected:
        unsigned int id; // Identificador unico para la entidad

    public:
        //Constructor
        Almacenable(){
            this->id = 0;
            this->persistido = false;
        }

        // Destructor librado a clases hijas
        virtual ~Almacenable(){
        }

        /**
         * Get the value of this->id
         * @return the value of this->id
         */
        unsigned int getId ( ) {
            return this->id;
        }

        /**
         * Setthe value of this->id
         */
        void setId (unsigned short int id ) {
            this->id = id;
        }

        /**
         * Get the value of persistido
         * @return the value of persistido
         */
        unsigned int esPersistido ( ) {
            return this->persistido;
        }

        /**
         * Setthe value of persistido
         */
        void setPersistido (bool persistido ) {
            this->persistido = persistido;
        }
};

#endif // ALMACENABLE_H_INCLUDED
