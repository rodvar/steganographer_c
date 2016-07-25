#include "UI.h"
#include "Motor.h"

UI* UI::_instancia = NULL;

UI* UI::getInstancia(){
            if (_instancia  == NULL){
                new UI();
            }
            return _instancia;
        };

UI::UI(){
};

void UI::imprimir (Error error ){
    string mensaje = "";
    switch (error){
        case OK:
            break;
        case RECURSOS_INSUFICIENTES:
            mensaje = "Los recursos que dispone el sistema actualmente no son suficientes para almacenar el texto. Considere agregar mas imagenes y/o otros directorios con imagenes al soporte";
            break;
        case DIRECTORIO_INVALIDO:
            mensaje = "El directorio especificado no existe, o no tiene permisos para accederlo";
            break;
        case DIRECTORIO_YAEXISTE:
            mensaje = "Imposible dar de alta, el directorio ya pertenece al soporte";
            break;
        case DIRECTORIO_NO_EXISTE:
            mensaje = "Imposible dar de baja, el directorio no pertenece al soporte";
            break;
        case FALLO_OCULTAR:
            mensaje = "Ocurrio un error inesperado durante el proceso de ocultar el texto. Verifique los permisos sobre los directorios que definio en el soporte";
            break;
        case FALLO_DESOCULTAR:
            mensaje = "Ocurrio un error inesperado durante el proceso de desocultar el texto. Verifique la existencia de los directorios que definio en el soporte, y sus permisos.";
            break;
        case FALTA_PORTADOR:
            mensaje = "Lo sentimos pero es imposible recuperar el texto posiblemente debido a que alguna de las imagenes del soporte ha sido removida del mismo.";
            break;
        case FALLO_ACTUALIZARREGS:
            mensaje = "Ocurrio un error inesperado al actualizar la informacion administrativa. Es posible que el soporte quede inestable.";
            break;
        case FALLO_CREARHILOOBS:
            mensaje = "Ocurrio un error al lanzar el observador del sistema de archivos Linux. Si se modifica el mismo no se vera reflejado en el sistema, se recomienda reiniciar.";
            break;
        case TEXTO_YAEXISTE:
            mensaje = "Imposible esteganografiar, ya existe un archivo con el nombre ingresado";
            break;
        case TEXTO_NO_EXISTE:
            mensaje = "Imposible realizar la operaciòn, no se encontro esteganografiado un archivo con el nombre ingresado.";
            break;
        case TEXTO_INVALIDO:
            mensaje = "No existe el archivo especificado";
            break;
        case TEXTO_VACIO:
            mensaje = "No es posible ocultar un texto que esta vacio";
            break;
        case PASSWORD_INVALIDO:
            mensaje = "El password ingresado es invalido.";
            break;
        case PERDIDA_RECURSOS:
            mensaje = "Uno o más recursos de su sistema han sido eliminados por no encontrarse en el sistema de archivos Linux. Es posible que existan textos que ya no puedan recuperarse.";
            break;
        case UI::ERROR_INICIAR_SISTEMA:
            mensaje = "Ocurrio un error al intentar iniciar el sistema.";
            break;
        case UI::SENTENCIA_MAL_FORMADA:
            mensaje = "Los campos no pueden contener espacios.";
            break;
        case UI::SIN_DIRECTORIOS:
            mensaje = "No se encontraron directorios";
            break;
        case UI::SIN_ARCHIVOS:
            mensaje = "No se encontraron archivos";
            break;
        case UI::PASSWORD_LARGO_INCORRECTO:
            mensaje = "El nuevo password debe tener un minimo de 6 caracteres.";
            break;
        case UI::PASSWORD_MAX_INTENTOS_SUPERADOS:
            mensaje = "Se ha superado el maximo establecido de tres intentos. Sistema bloqueado.";
            break;
    };

    if (mensaje != "")
        cout << "Error: " + mensaje << endl << endl;
}

void UI::imprimir ( string mensaje ){
    cout << mensaje << endl;
}

void UI::imprimir ( queue<string> cola ){
    while (!cola.empty()){
        cout << " " << cola.front();
        cola.pop();
    }
}

void UI::escucharInstrucciones(){
    string buffer;
    cout << "**************************************************" << endl;
    cout << "**   Bienvenido al Esteganografo del GRUPO 02   **" << endl;
    cout << "**************************************************" << endl;
    cout << "Ingrese la instrucción que desea ejecutar, 'help' para obtener ayuda o 'quit' para finalizar:" << endl;
    cout << ">";

    do {
        getline (cin,buffer);
        if (!this->parsearInstruccion(buffer))
            cout << "Instrucción inválida! Inténtelo nuevamente" << endl;
            if (buffer != "quit"){
                cout << "Ingrese una instrucción:" << endl;
                cout << ">";
            }
    }while (buffer != "quit");
}

bool UI::obtenerParametros(string linea,string &parserUno,string &parserDos,TipoParseo tipo){
    parserUno = linea.substr(0, linea.find_first_of(" ",0));
    parserDos = linea.substr(linea.find_first_of(" ",0) + 1);
    if(tipo == PARAMETRO_UNO_DOS){
        size_t found=parserDos.find_first_of(" ",0);
        if ((unsigned int)found < parserDos.length()){
            parserDos = parserUno;
            return false;
        }
    }
    if (tipo == INSTRUCCION_PARAMETROS){
         transform( parserUno.begin(), parserUno.end(), parserUno.begin(), ::tolower );
        }
    return true;
}

bool UI::parsearInstruccion(string buffer){
    string instruccion;
    string parametro;
    string parametroDos;
    string parametroUno;
    bool valida = true;
    this->obtenerParametros(buffer,instruccion,parametro,INSTRUCCION_PARAMETROS);

    if (instruccion == "adddirectory"){
        if (parametro.find_last_of('/') != (parametro.size() - 1))
        parametro += "/";
        Motor::getInstancia()->addDirectory(parametro);
    }
    else if (instruccion == "removedirectory"){
        if (parametro.find_last_of('/') != (parametro.size() - 1))
        parametro += "/";
        Motor::getInstancia()->removeDirectory(parametro);
    }
    else if (instruccion == "addfile"){
        Motor::getInstancia()->addFile(parametro);
    }
    else if (instruccion == "getfile"){
        if(this->obtenerParametros(parametro,parametroUno,parametroDos,PARAMETRO_UNO_DOS))
            Motor::getInstancia()->getFile(parametroUno,parametroDos);
        else{
            imprimir(SENTENCIA_MAL_FORMADA);
            valida = false;
        }
    }
    else if (instruccion == "removefile"){
        Motor::getInstancia()->removeFile(parametro);
    }
    else if (instruccion == "showfiles"){
        Motor::getInstancia()->showFiles();
    }
    else if (instruccion == "showdirectories"){
        if (parametro.find_last_of('/') != (parametro.size() - 1))
            parametro += "/";
        Motor::getInstancia()->showDirectories();
    }
    else if (instruccion == "showdirectoryinfo"){
        Motor::getInstancia()->showDirectoryInfo((unsigned int)atoi(parametro.c_str()));
    }
    else if (instruccion == "showfileinfo"){
        Motor::getInstancia()->showFileInfo((unsigned int)atoi(parametro.c_str()));
    }
    else if (instruccion == "showmediuminfo"){
        Motor::getInstancia()->showMediumInfo();
    }
    else if (instruccion == "changepassword"){
        if (this->obtenerParametros(parametro,parametroUno,parametroDos,PARAMETRO_UNO_DOS))
            Motor::getInstancia()->changePassword(parametroUno,parametroDos);
            else{
                imprimir(SENTENCIA_MAL_FORMADA);
                valida = false;
            }
    }
    else if (instruccion == "quit"){
        Motor::getInstancia()->quit();
    }
    else if (instruccion == "help"){

        cout << endl << "***Ingreso a los comandos de ayuda del sistema***" << endl;
        cout << "*************************************************" << endl << endl;
        cout << "+ addDirectory <directorio>" << endl;
        cout << "       Agrega un directorio de imágenes al ocultador" << endl << endl;
        cout << "+ removeDirectory <directorio>" << endl;
        cout << "       Quita un directorio de imágenes del ocultador" << endl <<endl;
        cout << "+ addFile <nombre_archivo>" << endl;
        cout << "       Oculta un archivo de texto en el ocultador" << endl << endl;
        cout << "+ removeFile <nombre_archivo>" << endl;
        cout << "       Quita un archivo de texto del ocultador" << endl << endl;
        cout << "+ getFile <nombre_archivo> <nombre_destino>" << endl;
        cout << "       Obtiene un archivo del ocultador. Si no se especifica nombre_destino se guarda con nombre_archivo" << endl << endl;
        cout << "+ showFiles" << endl;
        cout << "       Muestra un listado con los nombres de los archivos ocultos" << endl << endl;
        cout << "+ showDirectories" << endl;
        cout << "       Muestra un listado con la ruta de los directorios que contienen imagenes que se utilizan para ocultar textos" << endl << endl;
        cout << "+ showMediumInfo" << endl;
        cout << "       Muestra el listado con la ruta de los directorios dando ademas informacion sobre su rendimiento y estado actual" << endl << endl;
        cout << "+ changePassword <old_password> <new_password>" << endl;
        cout << "       Modifica la clave de acceso al sistema" << endl << endl;
        cout << "+ quit" << endl;
        cout << "       Cierra el sistema. Es importante cerrar el sistema apropiadamente para garantizar las prestaciones del mismo" << endl << endl;
        cout << "***Salida a los comandos de ayuda del sistema***" << endl;
        cout << "************************************************" << endl << endl;
    }else
        valida = false;
    return valida;
}

void UI::perdirPassword(){
    bool validado = false;
    int intentos = 0;
    string passwordIngresado;
    do {
        this->imprimir("Por favor ingrese la Clave de Acceso del Sistema:");
        getline (cin,passwordIngresado);
        if (Motor::getInstancia()->compararConPassActual(passwordIngresado)){
            Motor::getInstancia()->levantarMotor();
            validado = true;
        }
        else{
            this->imprimir(UI::PASSWORD_INVALIDO);
            intentos++;
            }


    }while ((!validado)&&(intentos < 3));

    if (intentos>2){
            this->imprimir(UI::PASSWORD_MAX_INTENTOS_SUPERADOS);
            Motor::getInstancia()->quit();
        }
    else this->escucharInstrucciones();

}
