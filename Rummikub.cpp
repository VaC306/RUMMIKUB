//Rishi Pursnani Mirpuri y Alejandro Ramos Yañez
//practica Rummikub
#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>
#include<stdlib.h>

using namespace std;

//constantes globales
const int NumJugadores = 2;
const int NumFichas = 8;
const int IniFichas = 6;
const int maxNumFichas = 50;
const int MaxJugadas = NumFichas * 2;

enum tColor { rojo, verde, azul, amarillo, blanco, libre };

struct tFicha
{
	int numero = -1;
	tColor color;
};

typedef tFicha tSoporteFichas[maxNumFichas]; // array de fichas de soporte

struct tSoporte
{
	tSoporteFichas fichas;
	int contador = 0;
};

typedef tSoporte tListaSoportes[NumJugadores]; // array de lista de soportes

struct tSoportes
{
	tListaSoportes soporte;
	int contador = 0;
};

typedef tFicha Bolsa[NumFichas][NumFichas]; // array de bolsa

struct tBolsa
{
	Bolsa matrizBolsa;
	int contador = 0;
};

typedef tFicha tJugada[maxNumFichas + 1];

typedef tJugada tListaJugadas[MaxJugadas];

struct tTablero
{
	tListaJugadas Jugadas;
	int contador = 0;
};

//cabecera de funciones
int menu(); //funcion que expresa el menú de las opciones

int nuevaJugada(tSoporte & soporte, tJugada & jugada);

void colorTexto(tColor color); // funcion usada para que el texto se pueda distinquir con colores

void inicializarBolsa(tBolsa& bolsa); //funcion para inicializar, es decir dar los valores iniciales a las fichas en la bolsa

void mostrar(const tBolsa& bolsa); //funcion para mostrar el estado de la bolsa

void mostrarSoportes(const tSoporte& soporte); //funcion para mostrar el estado de el soporte

void repartir(tBolsa& bolsa, tSoportes& soportes); //funcion para repartir Inifichas de la bolsa a cada jugador y las coloca en el soporte

void ordenarPorNum(tSoporte& soporte);

void mostrarFicha(const tFicha& ficha);

void ordenarPorColor(tSoporte& soporte);

void mostrarSeries(tSoporte & soporte);

void mostrarEscaleras(tSoporte &soporte);

void mostrarJugada(const tJugada &jugada);

bool jugar(tTablero& tablero, tSoporte& soporte);

bool colores(tFicha soporte, tFicha ficha);

tFicha robar(tBolsa& bolsa); //funcion para robar(coger) fichas de la bolsa

int main()
{
	tBolsa bolsa;
	tSoportes soportes;
	tTablero tablero;
	int option = 0;
	bool fichaPuesta = false;
	int turno = rand() % (NumJugadores + 1);

	inicializarBolsa(bolsa);
	repartir(bolsa, soportes);
	std::cout<<"\n";
	while(option != -1)
	{
		std::cout<<"Turno del jugador: "<<turno<<"\n";
		mostrarSoportes(soportes.soporte[turno]);
		do
		{
			option = menu();
			if(option == 1)
			{
				ordenarPorNum(soportes.soporte[turno]);
				mostrarSoportes(soportes.soporte[turno]);
			}
			else if(option == 2)
			{
				ordenarPorColor(soportes.soporte[turno]);
				mostrarSoportes(soportes.soporte[turno]);
			}
			else if(option == 3)
			{
				mostrarSeries(soportes.soporte[turno]);
				mostrarEscaleras(soportes.soporte[turno]);
			}
			else if(option == 4)
			{
				fichaPuesta = jugar(tablero, soportes.soporte[turno]);
			}
		} while(option != 0 && option != -1);

		if(option == 0 && !fichaPuesta)
		{
			soportes.soporte[turno].fichas[soportes.soporte[turno].contador] = robar(bolsa);
			soportes.soporte[turno].contador++;
			mostrarSoportes(soportes.soporte[turno]);
		}
		if(turno == soportes.contador)
		{
			turno = 1;
		}
		else
		{
			turno++;
		}
		cout<<"\n";
		fichaPuesta = false;
	}

	return 0;
}

//funciones
int menu()
{
	int num;
	cout << "\n"<<"1: Ordenar por num, 2: Ordenar por color, 3: Sugerir, 4: Poner, 0: Fin >>>  ";
	cin >> num;
	while(num < 0 || num > 4)
	{
		std::cout<<"EL VALOR QUE HA INTRODUCIDO ES INCORRECTO, VUELVA A INTENTARLO:";
		std::cin>>num;
	}
	return num;
}

void inicializarBolsa(tBolsa& bolsa)
{
	int color = 0;
	for (int i = 0; i < NumFichas; i++)
	{
		for (int j = 0; j < NumFichas; j++)
		{
			if (color == 0)
			{
				bolsa.matrizBolsa[i][j].color = rojo;
			}
			else if (color == 1)
			{
				bolsa.matrizBolsa[i][j].color = verde;
			}
			else if (color == 2)
			{
				bolsa.matrizBolsa[i][j].color = azul;
			}
			else if (color == 3)
			{
				bolsa.matrizBolsa[i][j].color = amarillo;
			}
			bolsa.matrizBolsa[i][j].numero = j + 1;
			color++;
			if (color == 4)
			{
				color = rojo;
			}
		}
	}
}

void colorTexto(tColor color) {
	switch (color) {
	case amarillo:
		std::cout << "\033[1;40;33m";
		//std::cout<<"amar"; 
		break;
	case azul:
		std::cout << "\033[40;34m";
		//std::cout<<"azul";
		break;
	case rojo:
		std::cout << "\033[40;31m";
		//std::cout<<"rojo";
		break;
	case verde:
		std::cout << "\033[40;32m";
		//std::cout<<"verd";
		break;
	case blanco:
		std::cout << "\033[40;37m";
		break;
	}
	cout << "  ";
}

void mostrar(const tBolsa& bolsa)
{
	std::cout << "Bolsa..." << "\n";
	for (int j = 0; j < NumFichas; j++)
	{
		for (int i = 0; i < NumFichas; i++)
		{
			mostrarFicha(bolsa.matrizBolsa[i][j]);
		}
		std::cout << "\n";
	}
	std::cout << "\n";
	colorTexto(blanco);
}

void mostrarSoportes(const tSoporte& soportes)
{
	std::cout << "Soporte:  ";

	for (int i = 0; i < IniFichas; i++)
	{
		mostrarFicha(soportes.fichas[i]);
	}
	std::cout << "\n";
	colorTexto(blanco);
}


void mostrarFicha(const tFicha& ficha) {

	if (ficha.color == rojo) {

		//cout << " rojo ";
		std::cout << "\033[40;31m";

	}
	else if (ficha.color == verde) {

		//cout << " verd ";
		std::cout << "\033[40;32m";

	}
	else if (ficha.color == azul) {

		//cout << " azul ";
		std::cout << "\033[40;34m";

	}
	else if (ficha.color == amarillo) {

		//cout << " amar ";
		std::cout << "\033[1;40;33m";

	}
	else if (ficha.color == libre) {
		std::cout << "\033[40;37m";
		//cout << setw(7);
	}
	cout << ficha.numero << "    ";
}

tFicha robar(tBolsa& bolsa)
{
	tFicha ficha;
	int i = rand() % NumFichas;
	int j = rand() % NumFichas;
	int a = i;
	int b = j;
	int c = 0;
	bool ok = false;


	while (!ok)
	{
		if (bolsa.matrizBolsa[a][b].numero == -1)
		{
			if (a == (NumFichas - 1) && b == (NumFichas - 1))
			{
				a = 0;
				b = 0;
				c++;
			}
			else if (b == (NumFichas - 1))
			{
				b = 0;
				a++;
			}
			else
			{
				b++;
			}
		}
		else if ((a == i && b == j) && c > 0)
		{
			ficha.numero = -1;
			ficha.color = libre;
		}
		else
		{
			ok = true;
			ficha.numero = bolsa.matrizBolsa[a][b].numero;
			ficha.color = bolsa.matrizBolsa[a][b].color;
			bolsa.matrizBolsa[a][b].numero = -1;
			bolsa.matrizBolsa[a][b].color = libre;
		}

	}
	return ficha;
}

void repartir(tBolsa& bolsa, tSoportes& soportes)
{
	soportes.contador = NumJugadores;
	for (int i = 1; i <= NumJugadores; i++)
	{
		for (int j = 0; j < IniFichas; j++)
		{
			soportes.soporte[i].fichas[j] = robar (bolsa);
		}
	}
}

int buscar(const tJugada& jugada, const tFicha& ficha){
int i= 0, indice = -1;
bool ok =false;
while ( !ok && i < NumFichas + 1){
     if (jugada[i].color == ficha.color && jugada[i].numero == ficha.numero){
          indice = i;
     }
     i++;
}
return indice;
};

void eliminaFichas(tSoporte& soporte, const tJugada& jugada){
    for( int i=0;i<soporte.contador;i++){
        if  (buscar(jugada, soporte.fichas[i])!= -1){
            for(int j = i; i < soporte.contador; j++)
			{
				soporte.fichas[j] = soporte.fichas[j + 1];
			}
			soporte.contador--;
        }
    };
}

void ordenarPorNum(tSoporte& soporte)

{
	int posicion;
	tFicha ficha;

	for (int i = 0; i < IniFichas - 1; i++) {
		for (int j = IniFichas - 1; j > i; j--) {
			if (soporte.fichas[j].numero < soporte.fichas[j - 1].numero) {
				int aux;
				aux = soporte.fichas[j].numero;
				soporte.fichas[j].numero = soporte.fichas[j - 1].numero;
				soporte.fichas[j - 1].numero = aux;
			}
		}
	}
}

bool colores(tFicha soporte, tFicha ficha)
{
	bool mayor = false;

	if (soporte.color < ficha.color)
	{
		mayor = true;
	}
	else if (soporte.color == ficha.color && soporte.numero < ficha.numero)
	{
		mayor = true;
	}

	return mayor;
}

void ordenarPorColor(tSoporte &soporte)
{
	int posicion;
	tFicha ficha;
	bool ok = false;


	for(int i = 1; i < IniFichas; i++)
	{
		ficha = soporte.fichas[i];
		posicion = 0;

		while(posicion < i && colores(soporte.fichas[posicion], ficha))
		{
			posicion++;
		}
		for (int j = i; j > posicion; j--) {

            soporte.fichas[j] = soporte.fichas[j - 1];
        }

        soporte.fichas[posicion] = ficha;
	}	
}

// void mostrarSeries(tSoporte &soporte){
//   tJugada jugada;
//     int j=0;
// 	bool ok = false;
//     ordenarPorNum(soporte);

//     for(int i=0; i < soporte.contador; i++)
// 	{
//        while(!ok)
// 	   {
// 		   if((soporte.fichas[i].numero == soporte.fichas[i+1].numero) && (soporte.fichas[i].color != soporte.fichas[i + 1].color))
// 	   		{
//          		j++ ;
// 	  		}
// 			else
// 			{
// 					if(j >= 3)
// 					{
// 						for(int a = i; a < i + j;a++)
// 						{
// 							std::cout<<to_string(soporte.fichas[i].color)<<" "<<soporte.fichas[j].numero<<" ";
// 						}
// 						std::cout<<"\n";
// 					}
					
// 				ok = true;
// 			}
// 	   }
// 	}
// }

// void mostrarSeries(tSoporte & soporte){
//   tJugada jugada;
//     int j=0;

//     ordenarPorNum(soporte);

//     for(int i=0; i<soporte.contador; i++){
       
//        if(soporte.fichas[i].numero == soporte.fichas[i+1].numero && soporte.fichas[i].color != soporte.fichas[i+1].color)
// 	   {  
// 		   i++;
// 	   }
//     	if (soporte.fichas[i] == jugada[j-1])
// 		{
//             soporte.fichas[i]=jugada[j-1];
//             j++;
//         }
//         if (soporte.fichas[i] != jugada[j])
// 		{
//             j=0;
//             soporte.fichas[i]=jugada[j];
//             j++;
//         }

//     	if(j>3)
// 		{
// 		mostrarJugada(jugada);
// 		}
// }

void mostrarSeries (tSoporte & soporte){

    tJugada jugada;
    int contadorJugada=0;

    ordenarPorNum(soporte);

    for(int i=0; i<soporte.contador; i++){

        //se ha llegado al numero max de fichas en una jugada o simplemente no hay suficientes fichas de ese numero para hacer una jugada
        if(soporte.fichas[i].numero!=jugada[contadorJugada-1].numero || contadorJugada==4 ){

            if(contadorJugada>=3){
                cout << endl;
                mostrarJugada(jugada);
            }

            contadorJugada=1;
            jugada[0]=soporte.fichas[i];
        }
        //la ficha tiene el mismo numero asi q lo añado a la jugada
        else{

            jugada[contadorJugada]=soporte.fichas[i];
            contadorJugada++;

        }

    }

    mostrarSoportes(soporte);

}

// void mostrarEscaleras(tSoporte &soporte)
// {
// 	tJugada jugada;
//     int j=0;

// 	ordenarPorColor(soporte);

// 	for(int i=0; i < soporte.contador; i++)
// 	{
//        if(soporte.fichas[i].numero == soporte.fichas[i+1].numero)
// 	   {
//         soporte.fichas[i] = jugada[j];
//         soporte.fichas[i+1] = jugada[j+1];
//         j++ ;
// 	   }
// 	}

// 	if(j>3)
// 	{
// 		mostrarJugada(jugada);
// 	}
// }

void mostrarEscaleras ( tSoporte & soporte){

    tJugada jugada;
    int contadorJugada=0;
    bool creciente=false, decreciente=false, escalera=true;

    ordenarPorColor(soporte);

    for (int i=0; i<soporte.contador; i++){

        if(soporte.fichas[i].color==jugada[contadorJugada-1].color){
            
            if(!creciente && !decreciente){
                
                if(soporte.fichas[i].numero==jugada[contadorJugada-1].numero+1){

                    jugada[contadorJugada]=soporte.fichas[i];
                    contadorJugada++;
                    creciente=true;
                }
                else if(soporte.fichas[i].numero==jugada[contadorJugada-1].numero+1){

                    jugada[contadorJugada]=soporte.fichas[i];
                    contadorJugada++;
                    decreciente=true;

                }
            }
            else if(creciente && soporte.fichas[i].numero==jugada[contadorJugada-1].numero+1 || (decreciente && soporte.fichas[i].numero==jugada[contadorJugada-1].numero-1)){

                jugada[contadorJugada]=soporte.fichas[i];
                contadorJugada++;
            }
            else{

                escalera=false;
            }
        }
        else{

            escalera=false;
        }

        if(!escalera){

            if(contadorJugada>=3){

                mostrarJugada(jugada);
            }

            contadorJugada=1;
            jugada[0]=soporte.fichas[i];
            decreciente=false, creciente=false;

        }

        escalera=true;
    }

}

void mostrarJugada(const tJugada &jugada){

    int contador=0;
    while(jugada[contador].numero!=-1){

        mostrarFicha(jugada[contador]);

        contador++;
    }
}

bool jugar(tTablero& tablero, tSoporte& soporte){

    tJugada jugada;
    int numfichas;
    bool exito=true;

    numfichas= nuevaJugada(soporte, jugada);

    cout << endl<< "Jugada:";
    mostrarJugada(jugada);

    if(numfichas>2){

        eliminaFichas(soporte, jugada);

        if(jugada[0].numero==jugada[1].numero){

            cout << "   - Serie correcta!" << endl;
        }
        else{
            
            cout << "   - Escalera correcta!" << endl;
        }

        exito=true;
    }

    else{

        cout << "   - No es una jugada correcta! Prueba de nuevo... ";
        exito=false;
    }

    mostrarSoportes(soporte);
    return exito;
}

int nuevaJugada(tSoporte & soporte, tJugada & jugada){

    int numfichas=0, fichaSoporte, contadorSerie=0, contadorEscalera=0, restaPrevia;
    
    mostrarSoportes(soporte);
    cout << "              "; 
    for(int i=1; i<=soporte.contador; i++){

        cout << i<< setw(8);
    }
    cout <<endl <<endl<< "    Fichas (0 al final):  ";
    cin >> fichaSoporte;
    cout << fichaSoporte<< " ";

    while(fichaSoporte!=0){

        jugada[numfichas]=soporte.fichas[fichaSoporte-1];
        numfichas++;

        cin >> fichaSoporte;
        cout << fichaSoporte<< " ";

    }

    if(numfichas>=3){

        while(jugada[contadorSerie].numero==jugada[contadorSerie+1].numero && jugada[contadorSerie].color != jugada[contadorSerie + 1].color){

            contadorSerie++;
        }
        //escalera hacerla viendo si comparten la resta+ actualizar ponerficha

        restaPrevia=jugada[0].numero-jugada[1].numero;

        while(jugada[contadorEscalera].color==jugada[contadorEscalera+1].color && restaPrevia==jugada[contadorEscalera].numero-jugada[contadorEscalera+1].numero){

            restaPrevia=jugada[contadorEscalera].numero-jugada[contadorEscalera+1].numero;
            contadorEscalera++;
        }

        if(contadorEscalera!=numfichas-1 && contadorSerie!=numfichas-1){

            numfichas=-1;   
        }

        return numfichas;
    

    }

    return numfichas;
}
