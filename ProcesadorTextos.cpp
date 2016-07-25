/************************************************************************
                        ProcesadorTextos.h.cpp - Copyright rodrigo

Here you can write a license for your code, some comments or any other
information you want to have in your generated code. To to this simply
configure the "headings" directory in uml to point to a directory
where you have your heading files.

or you can just replace the contents of this file with your own.
If you want to do this, this file is located at

/usr/share/apps/umbrello/headings/heading.cpp

-->Code Generators searches for heading files based on the file extension
   i.e. it will look for a file name ending in ".h" to include in C++ header
   files, and for a file name ending in ".java" to include in all generated
   java code.
   If you name the file "heading.<extension>", Code Generator will always
   choose this file even if there are other files with the same extension in the
   directory. If you name the file something else, it must be the only one with that
   extension in the directory to guarantee that Code Generator will choose it.

you can use variables in your heading files which are replaced at generation
time. possible variables are : author, date, time, filename and filepath.
just write %variable_name%

This file was generated on sáb oct 11 2008 at 17:21:24
The original location of this file is /home/rodrigo/uml-generated-code/ProcesadorTextos.cpp
**************************************************************************/

#include "ProcesadorTextos.h"
#include "TextoDao.h"

// Constructors/Destructors
//
ProcesadorTextos::ProcesadorTextos ( ) {
    this->cargarListaTextos();
}

ProcesadorTextos::~ProcesadorTextos ( ) {
    this->eliminarListaTextos();
}

void ProcesadorTextos::addTextos ( Texto * add_object ) {
    this->textoslist.push_back(add_object);
}

Texto* ProcesadorTextos::crearTexto(string nombre){
    //abrir texto, validar que sea un nombre valido, tomar tamaño y crear el texto.
    Texto* texto = NULL;
    Particion* particion = NULL;
    unsigned int tamano = 0;
    fstream archivo;
    char* buffer;
    string datos;
    datos.clear();
    archivo.open(nombre.c_str(), ios::binary | ios::in);
    if (archivo.good()){
        archivo.seekg(0, ios::end);
        tamano = archivo.tellg();
        if (tamano >= 0){
            buffer = (char*) calloc(tamano , sizeof(char));
            archivo.seekg(0, ios::beg);
            archivo.read(buffer,tamano*sizeof(char));
            datos.assign(buffer,tamano*sizeof(char));
            if (datos.size() > tamano);
                datos = datos.substr(0,tamano);

            particion = new Particion();
            particion->setDatos(datos);
            particion->setTamano(tamano);
            texto = new Texto(nombre, tamano);
            texto->agregarParticion(particion);
            free(buffer);
        }
    }
    return texto;
}


/**
 * Devuelve el Texto correspondiente al idTexto pasado por parametro si lo
 * encuentra.
 * @return Texto
 * @param  idTexto Obtiene el Texto pedido
 */
Texto* ProcesadorTextos::obtenerTexto (unsigned int idTexto ) {
    Texto* texto = NULL;
    list<Almacenable*>::iterator it = this->textoslist.begin();
    while ((!texto) && (it != this->textoslist.end())){
        if (((Texto*)*it)->getId() == idTexto)
            texto = (Texto*)*it;
        it++;
    }
    return texto;
}

unsigned int ProcesadorTextos::calcularOffsetEscritura(unsigned int idImagen, unsigned int offsetInicial){
    unsigned int offset = offsetInicial;
    list<Almacenable*> particiones = this->generarListaParticionesOrdenadaOffset(idImagen);
    Particion* particion;
    if (!particiones.empty()){
        particion = (Particion*)particiones.back();
        offset +=  particion->getOffsetImagen() + particion->getTamano();
    }
    return offset;
}

/**
 * Devuelve el size del archivo identificado por el file descripor.
 * @return int
 * @param  texto File descriptor
 */
unsigned int ProcesadorTextos::calcularTamano (Texto* texto ) {
    unsigned int tamano = 0;
    ifstream f;
    f.open(texto->getNombre().c_str(), ios_base::binary | ios_base::in); // TODO: concatenar con dir
    if (f.good() && !f.eof() && f.is_open()) {
        f.seekg(0, ios_base::beg);
        ifstream::pos_type begin_pos = f.tellg();
        f.seekg(0, ios_base::end);
        tamano = static_cast<int>(f.tellg() - begin_pos);
    }
    return tamano;
}


/**
 * Remove a Textos object from this->textoslist List
 */
bool ProcesadorTextos::removerTexto ( unsigned int idTexto ) {
    bool existe = false;
    list<Almacenable*>::iterator it =  this->textoslist.begin();
    while ((!existe) && (it != this->textoslist.end()) ){
        if (((Texto*)*it)->getId() ==idTexto){
            delete (*it);
            it = this->textoslist.erase(it);
            existe = true;
        }
        else
            it++;
     }
    return existe;
}

/**
 * Remove a Textos object from this->textoslist List
 */
bool ProcesadorTextos::removerTexto ( string nombre ) {
    bool existe = false;
    list<Almacenable*>::iterator it =  this->textoslist.begin();
    while ((!existe) && (it != this->textoslist.end()) ){
        if (((Texto*)*it)->getNombre() ==nombre){
            delete (*it);
            it = this->textoslist.erase(it);
            existe = true;
        }
        else
            it++;
     }
    return existe;
}

bool ProcesadorTextos::existeTexto (string nombre) {
    Texto* texto = NULL;
    bool existe = false;
    list<Almacenable*>::iterator it =  this->textoslist.begin();
    while ( (!existe) && (it != this->textoslist.end()) ){
        texto = (Texto*)*it;
        if (nombre.compare(texto->getNombre()) == 0){
            existe = true;
            }

        it++;
     }

    return existe;
}

Texto* ProcesadorTextos::obtenerTexto(string nombre) {
    Texto* texto = NULL;
    bool encontrado = false;
    list<Almacenable*>::iterator it =  this->textoslist.begin();
    while ( (!encontrado) && (it != this->textoslist.end()) ){
        texto = (Texto*)*it;
        if (nombre.compare(texto->getNombre()) == 0){
            encontrado = true;
            }
         else {
             texto = NULL;
            }

        it++;
     }

    return texto;
}


unsigned int ProcesadorTextos::obtenerCantidadTextos(){
    return this->textoslist.size();
}


list<Almacenable*>* ProcesadorTextos::obtenerTextos(){
    return (&this->textoslist);
}

void ProcesadorTextos::setTextos ( list<Almacenable*> new_var ){
    this->eliminarListaTextos();
    this->textoslist = new_var;
}

list<Almacenable*> ProcesadorTextos::obtenerCopiaTextos(){
    return this->textoslist;
}

bool ProcesadorTextos::grabar(Texto* texto, const string ruta){
    ofstream f;
    Particion* particion = (Particion*)texto->obtenerCopiaParticiones().front();
    string buffer = particion->getDatos();
    f.open(ruta.c_str(), ios::in | ios::out | ios::binary | ios::trunc);
    if (f.good()) {
        f.seekp(0, ios::beg);
        f.write(buffer.c_str(),buffer.size());
    }else
        return false;
    return true;
}

list<Almacenable*> ProcesadorTextos::obtenerTextos(const unsigned int idImagen){
    list<Almacenable *>::iterator it = this->textoslist.begin();
    list<Almacenable*> resultado;
    Texto* texto;
    while (it != this->textoslist.end()){
        texto = (Texto*) *it;
        if (texto->usaPortador(idImagen))
            resultado.push_back(*it);
        it++;
     }
     return resultado;
}


//private
void ProcesadorTextos::cargarListaTextos ( ) {
    //Dao* dao = new TextoDao();
    // Aca se llama a un metodo de la dao que carga la lista de textos q falta implementar
    //delete dao;
}

//CHECKEO QUE EL PRIMERO SEA MENOR QUE EL SEGUNDO
bool comprarOffsetAsc (void* ent1, void* ent2)
{
    Particion* particion1 = (Particion*) ent1;
    Particion* particion2 = (Particion*) ent2;
    if (particion1->getOffsetImagen() < particion2->getOffsetImagen())
        return true;
    else
       return false;
}

list<Almacenable*> ProcesadorTextos::generarListaParticionesOrdenadaOffset(unsigned int idImagen){
    list<Almacenable*> resultado;
    list<Almacenable*> particiones;
    list<Almacenable*>::iterator itText;
    list<Almacenable*>::iterator itParticiones;
    Particion* particion;
    itText = this->textoslist.begin();
    while(itText != this->textoslist.end()){
        particiones = ((Texto*)*itText)->obtenerCopiaParticiones();
        itParticiones= particiones.begin();
        while (itParticiones != particiones.end()){
            particion = (Particion*)*itParticiones;
            if (particion->getIdImagen() == idImagen)
                resultado.push_back(*itParticiones);
            itParticiones++;
        }
        itText++;
        particiones.clear();
    }
    resultado.sort(comprarOffsetAsc);
    return resultado;
}

void ProcesadorTextos::eliminarListaTextos(){
    list<Almacenable*>::iterator it = this->textoslist.begin();
    while (it != this->textoslist.end()){
        delete (*it);
        it = this->textoslist.erase(it);
    }
}
