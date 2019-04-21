//Declaracion de variables
var tablero=new Array(3);
var juego=new Array(3);
var h1 = document.querySelector("h1");
var message = document.querySelector("#message");
var turn="X";
var continueGame=true;
//Crear los arreglos
for(var i=0; i<3; i++){
	tablero[i]=new Array(3);
	juego[i]=new Array(3);
}
//llenar toda la matriz de juego con espacios
for(var i=0; i<3; i++){
	for(var j=0; j<3; j++){
		juego[i][j]=' ';
	}
}
//Agregar las casillas al arreglo del tablero
function setup(){
	//Selecionar los elementos del dom y meterlos en el tablero
	var cont=0;
	for(let i=0; i<3; i++){
		for(let j=0; j<3; j++){
			var nombreClase="#cuadrante"+cont.toString();
			tablero[i][j]=document.querySelector(nombreClase);
			cont++;
		}
	}
}

//arrancar el setup
setup();
//Funcion para alternar los turnos entre las API de AI
setInterval(function(){ 
	if(continueGame){
		//convertir la jugada actual en JSON para mandar llamar el API
		let matResultados = {"0" : {"0":' ',"1":' ',"2":' '}, "1" : {"0":' ',"1":' ',"2":' '}, "2" : {"0":' ',"1":' ',"2":' '}}
		for(var i=0; i<3 ; i++){
			for(var j=0; j<3; j++){
				var row=i.toString();
				var col=j.toString();
				matResultados[row][col]=juego[i][j];
			}
		}
		matResultados["dif"]="hard";
		//Si el turno es de la X, mandar llamar la segunda API
		if(turn=="X"){
			message.innerHTML="COMPUTER 1 TURN!";
			//llamar la API con el tablero actual que tenemos
			//ajax request
			jQuery.ajax({
			          url: "http://gato.orbi.mx/makePlay2",
			          type: "POST",
			          data: JSON.stringify(matResultados),
			          dataType: "json",
			          beforeSend: function(x) {
			            if (x && x.overrideMimeType) {
			              x.overrideMimeType("application/j-son;charset=UTF-8");
			            }
			          },
			          success: function(result) {
				 	    for(let i=0; i<3; i++){
				 	     	for(let j=0; j<3; j++){
				 	     		juego[i][j]=result[i.toString()][j.toString()];
				 	     		//actulizar el dom
				 	     		tablero[i][j].innerHTML='<div class="contenedor"><p>'+juego[i][j]+'</p></div>';
				 	     		if(juego[i][j]=="X"){
									tablero[i][j].classList.remove("o");
									tablero[i][j].classList.add("x");
								}
								else if(juego[i][j]=="O"){
									tablero[i][j].classList.remove("x");
									tablero[i][j].classList.add("o");
								}
							}
				 	    }
						//Check if someone wins
			          	if((result["status"]=="GAMEOVER")&&(result["winner"]=="N")){
			          		//terminar el juego
			          		//poner el background del h1 en gris
			          		h1.style.backgroundColor = "#232323";
			          		//cambiar el mensaje
			          		message.innerHTML="GAMEOVER!";
			          		continueGame=false;
			          	}
			          	else if(result["winner"]!="N"){
			          		//obtener el jugador y su color para desplegarlo
			          		var winner = result["winner"];
			          		//terminar el juego
			          		//poner el background del h1 en gris
			          		if(winner=="X"){
								h1.style.backgroundColor = "red";
							}
							else{
								h1.style.backgroundColor = "blue";
			          		}
							//cambiar el mensaje
			          		message.innerHTML=winner+" WIN THE GAME!";
			          		continueGame=false;
			          	}
					}
			});
			turn="Y";
		}
		else if(turn=="Y"){
			message.innerHTML="COMPUTER 2 TURN!";
			//ajax request
			jQuery.ajax({
			          url: "http://pi.orbi.mx/makePlay",
			          type: "POST",
			          data: JSON.stringify(matResultados),
			          dataType: "json",
			          beforeSend: function(x) {
			            if (x && x.overrideMimeType) {
			              x.overrideMimeType("application/j-son;charset=UTF-8");
			            }
			          },
			          success: function(result) {
				 	    for(let i=0; i<3; i++){
				 	     	for(let j=0; j<3; j++){
				 	     		juego[i][j]=result[i.toString()][j.toString()];
				 	     		//actulizar el dom
				 	     		tablero[i][j].innerHTML='<div class="contenedor"><p>'+juego[i][j]+'</p></div>';
				 	     		if(juego[i][j]=="X"){
									tablero[i][j].classList.remove("o");
									tablero[i][j].classList.add("x");
								}
								else if(juego[i][j]=="O"){
									tablero[i][j].classList.remove("x");
									tablero[i][j].classList.add("o");
								}
							}
				 	    }
						//Check if someone wins
			          	if((result["status"]=="GAMEOVER")&&(result["winner"]=="N")){
			          		//terminar el juego
			          		//poner el background del h1 en gris
			          		h1.style.backgroundColor = "#232323";
			          		//cambiar el mensaje
			          		message.innerHTML="GAMEOVER!";
			          		continueGame=false;
			          	}
			          	else if(result["winner"]!="N"){
			          		//obtener el jugador y su color para desplegarlo
			          		var winner = result["winner"];
			          		//terminar el juego
			          		//poner el background del h1 en gris
			          		if(winner=="X"){
								h1.style.backgroundColor = "red";
							}
							else{
								h1.style.backgroundColor = "blue";
			          		}
							//cambiar el mensaje
			          		message.innerHTML=winner+" WIN THE GAME!";
			          		continueGame=false;
			          	}
					}
			});
			turn="X";
		}
	}
}, 1000);
