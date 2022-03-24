
#include <iostream>

using namespace std;

int main()
{}
int buscar(const tJugada & jugada, const tFicha & ficha){

int i= 0, indice = -1;
bool encontrada =false;


while ( encontrada != true && i< numfichas +1){
     if (jugada[i].color = ficha.color && jugada[i].num = ficha.num){
          indice = i;
        
     }
     i++;
   
}
return indice;

};
 
void eliminaFichas(tSoporte & soporte, const tJugada & jugada){
    for( int i=0;i<soporte.contador;i++){
        int pos
        if  (buscar( jugada, soporte.fichas[i])!= -1){
            soporte.fichas[i].color = libre;
            soporte.fichas[i].num = -1;
        }



    };
}



int nuevaJugada(tSoporte & soporte, tJugada & jugada):



