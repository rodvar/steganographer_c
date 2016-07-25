/************************************************************************
                        ParticionDao.cpp

**************************************************************************/

#include "ParticionDao.h"

// Constructors/Destructors
//

ParticionDao::ParticionDao ( ) {

    this->sigLibre = new Campo("SIGLIBRE");
    this->campoId = new Campo("ID");
    this->campoIdOcultado = new Campo("IDOCULTADO");
    this->campoOrden = new Campo("ORDEN");
    this->campoIdImagen = new Campo("IDIMAGEN");
    this->campoOffset = new Campo("OFFSET");
    this->campoTamano = new Campo("TAMANO");
    this->campoComprimida = new Campo("COMPRIMIDA");

    this->campos.push_back(*sigLibre);
    this->campos.push_back(*campoId);
    this->campos.push_back(*campoIdOcultado);
    this->campos.push_back(*campoOrden);
    this->campos.push_back(*campoIdImagen);
    this->campos.push_back(*campoOffset);
    this->campos.push_back(*campoTamano);
    this->campos.push_back(*campoComprimida);

    this->reg = new Registro(campos);

    this->managerFixedReg = new ArchivoRegistros(PARTICIONES, reg);
}

ParticionDao::~ParticionDao ( ) {
    delete this->sigLibre;
    delete this->campoId;
    delete this->campoIdOcultado;
    delete this->campoOrden;
    delete this->campoIdImagen;
    delete this->campoOffset;
    delete this->campoTamano;
    delete this->campoComprimida;
    delete this->reg;
    delete this->managerFixedReg;
}

bool ParticionDao::get (Almacenable* entidad ){
    bool resultado = true;
    Particion* almacenable = (Particion*) entidad;
    /* setear campos del reg fijo */

    reg->setValorCampo("ID", almacenable->getId());

    // buscar el registro en el archivo
    resultado = this->managerFixedReg->getRegistro(reg);
    if(resultado) {
        almacenable->setId(reg->getCampo("ID").getValor());
        almacenable->setIdTexto(reg->getCampo("IDOCULTADO").getValor());
        almacenable->setOrden(reg->getCampo("ORDEN").getValor());
        almacenable->setIdImagen(reg->getCampo("IDIMAGEN").getValor());
        almacenable->setOffsetImagen(reg->getCampo("OFFSET").getValor());
        almacenable->setTamano(reg->getCampo("TAMANO").getValor());
        // si comprida es true, se persiste con valor 1
        almacenable->setComprimida((reg->getCampo("COMPRIMIDA").getValor()==1));
        almacenable->setPersistido(true);
    }
    return resultado;
}

bool ParticionDao::get (Almacenable* entidad, bool &valido ){
    bool resultado = true;
    valido = false;
    Particion* almacenable = (Particion*) entidad;
    /* setear campos del reg fijo */
    reg->setValorCampo("ID", almacenable->getId());
    //campoId->setValor(almacenable->getId());
    // buscar el registro en el archivo
    resultado = this->managerFixedReg->getRegistro(this->reg);
    if(resultado) {
        valido = (reg->getEstado() == 'O');
        almacenable->setId(reg->getCampo("ID").getValor());
        almacenable->setIdTexto(reg->getCampo("IDOCULTADO").getValor());
        almacenable->setOrden(reg->getCampo("ORDEN").getValor());
        almacenable->setIdImagen(reg->getCampo("IDIMAGEN").getValor());
        almacenable->setOffsetImagen(reg->getCampo("OFFSET").getValor());
        almacenable->setTamano(reg->getCampo("TAMANO").getValor());
        // si comprida es true, se persiste con valor 1
        almacenable->setComprimida((reg->getCampo("COMPRIMIDA").getValor()==1));
        almacenable->setPersistido(true);
    }
    return resultado;
}

void ParticionDao::alta (Almacenable* entidad ){
    int id =entidad->getId();
    if(!entidad->esPersistido()) {
        entidad->setPersistido(true);
        Particion* almacenable = (Particion*) entidad;
        /* setear campos del reg fijo */
        reg->setValorCampo("ID", almacenable->getId());
        //campoId->setValor(id);
        reg->setValorCampo("IDOCULTADO", almacenable->getIdTexto());
        //campoIdOcultado->setValor(almacenable->getIdTexto());
        reg->setValorCampo("ORDEN", almacenable->getOrden());
        //campoOrden->setValor(almacenable->getOrden());
        reg->setValorCampo("IDIMAGEN", almacenable->getIdImagen());
        //campoIdImagen->setValor(almacenable->getIdImagen());
        reg->setValorCampo("OFFSET", almacenable->getOffsetImagen());
        //campoOffset->setValor(almacenable->getOffsetImagen());
        reg->setValorCampo("TAMANO", almacenable->getTamano());
        //campoTamano->setValor(almacenable->getTamano());
        reg->setValorCampo("COMPRIMIDA", (almacenable->estaComprimida())?1:0);
        // si comprida es true, se persiste con valor 1
        //campoComprimida->setValor((almacenable->estaComprimida())?1:0);
        // dar de alta el registro en el archivo
        this->managerFixedReg->alta(reg);
    } else {
        Indexador* indexador = Motor::getInstancia()->getAdministradorRecursos()->getIndexador(PARTICIONES);
        indexador->getManagerLibresFijos()->addLibre(id);
        }
}

bool ParticionDao::modificacion (Almacenable* entidad ){
    bool resultado;
    Particion* almacenable = (Particion*) entidad;
    /* setear campos del reg fijo */
/*
    campoId->setValor(almacenable->getId());
    campoIdOcultado->setValor(almacenable->getIdTexto());
    campoOrden->setValor(almacenable->getOrden());
    campoIdImagen->setValor(almacenable->getIdImagen());
    campoOffset->setValor(almacenable->getOffsetImagen());
    campoTamano->setValor(almacenable->getTamano());
    // si comprida es true, se persiste con valor 1
    campoComprimida->setValor((almacenable->estaComprimida())?1:0);
*/
        reg->setValorCampo("ID", almacenable->getId());
        reg->setValorCampo("IDOCULTADO", almacenable->getIdTexto());
        reg->setValorCampo("ORDEN", almacenable->getOrden());
        reg->setValorCampo("IDIMAGEN", almacenable->getIdImagen());
        reg->setValorCampo("OFFSET", almacenable->getOffsetImagen());
        reg->setValorCampo("TAMANO", almacenable->getTamano());
        reg->setValorCampo("COMPRIMIDA", (almacenable->estaComprimida())?1:0);


    resultado = this->managerFixedReg->getRegistro(reg);
    if (resultado)
        this->managerFixedReg->alta(reg);
    return resultado;
}

void ParticionDao::baja (Almacenable* entidad ) {
    // dar de baja solo si la entidad fue persistida
    int id = entidad->getId();
    if(entidad->esPersistido()) {
        /* setear campos del reg fijo */
        reg->setValorCampo("ID", id);

        // dar de baja el registro en el archivo
        this->managerFixedReg->borrar(reg);
    } else {
        Indexador* indexador = Motor::getInstancia()->getAdministradorRecursos()->getIndexador(PARTICIONES);
        indexador->getManagerLibresFijos()->addLibre(id);
        }
}

void ParticionDao::getAlmacenables(list<Almacenable*>* lista) {
    unsigned int i=0;
    bool resultado = true;
    bool valido;
    while(resultado) {
        Particion* almacenable=new Particion( );
        almacenable->setId(i);
        resultado = this->get(almacenable, valido);
        if(resultado)  {
            if(valido)
                (*lista).push_back(almacenable);
            i++;
        }
    }
    Motor::getInstancia()->getAdministradorRecursos()->getIndexador(PARTICIONES)->setMaximoId(i);
}

void ParticionDao::levantarListaLibres(){
    this->managerFixedReg->levantarListaRegLibres();
}

void ParticionDao::actualizarEncadenamiento(){
    this->managerFixedReg->actualizarEncadenamiento();
}

void ParticionDao::inicializarVariables(){
}

void ParticionDao::protegerVariables(){
}

int ParticionDao::obtenerTamanoRegistro(){
    int sizeReg= (Motor::getInstancia()->getAdministradorRecursos()->getEncriptador()->getTamanoVEncriptado(this->reg->getNumCampos())*sizeof(int)) + sizeof(char);
    return sizeReg;
}

Registro* ParticionDao::getRegistro(){
    return this->reg;
}
