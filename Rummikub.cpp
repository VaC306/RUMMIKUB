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

typedef tFicha tSoporteFichas[maxNumFichas];

typedef tSoporteFichas tListaSoportes[NumJugadores];

typedef tFicha Bolsa[NumFichas][NumFichas];

typedef tFicha tJugada[NumFichas + 1];

typedef tFicha tListaJugadas[MaxJugadas];

struct tTablero
{
	tListaJugadas Jugadas;
	int contador = 0;
};

struct tBolsa
{
	Bolsa matrizBolsa;
	int contador = 0;
};

struct tSoportes
{
	tListaSoportes soporte;
	int contador = 0;
};

struct tSoporte 
{
    tSoporteFichas fichas;
	int contador = 0;
};

//cabecera de funciones
int menu(); //funcion que expresa el menú de las opciones

void colorTexto(tColor color); // funcion usada para que el texto se pueda distinquir con colores

void inicializarBolsa(tBolsa& bolsa); //funcion para inicializar, es decir dar los valores iniciales a las fichas en la bolsa

void mostrar(const tBolsa& bolsa); //funcion para mostrar el estado de la bolsa

void mostrarSoportes(const tSoportes& soporte); //funcion para mostrar el estado de el soporte

void repartir(tBolsa& bolsa, tSoportes& soportes); //funcion para repartir Inifichas de la bolsa a cada jugador y las coloca en el soporte

void ordenarPorNum(tSoporte& soporte);

bool colores(tFicha soporte, tFicha ficha); 

tFicha robar(tBolsa& bolsa); //funcion para robar(coger) fichas de la bolsa

int main()
{
	tBolsa bolsa;
	tSoportes soportes;
	tSoporte soporte;
	int turno;
	srand(time(NULL));
	//int opcionMenu = menu();

	inicializarBolsa(bolsa);
	mostrar(bolsa);
	repartir(bolsa,soportes);
	mostrarSoportes(soportes);
	mostrar(bolsa);
	ordenarPorNum(soporte);
	mostrarSoportes(soportes);
	// std::cin>>turno;
	// int option = 0;
	// while(option != -1)
	// {
	// 	std::cout<<"turno del jugador"<<turno<<"\n";

	// 	mostrarSoportes(soportes.soporte[turno]);
	// }

	return 0;
}



//funciones
int menu()
{
	int num;
    cin >> num;
    if (num != -1) {

        cout << "1: Ordenar por num, 2: Ordenar por color, 3: Sugerir, 4: Poner, 0: Fin >>>  " << num;
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
	cout<<"  ";
}

void mostrar(const tBolsa& bolsa)
{
	std::cout<<"Bolsa..."<<"\n";
	for (int j = 0; j < NumFichas; j++)
	{
		for (int i = 0; i < NumFichas; i++)
		{
			colorTexto(bolsa.matrizBolsa[i][j].color);
			std::cout <<setw(2)<< bolsa.matrizBolsa[j][i].numero;
		}
		std::cout << "\n";
	}
	std::cout<<"\n";
}

void mostrarSoportes(const tSoportes& soportes)
{
	std::cout<<"Soportes..."<<"\n";
	for(int i = 0; i < NumJugadores; i++)
	{
		for(int j = 0; j < IniFichas;j++)
		{
			colorTexto(soportes.soporte[i][j].color);
			std::cout<<soportes.soporte[i][j].numero <<setw(8);

		}
		std::cout<<"\n";
	}
}

tFicha robar(tBolsa& bolsa)
{
	tFicha ficha;
	int i = rand() % NumFichas;
	int j = rand() % NumFichas;
	int a = i;
	int b = j;
	int x = NumFichas;
	int y = NumFichas;
	bool ok = false;
	bool fin = false;


// 	while (!ok) {
//         if (bolsa.ficha[a][b].num == -1) {
//             if (b == bolsa.NumFichas - 1 && x == 7) {

//                 fin = true;
//                 a = 0;
//                 b = 0;
//             }
//             else if (b == bolsa.NumFichas - 1) {

//                 a++;
//                 b = 0;
//             }
//             else {

//                 b++;
//             }
//         }

//         else if (a == filaINI && b == columnaINI && fin) {

//             ok = true;
//             ficha.color = libre;
//             ficha.numero = -1;
//         }

//         else {

//             ok = true;
//             ficha = bolsa.ficha[fila][columna];

//             bolsa.matrizBolsa[fila][columna].color = libre;
//             bolsa.matrizBolsa[fila][columna].num = -1;

//         }
//     }

//     return sol;
// }


	if ((bolsa.matrizBolsa[i][j].numero == -1) && (bolsa.matrizBolsa[i][j].color == libre))
	{
		int a = i;
		int b = j;
		while(!ok){
				if ((bolsa.matrizBolsa[a][b].numero != -1) && (bolsa.matrizBolsa[a][b].color != libre))
				{
					ficha.numero = bolsa.matrizBolsa[a][b].numero;
					ficha.color = bolsa.matrizBolsa[a][b].color;
					bolsa.matrizBolsa[a][b].numero = -1;
					bolsa.matrizBolsa[a][b].color = libre;
					ok = true;
				}
				if(a == NumFichas && b == NumFichas)
				{
					a = 0;
					b = 0;
					x = i;
					y = j;
					ok = true;
				}
				if(a == i && b == j)
				{
					ficha.numero = -1;
					ok = true;
				}
				bolsa.contador++;
				a++;
				b++;
 		}
			
		
	}
	else
	{
		ficha.numero = bolsa.matrizBolsa[i][j].numero;
		ficha.color = bolsa.matrizBolsa[i][j].color;
		bolsa.matrizBolsa[i][j].numero = -1;
		bolsa.matrizBolsa[i][j].color = libre;
	}
	return ficha;
}

void repartir(tBolsa& bolsa, tSoportes& soportes)
{
	std::cout<<"Repartir fichas";
	for (int i = 0; i < NumJugadores; i++)
	{
		for (int j = 0; j < IniFichas; j++)
		{
			soportes.soporte[i][j] = robar(bolsa);
		}
	}
}

// int buscar(const tJugada& jugada, const tFicha& ficha){
// int i= 0, indice = -1;
// bool encontrada =false;
// while ( encontrada != true && i< NumFichas +1){
//      if (jugada[i].color = ficha.color && jugada[i].numero = ficha.numero){
//           indice = i;
//      }
//      i++;
// }
// return indice;
// };

// void eliminaFichas(tSoportes& soporte, const tJugada& jugada){
//     for( int i=0;i<soporte.contador;i++){
//         int pos;
//         if  (buscar(jugada, soporte.fichas[i])!= -1){
//             soporte.fichas[i].color = libre;
//             soporte.fichas[i].num = -1;
//         }
//     };
// }

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

	if(soporte.color < ficha.color)
	{
		mayor = true;
	}
	else if(soporte.color == ficha.color && soporte.numero < ficha.numero)
	{
		mayor = true;
	}

	return mayor;
}
