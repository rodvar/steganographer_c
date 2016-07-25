/************************************************************************
                        ArchivoRegistros.cpp

**************************************************************************/

#include "ArchivoRegistros.h"
#include "Motor.h"

using namespace std;

// Constructors/Destructors

ArchivoRegistros::ArchivoRegistros(string nombre, Registro* reg){
    this->nombre = nombre;
    this->sizeReg= (Motor::getInstancia()->getAdministradorRecursos()->getEncriptador()->getTamanoVEncriptado(reg->getNumCampos())*sizeof(int)) + sizeof(char);
    this->abrir();
    for(unsigned int i=0; i<reg->getNumCampos(); i++)
        reg->setValorCampo(reg->getCampo(i).getNombre(), 0);
    this->registro = reg;
}

ArchivoRegistros::~ArchivoRegistros() {
    if (this->archivo.is_open())
        this->cerrar();
}

bool ArchivoRegistros::alta(Registro* &reg ) {
    bool resultado = this->archivo.is_open(); // indica si tuvo exito el alta
    // setear informacion administrativa
    // estado del registro O: Ocupado
    if (resultado){
        reg->cambiarEstado('O');
        char* estado;
        estado = new char[1];
        estado[0] = reg->getEstado();
        int idRegistro = reg->getCampo("ID").getValor();
        // vector de valores de los campos
        std::vector<int> vcampos = reg->getCampos();
        this->archivo.clear(); // inicializar fail bits de fstream
        // obtener id del registro
        // Marca que indica que no hay un sgte libre
        vcampos[0] = MAXUINT;
        if(idRegistro>=0)
        {
            vcampos[1]= idRegistro;
            this->archivo.seekp(idRegistro*this->sizeReg, ios::beg);
            resultado = (!this->archivo.fail());
            if(resultado) {
                this->archivo.write(reinterpret_cast<char *> (estado), sizeof(estado[0]));
                resultado = (!this->archivo.bad());
                vcampos = Motor::getInstancia()->getAdministradorRecursos()->getEncriptador()->encriptarVector(vcampos);
                if(resultado)
                    for(unsigned int i=0; i<vcampos.size(); i++)
                        this->archivo.write(reinterpret_cast<char *> (&vcampos[i]), sizeof(vcampos[i]));
            }
        }
        resultado = (!this->archivo.bad());
        // si fallo el alta, reutlilizar el id asignado.
        if(!resultado) {
            Indexador* indexador = Motor::getInstancia()->getAdministradorRecursos()->getIndexador(this->nombre);
            indexador->setSiguienteId(idRegistro);
        }
        delete[] estado;
    }
    return resultado;
}

bool ArchivoRegistros::getRegistro(Registro* &reg){
    bool resultado = this->archivo.is_open();
    // estado del registro: L: Libre; O: Ocupado
    char* estado;

    if (resultado){
        estado = new char[1];
        // clave del registro:
        int pos = reg->getCampo("ID").getValor();
        for(unsigned int i=0; i<reg->getNumCampos(); i++)
            reg->setValorCampo(reg->getCampo(i).getNombre(), 0);
        // vector de valores de los campos
        std::vector<int> vcampos = reg->getCampos();
        vcampos.resize(Motor::getInstancia()->getAdministradorRecursos()->getEncriptador()->getTamanoVEncriptado(vcampos.size()), -1);
        this->archivo.clear(); // reset bits flags
        this->archivo.seekg(pos*this->sizeReg, ios::beg);
        this->archivo.read(estado, sizeof(estado[0]));
        resultado = (!this->archivo.fail());
        if(resultado) {

            for(unsigned int i=0; i< vcampos.size(); i++) {
                this->archivo.read(reinterpret_cast<char *> (&vcampos[i]), sizeof(vcampos[i]));
            }

            vcampos = Motor::getInstancia()->getAdministradorRecursos()->getEncriptador()->desencriptarVector(vcampos);

            // cargar el registro a devolver
            reg->cambiarEstado(estado[0]);

            for(unsigned int i=0; i<reg->getNumCampos(); i++)
            {
                reg->setValorCampo(reg->getCampo(i).getNombre(), vcampos[i]);
            }
        } else {
            Indexador* indexador = Motor::getInstancia()->getAdministradorRecursos()->getIndexador(this->nombre);
            indexador->setSiguienteId(pos);
            }
        delete estado;
    }
    return resultado;
}

bool ArchivoRegistros::borrar(Registro* &reg) {
    bool resultado = this->getRegistro(reg);
    char* estado;
    if(resultado) {
        estado = new char[1];
        estado[0] = 'L';
        // clave del registro:
        int pos = reg->getCampo("ID").getValor();
        this->archivo.clear();
        this->archivo.seekp(pos*this->sizeReg, ios::beg);
        // grabar marca de reg Libre
        this->archivo.write(reinterpret_cast<char *> (estado), sizeof(estado[0]));
        resultado = (!this->archivo.fail());
        if(resultado){
            Indexador* indexador = Motor::getInstancia()->getAdministradorRecursos()->getIndexador(this->nombre);
            indexador->getManagerLibresFijos()->addLibre(pos);
        }
        delete[] estado;
    }
    return resultado;
}

unsigned int ArchivoRegistros::sizeOfFile() {
    unsigned int ini;
    unsigned int fin;
    if (this->archivo.good() && this->archivo.is_open()) {
        this->archivo.seekg(0, ios_base::beg);
        ini = this->archivo.tellg();
        this->archivo.seekg(0, ios_base::end);
        fin=this->archivo.tellg();
        return (fin - ini);
    }
    return 0;
}

/**
* Obtiene la lista de registros libres, de este modo:
* Busca el primer registro Libre, una vez encontrado,
* obtiene los demas registros libres utilizando el encadenamiento
* de registro libres, evitando de este modo leer todo el archivo.
*/
void ArchivoRegistros::levantarListaRegLibres() {
    long sizeArchivo = this->sizeOfFile();
    int cantRegs = sizeArchivo / this->sizeReg;
    Indexador* indexador = Motor::getInstancia()->getAdministradorRecursos()->getIndexador(this->nombre);
    char* estado;
    int sgte;
    estado = new char[1];
    // buscar el primer registro libre
    int nroReg=0;
    bool encontrado = false;

    // para leer y descencriptar
    std::vector<int> vcampos = this->registro->getCampos();
    for(unsigned int i=0; i<vcampos.size(); i++) vcampos[i] = 0;

    vcampos.resize(Motor::getInstancia()->getAdministradorRecursos()->getEncriptador()->getTamanoVEncriptado(vcampos.size()), -1);

    this->archivo.clear();
    while((nroReg < cantRegs) && !(encontrado))  {
        // posicionarse en un registro
        this->archivo.seekg(nroReg*this->sizeReg, ios::beg);
        if(!this->archivo.fail()){
            // leer el estado
            this->archivo.read(estado, sizeof(estado[0]));
            if(estado[0]=='L')
                encontrado= true;
            else
                nroReg++;
        }
    }

    // si hay un registro libre
    if(encontrado) {
        sgte = nroReg;
        while((sgte != MAXUINT) && (encontrado)) {
            // agregarlo a la lista
            indexador->getManagerLibresFijos()->addLibre(sgte);
            // completar la lista de reg libres mediante el encadenamiento.
            for(unsigned int i=0; i<vcampos.size(); i++)
                this->archivo.read(reinterpret_cast<char *> (&vcampos[i]), sizeof(vcampos[i]));

            encontrado=(!this->archivo.fail());
            if(encontrado){
                vcampos = Motor::getInstancia()->getAdministradorRecursos()->getEncriptador()->desencriptarVector(vcampos);
                sgte = vcampos[0];
                if(sgte != MAXUINT) {
                    // posicionarse en un registro
                    this->archivo.seekg(sgte*this->sizeReg, ios::beg);
                    encontrado=(!this->archivo.fail());
                    if(encontrado){
                        // para verificar el encadenamiento, leer el estado
                        this->archivo.read(estado, sizeof(estado[0]));
                        encontrado=(!this->archivo.fail());
                        if(encontrado){
                            if(estado[0]!='L') sgte = MAXUINT;
                        }
                    }
                }
            }
        }
    }
    delete[] estado;
}


/**
* Actualiza el encadenamiento de registros libres en el archivo.
*/
void ArchivoRegistros::actualizarEncadenamiento() {
    Indexador* indexador = Motor::getInstancia()->getAdministradorRecursos()->getIndexador(this->nombre);
    int sgte;
    int pos;
    string estado = "L";
    bool resultado = true;
    std::vector<int> vcampos = this->registro->getCampos();
    for(unsigned int i=0; i<vcampos.size(); i++) vcampos[i] = 0;
    this->archivo.clear();
    // recorrer lista de registros libres
    pos=indexador->getManagerLibresFijos()->getLibre();
    while((pos>=0 ) && (resultado)){
            // posicionarse en un registro
            this->archivo.seekp(pos*this->sizeReg, ios::beg);
            resultado=(!this->archivo.fail());
            if(resultado){
                // grabar el estado
                this->archivo.write(estado.c_str(), estado.size());
                resultado=(this->archivo.good());
                if(resultado){
                    // grabar encadenamiento
                    vcampos[1] = pos;
                    // obtener el sgte libre para encadenar
                    sgte=indexador->getManagerLibresFijos()->getLibre();
                    pos = sgte;
                    if(sgte== -1) sgte = MAXUINT;
                    vcampos[0] = sgte;
                    vcampos = Motor::getInstancia()->getAdministradorRecursos()->getEncriptador()->encriptarVector(vcampos);
                    for(unsigned int i=0; i<vcampos.size(); i++){

                        this->archivo.write(reinterpret_cast<char *> (&vcampos[i]), sizeof(vcampos[i]));
                    }
                    resultado=(this->archivo.good());
                }
            }
    }
    this->archivo.close();
}

bool ArchivoRegistros::abrir(){
    this->archivo.open(this->nombre.c_str(), ios::in | ios::out | ios::binary);
    if (!this->archivo.good()){
        this->archivo.open(this->nombre.c_str(), ios::in | ios::out | ios::binary | ios::trunc);
    }
    return (this->archivo.is_open());
}

void ArchivoRegistros::cerrar(){
    this->archivo.close();
}
