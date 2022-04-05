// Autor/a: Nombre y apellidos
#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <iomanip>
#include <time.h>

using namespace std;

const int NumJugadores = 3;
const int NumFichas = 10;
const int IniFichas = 7;
const int MaxFichas = 50;
const int MaxJugadas = NumFichas * 2;

typedef enum { rojo, verde, azul, amarillo, blanco, libre }tColor;

struct tFicha {
	int numero = -1;
	tColor color = libre;
};

typedef tFicha tArrayFichas[MaxFichas];

struct tSoporte {
	int contador = 0;
	tArrayFichas ficha;
};

typedef tSoporte tSoportes[NumJugadores];

typedef tFicha tBolsaFicha[8][NumFichas];

struct tBolsa
{
	tBolsaFicha bolsaFicha;
	int contador = 0;
};

typedef tFicha tJugada[NumFichas + 1];
typedef tJugada tArrayJugadas[MaxJugadas];

struct tTablero
{
	tArrayJugadas jugada;
	int contador = 0;
};

int menu();
void inicializarBolsa(tBolsa& bolsa, int numFichas);
void repartir(tBolsa& bolsa, tSoportes& soportes, int numFichas, int numJugadores, int iniFichas);
tFicha robar(tBolsa& bolsa, int fila, int columna, int numFichas);
void mostrarBolsa(const tBolsa& bolsa, int numFichas);
void mostrarSoporte(const tSoporte& soporte);
bool recorrerBolsa(tBolsa& bolsa, int& fila, int& columna, int numFichas);
string toString(tColor color);
void resuelveCaso();
void ordenarPorColor(tSoporte& soporte, int numFichas);
void ordenarPorNum(tSoporte& soporte, int numFichas);
void obtenerFicha(tBolsa& bolsa, tSoportes& soportes, int fila, int columna, int turno, int numFichas);
void mostrarSeries(const tSoporte& soporte, int numFichas);
void mostrarEscaleras(const tSoporte& soporte, int numFichas);
void eliminarFichas(tSoporte& soporte, const tJugada& jugada);
int buscar(const tJugada& jugada, const tFicha& ficha);
int nuevaJugada(tSoporte& soporte, tJugada& jugada);
bool coloresRepetidos(tJugada& jugada);
bool jugar(tTablero& tablero, tSoporte& soporte);



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

int menu()
{
	int opcion;
	cin >> opcion;
	if (opcion != -1)
	{
		cout << "1: Ordenar por num., 2: Ordenar por color, 3: Sugerir, 4: Poner, 0: Fin >>> ";
		cout << opcion << endl;
	}
	return opcion;
}

void inicializarBolsa(tBolsa& bolsa, int numFichas)//Done
{
	for (int i = 0; i < 8; i++)//Por cada fila
	{
		for (int j = 0; j < numFichas; j++)//Por cada columna
		{
			bolsa.bolsaFicha[i][j].numero = j + 1;//Rellena cada ficha con su número corresponiente

			if (i < 4)//Rellena cada ficha con su color correspondiente.
			{
				bolsa.bolsaFicha[i][j].color = tColor(i);//Si es menor que 4 coge el color de i
			}
			else
			{
				bolsa.bolsaFicha[i][j].color = tColor(i - 4);//Si es mayor que 4 vuelve a rellenar desde el primer color para que haya 2 fichas de cada color con el mismo número
			}
		}
	}
}
tFicha robar(tBolsa& bolsa, int fila, int columna, int numFichas)//Done
{
	bool encontrado = false;
	tFicha ficha;
	ficha.color = libre;//Inicializamos la ficha
	ficha.numero = -1;//Inicializamos la ficha
	if (bolsa.bolsaFicha[fila][columna].numero == -1) //Si la ficha de la posición introducida está libre, entonces busca la siguiente icha a partir de esa posición
	{
		encontrado = recorrerBolsa(bolsa, fila, columna, numFichas);//Busca la ficha

	}
	else
	{
		encontrado = true;//Si encuentra la ficha
	}
	if (!encontrado)//Si no encuentra la ficha comienza a buscar desde el principio
	{
		fila = 0;
		columna = 0;
		encontrado = recorrerBolsa(bolsa, fila, columna, numFichas);//Busca la ficha
	}
	if (encontrado)
	{
		ficha = bolsa.bolsaFicha[fila][columna];//Si finalmente encuentra la ficha, coge la ficha a partir de la posición encontrada
		bolsa.bolsaFicha[fila][columna].color = libre;//Libera la posición donde esta guardada la ficha cogida
		bolsa.bolsaFicha[fila][columna].numero = -1;
	}
	return ficha;
}


bool recorrerBolsa(tBolsa& bolsa, int& fila, int& columna, int numFichas)//Done Busca la posición donde haya una ficha en la bolsa
{
	bool encontrado = false;
	while (!encontrado && fila < 8)//Se repite mientras que no haya encontrado la ficha y mientras que no haya llegado al final del array
	{
		while (!encontrado && columna < numFichas)//Mientras que no haya llegado al final de la columna y no se haya encontrado la ficha
		{
			if (bolsa.bolsaFicha[fila][columna].numero != -1)//Si la posición no está vacía, encuentra la ficha y el bucle se detiene
			{
				encontrado = true;
			}
			if (!encontrado)
			{
				columna++;//Si no la encuentra pasa a la asiguiente posición
			}
		}
		if (!encontrado)
		{
			columna = 0;
			fila++;
		}
	}
	return encontrado;
}

void repartir(tBolsa& bolsa, tSoportes& soportes, int numFichas, int numJugadores, int iniFichas)//Done
{
	int fila, columna;
	for (int i = 0; i < numJugadores; i++)//Una vuelta por cada jugador
	{
		for (int j = 0; j < iniFichas; j++)//Vuelta por cada ficha del soporte
		{
			cin >> fila;//fila
			cin >> columna;//columna
			obtenerFicha(bolsa, soportes, fila, columna, i, numFichas);//Roba la ficha y la pone en el soporte correspondiente
		}
	}
}

void obtenerFicha(tBolsa& bolsa, tSoportes& soportes, int fila, int columna, int turno, int numFichas)//Done
{
	if (soportes[turno].contador < MaxFichas)
	{
		soportes[turno].ficha[soportes[turno].contador] = robar(bolsa, fila, columna, numFichas);
		soportes[turno].contador++;
	}
}
void mostrarFicha(const tFicha& ficha)//Done
{
	cout << toString(ficha.color) << " " << ficha.numero << "  ";
}
void mostrarBolsa(const tBolsa& bolsa, int numFichas)//Done
{
	cout << endl << "Bolsa..." << endl;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < numFichas; j++)
		{
			if (bolsa.bolsaFicha[i][j].color == libre || bolsa.bolsaFicha[i][j].numero == -1)
			{
				cout << "    " << "-1" << "  ";
			}
			else
			{
				mostrarFicha(bolsa.bolsaFicha[i][j]);
			}
		}
		cout << endl;
	}
}

void mostrarSoporte(const tSoporte& soporte)//Done
{
	cout << "Soporte: ";
	for (int i = 0; i < soporte.contador; i++)
	{
		mostrarFicha(soporte.ficha[i]);
	}
	cout << endl << endl;
}
void mostrarJugada(const tJugada& jugada)//Done
{
	int i = 0;
	cout << "Jugada: ";
	while (jugada[i].numero != -1)
	{
		mostrarFicha(jugada[i]);
		i++;
	}
	cout << endl;
}
void mostrarTablero(const tTablero& tablero)//Done
{
	cout << "Tablero:" << endl;
	for (int i = 0; i < tablero.contador; i++)
	{
		cout << i + 1 << ": ";
		mostrarJugada(tablero.jugada[i]);
		cout << endl;
	}
}
void mostrarIndice(int numFichasSoporte)
{
	cout << "       ";
	for (int i = 0; i < numFichasSoporte; i++)
	{
		cout << setw(8) << i + 1;
	}
	cout << endl;
}
string toString(tColor color)
{
	string nombre;
	if (color == rojo)
	{
		nombre = "rojo";
	}
	else if (color == amarillo)
	{
		nombre = "amar";
	}
	else if (color == azul)
	{
		nombre = "azul";
	}
	else if (color == verde)
	{
		nombre = "verd";
	}
	return nombre;
}
void ordenarPorNum(tSoporte& soporte, int numFichas)//Done
{
	int  pos;
	tFicha elemento;
	for (int i = 1; i < soporte.contador; i++)
	{
		pos = i;
		while (pos > 0 && soporte.ficha[pos].numero < soporte.ficha[pos - 1].numero)
		{
			elemento = soporte.ficha[pos];
			soporte.ficha[pos] = soporte.ficha[pos - 1];
			soporte.ficha[pos - 1] = elemento;
			pos--;
		}
		if (pos > 0 && soporte.ficha[pos].color < soporte.ficha[pos - 1].color && soporte.ficha[pos].numero == soporte.ficha[pos - 1].numero)
		{
			while (pos > 0 && soporte.ficha[pos].color < soporte.ficha[pos - 1].color && soporte.ficha[pos].numero == soporte.ficha[pos - 1].numero)
			{
				elemento = soporte.ficha[pos];
				soporte.ficha[pos] = soporte.ficha[pos - 1];
				soporte.ficha[pos - 1] = elemento;
				pos--;
			}
		}
	}
}
void ordenarPorColor(tSoporte& soporte, int numFichas)//Done
{
	int  pos;
	tFicha elemento;
	for (int i = 1; i < soporte.contador; i++)
	{
		pos = i;
		while (pos > 0 && soporte.ficha[pos].color < soporte.ficha[pos - 1].color)
		{
			elemento = soporte.ficha[pos];
			soporte.ficha[pos] = soporte.ficha[pos - 1];
			soporte.ficha[pos - 1] = elemento;
			pos--;
		}
		if (pos > 0 && soporte.ficha[pos].numero < soporte.ficha[pos - 1].numero && soporte.ficha[pos].color == soporte.ficha[pos - 1].color)
		{
			while (pos > 0 && soporte.ficha[pos].numero < soporte.ficha[pos - 1].numero && soporte.ficha[pos].color == soporte.ficha[pos - 1].color)
			{
				elemento = soporte.ficha[pos];
				soporte.ficha[pos] = soporte.ficha[pos - 1];
				soporte.ficha[pos - 1] = elemento;
				pos--;
			}
		}
	}
}
int avanzarTurno(int numJugadores, int turno)
{
	if (turno == numJugadores - 1)
	{
		turno = 0;
	}
	else
	{
		turno++;
	}
	return turno;
}
void mostrarSeries(tSoporte& soporte, int numFichas)//Done
{
	int jugada = 0;
	tSoporte soporteAux;
	bool escalera = true;
	soporteAux = soporte;

	ordenarPorNum(soporte, numFichas);

	for (int i = 0; i < soporte.contador - 2; i++)
	{
		int numiguales = 1;
		escalera = true;

		while (escalera)
		{
			if (soporte.ficha[i].numero == soporte.ficha[i + numiguales].numero && soporte.ficha[i].color != soporte.ficha[i + numiguales].color)
			{
				numiguales++;
			}
			else
			{
				if (numiguales >= 3)
				{
					for (int j = i; j < i + numiguales; j++)
					{
						cout << toString(soporte.ficha[j].color) << " " << soporte.ficha[j].numero << "  ";
					}
					cout << endl;
				}
				escalera = false;
			}
		}
	}
	soporte = soporteAux;
}
void mostrarEscaleras(tSoporte& soporte, int numFichas)//Done
{
	tSoporte aux;
	bool escalera = true;
	ordenarPorColor(soporte, numFichas);
	aux = soporte;

	for (int i = 0; i < soporte.contador - 2; i++)
	{
		int numiguales = 1;
		escalera = true;
		while (escalera)
		{
			if (soporte.ficha[i].color == soporte.ficha[i + numiguales].color && soporte.ficha[i].numero + numiguales == soporte.ficha[i + numiguales].numero)
			{
				numiguales++;
			}
			else
			{
				if (numiguales >= 3)
				{
					for (int j = i; j < i + numiguales; j++)
					{
						mostrarFicha(soporte.ficha[j]);
					}
					cout << endl;
				}
				escalera = false;
			}
		}
	}
	soporte = aux;
}
bool coloresRepetidos(tJugada& jugada)
{
	int i = 0;
	bool repetidos = false;
	while (jugada[i].numero != -1)
	{
		i++;
	}
	for (int j = 0; j < i - 1; j++)
	{
		for (int k = j; k < i - 1; k++)
		{
			if (jugada[k].color == jugada[k + 1].color)
			{
				repetidos = true;
			}
		}
	}
	return repetidos;
}
int nuevaJugada(tSoporte& soporte, tJugada& jugada)
{
	tJugada njugada;
	int num = 1, numFichasJugada = 0, cont = 0, fichasRecorridas = 0;
	bool serie = true, escalera = true, colRepetidos = false,soloUnaFicha = false;
	int es = 0;
	mostrarSoporte(soporte);
	mostrarIndice(soporte.contador);

	cout << endl << "Fichas (0 al final) : ";

	while (num > 0 && num < soporte.contador + 1)
	{
		cin >> num;
		cout << " " << num;
		if (num != 0)
		{
			njugada[numFichasJugada] = soporte.ficha[num - 1];
			mostrarFicha(njugada[numFichasJugada]);
			numFichasJugada++;
		}
	}
	if(numFichasJugada == 1)
	{
		soloUnaFicha = true;
	}
	cout << endl;
	mostrarJugada(njugada);
	colRepetidos = coloresRepetidos(njugada);
	if (colRepetidos)
	{
		serie = false;
	}
	else
	{
		escalera = false;
	}
	if (numFichasJugada >= 3 && !soloUnaFicha)
	{
		if (numFichasJugada < 5) {
			while (fichasRecorridas < (numFichasJugada - 1) && serie)
			{
				if (njugada[fichasRecorridas].numero != njugada[fichasRecorridas + 1].numero)
				{
					serie = false;
				}
				fichasRecorridas++;
			}
		}
		else
		{
			serie = false;
		}
		fichasRecorridas = 0;
		while (fichasRecorridas < (numFichasJugada - 1) && escalera)
		{
			if ((njugada[fichasRecorridas].color != njugada[fichasRecorridas + 1].color) || (njugada[fichasRecorridas].numero + 1 != njugada[fichasRecorridas + 1].numero) /*|| (njugada[fichasRecorridas].numero != njugada[fichasRecorridas + 1].numero + 1))*/)
			{
				escalera = false;
			}
			fichasRecorridas++;
		}

		if (escalera || serie)
		{

			for (int i = 0; i < numFichasJugada; i++)
			{
				jugada[i] = njugada[i];
			}
			eliminarFichas(soporte, jugada);
			if (serie) {
				cout << "  Serie correcta!!";
			}
			else {
				cout << "  Escalera correcta!!";
			}
			cout << endl;
			// Si es = 1 es una serie, si es -1 será una escalera, mientras que 0 sera una jugada invalida;
		}
		else
		{
			cout << " - No es una jugada correcta! Prueba de nuevo..." << endl;
			numFichasJugada = 0;
		}
		// Falta por eliminar los datos que se introducen a jugada si esta no es correcta.
	}
	return numFichasJugada;
}
bool jugar(tTablero& tablero, tSoporte& soporte)
{
	tFicha ficha;
	tJugada jugada;
	int numFichasJugada, numJugadas = 0;
	bool hayJugada = false;
	numFichasJugada = nuevaJugada(soporte, jugada);
	if (numFichasJugada > 1 && tablero.contador < MaxJugadas)
	{
		tablero.jugada[tablero.contador] = jugada;
		tablero.contador++;
		hayJugada = true;
	}
	else if(numFichasJugada == 1 && tablero.contador < MaxJugadas)
	{
		ficha = jugada[0];
		while(numJugadas < tablero.contador && !hayJugada)
		{
			hayJugada = ponerFicha(tablero.jugada[numJugadas], ficha);
			numJugadas++;
		}
	}
	mostrarTablero(tablero);
	return hayJugada;
}
int buscar(const tJugada& jugada, const tFicha& ficha)//Done
{
	bool encontrado = false;
	int vueltas = 0, indice = -1;
	while (!encontrado && vueltas < NumFichas + 1)
	{
		if (jugada[vueltas].color == ficha.color && jugada[vueltas].numero == ficha.numero)
		{
			indice = vueltas;
			encontrado = true;
		}
		vueltas++;
	}
	return indice;
}
void eliminarFichas(tSoporte& soporte, const tJugada& jugada)//Done
{
	for (int i = 0; i < soporte.contador; i++) {

		if (buscar(jugada, soporte.ficha[i]) != -1)
		{
			for (int j = i; j < soporte.contador; j++)
			{
				soporte.ficha[j] = soporte.ficha[j + 1];
			}
			soporte.contador--;
		}
	}
}
bool ponerFicha(tJugada& jugada, tFicha& ficha) 
{

	int numFichas = 0;
	tFicha aux;
	bool paraEscalera = true, paraSerie = true, hayJugada = false, descendente = true, ascendente = true;

	while (jugada[numFichas].numero != -1)//Recorre toda la jugada y cuenta las fichas que tiene ésta
	{
		numFichas++;
	}

	for (int i = 0; i < numFichas; i++)//Recorre toda la jugada
	{
		if(ficha.color == jugada[i].color || ficha.numero != jugada[i].numero)//Si la ficha es del mismo color o no tiene el mismo número no vale para series
		{
			paraSerie = false;
		}
		else//Si la ficha es de diferente color o tiene el mismo número que una de las fichas de la jugada significa que no sirve para escaleras
		{
			paraEscalera = false;
		}
	}
	if(paraSerie)
	{
		hayJugada = true;
	}
	if(paraEscalera)
	{
		for(int i = 0; i < numFichas - 1; i++)
		{
			if(jugada[i].numero + 1 != jugada[i + 1].numero)
			{
				ascendente = false;
			}
			if(jugada[i].numero != jugada[i + 1].numero)
			{
				descendente = false;
			}
		}
		if(ascendente || descendente)
		{
			hayJugada = true;
		}
	}
	return hayJugada;
}
void resuelveCaso()
{
	tBolsa bolsa;
	tSoportes soportes;
	tTablero tablero;
	bool haJugado = false;
	int numFichas, iniFichas, numJugadores, fila, columna, opcion = 0, turno, numJugada = 0;
	cin >> numFichas;
	cin >> iniFichas;
	cin >> numJugadores;

	inicializarBolsa(bolsa, numFichas);//Rellena la bolsa con las fichas iniciales
	mostrarBolsa(bolsa, numFichas);//Muestra la bolsa para comprobar que se ha rellenado efectivamente
	//Inicializa la bolsa

	repartir(bolsa, soportes, numFichas, numJugadores, iniFichas);
	//Reparte las fichas

	cout << endl << "Fichas de la bolsa con todas las fichas repartidas" << endl;
	mostrarBolsa(bolsa, numFichas);//Muestra la bolsa
	//Muestra la bolsa después de repartir las fichas

	cin >> turno;//Recoge el turno del jugador al que le toca jugar
	cout << endl << "Turno para el jugador " << turno << " ..." << endl;
	turno = turno - 1;
	mostrarSoporte(soportes[turno]);//Bucle de jugadas
	do//Mientras que el turno no sea -1 el bucle reproduce todas las jugadas
	{
		opcion = menu();
		if (opcion == 0)
		{
			if (!haJugado)
			{
				cin >> fila;
				cin >> columna;
				obtenerFicha(bolsa, soportes, fila, columna, turno, numFichas);
			}
			haJugado = false;
			mostrarSoporte(soportes[turno]);
			turno = avanzarTurno(numJugadores, turno);
			cout << "Turno para el jugador " << turno + 1 << " ..." << endl;
			mostrarSoporte(soportes[turno]);
		}
		else if (opcion == 1)
		{
			ordenarPorNum(soportes[turno], numFichas);
			mostrarSoporte(soportes[turno]);
		}
		else if (opcion == 2)
		{
			ordenarPorColor(soportes[turno], numFichas);
			mostrarSoporte(soportes[turno]);
		}
		else if (opcion == 3)
		{
			mostrarEscaleras(soportes[turno], numFichas);
			mostrarSeries(soportes[turno], numFichas);
			mostrarSoporte(soportes[turno]);
		}
		else if (opcion == 4)
		{
			haJugado = jugar(tablero, soportes[turno]);
			mostrarSoporte(soportes[turno]);
		}
	} while (opcion != -1);
}