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

typedef tFicha tSoporte[maxNumFichas];

typedef tSoporte tListaSoportes[NumJugadores];

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

//cabecera de funciones
int menu(); //funcion que expresa el menú de las opciones

void colorTexto(tColor color); // funcion usada para que el texto se pueda distinquir con colores

void inicializarBolsa(tBolsa& bolsa); //funcion para inicializar, es decir dar los valores iniciales a las fichas en la bolsa

void mostrar(const tBolsa& bolsa); //funcion para mostrar el estado de la bolsa

void repartir(tBolsa& bolsa, tSoportes& soportes); //funcion para repartir Inifichas de la bolsa a cada jugador y las coloca en el soporte

tFicha robar(tBolsa& bolsa); //funcion para robar(coger) fichas de la bolsa

int main()
{
	tBolsa bolsa;
	srand(time(NULL));
	//int opcionMenu = menu();

	inicializarBolsa(bolsa);
	mostrar(bolsa);

	return 0;
}



//funciones
int menu()
{
	int num;
	std::cin >> num;
	std::cout << "Ordenar fichas por numeracion";
	std::cout << "Ordenar fichas por colores y numero en cada color";
	std::cout << "Ver ugerencias de series y/o escaleras que formar con sus fichas";
	std::cout << "Poner fichas en el tablero";
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
		break;
	case azul:
		std::cout << "\033[40;34m";
		break;
	case rojo:
		std::cout << "\033[40;31m";
		break;
	case verde:
		std::cout << "\033[40;32m";
		break;
	case blanco:
		std::cout << "\033[40;37m";
		break;
	}
}

void mostrar(const tBolsa& bolsa)
{
	for (int j = 0; j < NumFichas; j++)
	{
		for (int i = 0; i < NumFichas; i++)
		{
			colorTexto(bolsa.matrizBolsa[i][j].color);
			std::cout << bolsa.matrizBolsa[j][i].numero << "   ";
		}
		std::cout << "\n";
	}
}

//void repartir(tBolsa& bolsa, tSoportes &soportes)

tFicha robar(tBolsa& bolsa)
{
	tFicha ficha;
	int i = rand() % NumFichas;
	int j = rand() % NumFichas;
	int x = NumFichas;
	int y = NumFichas;
	if ((bolsa.matrizBolsa[i][j].numero != -1) && (bolsa.matrizBolsa[i][j].color != libre))
	{
		ficha.numero = bolsa.matrizBolsa[i][j].numero;
		ficha.color = bolsa.matrizBolsa[i][j].color;
		bolsa.matrizBolsa[i][j].numero = -1;
		bolsa.matrizBolsa[i][j].color = libre;
	}
	else
	{
		for (int a = i; a < x; a++)
		{
			for (int b = j; b < y; b++)
			{
				if ((bolsa.matrizBolsa[a][b].numero != -1) && (bolsa.matrizBolsa[a][b].color != libre))
				{
					ficha.numero = bolsa.matrizBolsa[a][b].numero;
					ficha.color = bolsa.matrizBolsa[a][b].color;
					bolsa.matrizBolsa[a][b].numero = -1;
					bolsa.matrizBolsa[a][b].color = libre;
				}
				else if(a == NumFichas && b == NumFichas)
				{
					a = 0;
					b = 0;
					x = i;
					y = j;
				}
				else if (a == i && b == j)
				{
					ficha.numero = -1;
				}
			}
		}
		
	}

	return ficha;
}

//void buscarFichaLibre(tBolsa& Bolsa)
//{
//	for (int i = 0; i < NumFichas; i++)
//	{
//		for (int j = 0; j < NumFichas; j++)
//		{
//
//		}
//	}
//}

void repartir(tBolsa& bolsa, tSoportes& soportes)
{
	
	for (int i = 0; i < NumJugadores; i++)
	{
		for (int j = 0; j < IniFichas; j++)
		{
			soportes.soporte[i].numero = robar(bolsa);
		}
	}
}
