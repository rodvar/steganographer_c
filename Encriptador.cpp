#include "Encriptador.h"

/** CONSTRUCTOR POR DEFECTO,
CON LOS VALORES SUGERIDOS COMO "STANDARDS"
(W=32, R=12, B=16) **/

Encriptador::Encriptador(){
    this->rounds= 12;
    this->tr= 2*((this->rounds)+1);
    this->bK= 16;
    this->w = OperacionesBinarias::longitudDoblePalabra();
    this->setearS();

}

/** @param r : cantidad de iteraciones
    @param b: cantidad de bits de la clave
**/
Encriptador::Encriptador(const unsigned char r, const unsigned char b ){
    this->rounds= r;
    this->tr= 2*((this->rounds)+1);
    this->bK= b;
    this->w = OperacionesBinarias::longitudDoblePalabra();
    this->setearS();
}

Encriptador::~Encriptador()
{

}
/* Devuelve el hash calculado como : sum ((valor acii en la pos i) * i )
 * con i contado desde la izq */
unsigned long int Encriptador::leftHash(std::string src){
    unsigned long int h=0, temp=0;
    for (unsigned int i=0; i<src.length(); i++){
        temp= (int(src.at(i))*(i+1));
        h += temp;}
    return h;
}

/* idem pero contando i desde la derecha*/
unsigned long int Encriptador::rightHash(std::string src){
    unsigned long int h=0, temp=0;
    unsigned long int longitud= src.length();
    for (unsigned int i=0; i<longitud; i++){
        temp= (int(src.at(i))*(longitud - i));
        h += temp;}
    return h;
}

std::vector<unsigned long int> Encriptador::encriptarPassword(std::string pass){
        std::vector<unsigned long int> hashing;
        hashing.clear();
        hashing.push_back(this->leftHash(pass));
        hashing.push_back(this->rightHash(pass));
        if (OperacionesBinarias::longitudDoblePalabra() == 32){ //si la arq es de 64 bits
            hashing.push_back(this->leftHash(pass));
            hashing.push_back(this->rightHash(pass));
            }

        return hashing;
}


void Encriptador::setearS(){

    unsigned char key[16]; //array de clave
    unsigned char valor, r;
    for (r=0 ;r<16 ;r++){
        valor = 54343434 % (255-r);
        key[r]= valor;
        }

    WORD  A, B,  auxA, auxB, u=(this->w/BITS_BYTE); //numero de bytes x palabra

    //c es num de palabras en la clave = techo ((max (1, b))/u)
    unsigned char c= std::max(1,(int)(ceil(8 * this->bK/w)));
    WORD L[c];//vector L apartir del cual se calcula S

    //incializacion de L
    L[c-1]=0;
    unsigned char fin= this->bK , j ;
    int i, k;
    fin -=1;

    for (i=0; i<c; i++)
        L[i]=0;


    for (i=fin ;i>=0; i--)
        L[i/u] = (L[i/u]<< 8 )+ key[i];

    //inicializo S en 0//
    for (i=0 ; i<this->tr; i++)
        S.push_back(0);

    S[0]= Pw32;
    for (i=1; i< this->tr ;i++)
        S[i] = S [i-1]+Qw32;

    unsigned int n;
    if (this->tr>c)
        n= tr;
    else
        n= c;

    int n2 = 0;
    n2= 3*n;
    A=B=0;
    i=j=0;
    for (k=0 ;k<n2; k++){
        auxA= ROTL(S[i]+(A+B),3);
        S[i] = auxA;
        A=auxA;
        auxB= ROTL(L[j]+(A+B),(A+B));
        L[j]= auxB;
        B = auxB;
        i=(i+1)%this->tr;
        j=(j+1)%c;
    }

}



/** FUNCION PARA ENCRIPTAR, RECIBE EN
    @param INPUT: LA ENTRADA QUE TIENE TAMAÑO 2 WORDS
    (EJ. PARA W= 32 --> INPUT ES DE 64 BITS
    Y LA SALIDA(OUTPUT) TAMBIEN TIENE TAMAÑO 2 WORDS

    **/
void Encriptador::encriptar( WORD *input, WORD *output){
    WORD i ,A, B;
    A=input[0]+S[0];
    B=input[1]+S[1];
    for (i=1;i<=this->rounds; i++){
        A = ROTL(A^B,B)+S[2*i];
        B = ROTL(B^A,A)+S[2*i+1];
    }
    output[0] = A;
    output[1] = B;
}

void Encriptador::desencriptar(WORD *input, WORD *output){
    WORD i ,A, B;
    A=input[0];
    B=input[1];

    for (i=this->rounds; i>0; i--){
        B = ROTR(B-S[2*i+1],A)^A;
        A = ROTR(A-S[2*i],B)^B;
    }
  output[1] = B-S[1];
  output[0] = A-S[0];
}

vector<int> Encriptador::encriptarVector(vector<int> input){
    vector<int> res, vEntrada = input ;
    int size = input.size();
    WORD entradaAux[2], salidaAux[2];
    if (OperacionesBinarias::longitudDoblePalabra() == 32){
        //si la cant de elementos del vector no es par, agrego un bloque mas para poder llamar al encriptador,
        //quien trabaja de a 2 bloques
        int  i;
        if (size %2 != 0)
            vEntrada.push_back(relleno); //pongo un valor q me indica q es basura mia de relleno
        for (i=0; i< size ; i+=2)
        {
            //cargo la entrada del encriptador
            entradaAux[0]= (WORD)vEntrada[i];
            entradaAux[1]= (WORD)vEntrada[i+1];
            this->encriptar(entradaAux, salidaAux);
            res.push_back((int)salidaAux[0]);
            res.push_back((int)salidaAux[1]);
        }
        return res;
    }
    else{//Arq 64 bits
        //si la cant de elementos del vector no multipo de 4, agrego elementos de mas para poder llamar al encriptador,
        //quien trabaja de a 2 bloques de long
        int mod= size % 4, semiLong1, semiLong2, sizeFinal;
        int i, i1,i2,i3;

        if (mod != 0)
            for (i =0; i< 4 - mod; i++) // agrego 4-mod elementos
                vEntrada.push_back(relleno); //agrego basura para rellenar

        sizeFinal = vEntrada.size();
        for ( i= 0; i<sizeFinal; i+=4){
            i1 = i+1;
            i2 = i+2;
            i3 = i+3;
            entradaAux[0]= OperacionesBinarias::fusionar(vEntrada[i], vEntrada[i1]);
            entradaAux[1]= OperacionesBinarias::fusionar(vEntrada[i2], vEntrada[i3]);
            this->encriptar(entradaAux, salidaAux);
            //aca tengo q partir cada long en 2 ints
            OperacionesBinarias::desFusionar(salidaAux[0], &semiLong1, &semiLong2);
            res.push_back(semiLong1);
            res.push_back(semiLong2);
            OperacionesBinarias::desFusionar(salidaAux[1], &semiLong1, &semiLong2);
            res.push_back(semiLong1);
            res.push_back(semiLong2);
        }
        return res;
    }
}

vector<int> Encriptador::desencriptarVector(vector<int> input){
    vector<int> res;
    unsigned int size = input.size();
    WORD entradaAux[2], salidaAux[2];
    if (OperacionesBinarias::longitudDoblePalabra() == 32){
        for (unsigned int i=0; i< size ; i+=2)
        {
            //cargo la entrada del desencriptador
            entradaAux[0]= (WORD)input[i];
            entradaAux[1]= (WORD)input[i+1];
            this->desencriptar(entradaAux, salidaAux);
            res.push_back((int)salidaAux[0]);
            res.push_back((int)salidaAux[1]);
        }
        //si el ultimo bloque tenia basura de relleno, la quito
        if (res[size-1] == relleno)
            res.pop_back();
        return res;

    }
    else{//arq 64 bits

        int semiLong1, semiLong2;
        unsigned int i, i1,i2,i3;
        for (i= 0; i<size; i+=4){
            i1 = i+1;
            i2 = i+2;
            i3 = i+3;
            entradaAux[0]= OperacionesBinarias::fusionar(input[i], input[i1]);
            entradaAux[1]= OperacionesBinarias::fusionar(input[i2], input[i3]);
            this->desencriptar(entradaAux, salidaAux);
            //aca tengo q partir cada long en 2 ints
            OperacionesBinarias::desFusionar(salidaAux[0], &semiLong1, &semiLong2);
            res.push_back(semiLong1);
            res.push_back(semiLong2);
            OperacionesBinarias::desFusionar(salidaAux[1], &semiLong1, &semiLong2);
            res.push_back(semiLong1);
            res.push_back(semiLong2);
        }
        //quito la basura de relleno
        for (unsigned int i=size-1; i>size-4; i--){
            if (res[i] == relleno)
                res.pop_back();
        }
        return res;
    }
}

vector<string> Encriptador::vStringSemiBloques(string cadena){
    int longitudCadena = cadena.length();
    vector<string> vSemiBloques;

    static unsigned short int bytesALeer= OperacionesBinarias::longitudDoblePalabra()/BITS_BYTE;

    string semiBloque;
    for (int i= 0; i< longitudCadena; i+=bytesALeer){
        if ((i + bytesALeer) <= longitudCadena -1 ){
                semiBloque= cadena.substr(i, bytesALeer);
                vSemiBloques.push_back(semiBloque);
        }
        else
            {
            // al ultomo bloque que quedo mas corto, le concateno basura
            // para q tenga la misma cant de caracteres que los otros
            semiBloque= cadena.substr(i);
            char agregar= bytesALeer - semiBloque.length();
            semiBloque.append(agregar, basura);
            vSemiBloques.push_back(semiBloque);
            }
    }
 // si la cantidad de elementos de vSemibloques no es par, sgnifica q un semibloque tiene q estar vacio
 // esto es para poder llamar a la funcion q encripta q recibe 2 bloques

    if (vSemiBloques.size()  %2 != 0)
        vSemiBloques.push_back("");
    return vSemiBloques;
}

    /**
     * Funcion que encripta un string.
     * @return string encriptado con longitud multiplo de la cantidad de bytes use la arquitectura de la computadora
     * que corre el sistema, para representar un "unsigned long int".
     * @param cadena Es la cadena a procesar.
     */
string Encriptador::encriptarString(std::string input){
    vector<string> vSemiBloques = vStringSemiBloques(input);
    string salida;

    WORD plainText[2], cipherText[2];
    for (unsigned int i=0; i< vSemiBloques.size(); i+=2){
            plainText[0]= OperacionesBinarias::convertirString(vSemiBloques[i]);
            plainText[1]= OperacionesBinarias::convertirString(vSemiBloques[i+1]);
            this->encriptar(plainText, cipherText);
            salida += OperacionesBinarias::convertirLongAString(cipherText[0]) + OperacionesBinarias::convertirLongAString(cipherText[1]);

    }

    return salida;

}

    /**
     * Funcion que desencripta un string.
     * @return string desencriptado (sacandole la basura)
     * @param cadena Es la cadena a procesar.
     */

string Encriptador::desencriptarString(std::string input){
    vector<string> vSemiBloques = vStringSemiBloques(input);
    string salidaSinBasura, salida ;

    WORD plainText2[2], cipherText[2];
    for (unsigned int i=0; i< vSemiBloques.size(); i+=2){
            cipherText[0]= OperacionesBinarias::convertirString(vSemiBloques[i]);
            cipherText[1]= OperacionesBinarias::convertirString(vSemiBloques[i+1]);
            this->desencriptar(cipherText, plainText2);
            salida += OperacionesBinarias::convertirLongAString(plainText2[0]) + OperacionesBinarias::convertirLongAString(plainText2[1]);
    }

    int indiceBasura = salida.find_first_of(basura, 0);
    if (indiceBasura > -1){ // Clau revisa esto
        string salidaSinBasura = salida.substr(0,indiceBasura);
        return salidaSinBasura;
    }

    else{
        return salida;
    }
}

unsigned int Encriptador::getTamanoSEncriptado(unsigned int size){

    int factor = ((OperacionesBinarias::longitudDoblePalabra() /BITS_BYTE) * 2);
    if (size % factor == 0)
        return size;
    else{
        int resultado = ((size) + (factor - (size % factor)));
        return resultado;
    }
}


unsigned int Encriptador::getTamanoVEncriptado(unsigned int longitud){
    if (OperacionesBinarias::longitudDoblePalabra()==32)
        return (longitud + (longitud % 2));
    else{
        if (longitud %4 ==0)
            return longitud;
        else
            return (longitud + (4 - (longitud % 4)));
    }
}
