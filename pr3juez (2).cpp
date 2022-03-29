#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>

using namespace std;

const int MAX_JUGADORES = 4;
const int MIN_JUGADORES = 2;

const int MAX_NUM_FICHAS = 13;
const int MIN_NUM_FICHAS = 8;

const int MAX_INI_FICHAS = 14;
const int MIN_INI_FICHAS = 6;

const int NUM_FICHAS = 50;

enum tColor { rojo, verde, azul, amarillo, blanco, libre };

struct tFicha {

    int num = -1;
    tColor color = libre;
};

typedef tFicha tArrayFichasJugador[NUM_FICHAS];

struct tSoporte {

    int contadorFichas = 0;
    tArrayFichasJugador fichas;
};

typedef tSoporte tArraySoporte[MAX_JUGADORES];

struct tSoportes {

    int contadorJugadores = 0;
    tArraySoporte soporte;
};

typedef tFicha tArrayBolsa[8][NUM_FICHAS];

struct tBolsa {

    int NUM_FICHAS = 0;
    tArrayBolsa ficha;
};

typedef tFicha tJugada[NUM_FICHAS + 1];

typedef tJugada tArrayJugadas[NUM_FICHAS * 2];

struct tTablero {

    int contador = 0;
    tArrayJugadas jugadas;
};

    void inicializarBolsa(tBolsa& bolsa);
    void repartir(tBolsa& bolsa, tSoportes& soportes);
    void mostrarSoporte(const tSoporte& soporte);
    tFicha robar(tBolsa& bolsa);
    void mostrarBolsa(const tBolsa& bolsa);
    void mostrarFicha(const tFicha& ficha);
    void resuelveCaso();
    void ordenarPorColor(tSoporte& soporte);
    void ordenarPorNum(tSoporte& soporte);
    bool compararColor(tFicha soporte, tFicha elemento);
    bool jugar(tTablero& tablero, tSoporte& soporte);
    void eliminaFichas(tSoporte & soporte, const tJugada & jugada);
    int buscar(const tJugada & jugada, const tFicha & ficha);
    int menu();
    void mostrarJugada(const tJugada & jugada);
    void mostrarSeries ( tSoporte & soporte);
    void mostrarEscaleras ( tSoporte & soporte);

void resuelveCaso() {

    tBolsa bolsa;
    tSoportes soportes;
    tJugada jugada;
    tTablero tablero;
    bool fichaPuesta=false;

    inicializarBolsa(bolsa);
    mostrarBolsa(bolsa);
    repartir(bolsa, soportes);

  
    int turno, opcion=0;

    cin >> turno;

    while (opcion != -1) {

        cout << endl <<"Turno para el jugador " << turno << " ...";

        mostrarSoporte(soportes.soporte[turno]);
        opcion = menu();

        

        while (opcion !=0 and opcion !=-1) {



            switch (opcion) {
                case 1: ordenarPorNum(soportes.soporte[turno]);
                        mostrarSoporte(soportes.soporte[turno]);
                break;
                case 2: ordenarPorColor(soportes.soporte[turno]);
                        mostrarSoporte(soportes.soporte[turno]);
                break;
                case 3: mostrarSeries(soportes.soporte[turno]);
                        mostrarEscaleras(soportes.soporte[turno]);
                break;
                case 4: fichaPuesta=jugar(tablero, soportes.soporte[turno]);
                break;
            }

            opcion = menu();

        }

        if (opcion == 0 and fichaPuesta==false) {

            soportes.soporte[turno].fichas[soportes.soporte[turno].contadorFichas] = robar(bolsa);
            soportes.soporte[turno].contadorFichas++;
            mostrarSoporte(soportes.soporte[turno]);
        }

        if (turno == soportes.contadorJugadores) {

            turno = 1;
        }

        else {

            turno++;
        }
        cout << endl;
        fichaPuesta=false;
    }

}



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
}



void inicializarBolsa(tBolsa& bolsa) {
    int num_fichas;

    cin >> num_fichas;
    bolsa.NUM_FICHAS = num_fichas;
    for (int i = 0; i < 8; i++) {

        for (int j = 0; j < num_fichas; j++) {

            bolsa.ficha[i][j].color = tColor(i % (4));
            bolsa.ficha[i][j].num = j + 1;
        }
    }
}

void repartir(tBolsa& bolsa, tSoportes& soportes) {
    int num_jugadores, ini_fichas;

    cin >> ini_fichas >> num_jugadores;
    soportes.contadorJugadores = num_jugadores;

    cout <<endl<< "Fichas de la bolsa con todas las fichas repartidas" << endl<< endl;

    //se reparten a num_jugadores jugadores
    for (int i = 1; i <= num_jugadores; i++) {

        //se reparten ini_fichas a cada jugador
        for (int j = 0; j < ini_fichas; j++) {

            //pasa la ficha de la bolsa al ultimo hueco del soporte del jugador
            soportes.soporte[i].fichas[soportes.soporte[i].contadorFichas] = robar(bolsa);

            //se marca que se ha a�adido una ficha mas al soporte
            soportes.soporte[i].contadorFichas++;

        }
    

    }

    mostrarBolsa(bolsa);

}

tFicha robar(tBolsa& bolsa) {
    int fila, columna, filaINI, columnaINI;
    bool encontrada = false, final = false;
    tFicha sol;

    cin >> filaINI >> columnaINI;

    fila = filaINI;
    columna = columnaINI;

    //recorre la bolsa hasta q encuentre una ficha
    while (!encontrada) {

        //la ficha ya ha sido pillada
        if (bolsa.ficha[fila][columna].num == -1) {

            //llega al final de la bolsa
            if (columna == bolsa.NUM_FICHAS - 1 and fila == 7) {

                final = true;
                fila = 0;
                columna = 0;
            }
            //llega al final de una fila
            else if (columna == bolsa.NUM_FICHAS - 1) {

                fila++;
                columna = 0;
            }

            else {

                columna++;
            }
        }

        //se ha dado una vuelta por completo y no se ha encontrado una ficha
        else if (fila == filaINI and columna == columnaINI and final) {

            encontrada = true;
            sol.color = libre;
            sol.num = -1;
        }

        //la ficha esta disponible por lo q se acaba el bucle
        else {

            encontrada = true;
            sol = bolsa.ficha[fila][columna];

            bolsa.ficha[fila][columna].color = libre;
            bolsa.ficha[fila][columna].num = -1;

        }
    }

    return sol;
}

void mostrarBolsa(const tBolsa& bolsa) {

    cout <<  "Bolsa..." << endl;
    for (int i = 0; i < 8; i++) {

        for (int j = 0; j < bolsa.NUM_FICHAS; j++) {

            mostrarFicha(bolsa.ficha[i][j]);
        }

        cout << endl;
    }
}

void mostrarSoporte(const tSoporte& soporte) {

    cout << endl;
    cout << "Soporte:";

    for (int i = 0; i < soporte.contadorFichas; i++) {
        
    
        mostrarFicha(soporte.fichas[i]);

    }
    cout << endl;
}

void mostrarJugada(const tJugada & jugada){

    int contador=0;
    while(jugada[contador].num!=-1){

        mostrarFicha(jugada[contador]);

        contador++;
    }
}

void mostrarFicha(const tFicha& ficha) {

    if (ficha.color == rojo) {

        cout << " rojo " ;

    }
    else if (ficha.color == verde) {

        cout << " verd " ;

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

    cout << ficha.num << " ";

}

void mostrarSeries (tSoporte & soporte){

    tJugada jugada;
    int contadorJugada=0;

    ordenarPorNum(soporte);

    for(int i=0; i<soporte.contadorFichas; i++){

        //se ha llegado al numero max de fichas en una jugada o simplemente no hay suficientes fichas de ese numero para hacer una jugada
        if(soporte.fichas[i].num!=jugada[contadorJugada-1].num or contadorJugada==4 ){

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

    mostrarSoporte(soporte);

}

void mostrarEscaleras ( tSoporte & soporte){

    tJugada jugada;
    int contadorJugada=0;
    bool creciente=false, decreciente=false, escalera=true;

    ordenarPorColor(soporte);

    for (int i=0; i<soporte.contadorFichas; i++){

        if(soporte.fichas[i].color==jugada[contadorJugada-1].color){
            
            if(!creciente and !decreciente){
                
                if(soporte.fichas[i].num==jugada[contadorJugada-1].num+1){

                    jugada[contadorJugada]=soporte.fichas[i];
                    contadorJugada++;
                    creciente=true;
                }
                else if(soporte.fichas[i].num==jugada[contadorJugada-1].num+1){

                    jugada[contadorJugada]=soporte.fichas[i];
                    contadorJugada++;
                    decreciente=true;

                }
            }
            else if(creciente and soporte.fichas[i].num==jugada[contadorJugada-1].num+1 or (decreciente and soporte.fichas[i].num==jugada[contadorJugada-1].num-1)){

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

int menu() {

    int opcion;
    cin >> opcion;
    if (opcion != -1) {

        cout << endl << "1: Ordenar por num., 2: Ordenar por color, 3: Sugerir, 4: Poner, 0: Fin >>> ";
        
        cout << "  " << opcion;
    }

   

    return opcion;
}

int buscar(const tJugada & jugada, const tFicha & ficha){

    int contador=0, indice;
    bool final=false;

    while(!final){

        if(jugada[contador].num==-1){

            final=true;
            indice=-1;
        }

        else if(jugada[contador].color==ficha.color and jugada[contador].num==ficha.num){

            final=true;
            indice=contador;
        }
        else{

            contador++;
        }
    }

    return indice;

}

void eliminaFichas(tSoporte & soporte, const tJugada & jugada){
    int contador=0;
    bool finalArray=false;

    while(!finalArray){

        if(soporte.fichas[contador].num==-1){

            finalArray=true;
        }

        else if(buscar(jugada, soporte.fichas[contador])==-1){

            contador++;
        }

        else{

            soporte.fichas[contador].num=-1;
            soporte.fichas[contador].color=libre;
            contador++;
        }
    }

    for (int i = 0; i <= soporte.contadorFichas; i++) {

        if (soporte.fichas[i].num == -1) {

            soporte.contadorFichas--;

            for (int j = i; j < soporte.contadorFichas; j++) {

                soporte.fichas[j] = soporte.fichas[j + 1];

            }

            i--;
        }
    }
}

void ordenarPorNum(tSoporte& soporte) {

    int pos;
    tFicha elemento;

    for (int i = 1; i < soporte.contadorFichas; i++) {

        elemento = soporte.fichas[i];
        pos = 0;

        while (pos < i and (soporte.fichas[pos].num < elemento.num or (soporte.fichas[pos].num == elemento.num and compararColor(soporte.fichas[pos],elemento)))) {

         pos++;
        }

        for (int j = i; j > pos; j--) {

            soporte.fichas[j] = soporte.fichas[j - 1];
        }

        soporte.fichas[pos] = elemento;
    }
}

void ordenarPorColor(tSoporte& soporte) {

    int pos;
    tFicha elemento;

    for (int i = 1; i < soporte.contadorFichas; i++) {

        elemento = soporte.fichas[i];
        pos = 0;

        //en caso de que sea mayor el color o igual color y mayor numero compararColor sera true
        while (pos < i and compararColor(soporte.fichas[pos], elemento)) {

            pos++;
        }


        for (int j = i; j > pos; j--) {

            soporte.fichas[j] = soporte.fichas[j - 1];
        }

        soporte.fichas[pos] = elemento;
    }
}

bool compararColor(tFicha soporte, tFicha elemento) {
    bool mayor = false;

    //el color es mayor
    if (soporte.color < elemento.color) {

        mayor = true;
    }
    //el color es igual pero mayor numero
    else if (soporte.color == elemento.color and soporte.num < elemento.num) {

        mayor = true;
    }

    return mayor;
}

int nuevaJugada(tSoporte & soporte, tJugada & jugada){

    int numfichas=0, fichaSoporte, contadorSerie=0, contadorEscalera=0, restaPrevia;
    
    mostrarSoporte(soporte);
    cout << "              "; 
    for(int i=1; i<=soporte.contadorFichas; i++){

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

        while(jugada[contadorSerie].num==jugada[contadorSerie+1].num and jugada[contadorSerie].color != jugada[contadorSerie + 1].color){

            contadorSerie++;
        }
        //escalera hacerla viendo si comparten la resta+ actualizar ponerficha

        restaPrevia=jugada[0].num-jugada[1].num;

        while(jugada[contadorEscalera].color==jugada[contadorEscalera+1].color and restaPrevia==jugada[contadorEscalera].num-jugada[contadorEscalera+1].num){

            restaPrevia=jugada[contadorEscalera].num-jugada[contadorEscalera+1].num;
            contadorEscalera++;
        }

        if(contadorEscalera!=numfichas-1 and contadorSerie!=numfichas-1){

            numfichas=-1;   
        }

        return numfichas;
    

    }

    return numfichas;
}

bool ponerFicha(tJugada & jugada, tFicha & ficha){

    int contadorJugada=0;
    bool serie=true, exito=false, ascendente=false;

    //cuenta cuantas fichas hay en la jugada
    while(jugada[contadorJugada].num!=-1){
        
        contadorJugada++;
    }

    if (contadorJugada==0){

        exito=true;
    }

    else{

        //mira a ver si es una serie o una escalera
        if(jugada[0].num!=jugada[1].num){

            serie=false;
        }

        //es una serie, por lo q si tiene el mismo numero q el resto la jugada es valida
        if(serie and jugada[0].num==ficha.num){

            jugada[contadorJugada]=ficha;
            exito=true;
        }

        //es una escalera por lo que la comparamos con la ultima y la primera ficha
        if (!serie and jugada[0].color==ficha.color){
        
            //en caso de q sea ascendnte
            if(jugada[0].num<jugada[1].num){

                ascendente=true;
            }


            //la ficha se mete al final
            if((jugada[contadorJugada-1].num+1==ficha.num and ascendente) or (jugada[contadorJugada-1].num-1==ficha.num and !ascendente)){

                jugada[contadorJugada]=ficha;
                exito=true;

            }

            //la ficha se mete al principio
            else if((jugada[0].num-1==ficha.num and ascendente) or (jugada[0].num+1==ficha.num and !ascendente)){

                for(int i=0; i<contadorJugada; i++){

                    jugada[i]=jugada[i+1];
                }
                jugada[0]=ficha;
                exito=true;
            }
        }

    }

    return exito;
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

        if(jugada[0].num==jugada[1].num){

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

    mostrarSoporte(soporte);
    return exito;
}


