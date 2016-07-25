/************************************************************************
                        PasswordDao.cpp


**************************************************************************/

#include "PasswordDao.h"
#include "OperacionesBinarias.h"

// Constructors/Destructors
//

PasswordDao::PasswordDao ( ) {

}

PasswordDao::~PasswordDao ( ) {

}

/**
* Busca en el medio de almacenamiento la entidad recibida por parametro
* Si no la encuentra, devuelve true, sino false.
* @param  entidad
*/

bool PasswordDao::get (Almacenable* entidad ){
    Encriptador* encriptador = NULL;
    char* lectura;
    string hpe = "";
    string posIzq = "";
    string posDer = "";
    unsigned long int izq = 0;
    unsigned long int der = 0;
	int length = 0, idxAux=0;
	vector<unsigned long int> hash;
    Password* password = (Password*) entidad;
    ifstream filestr;
    filestr.open(PASSWORD, ios::binary);
    if(filestr.good()){
        encriptador = new Encriptador();
        filestr.seekg (0, ios::end);
        length = filestr.tellg();
        lectura = new char[length];
        filestr.seekg (0, ios::beg);
        filestr.read (lectura,length);
        hpe.assign(lectura,length);

        if (OperacionesBinarias::longitudDoblePalabra() == 32)
            idxAux = length/4;
        else
            idxAux = length/2;

        hpe =  (string)encriptador->desencriptarString(hpe);
        posIzq = hpe.substr (0,idxAux);
        posDer = hpe.substr (idxAux,idxAux);
        izq = OperacionesBinarias::convertirString(posIzq);
        der = OperacionesBinarias::convertirString(posDer);
        hash.push_back(izq);
        hash.push_back(der);

        if (OperacionesBinarias::longitudDoblePalabra() == 32){
            hash.push_back(izq);
            hash.push_back(der);
        }
        password->setHashPassword(hash);
        entidad = password;
        filestr.close();
        delete(encriptador);
        delete(lectura);
    return true;

	}
     else{
		return false;
	}

}

bool PasswordDao::get (Almacenable* entidad, bool &valido ){
    valido = true;
    return this->get(entidad);
}


void PasswordDao::alta (Almacenable* entidad ){
    string posIzq = "";
    string posDer = "";
    unsigned long int izq = 0;
    unsigned long int der = 0;
    vector<unsigned long int> hashPassword;
    Encriptador* encriptador = new Encriptador();
    fstream filestr (PASSWORD, ios::in | ios::out);
    if(!filestr.good()){
    filestr.open(PASSWORD, ios::in | ios::out| ios::trunc | ios::binary);
    }
    filestr.seekp(ios::beg);
    Password* password = (Password*) entidad;
    hashPassword = password->getHashPassword();
    izq = (unsigned long int)hashPassword.at(0);
    der = (unsigned long int)hashPassword.at(1);
    posIzq = OperacionesBinarias:: convertirLongAString(izq);
    posDer = OperacionesBinarias:: convertirLongAString(der);
    posIzq.append(posDer);
    if (OperacionesBinarias::longitudDoblePalabra() == 32)
        posIzq.append(posIzq);
    posIzq =  (string)encriptador->encriptarString(posIzq);
    filestr.write(posIzq.c_str(), posIzq.size());
    filestr.close();
    delete(encriptador);
}

bool PasswordDao::modificacion (Almacenable* entidad ){
    this->alta(entidad);
    return true;
}

void PasswordDao::inicializarVariables(){
}

void PasswordDao::protegerVariables(){
}

void PasswordDao::baja (Almacenable* entidad ) {
    // no se permite la baja del password
}

void PasswordDao::getAlmacenables(list<Almacenable*>* lista) {
    // No en password dao
}

