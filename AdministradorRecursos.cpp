/************************************************************************
                        AdministradorRecursos.h.cpp - Copyright rodrigo

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

This file was generated on dom oct 12 2008 at 16:58:06
The original location of this file is /home/rodrigo/uml-generated-code/AdministradorRecursos.cpp
**************************************************************************/

#include "AdministradorRecursos.h"
#include "Motor.h"

// Constructors/Destructors
AdministradorRecursos::AdministradorRecursos (Soporte* soporte ) {
    this->soporte = soporte;
    this->ocultador = new Ocultador();
	this->encriptador = new Encriptador();
	this->segmentador = new Segmentador();
	this->compresor = new Compresor();
	this->indexadores.insert(pair<string,Indexador*>(OCULTADOS,new Indexador(OCULTADOS, true)));
    this->indexadores.insert(pair<string,Indexador*>(PARTICIONES,new Indexador(PARTICIONES, false)));
    this->indexadores.insert(pair<string,Indexador*>(DIRECTORIOS,new Indexador(DIRECTORIOS, true)));
    this->indexadores.insert(pair<string,Indexador*>(IMAGENES,new Indexador(IMAGENES, true)));
}

AdministradorRecursos::~AdministradorRecursos ( ) {
    delete this->ocultador;
	delete this->encriptador;
	delete this->segmentador;
	delete this->compresor;
	this->eliminarIndexadores();
}

Ocultador* AdministradorRecursos::getOcultador ( )	 {
    return ocultador;
}

Encriptador* AdministradorRecursos::getEncriptador ( )	 {
    return encriptador;
}

Segmentador* AdministradorRecursos::getSegmentador ( )	 {
    return segmentador;
}

Compresor* AdministradorRecursos::getCompresor ( )	 {
    return compresor;
}

Indexador* AdministradorRecursos::getIndexador (string nombre){
    Indexador* indexador = NULL;
    string real = nombre;
    unsigned long pos = real.find(NOMBRE);
    if ( pos != string::npos)
         real =  real.replace(pos, NOMBRE_SIZE,"");
    map<string,Indexador*>::iterator it = this->indexadores.find(real);
    if (it != this->indexadores.end())
        indexador = it->second;
    return indexador;
}


void AdministradorRecursos::actualizar(){
    //TODO: Implementar la actualizacion q va a hacer llamar el hilo observador
}

UI::Error AdministradorRecursos::esteganografiar (Texto* texto ) {
    UI::Error error = UI::OK;
    map<Informacion*,Portador*> apareo;
    list<Almacenable*> listaParticiones;
    list<Almacenable*>::iterator itPortadores;
    list<Almacenable*>::iterator itParticiones;
    list<int>::iterator nums_iter;
    list<int> directoriosId;
    Directorio* dir;
    Particion* particion;
    Portador* portador;
    ImagenDao* imageDao;
    TextoDao* textoDao;
    DirectorioDao* dirDao;
    ParticionDao* particionDao;
    map<Informacion*,Portador*>::iterator it;
    unsigned int orden;

    UI::getInstancia()->imprimir("Iniciando proceso de esteganografia...");
    UI::getInstancia()->imprimir("Recopilando recursos...");
    list<Almacenable*> listaRecursos = this->obtenerRecursos(texto);
    if (!listaRecursos.empty()){
        // Procedo a almacenar el texto
        // SEGMENTACION
        //this->segmentador->particionar(texto, listaRecursos.size());
        UI::getInstancia()->imprimir("Segmentando...");
        this->segmentador->particionar(texto,listaRecursos);
        // Asignacion de recursos para cada particion
        listaParticiones = texto->obtenerCopiaParticiones();
        itPortadores = listaRecursos.begin();
        itParticiones = listaParticiones.begin();
        UI::getInstancia()->imprimir("Comprimiendo...");
        while ((itPortadores != listaRecursos.end()) && (itParticiones != listaParticiones.end())){
            particion = (Particion*)*itParticiones;
            portador = (Portador*)*itPortadores;
            // COMPRESION
            if (particion->getTamano() <= MAX_COMPRIMIBLE){
                cout << ".";
                string comprimido = this->compresor->comprimir(particion->getDatos());
                if (comprimido.size() < particion->getTamano()){
                    particion->setDatos(comprimido);
                    particion->setComprimida(true);
                }
            }
            particion->setOffsetImagen(Motor::getInstancia()->getProcesadorTextos()->calcularOffsetEscritura(portador->getId(), portador->getOffsetInicial()));
            if ((particion->getIdImagen()) == (portador->getId())){
                apareo.insert(pair<Particion*,Portador*>(particion, portador));
            }
            itParticiones++;
            itPortadores++;
        }
        listaParticiones.clear();
        UI::getInstancia()->imprimir("");
        UI::getInstancia()->imprimir("Ocultando...");
        // Ocultar cada Informacion en su Portador
        if (this->ocultador->ocultar(apareo)){
            imageDao = new ImagenDao();
            textoDao = new TextoDao();
            dirDao = new DirectorioDao();
            particionDao = new ParticionDao();
            it = apareo.begin();

            UI::getInstancia()->imprimir("Actualizando informacion administrativa...");
            // Actualizar informacion administrativa en memora Y disco
            texto->setId(this->getIndexador(OCULTADOS)->proximoId());
            orden = 1;
            bool actualizado = true;
            while(it != apareo.end()){
                particion = (Particion*)(*it).first;
                portador = (*it).second;
                particion->setOrden(orden++);
                particion->setIdTexto(texto->getId());
                if ((portador->getBytesLibres() - particion->getTamano()) >= 0)
                    portador->setBytesLibres(portador->getBytesLibres() - particion->getTamano());
                else
                    portador->setBytesLibres(0);

                if (!portador->esPersistido()){
                    //portador->setId(this->getIndexador(IMAGENES)->proximoId());
                    imageDao->alta(portador);
                }
                else
                    actualizado = imageDao->modificacion(portador);
                particion->setId(this->getIndexador(PARTICIONES)->proximoId());
                particionDao->alta(particion);

                dir = (Directorio* ) this->soporte->obtenerDirectorioContenedor(portador->getId());
                if (directoriosId.empty()){ // si no esta en la lista de dir usados
                    directoriosId.push_back (dir->getId());
                    if (!dir->esPersistido()){
                        //dir->setId(this->getIndexador(DIRECTORIOS)->proximoId());
                        dirDao->alta(dir);
                    }
                    dir->actualizarEspacioLibre();
                }
                else{
                    nums_iter = find(directoriosId.begin(), directoriosId.end(), dir->getId()); // Busca en la lista.
                    if (nums_iter != directoriosId.end()){
                        directoriosId.push_back (dir->getId());
                        if (!dir->esPersistido()){
                            //dir->setId(this->getIndexador(DIRECTORIOS)->proximoId());
                            dirDao->alta(dir);
                        }
                        dir->actualizarEspacioLibre();
                    }
                }
                it++;
            }
            if (!actualizado)
                error = UI::FALLO_ACTUALIZARREGS;
            this->soporte->actualizarEspacioLibre();
            texto->setCantidadParticionesOriginal(texto->getNroParticiones());
            textoDao->alta(texto);

            delete (imageDao);
            delete (textoDao);
            delete (dirDao);
            delete (particionDao);
        }
        else
            error = UI::FALLO_OCULTAR;
    }
    else
        error = UI::RECURSOS_INSUFICIENTES;
    return error;
}

UI::Error AdministradorRecursos::desEsteganografiar (Texto* texto ) {
    UI::Error error = UI::OK;
    list<Almacenable*> particiones = texto->obtenerCopiaParticiones();
    list<Almacenable*>::iterator itPortadores;
    list<Almacenable*>::iterator itParticiones;
    map<Informacion*,Portador*> apareo;
    Particion* particion;
    Portador* portador;
    UI::getInstancia()->imprimir("Recopilando informacion...");
    itParticiones = particiones.begin();
    while ((error == UI::OK) && (itParticiones != particiones.end())){
        particion = (Particion*)*itParticiones;
        portador = this->soporte->obtenerPortador(particion->getIdImagen());
        apareo.insert(pair<Particion*,Portador*>(particion, portador));
        itParticiones++;
    }
    UI::getInstancia()->imprimir("Recuperando datos...");
    if (error == UI::OK){
        if (this->ocultador->desOcultar(apareo)){
            // ACA DESCOMPRIMO LAS PARTICIONES SI ESTABAN COMPRIMIDAS
            itParticiones = particiones.begin();
            UI::getInstancia()->imprimir("Descomprimiendo...");
            while (itParticiones != particiones.end()){
                particion = (Particion*)*itParticiones;
                if (particion->estaComprimida()){
                    particion->setDatos(this->compresor->descomprimir(particion->getDatos()));
                    cout << ".";
                }
                itParticiones++;
            }
            UI::getInstancia()->imprimir("");
            UI::getInstancia()->imprimir("Reuniendo archivo...");
            // REUNO LAS PARTICIONES RECUPERANDO EL TEXTO ORIG
            this->segmentador->desParticionar(texto);
        }
        else
            error = UI::FALLO_OCULTAR;
    }
    particiones.clear();
    apareo.clear();
    return error;
}

UI::Error AdministradorRecursos::desEsteganografiar(Texto* texto,string nombre){
    UI::Error error = UI::OK;
    string ruta = "";
    short int index = nombre.find_last_of('/');
    Directorio* dir;
    Texto* copia = NULL;
    // Validar que en la ruta haya permisos de escritura, y q el dir exista
    if (index >=0)
        ruta = nombre.substr(0, index);
    if (!ruta.empty()){
        dir = this->crearDirectorio(ruta);
        if (dir){
            delete (dir);
        }
        else
            error = UI::DIRECTORIO_INVALIDO;
    }
    if (error == UI::OK){ // Solo recupero si el path es valido
        // Se trabaja con una copia del texto para no dejar su contenido en memoria
        copia = texto->copiar();
        error = this->desEsteganografiar(copia);
        Motor::getInstancia()->getProcesadorTextos()->grabar(copia,nombre);
        delete copia;
    }
    return error;
}


list<Almacenable*> AdministradorRecursos::obtenerRecursos (Texto* texto ) {
    list<Almacenable*> portadores;
    list<Almacenable*> aux;
    list<Almacenable*> directorios;
    list<Almacenable*>::iterator it;
    Directorio* directorio;

    if (this->soporte->getEspacioLibre() >= texto->getTamano()){
        directorios = this->soporte->obtenerCopiaDirectorios(texto->getTamano());
        if (!directorios.empty()){
            directorio = (Directorio*)directorios.front(); // WORST FIT
            portadores = directorio->obtenerCopiaPortadores(texto->getTamano()); // BEST FIT(s)
        }
        else { // No entra en ningun directorio unico, tengo q tomar varios dirs...
            directorios = this->soporte->obtenerCopiaDirectoriosParaCompletar(texto->getTamano());
            it = directorios.begin();
            while (it != directorios.end()){
                aux = ((Directorio*)*it)->obtenerCopiaPortadores();
                portadores.insert(portadores.end(),aux.begin(), aux.end());
                aux.clear();
                it++;
            }
        }
        directorios.clear();
    }
    return portadores;
}


//private
void AdministradorRecursos::eliminarIndexadores(){
    map<string,Indexador*>::iterator it=this->indexadores.begin() ;
    while(it != this->indexadores.end()){
        delete (it->second);
        it++;
    }
    this->indexadores.clear();
}

Directorio* AdministradorRecursos::crearDirectorio(string ruta){
    Directorio* directorio = NULL;
    DIR* fsdir = opendir(ruta.c_str());

    if (fsdir){
        closedir(fsdir);
        unsigned int id = this->getIndexador(DIRECTORIOS)->proximoId();
        directorio = new Directorio(id,ruta);
    }

    return directorio;
}

void AdministradorRecursos::indexarLibres(){
    map<string,Indexador*>::iterator it=this->indexadores.begin() ;
    while (it != this->indexadores.end()){
        ((Indexador*)(it->second))->indexarLibres();
        it++;
    }
}

void AdministradorRecursos::inicializar(){
    map<string,Indexador*>::iterator it=this->indexadores.begin() ;
    while (it != this->indexadores.end()){
        ((Indexador*)(it->second))->inicializar();
        it++;
    }
}

void AdministradorRecursos::liberar(){
    map<string,Indexador*>::iterator it=this->indexadores.begin() ;
    while (it != this->indexadores.end()){
        ((Indexador*)(it->second))->liberar();
        it++;
    }
}

void AdministradorRecursos::reorganizarArchivos(double porcentage){
    map<string,Indexador*>::iterator it=this->indexadores.begin() ;
    while (it != this->indexadores.end()){
        ((Indexador*)(it->second))->reorganizarArchivoRegistro(porcentage);
        it++;
    }
}

void AdministradorRecursos::actualizarEncadenamiento(){
    map<string,Indexador*>::iterator it=this->indexadores.begin() ;
    while (it != this->indexadores.end()){
        ((Indexador*)(it->second))->actualizarEncadenamientos();
        it++;
    }
}

string AdministradorRecursos::getTipoArchivo(string nombre){
    string tipo = "";
    size_t pos;
    pos = nombre.find_last_of(".");
    if (pos!=string::npos){
        tipo = nombre.substr (pos,4);
        transform( tipo.begin(), tipo.end(), tipo.begin(), ::tolower );
    }
    return tipo;
}

 bool AdministradorRecursos::analisisInicial (Directorio* &directorio,TipoAnalisis tipo)
{
    int idIncongruencias = 0;
    struct stat tamanio;
    string pathArchivo;
    string tipoArchivo;
    ArchivoImagen* imagen = NULL;
    DIR *dp;
    struct dirent *dirp;

    if((dp  = opendir((directorio->getRuta()).data())) == NULL) {
        return false;
    }

    while ((dirp = readdir(dp)) != NULL) {
        //Esto es para obtener el path del archivo, ya que readdir me devuelve el nombre.extension
        //y el stat usado para obtener el tamaño espera el path del archivo.

        pathArchivo = directorio->getRuta();
        pathArchivo.append(string(dirp->d_name));
        stat(pathArchivo.c_str(), &tamanio);
        //Como readdir trae tambien directorios, valido que lo que voy a cargar sea un archivo.
        if (S_ISDIR (tamanio.st_mode) == 0){
            tipoArchivo = getTipoArchivo(string(dirp->d_name));

            if (tipoArchivo.compare(".jpg") == 0){
                 imagen = new ArchivoImagenJPG(0,directorio->getId(),string(dirp->d_name),tamanio.st_size);
            }
            else if (tipoArchivo.compare(".gif") == 0){
                imagen = new ArchivoImagenGIF(0,directorio->getId(),string(dirp->d_name),tamanio.st_size);
            }
            else if (tipoArchivo.compare(".bmp") == 0){
                imagen = new ArchivoImagenBMP(0,directorio->getId(),string(dirp->d_name),tamanio.st_size);
            }
            else if (tipoArchivo.compare(".png") == 0){
                imagen = new ArchivoImagenPNG(0,directorio->getId(),string(dirp->d_name),tamanio.st_size);
            }

            if (imagen){

               if(tipo == ANALISIS_INICIAL){
                    unsigned int idImagen = this->getIndexador(IMAGENES)->proximoId();
                    imagen->setId(idImagen);
                    directorio->agregarPortador(imagen);
                    }
                if(tipo == ANALISIS_INCONGRUENCIAS){
                    imagen->setId(idIncongruencias);
                    directorio->agregarPortador(imagen);
                    idIncongruencias++;
                    }

                imagen = NULL;
            }
        }
    }
    closedir(dp);

    return true;
}

/**
* Funcion utilizada para el ordenamiento de las listas necesarias para el funcionamiento del
* metodo llamado analizarIncongruenciasEstaticas
* Este metodo devuelve true si el primero termino es menor al segundo y si son iguales,false en caso contrario.
*/
bool ordenarPortadores (Almacenable* portador1, Almacenable* portador2){
//    unsigned int i=0;
    string nombre1 = ((ArchivoImagen*)portador1)->getNombre();
    string nombre2 = ((ArchivoImagen*)portador2)->getNombre();
    if(nombre1 < nombre2)
        return true;
    else
        if(nombre1 > nombre2)
            return false;
        else
            return true;
}

//CHECKEO QUE EL PRIMERO SEA MENOR QUE EL SEGUNDO
int compararRutas (string ruta1, string ruta2)
{
if (ruta1 < ruta2)
    return 1;
else
    if(ruta1 > ruta2)
        return -1;
    else
        return 0;
}

pthread_cond_t  condition  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void* AdministradorRecursos::observarSoporte (void* objeto){
    Motor* motor = (Motor*) objeto;
    int sleep;
    struct timespec timeout;

    while (motor->estaCorriendo()){ // LLama a analizarIncongruencias cada TIMEOUT seg
        sleep = 0;
        clock_gettime(CLOCK_REALTIME, &timeout);
        timeout.tv_sec += TIMEOUT;
        timeout.tv_nsec = 0;
        pthread_mutex_lock(&mutex);
        while (sleep != ETIMEDOUT) {
            sleep = pthread_cond_timedwait(&condition, &mutex, &timeout);
        }
        pthread_mutex_unlock(&mutex);
        motor->getAdministradorRecursos()->analizarIncongruencias();
    }
    return NULL;
}

void AdministradorRecursos::analizarIncongruencias(){
    DirectorioDao* dir = NULL;
    ImagenDao* img = NULL;
    unsigned short int id = 0;
    Indexador* indexador = Motor::getInstancia()->getAdministradorRecursos()->getIndexador(IMAGENES);
    string nombrePortadorSistema;
    string nombrePortadorFS;
    //LISTA DE DIRECTORIOS DE NUESTRO SISTEMA
    list<Almacenable*>* listaDirectorios = this->soporte->obtenerDirectorios();
    list<Almacenable*>::iterator itDirectorios = listaDirectorios->begin();
    //LISTA DE PORTADORES DE NUESTRO SISTEMA
    list<Almacenable*> listaCopiaPortadores;
    list<Almacenable*>::iterator itPortadores;
    //LISTA DE PORTADORES DEL FILE SYSTEM
    list<Almacenable*>* listaFSPortadores;
    list<Almacenable*>::iterator itFSPortadores;
    Directorio* directorioFS = new Directorio();
    Directorio* directorio = new Directorio();
    string ruta = "";
    string comunicado;
    bool bajas = false;
    bool altas = false;
    list<Almacenable*> listaAux;
    list<Almacenable*>::iterator it;


    //RECORRO TODA LA LISTA DE DIRECTORIOS DEL SISTEMA
    while(itDirectorios != listaDirectorios->end()){
        //SE SETEA EL ID Y LA RUTA EN DIRECTORIO DEL FS PARA CONSULTAR SI EXISTE REALMENTE

        directorio = (Directorio*)*itDirectorios;
        directorioFS = new Directorio(directorio->getId(),directorio->getRuta());
//        directorioFS->setId();
//        directorioFS->setRuta();
        ruta = directorio->getRuta();
        //SI EL DIRECTORIO EXISTE SE LE CARGAN LOS PORTADORES SINO SE BORRA EL DIRECTORIO Y TODOS SUS PORTADORES
        if(this->analisisInicial(directorioFS,ANALISIS_INCONGRUENCIAS)){
            //CARGO LA LISTA DE PORTADORES DEL DIRECTORIO DEL FILESYSTEM
            listaFSPortadores = directorioFS->obtenerPortadores();
            //CARGO LA LISTA DE PORTADORES DEL DIRECTORIO NUESTRO SISTEMA
            listaCopiaPortadores = ((Directorio*)*itDirectorios)->obtenerCopiaPortadores();
            listaCopiaPortadores.sort(ordenarPortadores);
            listaFSPortadores->sort(ordenarPortadores);
            itFSPortadores = listaFSPortadores->begin();
            itPortadores = listaCopiaPortadores.begin();
            //SE RECORREN LOS PORTADORES DE NUESTO SISTEMA
            while(itPortadores != listaCopiaPortadores.end() && itFSPortadores != listaFSPortadores->end()){
                nombrePortadorSistema = ((ArchivoImagen*)*itPortadores)->getNombre();
                nombrePortadorFS =  ((ArchivoImagen*)*itFSPortadores)->getNombre();
                //SI EL PRIMER ELEMENTO ES MENOR QUE EL SEGUNDO SE DEBE DAR DE BAJA ESE PORTADOR
                if(compararRutas(nombrePortadorSistema , nombrePortadorFS) == 1){
                    img = new ImagenDao();
                    //SE MUEVE EL ITERADOR DE SISTEMA Y SE DAN LAS BAJAS DE LOS PORTADORES MIENTRAS QUE
                    //EL PORTADOR DE SISTEMA SEA MENOR AL PORTADOR DEL FILE SYSTEM
                    while(compararRutas(nombrePortadorSistema, nombrePortadorFS) == 1 && itPortadores != listaCopiaPortadores.end()){
                        img->baja((Almacenable*)*itPortadores);
                        this->limpiarInfoAdministrativa(((Almacenable*)*itPortadores)->getId());
                        bajas = true;
                        comunicado = "La imagen ";
                        comunicado += ((ArchivoImagen*)*itPortadores)->getNombre();
                        comunicado += " del directorio ";
                        comunicado += ((Directorio*)*itDirectorios)->getRuta();
                        comunicado +=" no se encuentra disponible en sus sistema y ha sido eliminada.";
                        UI::getInstancia()->imprimir(comunicado);
                        ((Directorio*)*itDirectorios)->borrarPortador((Portador*)*itPortadores);
                        itPortadores++;

                        if(itPortadores != listaCopiaPortadores.end())
                            nombrePortadorSistema = ((ArchivoImagen*)*itPortadores)->getNombre();
                    }
                    delete img;
                }else
                   if(compararRutas(((ArchivoImagen*)*itPortadores)->getNombre() , ((ArchivoImagen*)*itFSPortadores)->getNombre()) == -1){
                        nombrePortadorSistema = ((ArchivoImagen*)*itPortadores)->getNombre();
                        nombrePortadorFS = ((ArchivoImagen*)*itFSPortadores)->getNombre();
                        //SE MUEVE EL ITERADOR DE SISTEMA Y SE DAN LAS ALTAS DE LOS PORTADORES MIENTRAS QUE
                        //EL PORTADOR DE SISMTEA SEA MENOR AL PORTADOR DEL FILE SYSTEM
                        while(compararRutas(nombrePortadorSistema, nombrePortadorFS) == -1 && itFSPortadores != listaFSPortadores->end()){
                            id = indexador->proximoId();
                            ((ArchivoImagen*)*itFSPortadores)->setId(id);
                            ((ArchivoImagen*)*itFSPortadores)->preProcesar();
                            ((ArchivoImagen*)*itFSPortadores)->setIdDirectorio(((Directorio*)*itDirectorios)->getId());
                            ((Directorio*)*itDirectorios)->agregarPortador((Portador*)*itFSPortadores);
                            itFSPortadores = listaFSPortadores->erase(itFSPortadores);
                            altas = true;
                            if(itFSPortadores != listaFSPortadores->end())
                                nombrePortadorFS = ((ArchivoImagen*)*itFSPortadores)->getNombre();
                        }

                   }else{
                       itPortadores++;
                       itFSPortadores++;
                    }
            }//END WHILE

            if(itPortadores != listaCopiaPortadores.end() && itFSPortadores == listaFSPortadores->end()){
                img = new ImagenDao();
                while(itPortadores != listaCopiaPortadores.end()){
                    img->baja((Almacenable*)*itPortadores);
                    this->limpiarInfoAdministrativa(((Almacenable*)*itPortadores)->getId());
                    bajas = true;
                    comunicado = "La imagen ";
                    comunicado += ((ArchivoImagen*)*itPortadores)->getNombre();
                    comunicado += " del directorio ";
                    comunicado += ((Directorio*)*itDirectorios)->getRuta();
                    comunicado +=" no se encuentra disponible en sus sistema y ha sido eliminada.";
                    UI::getInstancia()->imprimir(comunicado);
                    ((Directorio*)*itDirectorios)->borrarPortador((Portador*)*itPortadores);
                    itPortadores++;
                }
                delete img;
            }

            if(itPortadores == listaCopiaPortadores.end() && itFSPortadores != listaFSPortadores->end()){
                while(itFSPortadores != listaFSPortadores->end()){
                    id = indexador->proximoId();
                    ((ArchivoImagen*)*itFSPortadores)->setId(id);
                    ((ArchivoImagen*)*itFSPortadores)->setIdDirectorio(((Directorio*)*itDirectorios)->getId());
                    ((ArchivoImagen*)*itFSPortadores)->preProcesar();
                    ((Directorio*)*itDirectorios)->agregarPortador((Portador*)*itFSPortadores);
                    itFSPortadores = listaFSPortadores->erase(itFSPortadores);

                    altas = true;
                }
            }
            if (altas){
                comunicado = "Se han agregado imagenes al directorio ";
                comunicado += ((Directorio*)*itDirectorios)->getRuta() + ".";
                UI::getInstancia()->imprimir(comunicado);
                altas = false;
            }
            ((Directorio*)*itDirectorios)->actualizarEspacioLibre();
        listaCopiaPortadores.clear();
        //listaFSPortadores.clear();
        delete directorioFS;

            itDirectorios++;
        }//END IF this->analisisInicial(directorioFS)
        else{
            listaAux = ((Directorio*)*itDirectorios)->obtenerCopiaPortadores();
            it = listaAux.begin();
            img = new ImagenDao();
            while(it != listaAux.end()){
                img->baja((Almacenable*)*it);
                this->limpiarInfoAdministrativa(((Almacenable*)*it)->getId());
                it++;
            }
            delete img;
            dir = new DirectorioDao();
            dir->baja(((Directorio*)*itDirectorios));
            delete dir;

            comunicado = "El directorio ";
            comunicado += ((Directorio*)*itDirectorios)->getRuta();
            comunicado +=" no se encuentra disponible en sus sistema y ha sido eliminado.";
            UI::getInstancia()->imprimir(comunicado);
            bajas = true;
            delete *itDirectorios;
            itDirectorios = listaDirectorios->erase(itDirectorios);
        }

    }//END WHILE
    if (bajas)
        UI::getInstancia()->imprimir(UI::PERDIDA_RECURSOS);

    this->soporte->actualizarEspacioLibre();
}//END ANALIZARINCONGRUENCIASESTATICAS


void AdministradorRecursos::limpiarInfoAdministrativa(unsigned int idImagen){
    list<Almacenable*> textos;
    list<Almacenable*>* particiones;
    list<Almacenable*>::iterator itTexto;
    list<Almacenable*>::iterator itParticion;
    Texto* texto = NULL;
    Particion* particion = NULL;
    Dao* particionDao = new ParticionDao();

    textos = Motor::getInstancia()->getProcesadorTextos()->obtenerTextos(idImagen);
    itTexto = textos.begin();
    while(itTexto!=textos.end()){
        texto = (Texto*)*itTexto;
        //obtener particiones del texto
        particiones = texto->obtenerParticiones();
        // borrar particiones
        itParticion = particiones->begin();
        while(itParticion!=particiones->end()){
            particion = (Particion*)*itParticion;
            if (particion->getIdImagen() == idImagen){
                particionDao->baja(particion);
                delete particion;
                itParticion = particiones->erase(itParticion);
            }
            else
                itParticion++;
        }
        itTexto++;
    }
    delete particionDao;
}

bool AdministradorRecursos::verificarDirectorioAdministrativo(){
    bool retorno = false;
    DIR* fsdir = opendir("adm");

    if (fsdir){
        closedir(fsdir);
        retorno = true;
    }
    else{
        retorno = !mkdir("adm/", S_IRWXU | S_IRWXG | S_IRWXO);
    }
    return retorno;
}
