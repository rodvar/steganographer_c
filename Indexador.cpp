/************************************************************************
                        Indexador.cpp
**************************************************************************/

#include "Indexador.h"
#include "Motor.h"


// Constructors/Destructors
//

Indexador::Indexador ( string nombre, bool variablesAsociado ) {
    this->variablesAsociado = variablesAsociado;
    this->nombreArchivo = nombre;
    this->maximoId = 0;
    this->managerFijos = new ManagerFreeReg();
    this->managerVariables = new ManagerFreeRegVars();
}

Indexador::~Indexador ( ) {
    delete (this->managerVariables);
    delete (this->managerFijos);
}

ManagerFreeReg* Indexador::getManagerLibresFijos(){
    return this->managerFijos;
}

ManagerFreeRegVars* Indexador::getManagerLibresVariables(){
    return this->managerVariables;
}

string Indexador::getNombre(){
    return this->nombreArchivo;
}

string Indexador::getNombreVariableAsociado(){
    string nombre = "";
    if (this->variablesAsociado)
        nombre = this->variablesAsociado ? ("nombre" + this->nombreArchivo) : this->nombreArchivo;
    return nombre;
}

unsigned short int Indexador::proximoId() {
    // utilizar si existe un id libre.
    short int id = this->getManagerLibresFijos()->getLibre();
    if(id >= 0) {
        //this->siguienteId = id;
        return id;
    } else // sino, el siguiente id.
        return this->maximoId++;
}

void Indexador::setMaximoId(unsigned short int id) {
    this->maximoId = id;
}

void Indexador::setSiguienteId(unsigned short int id) {
    if(id < this->maximoId) // id libre no usado
        this->getManagerLibresFijos()->addLibre(id);
    else
        this->maximoId = id;
}

bool Indexador::reorganizarArchivoVariablesAsociado(){
    ManagerFreeRegVars* managerVariables;
    int pos;
    fstream fileOld;
    fstream fileNew;
    char* bufferEstado = (char*)calloc(1,sizeof(char));
    char* buffer = (char*)calloc(4,sizeof(char));
    char* bufferNombre;
    unsigned short int sizeSize = 4*sizeof(char);
    unsigned short int sizeLibre = 4*sizeof(char);
    unsigned short int sizeSiguiente = 4*sizeof(char);
    unsigned short int sizeEstado = sizeof(char);
    unsigned short int sizeNombre = 0;
    string resultado;
    string sEstado;
    string sSize;
    string sLibre;
    string sSiguiente;
    string sNombre;
    string viejoNombreArchivo;
    string nuevoNombreArchivo = "newnombre.dat";
    Indexador* indexador;
    indexador = Motor::getInstancia()->getAdministradorRecursos()->getIndexador(this->getNombreVariableAsociado());
    viejoNombreArchivo = indexador->getNombreVariableAsociado();
    managerVariables = indexador->getManagerLibresVariables();
    indexador->getNombreVariableAsociado();
    fileOld.open(viejoNombreArchivo.c_str(),ios::in | ios::out | ios::binary);
    fileNew.open(nuevoNombreArchivo.c_str(), ios::in | ios::out | ios::binary);
    if(!fileNew.good())
        fileNew.open(nuevoNombreArchivo.c_str(), ios::in | ios::out | ios::trunc | ios::binary);
    fileOld.seekg(0,ios::beg);
    pos = fileOld.tellg();
    fileOld.read(bufferEstado,sizeEstado);
    sEstado.assign(bufferEstado,sizeEstado);

    while(!fileOld.eof()){
        //SE RECORRE EL ARCHIVO HASTA EL FIN DEL MISMO BUSCANDO TODOS LOS REGISTROS
        //QUE ESTEN OCUPADOS GRABANDOLOS EN UN ARCHIVO NUEVO.
        if(sEstado == "O"){
            //LEO EL TAMAÑO DEL DATO VARIABLE (SUMA DE OCUPADOS + LIBRES)
            fileOld.read(buffer,sizeSize);
            sSize.assign(buffer,sizeSize);
            sizeNombre = OperacionesBinarias::hexStringAShort(sSize);
            fileOld.read(buffer,sizeLibre);
            sLibre.assign(buffer,sizeLibre);
            fileOld.read(buffer,sizeSiguiente);
            sSiguiente.assign(buffer,sizeSiguiente);
            sizeNombre = sizeNombre + OperacionesBinarias::hexStringAShort(sLibre);
            bufferNombre = (char*)calloc(sizeNombre,sizeof(char));
            fileOld.read(bufferNombre,sizeNombre*sizeof(char));
            sNombre.assign(bufferNombre,sizeNombre*sizeof(char));
            resultado.append(sEstado);
            resultado.append(sSize);
            resultado.append(sLibre);
            resultado.append(sSiguiente);
            resultado.append(sNombre);
            fileNew.seekp(0,ios::end);
            fileNew.write(resultado.c_str(),resultado.size());
            resultado = "";
            if(pos != 0)
                fileOld.seekg(pos + sizeSize + sizeLibre + sizeSiguiente + sizeNombre);
            else
                fileOld.seekg(pos + sizeSize + sizeLibre + sizeSiguiente + sizeNombre + 1);
            fileOld.read(bufferEstado,sizeEstado);
            sEstado.assign(bufferEstado,sizeEstado);
            free(bufferNombre);
            if(!fileOld.eof()){
                 pos = fileOld.tellg();
            }
        }else{
            while(sEstado == "L" && !fileOld.eof()){
                fileOld.read(buffer,sizeSize);
                sSize.assign(buffer,sizeSize);
                sizeNombre = OperacionesBinarias::hexStringAShort(sSize);
                fileOld.read(buffer,sizeLibre);
                sLibre.assign(buffer,sizeLibre);
                sizeNombre = sizeNombre + OperacionesBinarias::hexStringAShort(sLibre);
                if(pos != 0)
                    fileOld.seekg(pos + sizeSize + sizeLibre + sizeSiguiente + sizeNombre);
                else
                    fileOld.seekg(pos + sizeSize + sizeLibre + sizeSiguiente + sizeNombre + 1);
                fileOld.read(bufferEstado,sizeEstado);
                sEstado.assign(bufferEstado,sizeEstado);
                //OBTENGO LA POSICION DEL SIGUIENTE REGISTRO
                if(!fileOld.fail() && !fileOld.eof()){
                    pos = fileOld.tellg();
                }
            }//END WHILE
        }//END IF-ELSE
    }//END WHILE
    managerVariables->eliminarListaLibres();
    free(bufferEstado);
    free(buffer);
    fileOld.close();
    fileNew.close();
    remove(viejoNombreArchivo.c_str());
    rename(nuevoNombreArchivo.c_str(),viejoNombreArchivo.c_str());
    return true;
}

void Indexador::indexarLibres(){
    this->crearDaoAsociada();
    this->dao->levantarListaLibres();
    this->liberarDao();
}

void Indexador::inicializar(){
    this->crearDaoAsociada();
    this->dao->inicializarVariables();
    this->liberarDao();
}


void Indexador::liberar(){
    this->crearDaoAsociada();
    this->dao->protegerVariables();
    this->liberarDao();
}

//private

void Indexador::crearDaoAsociada(){
    if (this->nombreArchivo == DIRECTORIOS)
        this->dao = new DirectorioDao();
    else if (this->nombreArchivo == OCULTADOS)
        this->dao = new TextoDao();
    else if (this->nombreArchivo == IMAGENES)
        this->dao = new ImagenDao();
    else if (this->nombreArchivo == PARTICIONES)
        this->dao = new ParticionDao();
}

/**
 * Destruye la dao correspondiente
 */
void Indexador::liberarDao(){
    delete this->dao;
}

/**
 * Reorganizacion de los archivos de la estructura administrativa
 */
bool Indexador::reorganizarArchivoRegistro(double porcentage){
    unsigned int tamanoArchivo = 0;
    fstream fileOld;
    fstream fileNew;
    string viejoNombreArchivo = this->getNombre();
    string nuevoNombreArchivo = viejoNombreArchivo + ".tmp";
    bool reorganizar = false;
    //calcular tamano archivo
    fileOld.open(viejoNombreArchivo.c_str(),ios::in | ios::out | ios::binary);
    if (fileOld.good() && !fileOld.eof() && fileOld.is_open()) {
        fileOld.seekg(0, ios_base::beg);
        ifstream::pos_type begin_pos = fileOld.tellg();
        fileOld.seekg(0, ios_base::end);
        tamanoArchivo = static_cast<int>(fileOld.tellg() - begin_pos);
    } else fileOld.close();

    if(tamanoArchivo > 0) {
        // cantidad de registros persistidos
        this->crearDaoAsociada();
        int tamanoRegistro=this->dao->obtenerTamanoRegistro();
        this->liberarDao();
        int cantRegs = tamanoArchivo / tamanoRegistro;
        int idMaximo = cantRegs -1; // ultimo registro del archivo
        int idLibre;
        int idUltimoLibre = -1;
        list<int> libres = this->getManagerLibresFijos()->getLista();
        bool continuar = (libres.size()>0);
        // buscar el ultimo Id Libre

        while(continuar) {
            idLibre = libres.back();
            libres.pop_back();
            if(idMaximo == idLibre) idUltimoLibre = idMaximo;
            continuar = ((idMaximo == idUltimoLibre) && (libres.size()>0));
            if(continuar) idMaximo--;
        }
        if(idUltimoLibre >=0){
            double porcentageLibre = idUltimoLibre*100/cantRegs;
            porcentageLibre = 100 - porcentageLibre;
            if(porcentageLibre >= porcentage) {
                char* buffer  = new char[tamanoRegistro];
                fileNew.open(nuevoNombreArchivo.c_str(), ios::in | ios::out | ios::trunc | ios::binary);
                if(fileNew.good())
                {
                    reorganizar = true;
                    fileOld.seekg(0, ios::beg);
                    continuar=(!fileOld.eof() && fileOld.good());
                    while(continuar) {
                        fileOld.read(buffer, sizeof(buffer));
                        continuar = fileOld.good();
                        if(continuar) {
                            fileNew.write(buffer, sizeof(buffer));
                            continuar = fileNew.fail();
                        }
                    }
                }
                fileNew.close();
                delete buffer;
            }
        }
        fileOld.close();
        if(reorganizar) {
            remove(viejoNombreArchivo.c_str());
            rename(nuevoNombreArchivo.c_str(),viejoNombreArchivo.c_str());
        }
    }
    return reorganizar;
}

void Indexador::actualizarEncadenamientos(){
    this->crearDaoAsociada();
    dao->actualizarEncadenamiento();
    this->liberarDao();
}
