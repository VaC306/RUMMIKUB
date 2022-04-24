// Autor/a: Rishi Pursnani Mirpuri y Alejandro Ramos Yañez
// email: ripursna@ucm.es
// visual studio code
// PR1 rummikub

#include <iostream>
#include <fstream>
#include<iomanip>
// Introduce más librerías si son necesarias
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

void inicializarBolsa(tBolsa& bolsa); //funcion para inicializar, es decir dar los valores iniciales a las fichas en la bolsa

void mostrar(const tBolsa& bolsa); //funcion para mostrar el estado de la bolsa

void mostrarSoportes(const tSoporte& soporte); //funcion para mostrar el estado de el soporte

void repartir(tBolsa& bolsa, tSoportes& soportes); //funcion para repartir Inifichas de la bolsa a cada jugador y las coloca en el soporte

void mostrarFicha(const tFicha& ficha);

tFicha robar(tBolsa& bolsa); //funcion para robar(coger) fichas de la bolsa

void resuelveCaso();

int main() {
	// ajustes para que cin extraiga directamente de un fichero
#ifndef DOMJUDGE
	std::ifstream in("datos.in");
	auto cinbuf = std::cin.rdbuf(in.rdbuf());
	std::ofstream out("datos.out");
	auto coutbuf = std::cout.rdbuf(out.rdbuf());
#endif

	resuelveCaso();

	// para dejar todo como estaba al principio
#ifndef DOMJUDGE
	std::cin.rdbuf(cinbuf);
	std::cout.rdbuf(coutbuf);
	system("PAUSE");
#endif
	return 0;
}


// resuelve el caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
void resuelveCaso() {
    tBolsa bolsa;
    tSoportes soportes;
    inicializarBolsa(bolsa);
    mostrar(bolsa);
    for(int i = 1; i <= NumJugadores;i++)
    {
        repartir(bolsa, soportes);
        mostrar(bolsa);
		mostrarSoportes(soportes.soporte[i]);
    }
}

//funciones
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
			bolsa.matrizBolsa[i][j].numero = i + 1;
			color++;
			if (color == 4)
			{
				color = rojo;
			}
		}
	}
}

void repartir(tBolsa& bolsa, tSoportes& soportes)
{
	std::cout << "Repartir fichas" << "\n";
	soportes.contador = NumJugadores;
	for (int i = 1; i <= NumJugadores; i++)
	{
		for (int j = 0; j < IniFichas; j++)
		{
			soportes.soporte[i].fichas[j] = robar (bolsa);
		}
	}
}

tFicha robar(tBolsa& bolsa)
{
	tFicha ficha;
	int i;
	int j;
	int a = i;
	int b = j;
	int c = 0;
	bool ok = false;
    cin>>i>>j;

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
			ok = true;
		}
		else
		{
			ficha = bolsa.matrizBolsa[a][b];
			bolsa.matrizBolsa[a][b].numero = -1;
			bolsa.matrizBolsa[a][b].color = libre;
			ok = true;
		}

	}
	return ficha;
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
}

void mostrarSoportes(const tSoporte& soportes)
{
	std::cout << "Soporte: ";
	for (int i = 0; i < IniFichas; i++)
	{
		mostrarFicha(soportes.fichas[i]);
	}
	std::cout<<"\n";
}

void mostrarFicha(const tFicha& ficha) {

	if (ficha.color == rojo) {

		cout << " rojo ";

	}
	else if (ficha.color == verde) {

		cout << " verd ";

	}
	else if (ficha.color == azul) {

		cout << " azul ";

	}
	else if (ficha.color == amarillo) {

		cout << " amar ";

	}
	else if (ficha.color == libre) {

		cout << setw(7);
	}

	cout << ficha.numero << " ";

}