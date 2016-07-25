#include "Motor.h"

using namespace std;

/**
 *********** PROGRAMA PRINCIPAL - TP - GRUPO 02 **************
*/
int main()
{
    // Levantamos el motor del sistema
    Motor::getInstancia();
    // y cargamos las listas de libres de los archivos de estructura administriva
    Motor::getInstancia()->getAdministradorRecursos()->inicializar();
    Motor::getInstancia()->getAdministradorRecursos()->indexarLibres();

    // Y la interfaz con el usuario
    UI::getInstancia()->perdirPassword();
    UI::limpiar();
    Motor::limpiar();
    return 0;
}
