/************************************************************************
                        ArchivoRegistrosVariables.cpp

**************************************************************************/

#include "ArchivoRegistrosVariables.h"
#include "Motor.h"

using namespace std;

ArchivoRegistrosVariables::ArchivoRegistrosVariables (string n){
    this->nombre = n;
}

ArchivoRegistrosVariables::~ArchivoRegistrosVariables(){
}

int ArchivoRegistrosVariables::alta(RegVariable* registro ) {
    int pos;
    unsigned short int sizeSize = 4*sizeof(char);
    unsigned short int sizeLibre = 4*sizeof(char);
    unsigned short int sizeValor = 0;
    char* bufferIn = (char*)calloc(4,sizeof(char));
    string buffer;
    string resultado;
    unsigned short int offsetRegistro;
    fstream file;
    file.open(this->nombre.c_str(),ios::in | ios::out | ios::binary);
    if(!file.good())
        file.open(this->nombre.c_str(),ios::in | ios::out | ios::binary | ios::trunc);


    buffer = registro->getCamposConcat();
    Indexador* indexador = Motor::getInstancia()->getAdministradorRecursos()->getIndexador(this->nombre);

    //VERIFICO QUE LA LISTA DE REGISTROS LIBRES ESTE VACIA, EN ESE CASO SE DA DE ALTA EL REGISTRO EN MODO APPEND
     if(indexador->getManagerLibresVariables()->getLista().empty() == true){
        file.seekg(0,ios::end);
        pos = file.tellg();
        file.seekp(pos);

        offsetRegistro = file.tellp();
        file.write(buffer.c_str(),buffer.size());
    }
    else{
            pos = indexador->getManagerLibresVariables()->obtenerVariablesPrimerLibre(registro->getSize());
            if(pos != -1){
            //POSICIONO EL PUNTERO EN EL LUGAR QUE INDICE EL METODO PEDIR_POSICION_A_LISTA
            //file.seekp(indexador->getManagerLibresVariables()->obtenerVariablesPrimerLibre(registro->getSize()));
            //POSICIONO EL PUNTERO Y LEO EL TAMAÑO DEL REGISTRO VARIABLE (OCUPADOS + LIBRES)
            file.seekp(pos+sizeof(char));
            file.read(bufferIn,sizeSize);
            resultado.assign(bufferIn, sizeSize);
            sizeValor = OperacionesBinarias::hexStringAShort(resultado);
            file.read(bufferIn,sizeLibre);
            //LEO EL TAMAÑO DEL ESPACIO LIBRE DENTRO DEL REGISTRO
            resultado.assign(bufferIn, sizeLibre);
            sizeValor = sizeValor + OperacionesBinarias::hexStringAShort(resultado);
            registro->setLibre(sizeValor - registro->getSize());
            encadenarAnteriorSiguiente(pos);
            buffer = registro->getCamposConcat();
            file.seekp(pos);
            file.write(buffer.c_str(),buffer.size());
            offsetRegistro = pos;
            }
            else{
                file.seekp(0,ios::end);
                offsetRegistro = file.tellp();
                file.write(buffer.c_str(),buffer.size());
            }
    }//END IF GETLISTA().EMPTY()
    file.close();
    free(bufferIn);
    return offsetRegistro;
}


int ArchivoRegistrosVariables::alta(string dato){
    return this->alta(new RegVariable(dato));
}

string ArchivoRegistrosVariables::getNombre(){
   return this->nombre;
}

void ArchivoRegistrosVariables::levantarListaRegLibres(){
    fstream file;

    file.open(this->nombre.c_str(), ios::in | ios::out | ios::binary);
    if(!file.good())
        file.open(this->nombre.c_str(), ios::in | ios::out | ios::trunc | ios::binary);
    char* bufferEstado;
    char* buffer;//SE UTILIZA OTRO BUFFER PARA LEER EL CAMPO SIZE Y SIGUIENTE
    Indexador* indexador = Motor::getInstancia()->getAdministradorRecursos()->getIndexador(this->nombre);
    unsigned short int posicionEstado = 0;
    unsigned short int inicio = 0;
    unsigned short int fin = 0;
    unsigned short int sizeSize = 4*sizeof(char);
    unsigned short int sizeLibre = 4*sizeof(char);
    unsigned short int sizeSiguiente = 4*sizeof(char);
    unsigned short int sizeEstado = sizeof(char);
    unsigned short int sizeValor = 0;
    string resultado = "O";

    bufferEstado = (char*)calloc(1, sizeof(char));
    buffer = (char*)calloc(4, sizeof(char));
    file.seekg(0,ios::end);
    fin = file.tellg();
    file.seekg(0,ios::beg);
    inicio = file.tellg();
    if(fin > 9){
        posicionEstado = inicio;
        //ME MUEVO EN EL ARCHIVO MIENTAS QUE NO ENCUENTRE UN REGISTRO LIBRE Y NO SEA EL FIN DEL MISMO
        //tellg() RETORNA -1 SI ES UNA POSICION INVALIDA
        while(resultado != "L" && (!file.eof())){
            //LEO ESTADO
            file.read(bufferEstado,sizeEstado);
            //CASTEO DE CHAR* A STRING EL ESTADO
            resultado.assign(bufferEstado, sizeEstado);
            if(resultado != "L"){
                //LEO EL TAMAÑO DEL DATO VARIABLE
                file.read(buffer,sizeSize);
                resultado.assign(buffer, sizeSize);
                sizeValor = OperacionesBinarias::hexStringAShort(resultado);
                //LEO EL TAMAÑO DEL ESPACIO LIBRE DENTRO DEL REGISTRO
                file.read(buffer,sizeLibre);
                resultado.assign(buffer, sizeLibre);
                //LUEGO DE LEER SIZE Y LIBRE SE CUANTO OCUPA EL CAMPO VARIABLE
                sizeValor = sizeValor + OperacionesBinarias::hexStringAShort(resultado);
                //ME MUEVO TANTAS POSICIONES COMO ESPACIO OCUPE EL REGISTRO ACTUAL CON RESPECTO AL INICIO DEL ARCHIVO HASTA EL SIGUIENTE REGISTRO
                file.seekg(sizeSiguiente + sizeValor, ios::cur);
                posicionEstado = file.tellg();
            }//END IF

        }//END WHILE
        file.clear();
        //SI OCURRE ESTO ES QUE HAY UN REGISTRO LIBRE!, ME MUEVO POR LOS REGISTROS LIBRES GRACIAS A QUE ESTAN ENCADENADOS
        if (resultado == "L"){
            file.read(buffer,sizeSize);
            //LEO EL TAMAÑO DEL DATO VARIABLE
            resultado.assign(buffer,sizeSize );
            sizeValor = OperacionesBinarias::hexStringAShort(resultado);
            file.read(buffer,sizeLibre);
            //LEO EL TAMAÑO DEL ESPACIO LIBRE DENTRO DEL REGISTRO
            resultado.assign(buffer,sizeLibre );
            sizeValor = sizeValor + OperacionesBinarias::hexStringAShort(resultado);
            //GRABO EN LA LISTA DE LIBRES LA POSICION Y EL TAMAÑO DEL REGISTRO LIBRE
            indexador->getManagerLibresVariables()->insertarOrdenadoEnListaVariables(posicionEstado,sizeValor);
            //LEO EL OFFSET AL SIGUIENTE LIBRE
            file.read(buffer,sizeSiguiente);
            //LEO EL TAMAÑO DEL DATO VARIABLE
            resultado.assign(buffer,sizeSiguiente );
            while(resultado != "0000"){
                posicionEstado = OperacionesBinarias::hexStringAShort(resultado);
                file.seekg(posicionEstado);
                //LEO EL ESTADO
                file.read(bufferEstado,sizeEstado);
                //LEO EL TAMAÑO DEL DATO VARIABLE (SUMA DE OCUPADOS + LIBRES)
                file.read(buffer,sizeSize);
                resultado.assign(buffer, sizeSize);
                sizeValor = OperacionesBinarias::hexStringAShort(resultado);
                file.read(buffer,sizeLibre);
                resultado.assign(buffer,sizeLibre );
                sizeValor = sizeValor + OperacionesBinarias::hexStringAShort(resultado);
                //GRABO EN LA LISTA DE LIBRES LA POSICION Y EL TAMAÑO DEL REGISTRO LIBRE
                indexador->getManagerLibresVariables()->insertarOrdenadoEnListaVariables(posicionEstado,sizeValor);
                 //LEO EL OFFSET AL SIGUIENTE LIBRE
                file.read(buffer,sizeSiguiente);
                resultado.assign(buffer,sizeSiguiente );
            }//END WHILE
        }//EN INF POSICIONESTADO != -1
    }//END IF (FIN > 10)
    free(bufferEstado);
    free(buffer);
    file.close();
}//END LEVANTARLISTAREGLIBRES

int ArchivoRegistrosVariables::baja(unsigned short int offsetRegistro){
    unsigned short int sizeValor = 0;
    fstream file;
    file.open(this->nombre.c_str(),ios::in | ios::out | ios::binary);
    Indexador* indexador = Motor::getInstancia()->getAdministradorRecursos()->getIndexador(this->nombre);

    //DEFINO UN ESTADO OCUPADO PARA DAR LAS BAJAS CORRESPONDIENTES
    int retorno;
    string sBuffer;
    char* buffer;//SE UTILIZA OTRO BUFFER PARA LEER EL CAMPO SIZE Y SIGUIENTE

    file.seekg(offsetRegistro);
    //int pos = tellg();
    buffer = (char*)calloc(1, sizeof(char));
    file.read(buffer, sizeof(char));

    //file.read((char*)buffer.c_str(),sizeEstado);
    sBuffer.assign(buffer, sizeof(char));

    //SE VERIFICA QUE EL ESTADO DEL REGISTRO SEA OCUPADO
    if(sBuffer == "O"){
        //GRABO EN EL REGISTRO LIBRE LA INFORMACION NECESARIA PARA GUARDARLO EN LA LISTA DE REGISTROS LIBRES
        //file.seekg(offsetRegistro);
        free(buffer);
        buffer = (char*)calloc(4, sizeof(char));
        file.read(buffer,4*sizeof(char));
        sBuffer.assign(buffer, 4*sizeof(char));
        sizeValor = OperacionesBinarias::hexStringAShort(sBuffer);
        file.read(buffer,4*sizeof(char));
        sBuffer.assign(buffer, 4*sizeof(char));
        sizeValor = sizeValor + OperacionesBinarias::hexStringAShort(sBuffer);
        indexador->getManagerLibresVariables()->insertarOrdenadoEnListaVariables(offsetRegistro, sizeValor);
        //DOY DE BAJA EL REGISTRO CAMBIANDOLE SU ESTADO
        file.seekp(offsetRegistro,ios::beg);
        file.write("L",sizeof(char));
        //ACTUALIZO LOS ENCADENAMIENTOS DE LOS REGISTROS LIBRES EN EL DISCO
        this->actualizarListaEnDisco(offsetRegistro);
        file.close();

        free(buffer);
        retorno = 0;
    }
    else{
        retorno = -1;
        file.close();
    }

    return retorno;
}
bool ArchivoRegistrosVariables::actualizarListaEnDisco(){
    Indexador* indexador = Motor::getInstancia()->getAdministradorRecursos()->getIndexador(this->nombre);
    list<ManagerFreeRegVars::RegLibre*> lista = indexador->getManagerLibresVariables()->getLista();
    list<ManagerFreeRegVars::RegLibre*>::iterator it = lista.begin();
    bool resultado = true;

    while(it != lista.end() && resultado){
        resultado = this->actualizarListaEnDisco(((ManagerFreeRegVars::RegLibre*)*it)->offset);
        delete *it;
        it = lista.erase(it);
    }
return resultado;
    }



bool ArchivoRegistrosVariables::actualizarListaEnDisco(unsigned short int offsetRegistro){
    fstream file;
    file.open(this->nombre.c_str(), ios::in | ios::out | ios::binary);
    bool estado = false; // TODO: HACER QUE DEVUELVA TRUE CUANDO ESTE TODO OK
    vector<short int> hermanos;
    unsigned short int sizeEstado = sizeof(char);
    unsigned short int sizeSize = 4*sizeof(char);
    unsigned short int sizeLibre = 4*sizeof(char);
    string offset;
    Indexador* indexador = Motor::getInstancia()->getAdministradorRecursos()->getIndexador(this->nombre);
    hermanos = indexador->getManagerLibresVariables()->obtenerVariablesHermanos(offsetRegistro);

    if(hermanos[0] != -1){
        //EL PUNTERO SE SITUA EN EL HERMANO ANTERIOR
        file.seekp(sizeEstado + sizeSize + sizeLibre + (unsigned short int)hermanos[0]);
        offset = OperacionesBinarias::shortAHexString(offsetRegistro);
        file.write(offset.c_str(),offset.length());
    }
    if(hermanos[1] != -1){
        //EL PUNTERO SE SITUA EN EL REGISTRO DADO DE BAJA
        file.seekp(sizeEstado + sizeSize + sizeLibre + offsetRegistro);
        offset = OperacionesBinarias::shortAHexString((unsigned short int)hermanos[1]);
        file.write(offset.c_str(),offset.length());
    }
    file.close();
    return estado;
}

void ArchivoRegistrosVariables::encadenarAnteriorSiguiente(unsigned short int offsetRegistro){
    fstream file;
    file.open(this->nombre.c_str(), ios::in | ios::out | ios::binary);
    vector<short int> hermanos;
    unsigned short int sizeEstado = sizeof(char);
    unsigned short int sizeSize = 4*sizeof(char);
    unsigned short int sizeLibre = 4*sizeof(char);
    string offset;
    Indexador* indexador = Motor::getInstancia()->getAdministradorRecursos()->getIndexador(this->nombre);
    hermanos = indexador->getManagerLibresVariables()->obtenerVariablesHermanos(offsetRegistro);

    if(hermanos[0] != -1 && hermanos[1] != -1){
        //EL PUNTERO SE SITUA EN EL HERMANO ANTERIOR
        file.seekp(sizeEstado + sizeSize + sizeLibre + (unsigned short int)hermanos[0]);
        offset = OperacionesBinarias::shortAHexString((unsigned short int)hermanos[1]);
        file.write(offset.c_str(),offset.length());
    }
    file.close();
}


string ArchivoRegistrosVariables::getRegistro(unsigned short int offset){
    char* buffer = (char*)calloc(4,sizeof(char));
    char* bufferNombre;
    unsigned short int sizeSize = 4*sizeof(char);
    unsigned short int sizeLibre = 4*sizeof(char);
    unsigned short int sizeSiguiente = 4*sizeof(char);
    unsigned short int sizeNombre = 0;
    string resultado;
    string sEstado;
    string sSize;
    string sNombre;
    fstream file;
    file.open((this->nombre).c_str(),ios::in | ios::out | ios::binary);
    file.seekg(offset + sizeof(char));
    file.read(buffer,sizeSize);
    sSize.assign(buffer,sizeSize );
    sizeNombre = OperacionesBinarias::hexStringAShort(sSize);
    file.read(buffer,sizeLibre);
    file.read(buffer,sizeSiguiente);
    bufferNombre = (char*)calloc(sizeNombre,sizeof(char));
    file.read(bufferNombre,sizeNombre);
    sNombre.assign(bufferNombre,sizeNombre);
    delete bufferNombre;
    delete buffer;
    file.close();
    return sNombre;

}

void ArchivoRegistrosVariables::cargarArchivoEncriptado(){
    string encriptado="", desencriptado="";
    int pos=0, fin=0, i=0;
    fstream file;
    file.open(this->nombre.c_str(),ios::in | ios::out | ios::binary);
    //ABRIMOS EL ARCHIVO ENCRIPTADO
    if(!file.good())
        file.open(this->nombre.c_str(),ios::in | ios::out | ios::binary | ios::trunc);
    //DESENCRIPTAMOS EL ARCHIVO SI ESTE NO ESTA VACIO
    file.seekg(0,ios::end);
    fin= file.tellg();
    if (fin != 0){
        char *buffer = (char*)calloc(fin, sizeof(char));
        file.seekg(0,ios::beg);
        //LEO TODO EL ARCHIVO ENCRIPTADO
        file.read(buffer, fin*sizeof(char));
        file.close();
        //c_str() FUNCIONABA MAL POR ESO SE HACE EL RUSTIC-FOR
        for (i=0; i<fin ; i++){
            encriptado += buffer[i];
        }

        //DESENCRIPTO
        desencriptado=Motor::getInstancia()->getAdministradorRecursos()->getEncriptador()->desencriptarString(encriptado);
        //SOBREESCRIBO EL ARCHIVO POR EL DESENCRIPTADO PARA PODER TRABAJARLO COMO ANTES
        file.open(this->nombre.c_str(),ios::in | ios::out | ios::binary | ios::trunc);
        file.seekg(0,ios::beg);
        pos= file.tellg();
        file.seekp(pos);
        fin= desencriptado.length();
        //Cargo en el char* el string porque c_str() funciona mal!
        buffer= (char*)calloc(fin, sizeof(char));
        //c_str() FUNCIONABA MAL POR ESO SE HACE EL RUSTIC-FOR
        for (i=0; i<fin ; i++){
            buffer[i] = desencriptado[i];
        }
        //GRABO EL ARCHIVO DESENCRIPTADO
        file.write(buffer,fin);
        free(buffer);
    }
    file.close();
}

void ArchivoRegistrosVariables::guardarArchivoEncriptado(){
    string encriptado, desencriptado;
    int pos=0, fin=0, i=0;
    fstream file;
    file.open(this->nombre.c_str(),ios::in | ios::out | ios::binary);

    //ABRIMOS EL ARCHIVO
    if(!file.good())
        file.open(this->nombre.c_str(),ios::in | ios::out | ios::binary | ios::trunc);
    //ENCRIPTAMOS EL ARCHIVO SI ESTE NO ESTA VACIO
    file.seekg(0,ios::end);
    fin= file.tellg();
    if (fin != 0){
        char *buffer = (char*)calloc(fin, sizeof(char));
        file.seekg(0,ios::beg);
        //LEO TODO EL ARCHIVO
        file.read(buffer, fin*sizeof(char));
        file.close();
        for (i=0; i<fin ; i++){
            desencriptado += buffer[i];
        }

        //ENCRIPTO
        encriptado=Motor::getInstancia()->getAdministradorRecursos()->getEncriptador()->encriptarString(desencriptado);

        fin= encriptado.length();
        //SOBREESCRIBO EL ARCHIVO POR EL DESENCRIPTADO PARA PODER TRABAJARLO COMO ANTES
        file.open(this->nombre.c_str(),ios::in | ios::out | ios::binary | ios::trunc);
        file.seekg(0,ios::beg);
        pos= file.tellg();
        file.seekp(pos);
        buffer = (char*)calloc(fin, sizeof(char));
        //c_str() FUNCIONABA MAL POR ESO SE HACE EL RUSTIC-FOR
        for (i=0; i<fin ; i++){
            buffer[i] = encriptado[i]  ;
        }
        file.write(buffer,fin);

        free(buffer);
    }
    file.close();
}
