/************************************************************************
                        Motor.h.cpp - Copyright rodrigo

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
The original location of this file is /home/rodrigo/uml-generated-code/Motor.cpp
**************************************************************************/

#include "Motor.h"


void Motor::addDirectory (string ruta ) {
    Directorio* directorio = NULL;
    list<Almacenable*> portadores;
    list<Almacenable*>::iterator it;
    if (!Motor::getInstancia()->getSoporte()->existeDirectorio(ruta)){
        directorio =  this->getAdministradorRecursos()->crearDirectorio(ruta);
        if (directorio){
            this->soporte->agregarDirectorio(directorio);
            this->getAdministradorRecursos()->analisisInicial(directorio,AdministradorRecursos::ANALISIS_INICIAL);
            portadores = directorio->obtenerCopiaPortadores();
            it = portadores.begin();
            while (it != portadores.end()){
                 ((ArchivoImagen*)*it)->preProcesar();
                    it++;
                }
            directorio->actualizarEspacioLibre();
            this->soporte->agregarEspacioLibre(directorio->getEspacioLibre());
            portadores.clear();

            UI::getInstancia()->imprimir("Directorio agregado al soporte satisfactoriamente");
        }
        else
            UI::getInstancia()->imprimir(UI::DIRECTORIO_INVALIDO);
    }
    else
        UI::getInstancia()->imprimir(UI::DIRECTORIO_YAEXISTE);
}


void Motor::removeDirectory (string ruta ) {
    DirectorioDao* dirDao;
    ImagenDao* imageDao;
    ArchivoImagen* imagen = NULL;
    list<Almacenable*> portadores;
    list<Almacenable*> ::iterator itImage;
    Directorio* dir = this->getSoporte()->obtenerDirectorio(ruta);
    if (dir){
        portadores = dir->obtenerCopiaPortadores();
        dirDao = new DirectorioDao();
        imageDao = new ImagenDao();
        itImage = portadores.begin();
        while(itImage!=portadores.end()){
             imagen = (ArchivoImagen*)*itImage;
             this->administradorRecursos->limpiarInfoAdministrativa(imagen->getId());
             imageDao->baja(imagen);
             itImage++;
        }
        this->soporte->borrarDirectorio(ruta);
        this->soporte->reducirEspacioLibre(dir->getEspacioLibre());
        dirDao->baja(dir);
        UI::getInstancia()->imprimir("Directorio eliminado del soporte exitosamente");
        delete(dirDao);
        delete(imageDao);
    }
    else
        UI::getInstancia()->imprimir(UI::DIRECTORIO_NO_EXISTE);
}

void Motor::showDirectories ( ) {
     list<Almacenable*> lista = this->getSoporte()->obtenerCopiaDirectorios();
     list<Almacenable*> ::iterator it;
     if (!lista.empty()){
          it=lista.begin();
          while(it!=lista.end()){
                 UI::getInstancia()->imprimir(((Directorio*)*it)->getRuta());
                 it++;
          }

     }else
        UI::getInstancia()->imprimir(UI::SIN_DIRECTORIOS);
}



void Motor::addFile (string nombre ) {
    Texto* txt = NULL;
    UI::Error resultadoEjecucion;
    if (!this->getProcesadorTextos()->existeTexto(nombre)){
        txt = this->procesadorTextos->crearTexto(nombre);
        if (txt){
            if (txt->getTamano() > 0){
                resultadoEjecucion =  this->getAdministradorRecursos()->esteganografiar(txt);
                if (resultadoEjecucion == UI::OK){
                    this->procesadorTextos->addTextos(txt);
                    UI::getInstancia()->imprimir("El archivo ha sido ocultado exitosamente");
                }
                else
                    UI::getInstancia()->imprimir(resultadoEjecucion);
            }
            else
                UI::getInstancia()->imprimir(UI::TEXTO_VACIO);
        }
        else
            UI::getInstancia()->imprimir(UI::TEXTO_INVALIDO);
    }
    else
        UI::getInstancia()->imprimir(UI::TEXTO_YAEXISTE);
}


void Motor::getFile (string nombre, string destino ) {
    UI::Error resultadoEjecucion;
    Texto* texto = this->getProcesadorTextos()->obtenerTexto(nombre);
    if(!texto){
         UI::getInstancia()->imprimir(UI::TEXTO_NO_EXISTE);
        }
    else{
        if (texto->esValido()){
            resultadoEjecucion = this->administradorRecursos->desEsteganografiar(texto,destino);
            if (resultadoEjecucion!= UI::OK)
                UI::getInstancia()->imprimir(resultadoEjecucion);
            else
                UI::getInstancia()->imprimir("El archivo se recupero exitosamente");
        }
        else
            UI::getInstancia()->imprimir(UI::FALTA_PORTADOR);
    }
}

void Motor::removeFile (string nombre ) {
    Directorio* dirAnterior = NULL;
    Directorio* dirActual = NULL;
    Portador* portador;
    TextoDao* textoDao;
    ParticionDao* particionDao;
    ImagenDao* portadorDao;
    Particion* particion = NULL;
    list<Almacenable*> particiones;
    list<Almacenable*> ::iterator itPart;
    unsigned int idDir = 0;
    Texto* texto = this->getProcesadorTextos()->obtenerTexto(nombre);
    if (texto){
        particiones = texto->obtenerCopiaParticiones();
        textoDao = new TextoDao();
        particionDao = new ParticionDao();
        portadorDao = new ImagenDao();
        itPart = particiones.begin();
        while(itPart!=particiones.end()){
             particion = (Particion*)*itPart;

             portador = this->soporte->obtenerPortador(particion->getIdImagen());
             if (portador){ // Si el portador aun esta en el soporte, lo actualizo
                 if ((portador->getBytesLibres() + particion->getTamano()) < portador->getEspacioDisponible())
                    portador->setBytesLibres(portador->getBytesLibres() + particion->getTamano());
                 else
                    portador->setBytesLibres(portador->getEspacioDisponible());

                 idDir = ((ArchivoImagen*)portador)->getIdDirectorio();
                 dirActual = this->soporte->obtenerDirectorio(idDir);
                 if (dirActual){
                     if (dirAnterior == NULL){
                        dirAnterior = dirActual;
                     }
                     else if (dirActual->getId()!= dirAnterior->getId()){
                        dirAnterior->actualizarEspacioLibre();
                        dirAnterior = dirActual;
                     }
                }
                portadorDao->modificacion(portador);
             }
             particionDao->baja(particion);
             itPart++;
        }
        if (dirActual)
            dirActual->actualizarEspacioLibre();//Actualiza el último directorio.
        this->soporte->actualizarEspacioLibre();
        this->getProcesadorTextos()->removerTexto(nombre);
        textoDao->baja(texto);
        UI::getInstancia()->imprimir("Archivo eliminado exitosamente");
        delete(portadorDao);
        delete(textoDao);
        delete(particionDao);
    }
    else
        UI::getInstancia()->imprimir(UI::TEXTO_NO_EXISTE);
}


void Motor::showFiles ( ) {
    list<Almacenable*> lista = this->getProcesadorTextos()->obtenerCopiaTextos();
    list<Almacenable*>::iterator it;
    if (!lista.empty()){
        it =lista.begin();
        while(it != lista.end()){
            UI::getInstancia()->imprimir(((Texto*)*it)->getNombre());
            it++;
        }
     }else
        UI::getInstancia()->imprimir(UI::SIN_ARCHIVOS);
}

void Motor::showDirectoryInfo( unsigned int id ){
    Directorio* directorio = this->soporte->obtenerDirectorio(id);
    if (directorio){
        list<Almacenable*> portadores = directorio->obtenerCopiaPortadores();
        list<Almacenable*>::iterator it = portadores.begin();
        cout << endl << endl;
        cout << "Directorio: " << directorio->getId() << " " << directorio->getRuta() << endl;
        while (it != portadores.end()){
            cout << "Id: " << ((ArchivoImagen*)*it)->getId() << endl;
            cout << "Nombre: " << ((ArchivoImagen*)*it)->getNombre() << endl;
            cout << "Directorio: " << ((ArchivoImagen*)*it)->getIdDirectorio() << endl;
            cout << "Tamano: " << ((ArchivoImagen*)*it)->getTamano() << endl;
            cout << "Offset Inicial: " << ((ArchivoImagen*)*it)->getOffsetInicial() << endl;
            cout << "Espacio disponible: " << ((ArchivoImagen*)*it)->getEspacioDisponible() << endl;
            cout << "Espacio libre: " << ((ArchivoImagen*)*it)->getBytesLibres() << endl << endl;
            it++;
        }
    }
    else
        UI::getInstancia()->imprimir("No existe directorio con ese id");
}

void Motor::showFileInfo( unsigned int id ){
    Texto* texto = this->procesadorTextos->obtenerTexto(id);
    if (texto){
        list<Almacenable*> particiones = texto->obtenerCopiaParticiones();
        list<Almacenable*>::iterator it = particiones.begin();
        cout << endl << endl;
        cout << "Texto: " << texto->getNombre() << " #part: " << texto->getNroParticiones() << endl;
        while (it != particiones.end()){
            cout << "Id: " << ((Particion*)*it)->getId() << endl;
            cout << "IdTexto: " << ((Particion*)*it)->getIdTexto() << endl;
            cout << "IdImagen: " << ((Particion*)*it)->getIdImagen() << endl;
            cout << "Orden: " << ((Particion*)*it)->getOrden() << endl;
            cout << "Comprimida: " << (((Particion*)*it)->estaComprimida()? "Si" : "No") << endl;
            cout << "Tamano: " << ((Particion*)*it)->getTamano() << endl;
            cout << "Offset Imagen: " << ((Particion*)*it)->getOffsetImagen() << endl << endl;
            it++;
        }
    }
    else
        UI::getInstancia()->imprimir("No existe texto con ese id");
}


void Motor::showMediumInfo(){
    list<Almacenable*> directorios = this->soporte->obtenerCopiaDirectorios();
    list<Almacenable*>::iterator it = directorios.begin();
    Directorio* dir;
    if (!directorios.empty()){
        cout << endl << "Obteniendo al informacion del soporte..." << endl;
        cout << "Cantidad directorios: " << this->soporte->obtenerCantidadDirectorios() << endl;
        cout << "Capacidad(bytes): " << this->soporte->getEspacioLibre() << endl <<endl;
        cout << "Rendimiento por directorio: " << endl;
    }
    else
        cout << "No hay directorios en el sistema" << endl;
    while (it != directorios.end()){
        dir = (Directorio*)*it;
        cout << dir->getRuta() << " Rendimiento: " <<  dir->calcularRendimiento() <<  "%" << " Uso: " << dir->calcularUso() <<  "%" <<  endl;
        it++;
    }
    cout << endl;
}


void Motor::changePassword (string oldPassword, string newPassword ) {
   PasswordDao* passDao = new PasswordDao();
   vector<unsigned long int> hashOld;
   vector<unsigned long int> hashNew;
   vector<unsigned long int> hashPersistido;
   hashOld = this->getAdministradorRecursos()->getEncriptador()->encriptarPassword(oldPassword);
   hashNew = this->getAdministradorRecursos()->getEncriptador()->encriptarPassword(newPassword);
   passDao->get(this->password);
   hashPersistido = this->password->getHashPassword();

   if ( (hashOld[0] == hashPersistido[0]) &&(hashOld[0] == hashPersistido[0]) ){
       if (newPassword.length()>=6){
            this->password->setHashPassword(hashNew);
            passDao->alta(this->password);
            UI::getInstancia()->imprimir("Se realizo exitosamente el cambio de password");
       }
        else
            UI::getInstancia()->imprimir(UI::PASSWORD_LARGO_INCORRECTO);
        }
    else{
            UI::getInstancia()->imprimir(UI::PASSWORD_INVALIDO);
        }
    delete(passDao);

}


void Motor::quit ( ) {
    UI::getInstancia()->imprimir("Cerrando el sistema correctamente, aguarde un instante por favor...");
    // Terminamos el hilo observador
    if (this->corriendo){
        this->corriendo = false;
        pthread_join(this->hiloObservador, NULL);
    }

    DirectorioDao* dirDao = new DirectorioDao();
    ImagenDao* imageDao = new ImagenDao();
    list<Almacenable*> lista = this->getSoporte()->obtenerCopiaDirectorios();
    list<Almacenable*>::iterator it;
    Directorio* dir = NULL;
    it =lista.begin();
    while(it!=lista.end()){
        dir = (Directorio*)*it;
        imageDao->setAlmacenables(dir->obtenerCopiaPortadores());
        dirDao->alta(dir);
        it++;
    }
    lista.clear();
    delete(dirDao);
    delete(imageDao);

    this->administradorRecursos->actualizarEncadenamiento();
    this->administradorRecursos->reorganizarArchivos(PORCENTAJE_LIBRES);
    this->administradorRecursos->liberar();
    UI::getInstancia()->imprimir("La sesion ha concluido. Gracias por usar el esteganografo del grupo 2.");
}

void Motor::cargarDirectoriosImagenes(){
    Directorio* dir = NULL;
    ArchivoImagen* image = NULL;
    list<Almacenable*> imagenes;
    list<Almacenable*> directorios;
    DirectorioDao* dirDao = new DirectorioDao();
    ImagenDao* imageDao = new ImagenDao();
    dirDao->getAlmacenables(&directorios);//Se cargan la lista con los directorios persistidos.
    imageDao->getAlmacenables(&imagenes);
    list<Almacenable*>::iterator itDir;
    list<Almacenable*>::iterator itIm;
     itDir = directorios.begin();
    while(itDir!=directorios.end()){
        dir = (Directorio*)*itDir;
        itIm =imagenes.begin();
        while(itIm!=imagenes.end()){
               image = (ArchivoImagen*)*itIm;
               if ((image->getIdDirectorio()) == (dir->getId())){
                   dir->agregarPortador(image);
                }
                itIm++;
        }
        dir->actualizarEspacioLibre();
        itDir++;
    }
    this->soporte->setDirectorios(directorios);
    this->soporte->actualizarEspacioLibre();
    delete(dirDao);
    delete(imageDao);

    }

void Motor::cargarTextosParticiones(){
    Texto* texto = NULL;
    list<Almacenable*> textos;
    TextoDao* textoDao = new TextoDao();
    textoDao->getAlmacenables(&textos);
    Particion* particion = NULL;
    list<Almacenable*> particiones;
    ParticionDao* particionDao = new ParticionDao();
    particionDao->getAlmacenables(&particiones);
    list<Almacenable*>::iterator itTexto;
    list<Almacenable*>::iterator itParticion;
    itTexto = textos.begin();
    while(itTexto!=textos.end()){
        texto = (Texto*)*itTexto;
        itParticion =particiones.begin();
        while(itParticion!=particiones.end()){
           particion = (Particion*)*itParticion;
           if ((particion->getIdTexto()) == (texto->getId())){
               texto->agregarParticion(particion);
            }
            itParticion++;
        }
        itTexto++;
    }
    this->getProcesadorTextos()->setTextos(textos);
    textos.clear();
    particiones.clear();
    delete(textoDao);
    delete(particionDao);

    }


void Motor::levantarMotor(){
    this->cargarDirectoriosImagenes();
    this->cargarTextosParticiones();
    UI::getInstancia()->imprimir("");
    UI::getInstancia()->imprimir("Iniciando analisis del sistema de archivos...");
    UI::getInstancia()->imprimir("");
    this->administradorRecursos->analizarIncongruencias();
    UI::getInstancia()->imprimir("");
    UI::getInstancia()->imprimir("El analisis ha concluido satisfactoriamente");
    UI::getInstancia()->imprimir("");

    this->corriendo = true;
    if (pthread_create( &this->hiloObservador, NULL, AdministradorRecursos::observarSoporte, (void*) this) != 0)
        UI::getInstancia()->imprimir(UI::FALLO_CREARHILOOBS);
    else{
        UI::getInstancia()->imprimir("Modulo de analisis del sistema de archivos activado.");
        UI::getInstancia()->imprimir("");
    }
}

bool Motor::estaCorriendo(){
    return this->corriendo;
}


bool Motor::compararConPassActual(string passIngresado){
    vector<unsigned long int> hashoriginal;
    vector<unsigned long int> hashIngresado;
    hashIngresado = this->getAdministradorRecursos()->getEncriptador()->encriptarPassword(passIngresado);
    hashoriginal = this->password->getHashPassword();
    return ( (hashIngresado[0] == hashoriginal[0]) && (hashIngresado[1] == hashoriginal[1]));

    }
