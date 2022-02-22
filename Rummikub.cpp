//Rishi Pursnani Mirpuri y Alejandro Ramos Yañez
//practica Rummikub
#include<iostream>
#include<fstream>
#include<string>

//constantes globales
const int NumJugadores = 2;
const int NumFichas = 10;
const int IniFichas = 13;
const int maxNumFichas = 50;
const int MaxJugadas = NumFichas * 2;

typedef enum tColor { rojo, verde, azul, amarillo, blanco, libre};

typedef struct tFicha
{
	int numero;
	tColor color;
};

typedef tFicha tSoporte[maxNumFichas];

typedef tSoporte tSoportes[NumJugadores];

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



//cabecera de funciones
int menu();

void colorTexto(tColor color);

void inicializarBolsa(tBolsa& bolsa);

tFicha robar(tBolsa& bolsa);

int main()
{
	//int opcionMenu = menu();



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
	for (int i = 0; i < (NumFichas); i++)
	{
		for (int j = 0; j < (NumFichas); j++)
		{
			if(color == 0)
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
				color = 0;
			}
			bolsa.matrizBolsa[i][j].numero = j;
			color++;
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

tFicha robar(tBolsa& bolsa)
{

}
