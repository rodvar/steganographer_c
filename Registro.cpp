#include "Registro.h"

using namespace std;

Registro::Registro(std::vector<Campo> campos){
      unsigned int pos = 0;
      Campo campo = campos[pos];

      while( campo.getNombre() != "" && pos<= (unsigned int)campos.size()-1){

          this->campos.push_back(campo);
          pos++;
          if(pos <= (unsigned int)campos.size()-1){
            campo = campos[pos];
          }

      }
      this->numCampos = pos;
}

void Registro::cambiarEstado(char estado){
    this->estado = estado;
}

Campo Registro::getCampo(std::string nombre){
    bool encontrado = false;
    int pos = 0;
    Campo campo = campos[pos];
    if(campo.getNombre() == nombre)
        encontrado = true;
    while (pos <= this->numCampos && !encontrado){
        if (campo.getNombre() == nombre )
            encontrado = true;
        else{
            pos++;
            if (pos <=  this->numCampos)
                campo = this->campos[pos];
            }
        }

     return campo;
}

Campo Registro::getCampo(int indice){
    Campo campo;
    if(indice <= this->numCampos) campo = campos[indice];
    return campo;
}

std::vector<int> Registro::getCampos(){
    vector<int> v;
    for(unsigned int i=0;i<campos.size();i++) {
        v.push_back(campos[i].getValor());
    }
    return v;
    }

unsigned int Registro::getNumCampos(){
    return this->numCampos;
    }

unsigned int Registro::getSizeCampos(){
    unsigned int size=0;
    for(unsigned int i=0; i< this->campos.size(); i++)
        size += sizeof(campos[i].getValor());
    return size + sizeof(this->estado);
    }

char Registro::getEstado() {
    return this->estado;
    }

void Registro::setValorCampo(std::string nombre, int valor){
    bool encontrado = false;
    int pos = 0;
    Campo campo = campos[pos];
    if(campo.getNombre() == nombre)
        encontrado = true;
    while (pos <= this->numCampos && !encontrado){
        if (campo.getNombre() == nombre )
            encontrado = true;
        else{
            pos++;
            if (pos <=  this->numCampos)
                campo = this->campos[pos];
            }
        }
    campos[pos].setValor(valor);
    }

Registro::~Registro(){

    }
