#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <climits>
#include "crow.h"
#include "Tablero.h"
#include <time.h>
//Declaracion de la estructura movimiento
struct Movimiento{
    int row;
    int col;
    int score;
};
//Funcion algoritmo mianMax
int miniMax(Tablero currentTablero, char jugador){
    //Conseguir numero de espacios disponbles
    int numeroEspacios=currentTablero.numberOfEmptySpaces();
    int mayor=INT_MIN;
    int menor=INT_MAX;
    std::vector< std::vector<int> > emptySpaces= currentTablero.emptySpaces();
    std::vector<Movimiento> moves;
    //cout << currentTablero <<endl;
    //Ver si tenemos un ganador
    if(currentTablero.getWiner()=='X'){
            return -10;
    }
    else if(currentTablero.getWiner()=='O'){
            return 10;
    }
    else if(numeroEspacios==0){
            return 0;
    }
    //Ciclo for para recorrer los espacios vacios
    for(int i=0; i<numeroEspacios; i++){
        //hacer el nuevo movimiento
        Tablero cache=currentTablero;
        Movimiento movCache;
        movCache.row=emptySpaces[i][0];
        movCache.col=emptySpaces[i][1];
        cache.makePlay(emptySpaces[i][0],emptySpaces[i][1],jugador);
        //ver a quien le toca jugar
        if(jugador=='O'){
            movCache.score=miniMax(cache, 'X');
        }
        else{
            movCache.score=miniMax(cache,'O');
        }
        moves.push_back(movCache);
    }
    //Ver la mejor movida
    if(jugador=='O'){
        //Regresar el mayor de los movimientos
        for(std::vector<Movimiento>::iterator i=moves.begin();i!=moves.end();i++){
            if(mayor < (*i).score){
                mayor = (*i).score;
            }
        }
        //regresar resultado
        return mayor;
    }
    else{
        //regresar el menor de los movimientos
        for(std::vector<Movimiento>::iterator i=moves.begin();i!=moves.end();i++){
            if(menor > (*i).score){
                menor= (*i).score;
            }
        }
        return menor;
    }
}

//Declaracion funcion de arranque
std::vector<int> getBestMove(Tablero currentTablero){
    //Declaracion de variables
    std::vector< std::vector<int> > emptySpaces= currentTablero.emptySpaces();
    std::vector<int> peso;
    std::vector<int> returnVec;
    int mayor=INT_MIN;
    int indexMayor=0;
    int pesoCache=0;
    //Ciclo for para llenar el tablero con los movimientos posibles
    for(int i=0; i<currentTablero.numberOfEmptySpaces(); i++){
        Tablero newTablero=currentTablero;
        //hacer la jugada
        newTablero.makePlay(emptySpaces[i][0],emptySpaces[i][1],'O');
        //llamar funcion minimax para conseguir el mejor movimiento
        pesoCache=miniMax(newTablero, 'X');
        std::cout<<"Movement [" << emptySpaces[i][0] << "] [" << emptySpaces[i][1] << "] calcualted!! Value: " << pesoCache << std::endl;
        peso.push_back(pesoCache);
    }
    //Ciclo for para conseguir el menor peso
    for(int i=0; i<currentTablero.numberOfEmptySpaces(); i++){
        if(mayor<peso[i]){
            mayor=peso[i];
            indexMayor=i;
        }
    }
    returnVec.push_back(emptySpaces[indexMayor][0]);
    returnVec.push_back(emptySpaces[indexMayor][1]);
    return returnVec;
}

int main()
{
    //Declaracion de variables
    crow::SimpleApp app;
    //Declarar ruta para rec juego y procesarlo
    CROW_ROUTE(app, "/makePlay").methods("POST"_method)([](const crow::request& req){
      crow::json::wvalue answer;
      auto x = crow::json::load(req.body);
      //check if we got something, if we dont get something return error 400
      if (!x)
        return answer;
      //Declaracion de variables
      std::string dif="hard";
      Tablero mainGame;
      std::vector<int> computer;
      std::vector<std::vector<char>> tablero(3, std::vector<char>(3));
      std::cout<<"Getting Game!" <<std::endl;
      //Llenar el vector con el juego que llego del JSON
      //std::cout<<x["valores"]["0"]["0"]<<std::endl;
	    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
          std::string cordRow=std::to_string(i);
          std::string cordCol=std::to_string(j);
          std::string juego= x[cordRow][cordCol].s();
          char play = juego[0];
          tablero[i][j]=play;
        }
      }
      mainGame.setJuego(tablero);
      //Si ya es un gameover no regresar nada
    	if(mainGame.gameOver()&&(mainGame.getWiner()=='N')){
        //construct the JSON with the result and send it back
        std::vector<std::vector<char>> resultadosFinales=mainGame.getJuego();
        for(int i= 0; i<3; i++){
          for(int j=0; j<3; j++){
            std::string row=std::to_string(i);
            std::string col=std::to_string(j);
            char resultado=resultadosFinales[i][j];
            answer[row][col]=std::string(1,resultado);
          }
        }
		    answer["winner"]="N";
    		answer["status"]="GAMEOVER";
    		return answer;
    	}
      else if(mainGame.getWiner()!='N'){
        //construct the JSON with the result and send it back
        std::vector<std::vector<char>> resultadosFinales=mainGame.getJuego();
        for(int i= 0; i<3; i++){
          for(int j=0; j<3; j++){
            std::string row=std::to_string(i);
            std::string col=std::to_string(j);
            char resultado=resultadosFinales[i][j];
            answer[row][col]=std::string(1,resultado);
          }
        }
        char ganador=mainGame.getWiner();
	       answer["winner"]=std::string(1,ganador);
        answer["status"]="NONE";
      	return answer;
	}

	    std::cout<<"Game from remote received!" <<std::endl;
      std::cout<<mainGame;
      //get the dificulty of the game
      dif=x["dif"].s();
      std::cout<<"Generating Computer Move! " <<dif<<" MODE!!!" <<std::endl;
      //generate computer move
      computer = getBestMove(mainGame);
      //check to see if the move is posible
      if(!mainGame.makePlay(computer[0],computer[1],'O')){
        std::cout<<"Internal Error! Invalid Computer Move!" <<std::endl;
        std::cout<<"Terminating Game!" <<std::endl;
        return answer;
      }
      else{
        std::cout<<"Computer Done!" <<std::endl;
        std::cout<<mainGame;
        //construct the JSON with the result and send it back
        std::vector<std::vector<char>> resultadosFinales=mainGame.getJuego();
        for(int i= 0; i<3; i++){
          for(int j=0; j<3; j++){
            std::string row=std::to_string(i);
            std::string col=std::to_string(j);
            char resultado=resultadosFinales[i][j];
	          answer[row][col]=std::string(1,resultado);
          }
        }
        //add the status to the JSON and the winner
        answer["winner"]=std::string(1,mainGame.getWiner());
        answer["status"]=mainGame.gameOver()?"GAMEOVER":"NONE";
        //return the JSON
        return answer;
      }
      return answer;
    });
    //Declarar ruta para rec juego y procesarlo
    CROW_ROUTE(app, "/getWinner").methods("POST"_method)([](const crow::request& req){
      crow::json::wvalue answer;
      auto x = crow::json::load(req.body);
      //check if we got something, if we dont get something return error 400
      if (!x)
        return answer;
      //Declaracion de variables
      Tablero mainGame;
      std::vector<std::vector<char>> tablero(3, std::vector<char>(3));
      std::cout<<"Getting Game!" <<std::endl;
      //Llenar el vector con el juego que llego del JSON
      //std::cout<<x["valores"]["0"]["0"]<<std::endl;
	    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
          std::string cordRow=std::to_string(i);
          std::string cordCol=std::to_string(j);
          std::string juego= x[cordRow][cordCol].s();
          char play = juego[0];
          tablero[i][j]=play;
        }
      }
      mainGame.setJuego(tablero);
      std::cout<<"Calculating Winner!" <<std::endl;
      std::cout<<mainGame;
      //Si ya es un gameover no regresar nada
    	if(mainGame.gameOver()&&(mainGame.getWiner()=='N')){
        //construct the JSON with the result and send it back
		    answer["winner"]="N";
    		answer["status"]="GAMEOVER";
        std::cout<<"GAMEOVER!"<<std::endl;
    		return answer;
    	}
      else if(mainGame.getWiner()!='N'){
        //construct the JSON with the result and send it back
        char ganador=mainGame.getWiner();
	      answer["winner"]=std::string(1,ganador);
        answer["status"]="NONE";
      	return answer;
        std::cout<<"The winner is: " << std::string(1,ganador) << std::endl;
	     }
       else{
         answer["winner"]="N";
         answer["status"]="CONTINUE";
       }
    });
    app.port(18080)
        .multithreaded()
        .run();
}

