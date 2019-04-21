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
//Funcion para calcular si tenemos un ganador
function getWinner(){
	//mandar el tablero actual en Json al API para que nos conteste con el ganador
	//convertir la jugada actual en JSON para mandar llamar el API
	let matResultados = {"0" : {"0":' ',"1":' ',"2":' '}, "1" : {"0":' ',"1":' ',"2":' '}, "2" : {"0":' ',"1":' ',"2":' '}}
	for(var i=0; i<3 ; i++){
		for(var j=0; j<3; j++){
			var row=i.toString();
			var col=j.toString();
			matResultados[row][col]=juego[i][j];
		}
	}
	//ajax request
	jQuery.ajax({
	          url: "http://gato.orbi.mx/getWinner",
	          type: "POST",
	          data: JSON.stringify(matResultados),
	          dataType: "json",
	          beforeSend: function(x) {
	            if (x && x.overrideMimeType) {
	              x.overrideMimeType("application/j-son;charset=UTF-8");
	            }
	          },
	          success: function(result) {
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
}
//Agregar las casillas al arreglo del tablero
function setup(){
	//Selecionar los elementos del dom y meterlos en el tablero
	message.innerHTML="X TURN!"
	var cont=0;
	for(let i=0; i<3; i++){
		for(let j=0; j<3; j++){
			var nombreClase="#cuadrante"+cont.toString();
			tablero[i][j]=document.querySelector(nombreClase);
			tablero[i][j].addEventListener("click",function(){
				//Ver si el juego continua
				if(continueGame){
					//Ver si es turno de la X o de la Y
					if(turn=="X"){
						//ver si el movimiento es posible
						if(juego[i][j]==" "){
							//hacer el juego
							juego[i][j]="X";
							//marcarlo en el DOM
							tablero[i][j].innerHTML='<div class="contenedor"><p>X</p></div>';
							tablero[i][j].classList.add("x");
							//cambiar el turno a la Y
							turn="O";
							message.innerHTML="O TURN!";
							//calculamos el ganador
							getWinner();
						}
					}
					else if(turn=="O"){
						if(juego[i][j]==" "){
							juego[i][j]="O";
							//marcarlo en el DOM
							tablero[i][j].innerHTML='<div class="contenedor"><p>O</p></div>';
							tablero[i][j].classList.add("o");
							//cambiar el turno a la X
							turn="X";
							message.innerHTML="X TURN!";
							//calculamos el ganador
							getWinner();
						}
					}
				}
			});
			cont++;
		}
	}
}
//arrancar el setup
setup();
