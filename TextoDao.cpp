/************************************************************************
                        TextoDao.cpp

**************************************************************************/

#include "TextoDao.h"

// Constructors/Destructors
//

TextoDao::TextoDao ( ) {

    this->sigLibre = new Campo("SIGLIBRE");
    this->campoId = new Campo("ID");
    this->campoNom = new Campo("NOMBRE");
    this->campoTamano = new Campo("TAMANO");
    this->campoParticiones = new Campo("PARTICIONES");

    this->campos.push_back(*sigLibre);
    this->campos.push_back(*campoId);
    this->campos.push_back(*campoNom);
    this->campos.push_back(*campoTamano);
    this->campos.push_back(*campoParticiones);

    this->reg = new Registro(campos);

    this->managerFixedReg = new ArchivoRegistros(OCULTADOS, reg);
    this->managerRegVars = new ArchivoRegistrosVariables(NOMBRE_OCULTADOS);
}

TextoDao::~TextoDao ( ) {
    delete this->sigLibre;
    delete this->campoId;
    delete this->campoNom;
    delete this->campoTamano;
    delete this->campoParticiones;
    delete this->reg;
    delete this->managerFixedReg;
    delete this->managerRegVars;
}

bool TextoDao::get (Almacenable* entidad ){
    bool resultado = true;
    Texto* texto = (Texto*) entidad;
    /* setear campos del reg fijo */
    reg->setValorCampo("ID", entidad->getId());
    // buscar el registro en el archivo
    resultado = this->managerFixedReg->getRegistro(reg);
    if(resultado) {
        // obtener el nombre con el offset
        string nombre = managerRegVars->getRegistro(reg->getCampo("NOMBRE").getValor());
        texto->setTamano(reg->getCampo("TAMANO").getValor());
        texto->setId(reg->getCampo("ID").getValor());
        texto->setCantidadParticionesOriginal(reg->getCampo("PARTICIONES").getValor());
        texto->setNombre(nombre);
        texto->setPersistido(true);
    }
    return resultado;
}

bool TextoDao::get (Almacenable* entidad, bool &valido ){
    bool resultado = true;
    valido = false;
    Texto* texto = (Texto*) entidad;
    /* setear campos del reg fijo */
    /* el ID es la posicion */
    reg->setValorCampo("ID", entidad->getId());
    // buscar el registro en el archivo
    resultado = this->managerFixedReg->getRegistro(reg);
    if(resultado) {
        texto->setId(reg->getCampo("ID").getValor());
        valido = (reg->getEstado() == 'O');
        if(valido) {
            // obtener el nombre con el offset
            string nombre = managerRegVars->getRegistro(reg->getCampo("NOMBRE").getValor());
            texto->setTamano(reg->getCampo("TAMANO").getValor());
            texto->setCantidadParticionesOriginal(reg->getCampo("PARTICIONES").getValor());
            texto->setNombre(nombre);
            texto->setPersistido(true);
        }
    }
    return resultado;
}

void TextoDao::alta (Almacenable* entidad ){
    int offset=0; // offset del nombre en archivo de nombres
    if(!entidad->esPersistido()) {
        entidad->setPersistido(true);
        Texto* almacenable = (Texto*) entidad;
        /* setear campos del reg fijo */
        reg->setValorCampo("ID", entidad->getId());
        // dar de alta el nombre en el stream de nombres
        offset = this->managerRegVars->alta(almacenable->getNombre());
        reg->setValorCampo("NOMBRE", offset);
        reg->setValorCampo("TAMANO", almacenable->getTamano());
        reg->setValorCampo("PARTICIONES", almacenable->getCantidadParticionesOriginal());
        this->managerFixedReg->alta(reg);
    }
}

bool TextoDao::modificacion (Almacenable* entidad ){
    bool resultado;
    Texto* almacenable = (Texto*) entidad;
    /* setear campos del reg fijo */
    reg->setValorCampo("ID", entidad->getId());
    reg->setValorCampo("TAMANO", almacenable->getTamano());
    reg->setValorCampo("PARTICIONES", almacenable->getCantidadParticionesOriginal());
    resultado = this->managerFixedReg->getRegistro(reg);
    if (resultado)
        this->managerFixedReg->alta(reg);
    return resultado;
}

void TextoDao::baja (Almacenable* entidad ) {
    int id = entidad->getId();
    // dar de baja solo si la entidad fue persistida
    if(entidad->esPersistido()) {
        /* setear campos del reg fijo */
        reg->setValorCampo("ID", id);
        // dar de baja el registro en el archivo
        this->managerFixedReg->borrar(reg);
        // dar de baja el registro en el archivo de registros variables
        this->managerRegVars->baja(reg->getCampo("NOMBRE").getValor());
    } else {
        Indexador* indexador = Motor::getInstancia()->getAdministradorRecursos()->getIndexador(OCULTADOS);
        indexador->getManagerLibresFijos()->addLibre(id);
        }
}

void TextoDao::getAlmacenables(list<Almacenable*>* lista) {
    unsigned int i=0;
    bool resultado = true;
    bool valido;
    while(resultado) {
        Texto* texto=new Texto(0," ",0);
        texto->setId(i);
        resultado = this->get(texto, valido);
        if(resultado) {
            if(valido)
                (*lista).push_back(texto);
            i++;
        }
    }
    Motor::getInstancia()->getAdministradorRecursos()->getIndexador(OCULTADOS)->setMaximoId(i);
}

void TextoDao::levantarListaLibres(){
    this->managerFixedReg->levantarListaRegLibres();
    this->managerRegVars->levantarListaRegLibres();
}

void TextoDao::actualizarEncadenamiento(){
    this->managerFixedReg->actualizarEncadenamiento();
    this->managerRegVars->actualizarListaEnDisco();
}

void TextoDao::inicializarVariables(){
    this->managerRegVars->cargarArchivoEncriptado();
}

void TextoDao::protegerVariables(){
    this->managerRegVars->guardarArchivoEncriptado();
}

int TextoDao::obtenerTamanoRegistro(){
    int sizeReg= (Motor::getInstancia()->getAdministradorRecursos()->getEncriptador()->getTamanoVEncriptado(this->reg->getNumCampos())*sizeof(int)) + sizeof(char);
    return sizeReg;
}

Registro* TextoDao::getRegistro(){
    return this->reg;
}
