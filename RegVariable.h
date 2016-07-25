#ifndef REGVARIABLE_H_INCLUDED
#define REGVARIABLE_H_INCLUDED
#include "eRegLleno.h"
#include "OperacionesBinarias.h"

/**
*RegVariable: Permite almacenar nombres de tamaño variable. Una vez creada se puede
*cambiar el nombre pero debe ser del mismo tamaño o menor que el anterior. Posee
*tambien atributos administrativos para su correcta utilizacion.
*La excepcion que lanza es de tipo eRegLleno y se da justamente cuando el nombre que
*quiero grabar es mayor al anterior.
*/

class RegVariable{
private:
        char estado;
        unsigned short int size;
        unsigned short int libre;
        unsigned short int siguiente;
        string nombre;

public:

        /**
        *RegVariable: Constructor de la clase, deja en estado valido un objeto con el nombre establecido.
        *@param nombre: Variable tipo string que se ajustara al registro variable.
        *@return
        */
        RegVariable(string nombre);

        /**
         * Destructor
         */
         ~RegVariable();

        /**
        *setNombre: Se ajusta el nombre dependiendo del tamaño que se tenga inicialmente.
        *@param n: Variable tipo string que se ajustara al registro variable.
        *@return
        */
        void setNombre(std::string n);
        /**
        *setEstado: Se ajusta el estado del registro variable.
        *@param estado: Variable tipo bool que se ajustara al registro variable.
        *@return
        */
        void setEstado(bool estado);
        /**
        *getSize: Se devuelve el tamanio del campo variable del registro.
        *@param
        *@return unsigned short int
        */
        unsigned short int getSize();
        /**
        *getNombre: Se devuelve el nombre del registro.
        *@param
        *@return string
        */
        string getNombre();
        /**
        *getNombre: Se devuelve el estado del registro.
        *@param
        *@return char
        */
        char getEstado();
        /**
        *getNombre: Se concatenan todos los campos del registro variable. Esta funcion es util en el caso de tener que grabar en disco un registro.
        *@param
        *@return string
        */
        string getCamposConcat();
        /**
        *getNombre: Se devuelve el puntero al siguiente registro.
        *@param
        *@return unsigned short int
        */
        unsigned short int getSiguiente();
        /**
        *getNombre: Se setea el campo siguiente.
        *@param siguiente: Variable de tipo unsigned short int que se seteara en el registro variable.
        *@return
        */
        void setSiguiente(unsigned short int siguiente);
        /**
        *getNombre: Se devuelve la cantidad de espacio libre que tiene ese registro.
        *@param
        *@return unsigned short int
        */
        unsigned short int getLibre();

        /**
        *getNombre: Se setea el campo free.
        *@param libre: Variable de tipo unsigned short int que se seteara en el registro variable.
        *@return
        */
        void setLibre(unsigned short int libre);
};
#endif // REGVARIABLE_H_INCLUDED
