#include <iostream>
#include <queue>
#include "zegar.h"
#include "konfig.h"
#include "zgloszenia.h"

int main(){
    Config mojeDane = wczytajConfig("pliki/config.txt");
    Zegar zegar = mojeDane.data == "" ? Zegar(1) : Zegar(mojeDane.mnoznik, mojeDane.data);
    switch(mojeDane.rodzajObsl) {
    case 1:
        {  //FIFO, nie- nie muszę sprawdzać tutaj nawet wykonawców, poniewaz 1. ma być słaby dla kontrastu 2. nie chce mi sie
        queue<Zgloszenie> lista = zgrajZgloszenia("pliki/lista.txt");
        while(!lista.empty()){
            if (lista.front().czasNadejscia < zegar.Getczas()){
                
            }
            zegar.tick();
        }
        }
        break;
        
    case 2:
        break;
    case 3:
        break;
    }
}