/************************************************************************
                        ImagenDao.cpp

**************************************************************************/

#include "ImagenDao.h"

// Constructors/Destructors
//

ImagenDao::ImagenDao ( ) {

    this->sigLibre = new Campo("SIGLIBRE");
    this->campoId = new Campo("ID");
    this->campoNom = new Campo("NOMBRE");
    this->campoTamano = new Campo("TAMANO");
    this->campoOffsetInicial = new Campo("OFFSETINICIAL");
    this->campoBytesLibres = new Campo("BYTESLIBRES");
    this->campoEspacioDisponible = new Campo("ESPACIODISPONIBLE");
    this->campoIdDirectorio = new Campo("ID_DIRECTORIO");

    this->campos.push_back(*sigLibre);
    this->campos.push_back(*campoId);
    this->campos.push_back(*campoNom);
    this->campos.push_back(*campoTamano);
    this->campos.push_back(*campoOffsetInicial);
    this->campos.push_back(*campoBytesLibres);
    this->campos.push_back(*campoEspacioDisponible);
    this->campos.push_back(*campoIdDirectorio);

    this->reg = new Registro(campos);

    this->managerFixedReg = new ArchivoRegistros(IMAGENES, reg);
    this->managerRegVars = new ArchivoRegistrosVariables(NOMBRE_IMAGENES);

}

ImagenDao::~ImagenDao ( ) {
    delete this->sigLibre;
    delete this->campoId;
    delete this->campoNom;
    delete this->campoTamano;
    delete this->campoOffsetInicial;
    delete this->campoBytesLibres;
    delete this->campoEspacioDisponible;
    delete this->campoIdDirectorio;
    delete this->reg;
    delete this->managerFixedReg;
    delete this->managerRegVars;

}

bool ImagenDao::get (Almacenable* entidad ){
    bool resultado = true;
    ArchivoImagen* almacenable = (ArchivoImagen*) entidad;
    /* setear campos del reg fijo */
    Campo* sigLibre = new Campo("SIGLIBRE");
    Campo* campoId = new Campo("ID");
    Campo* campoNom = new Campo("NOMBRE");
    Campo* campoTamano = new Campo("TAMANO");
    Campo* campoOffsetInicial = new Campo("OFFSETINICIAL");
    Campo* campoBytesLibres = new Campo("BYTESLIBRES");
    Campo* campoEspacioDisponible = new Campo("ESPACIODISPONIBLE");
    Campo* campoIdDirectorio = new Campo("ID_DIRECTORIO");

    /* el ID es la posicion */
    campoId->setValor(almacenable->getId());

    vector<Campo> campos;
    campos.push_back(*sigLibre);
    campos.push_back(*campoId);
    campos.push_back(*campoNom);
    campos.push_back(*campoTamano);
    campos.push_back(*campoOffsetInicial);
    campos.push_back(*campoBytesLibres);
    campos.push_back(*campoEspacioDisponible);
    campos.push_back(*campoIdDirectorio);

    Registro* reg = new Registro(campos);

    // buscar el registro en el archivo
    resultado = this->managerFixedReg->getRegistro(reg);
    if(resultado) {
        // obtener el nombre con el offset
        string nombre = managerRegVars->getRegistro(reg->getCampo("NOMBRE").getValor());
        almacenable->setTamano(reg->getCampo("TAMANO").getValor());
        almacenable->setId(reg->getCampo("ID").getValor());
        almacenable->setIdDirectorio(reg->getCampo("ID_DIRECTORIO").getValor());
        almacenable->setNombre(nombre);
        almacenable->setPersistido(true);
    }
    delete reg;
    delete sigLibre;
    delete campoId;
    delete campoNom;
    delete campoTamano;
    delete campoOffsetInicial;
    delete campoBytesLibres;
    delete campoEspacioDisponible;
    delete campoIdDirectorio;

    return resultado;
}

bool ImagenDao::get (Almacenable* entidad, bool &valido ){
    bool resultado = true;
    valido = false;
    ArchivoImagen* almacenable = (ArchivoImagen*) entidad;
    /* setear campos del reg fijo */
    Campo* sigLibre = new Campo("SIGLIBRE");
    Campo* campoId = new Campo("ID");
    Campo* campoNom = new Campo("NOMBRE");
    Campo* campoTamano = new Campo("TAMANO");
    Campo* campoOffsetInicial = new Campo("OFFSETINICIAL");
    Campo* campoBytesLibres = new Campo("BYTESLIBRES");
    Campo* campoEspacioDisponible = new Campo("ESPACIODISPONIBLE");
    Campo* campoIdDirectorio = new Campo("ID_DIRECTORIO");

    campoId->setValor(almacenable->getId());

    vector<Campo> campos;
    campos.push_back(*sigLibre);
    campos.push_back(*campoId);
    campos.push_back(*campoNom);
    campos.push_back(*campoTamano);
    campos.push_back(*campoOffsetInicial);
    campos.push_back(*campoBytesLibres);
    campos.push_back(*campoEspacioDisponible);
    campos.push_back(*campoIdDirectorio);

    Registro* reg = new Registro(campos);

    resultado = this->managerFixedReg->getRegistro(reg);
    if(resultado) {
        almacenable->setId(reg->getCampo("ID").getValor());
        valido = (reg->getEstado() == 'O');
        if(valido) {
            // obtener el nombre con el offset
            string nombre = managerRegVars->getRegistro(reg->getCampo("NOMBRE").getValor());
            almacenable->setNombre(nombre);
            almacenable->setTamano(reg->getCampo("TAMANO").getValor());
            almacenable->setOffsetInicial(reg->getCampo("OFFSETINICIAL").getValor());
            almacenable->setBytesLibres(reg->getCampo("BYTESLIBRES").getValor());
            almacenable->setEspacioDisponible(reg->getCampo("ESPACIODISPONIBLE").getValor());
            almacenable->setIdDirectorio(reg->getCampo("ID_DIRECTORIO").getValor());
            almacenable->setPersistido(true);
        }
    }
    delete reg;
    delete sigLibre;
    delete campoId;
    delete campoNom;
    delete campoTamano;
    delete campoOffsetInicial;
    delete campoBytesLibres;
    delete campoEspacioDisponible;
    delete campoIdDirectorio;

    return resultado;
}

void ImagenDao::alta (Almacenable* entidad ){

    int offset=0; // offset del nombre en archivo de nombres
    ArchivoImagen* almacenable = (ArchivoImagen*) entidad;
    /* setear campos del reg fijo */
    Campo* sigLibre = new Campo("SIGLIBRE");
    Campo* campoId = new Campo("ID");
    Campo* campoNom = new Campo("NOMBRE");
    Campo* campoTamano = new Campo("TAMANO");
    Campo* campoOffsetInicial = new Campo("OFFSETINICIAL");
    Campo* campoBytesLibres = new Campo("BYTESLIBRES");
    Campo* campoEspacioDisponible = new Campo("ESPACIODISPONIBLE");
    Campo* campoIdDirectorio = new Campo("ID_DIRECTORIO");

    // dar de alta el nombre en el stream de nombres
    offset = this->managerRegVars->alta(almacenable->getNombre());
    campoId->setValor(almacenable->getId());
    campoNom->setValor(offset);
    campoTamano->setValor(almacenable->getTamano());
    campoOffsetInicial->setValor(almacenable->getOffsetInicial());
    campoBytesLibres->setValor(almacenable->getBytesLibres());
    campoEspacioDisponible->setValor(almacenable->getEspacioDisponible());
    campoIdDirectorio->setValor(almacenable->getIdDirectorio());

    vector<Campo> campos;
    campos.push_back(*sigLibre);
    campos.push_back(*campoId);
    campos.push_back(*campoNom);
    campos.push_back(*campoTamano);
    campos.push_back(*campoOffsetInicial);
    campos.push_back(*campoBytesLibres);
    campos.push_back(*campoEspacioDisponible);
    campos.push_back(*campoIdDirectorio);

    Registro* reg = new Registro(campos);
    // dar de alta el registro en el archivo
    this->managerFixedReg->alta(reg);
    entidad->setPersistido(true);

    delete reg;
    delete sigLibre;
    delete campoId;
    delete campoNom;
    delete campoTamano;
    delete campoOffsetInicial;
    delete campoBytesLibres;
    delete campoEspacioDisponible;
    delete campoIdDirectorio;
}

bool ImagenDao::modificacion (Almacenable* entidad ){
    ArchivoImagen* almacenable = (ArchivoImagen*) entidad;
    bool resultado;
    vector<Campo> campos;
    /* setear campos del reg fijo */
    Campo* sigLibre = new Campo("SIGLIBRE");
    Campo* campoId = new Campo("ID");
    Campo* campoNom = new Campo("NOMBRE");
    Campo* campoTamano = new Campo("TAMANO");
    Campo* campoOffsetInicial = new Campo("OFFSETINICIAL");
    Campo* campoBytesLibres = new Campo("BYTESLIBRES");
    Campo* campoEspacioDisponible = new Campo("ESPACIODISPONIBLE");
    Campo* campoIdDirectorio = new Campo("ID_DIRECTORIO");

    campoId->setValor(almacenable->getId());// seteamos el id

    campos.push_back(*sigLibre);
    campos.push_back(*campoId);
    campos.push_back(*campoNom);
    campos.push_back(*campoTamano);
    campos.push_back(*campoOffsetInicial);
    campos.push_back(*campoBytesLibres);
    campos.push_back(*campoEspacioDisponible);
    campos.push_back(*campoIdDirectorio);

    Registro* reg = new Registro(campos);
    resultado = this->managerFixedReg->getRegistro(reg); // y buscamos el reg
    if (resultado){ // modificamos los bytes libres y sobreescribimos
        reg->setValorCampo("BYTESLIBRES",almacenable->getBytesLibres());
        this->managerFixedReg->alta(reg);
    }

    delete reg;
    delete sigLibre;
    delete campoId;
    delete campoNom;
    delete campoTamano;
    delete campoOffsetInicial;
    delete campoBytesLibres;
    delete campoEspacioDisponible;
    delete campoIdDirectorio;
    return resultado;
}

void ImagenDao::baja (Almacenable* entidad ) {
    // dar de baja solo si la entidad fue persistida
    int id = entidad->getId();
    if(entidad->esPersistido()) {
        //ArchivoImagen* almacenable = (ArchivoImagen*) entidad;
        /* setear campos del reg fijo */
        Campo* sigLibre = new Campo("SIGLIBRE");
        Campo* campoId = new Campo("ID");
        Campo* campoNom = new Campo("NOMBRE");
        Campo* campoTamano = new Campo("TAMANO");
        Campo* campoOffsetInicial = new Campo("OFFSETINICIAL");
        Campo* campoBytesLibres = new Campo("BYTESLIBRES");
        Campo* campoEspacioDisponible = new Campo("ESPACIODISPONIBLE");
        Campo* campoIdDirectorio = new Campo("ID_DIRECTORIO");
        campoId->setValor(id);
        vector<Campo> campos;
        campos.push_back(*sigLibre);
        campos.push_back(*campoId);
        campos.push_back(*campoNom);
        campos.push_back(*campoTamano);
        campos.push_back(*campoOffsetInicial);
        campos.push_back(*campoBytesLibres);
        campos.push_back(*campoEspacioDisponible);
        campos.push_back(*campoIdDirectorio);
        Registro* reg = new Registro(campos);
        // dar de baja el registro en el archivo
        if(this->managerFixedReg->borrar(reg)) {
            // dar de baja el registro en el archivo de registros variables
            this->managerRegVars->baja(reg->getCampo("NOMBRE").getValor());
        }
        delete reg;
        delete sigLibre;
        delete campoId;
        delete campoNom;
        delete campoTamano;
        delete campoOffsetInicial;
        delete campoBytesLibres;
        delete campoEspacioDisponible;
        delete campoIdDirectorio;
    } else {
        Indexador* indexador = Motor::getInstancia()->getAdministradorRecursos()->getIndexador(IMAGENES);
        indexador->getManagerLibresFijos()->addLibre(id);
        }
}

void ImagenDao::getAlmacenables(list<Almacenable*>* lista) {
    unsigned int i=0;
    bool resultado = true;
    bool valido;
    string tipoArchivo;
    ArchivoImagenBMP* almacenable=new ArchivoImagenBMP(0, 0, "", 0);
    while(resultado) {
        almacenable->setId(i);
        resultado = this->get(almacenable, valido);
        if(resultado){
            if(valido){
                ArchivoImagen* imagen = NULL;
                tipoArchivo = this->getTipoArchivo(almacenable->getNombre());
                if (tipoArchivo.compare(".jpg") == 0){
                    imagen =new ArchivoImagenJPG(almacenable->getId(), almacenable->getIdDirectorio(), almacenable->getNombre(), almacenable->getTamano());            }
                else if (tipoArchivo.compare(".gif") == 0){
                    imagen =new ArchivoImagenGIF(almacenable->getId(), almacenable->getIdDirectorio(), almacenable->getNombre(), almacenable->getTamano());
                }
                else if (tipoArchivo.compare(".bmp") == 0){
                    imagen =new ArchivoImagenBMP(almacenable->getId(), almacenable->getIdDirectorio(), almacenable->getNombre(), almacenable->getTamano());
                }
                else if (tipoArchivo.compare(".png") == 0){
                    imagen =new ArchivoImagenPNG(almacenable->getId(), almacenable->getIdDirectorio(), almacenable->getNombre(), almacenable->getTamano());
                }
                if(imagen) {
                    imagen->setPersistido(true);
                    imagen->setBytesLibres(almacenable->getBytesLibres());
                    imagen->setEspacioDisponible(almacenable->getEspacioDisponible());
                    imagen->setOffsetInicial(almacenable->getOffsetInicial());
                    (*lista).push_back(imagen);
                }
            }
            i++;
        }
    }
    Motor::getInstancia()->getAdministradorRecursos()->getIndexador(IMAGENES)->setMaximoId(i);
    delete almacenable;
}

string ImagenDao::getTipoArchivo(string nombre){
    string tipo = "";
    size_t pos;
    pos = nombre.find_last_of(".");
    if (pos!=string::npos){
        tipo = nombre.substr (pos,4);
        transform( tipo.begin(), tipo.end(), tipo.begin(), ::tolower );
    }
    return tipo;
}

void ImagenDao::levantarListaLibres(){
    this->managerFixedReg->levantarListaRegLibres();
    this->managerRegVars->levantarListaRegLibres();
}

void ImagenDao::actualizarEncadenamiento(){
    this->managerFixedReg->actualizarEncadenamiento();
    this->managerRegVars->actualizarListaEnDisco();
}

void ImagenDao::inicializarVariables(){
    this->managerRegVars->cargarArchivoEncriptado();
}

void ImagenDao::protegerVariables(){
    this->managerRegVars->guardarArchivoEncriptado();
}

int ImagenDao::obtenerTamanoRegistro(){
    int sizeReg= (Motor::getInstancia()->getAdministradorRecursos()->getEncriptador()->getTamanoVEncriptado(this->reg->getNumCampos())*sizeof(int)) + sizeof(char);
    return sizeReg;
}

Registro* ImagenDao::getRegistro(){
    return this->reg;
}
