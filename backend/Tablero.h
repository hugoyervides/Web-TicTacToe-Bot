#ifndef TABLERO_H_INCLUDED
#define TABLERO_H_INCLUDED
#include <iostream>
#include <vector>

class Tablero{
  private:
    std::vector< std::vector<char> > juego;
  public:
    //Constructores
    Tablero();
    Tablero(std::vector< std::vector<char> >,bool,char);
    //metodos set
    void setJuego(std::vector< std::vector<char> >);
    //metodos get
    std::vector< std::vector<char> > getJuego();
    //metodos especiales
    bool makePlay(int reg,int col,char player);
    char getWiner();
    bool gameOver();
    bool isMovePosible(int reg,int col);
    int numberOfEmptySpaces();
    std::vector< std::vector<int> > emptySpaces();
    //sobrecarga operadores
    friend std::ostream& operator <<(std::ostream &os, Tablero tab);
};
  //Constructores
  Tablero::Tablero(){
    //llenar el vector con 0
    std::vector<char> cache;
    for(int i = 0 ; i < 3; i++){
      cache.push_back(' ');
    }
    for(int i=0; i<3; i++){
      juego.push_back(cache);
    }
  }
  Tablero::Tablero(std::vector< std::vector<char> > juego,bool completado,char ganador){
    this->juego=juego;
  }
  //metodos set
  void Tablero::setJuego(std::vector< std::vector<char> > juego){
    this->juego=juego;
  }
  //metodos get
  std::vector< std::vector<char> > Tablero::getJuego(){
    return juego;
  }
  bool Tablero::isMovePosible(int reg,int col){
    if(juego[reg][col] == ' '){
      return true;
    }
    else{
      return false;
    }
  }
  //metodos especiales
  bool Tablero::makePlay(int reg,int col,char player){
    //ver si no esta ocupado ya
    if(juego[reg][col]==' '){
      juego[reg][col]=player;
      return true;
    }
    else{
      return false;
    }
  }
  bool Tablero::gameOver(){
    //Ciclo for para recorrer todo, si tenemos un espacio regresar false
    for(int i=0; i<3; i++){
      for(int j=0; j<3; j++){
        if(juego[i][j]==' '){
          return false;
        }
      }
    }
    return true;
  }
  char Tablero::getWiner(){
    //ver si el juego esta completo, de lo contrario regresar una N
      for(int g=0; g<2; g++){
        char check=(g==0)?'X':'O';
        //prmer caso primero
        for(int i=0; i<3; i++){
          if(juego[i][0] != check){
            i=3;
          }
          else if(i==2){
            return check;
            g=2;
          }
        }
        //primer caso medio
        for(int i=0; i<3; i++){
          if(juego[i][1] != check){
            i=3;
          }
          else if(i==2){
            return check;
            g=2;
          }
        }
        //primer caso final
        for(int i=0; i<3; i++){
          if(juego[i][2] != check){
            i=3;
          }
          else if(i==2){
            return check;
            g=2;
          }
        }
        //segundo caso primero
        for(int i=0; i<3; i++){
          if(juego[0][i] != check){
            i=3;
          }
          else if(i==2){
            return check;
            g=2;
          }
        }
        //segundo caso medio
        for(int i=0; i<3; i++){
          if(juego[1][i] != check){
            i=3;
          }
          else if(i==2){
            return check;
            g=2;
          }
        }
        //segundo caso final
        for(int i=0; i<3; i++){
          if(juego[2][i] != check){
            i=3;
          }
          else if(i==2){
            return check;
            g=2;
          }
        }
        //tercer caso
        for(int i=0; i<3; i++){
          if(juego[i][i] != check){
            i=3;
          }
          else if(i==2){
            return check;
            g=2;
          }
        }
        //cuarto caso
        int aux=2;
        for(int i=0; i<3; i++){
          if(juego[i][aux] != check){
            i=3;
          }
          else if(i==2){
            return check;
            g=2;
          }
          aux--;
        }
      }
      return 'N';
  }
  //Sobrecarga del metodo numberOfEmptySpaces
  int Tablero::numberOfEmptySpaces(){
    //Declaracion de variables
    int contador=0;
    //Ciclo for para recorrer la matriz en busca de espacios solos
    for(int reg=0; reg<3; reg++){
        for(int col=0; col<3; col++){
            //Ver si es un espacio en blanco
            if(juego[reg][col]==' '){
                contador++;
            }
        }
    }
    //regresar el contador
    return contador;
  }
  //Sobrecarga del metodo emptySpaces
  std::vector< std::vector<int> > Tablero::emptySpaces(){
      std::vector< std::vector<int> > returnVector;
    //For para ver espacios vacios y meter las cordenadas en el vector
    for(int reg=0; reg<3; reg++){
        for(int col=0; col<3; col++){
            //Ver si esta vacio
            if(juego[reg][col]==' '){
                std::vector<int> cache;
                cache.push_back(reg);
                cache.push_back(col);
                returnVector.push_back(cache);
            }
        }
    }
    //regresar el vector
    return returnVector;
  }
  //Sobrecarga del operador <<
  std::ostream& operator <<(std::ostream &os, Tablero tab){
      //ciclo for para desplegarlo
      for(int reg=0; reg<3; reg++){
        os<<'|';
        for(int col=0; col<3; col++){
          os<<tab.juego[reg][col] <<'|';
        }
        os<<std::endl;
        os<<"------";
        os<<std::endl;
      }
      return os;
  }
#endif // TABLERO_H_INCLUDED

