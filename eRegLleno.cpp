#include "eRegLleno.h"

const char* eRegLleno::what() const throw() {
   switch(motivo) {
     case 1:
        return "Registro Lleno";
   }
   return "Error inesperado";
}
