/************************************************************************
                        DirectorioDao.cpp

**************************************************************************/

#include "DirectorioDao.h"

DirectorioDao::DirectorioDao ( ){

    this->sigLibre = new Campo("SIGLIBRE");
    this->campoId = new Campo("ID");
    this->campoRuta = new Campo("RUTA");
    this->campos.push_back(*sigLibre);
    this->campos.push_back(*campoId);
    this->campos.push_back(*campoRuta);
    this->reg = new Registro(campos);
    this->managerFixedReg = new ArchivoRegistros(DIRECTORIOS, reg);
    this->managerRegVars = new ArchivoRegistrosVariables(NOMBRE_DIRECTORIOS);
}

DirectorioDao::~DirectorioDao ( ) {
    delete this->reg;
    delete this->sigLibre;
    delete this->campoId;
    delete this->campoRuta;
    delete this->managerFixedReg;
    delete this->managerRegVars;
}

bool DirectorioDao::get (Almacenable* entidad ){
    bool resultado = true;
    Directorio* almacenable = (Directorio*) entidad;
    /* setear campos del reg fijo */
    Campo* sigLibre = new Campo("SIGLIBRE");
    Campo* campoId = new Campo("ID");
    Campo* campoRuta = new Campo("RUTA");

    /* el ID es la posicion */
    campoId->setValor(almacenable->getId());

    vector<Campo> campos;
    campos.push_back(*sigLibre);
    campos.push_back(*campoId);
    campos.push_back(*campoRuta);

    Registro* reg = new Registro(campos);
    resultado = this->managerFixedReg->getRegistro(reg);
    if(resultado) {
        // obtener la ruta con el offset
        string ruta = managerRegVars->getRegistro(reg->getCampo("RUTA").getValor());
        almacenable->setId(reg->getCampo("ID").getValor());
        almacenable->setRuta(ruta);
        almacenable->setPersistido(true);
    }
    delete reg;
    delete sigLibre;
    delete campoId;
    delete campoRuta;
    return resultado;
}

  bool DirectorioDao::get (Almacenable* entidad, bool &valido ){
    bool resultado = true;
    valido = false;
    Directorio* almacenable = (Directorio*) entidad;
    /* setear campos del reg fijo */
    Campo* sigLibre = new Campo("SIGLIBRE");
    Campo* campoId = new Campo("ID");
    Campo* campoRuta = new Campo("RUTA");
    /* el ID es la posicion */
    campoId->setValor(almacenable->getId());

    vector<Campo> campos;
    campos.push_back(*sigLibre);
    campos.push_back(*campoId);
    campos.push_back(*campoRuta);

    Registro* reg = new Registro(campos);

    // buscar el registro en Ocultados.dat
    //RESULTADO SERA FALSE CUANDO SE LLEGA AL FIN DEL ARCHIVO
    resultado = this->managerFixedReg->getRegistro(reg);
    if(resultado) {
        valido = (reg->getEstado() == 'O');
        almacenable->setId(reg->getCampo("ID").getValor());
        if(valido){
             // obtener el nombre con el offset
            string ruta = managerRegVars->getRegistro(reg->getCampo("RUTA").getValor());
            almacenable->setRuta(ruta);
            almacenable->setPersistido(true);
        }
    }
    delete reg;
    delete sigLibre;
    delete campoId;
    delete campoRuta;
    return resultado;
}

void DirectorioDao::alta (Almacenable* entidad ){
    int offset=0; // offset del nombre en archivo de nombres
    Directorio* almacenable = (Directorio*) entidad;
    /* setear campos del reg fijo */
    Campo* sigLibre = new Campo("SIGLIBRE");
    Campo* campoId = new Campo("ID");
    Campo* campoRuta = new Campo("RUTA");
    // dar de alta el nombre en el stream de nombres
    offset = this->managerRegVars->alta(almacenable->getRuta());
    entidad->setPersistido(true);

    campoId->setValor(almacenable->getId());
    campoRuta->setValor(offset);
    vector<Campo> campos;
    campos.push_back(*sigLibre);
    campos.push_back(*campoId);
    campos.push_back(*campoRuta);
    Registro* reg = new Registro(campos);
    // dar de alta el registro en el archivo
    this->managerFixedReg->alta(reg);
    delete reg;
    delete campoId;
    delete campoRuta;
}

bool DirectorioDao::modificacion (Almacenable* entidad ){
    bool resultado;
    Directorio* almacenable = (Directorio*) entidad;
    /* setear campos del reg fijo */
    Campo* sigLibre = new Campo("SIGLIBRE");
    Campo* campoId = new Campo("ID");
    Campo* campoRuta = new Campo("RUTA");

    campoId->setValor(almacenable->getId());
    vector<Campo> campos;
    campos.push_back(*sigLibre);
    campos.push_back(*campoId);
    campos.push_back(*campoRuta);
    Registro* reg = new Registro(campos);
    resultado = this->managerFixedReg->getRegistro(reg);
    if (resultado)
        this->managerFixedReg->alta(reg);
    delete reg;
    delete campoId;
    delete campoRuta;
    return resultado;
}

void DirectorioDao::baja (Almacenable* entidad ){
    int id = entidad->getId();
    // dar de baja solo si la entidad fue persistida
    if(entidad->esPersistido()) {
        //Directorio* almacenable = (Directorio*) entidad;
        /* setear campos del reg fijo */
        Campo* sigLibre = new Campo("SIGLIBRE");
        Campo* campoId = new Campo("ID");
        Campo* campoRuta = new Campo("RUTA");
        campoId->setValor(id);
        vector<Campo> campos;
        campos.push_back(*sigLibre);
        campos.push_back(*campoId);
        campos.push_back(*campoRuta);
        Registro* reg = new Registro(campos);
        // dar de baja el registro en el archivo
        if(this->managerFixedReg->borrar(reg)) {
            // dar de baja el registro en el archivo de registros variables
            this->managerRegVars->baja(reg->getCampo("RUTA").getValor());
        }
        campoId->setValor(id);
        delete reg;
        delete sigLibre;
        delete campoId;
        delete campoRuta;
    } else {
        Indexador* indexador = Motor::getInstancia()->getAdministradorRecursos()->getIndexador(DIRECTORIOS);
        indexador->getManagerLibresFijos()->addLibre(id);
        }
}

void DirectorioDao::getAlmacenables (list<Almacenable*>* lista){
    unsigned int i=0;
    bool resultado = true;
    bool valido;
    string tipoArchivo;

    while(resultado) {
        Directorio* almacenable = new Directorio();
        almacenable->setId(i);
        resultado = this->get(almacenable, valido);
        if(resultado){
            if(valido){
                (*lista).push_back(almacenable);
            }
            i++;
        }

    }
    Indexador* indexador = Motor::getInstancia()->getAdministradorRecursos()->getIndexador(DIRECTORIOS);
    indexador->setMaximoId(i);
}

void DirectorioDao::levantarListaLibres(){
    this->managerFixedReg->levantarListaRegLibres();
    this->managerRegVars->levantarListaRegLibres();
}

void DirectorioDao::actualizarEncadenamiento(){
    this->managerFixedReg->actualizarEncadenamiento();
    this->managerRegVars->actualizarListaEnDisco();
}

void DirectorioDao::inicializarVariables(){
    this->managerRegVars->cargarArchivoEncriptado();
}

void DirectorioDao::protegerVariables(){
    this->managerRegVars->guardarArchivoEncriptado();
}

int DirectorioDao::obtenerTamanoRegistro(){
    int sizeReg= (Motor::getInstancia()->getAdministradorRecursos()->getEncriptador()->getTamanoVEncriptado(this->reg->getNumCampos())*sizeof(int)) + sizeof(char);
    return sizeReg;
}

Registro* DirectorioDao::getRegistro(){
    return this->reg;
}
