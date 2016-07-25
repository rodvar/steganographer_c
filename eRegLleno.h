#ifndef EREGLLENO_H_INCLUDED
#define EREGLLENO_H_INCLUDED
#include <exception>
using namespace std;

/**
*eRegLleno: Excepcion creada para salvaguardar el problema de que un registro
*no permita que un nuevo nombre se almacene dentro de el ya que el tamaño del
*mismo supera el tamaño necesario para salvar los datos administrativos y el
*nombre propiamente dicho.
*/

 class eRegLleno: public exception
 {
 public:
   eRegLleno(int mot) : exception(), motivo(mot) {}
   const char* what() const throw();
  private:
   int motivo;
};

#endif // EREGLLENO_H_INCLUDED
