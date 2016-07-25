#ifndef CAMPO_H_INCLUDED
#define CAMPO_H_INCLUDED

#include <string>
/**
*Campo: Clase encargada de formar parte de lo que hacen a un registro.
*Se le debe definir un nombre y el valor inicial para ese campo es 0.
*/

class Campo{

    private:
            unsigned int valor;
            std::string nombre;
    public:
            Campo();
            Campo(std::string nombre);
            unsigned int getValor();
            std::string getNombre();
            void setValor(unsigned int);
            ~Campo();
};

#endif // CAMPO_H_INCLUDED
