#include <iostream>
#include <queue>
#include <ctime>
#include "zegar.h"
#include "konfig.h"
#include "zgloszenia.h"
string getCzas(const int& sekundy, const Zegar zegar) {
    time_t czas_t = static_cast<std::time_t>(zegar.Getczas() + sekundy);
    tm* data = localtime(&czas_t);
    char bufor[20];
    strftime(bufor, sizeof(bufor), "%H:%M %d.%m.%Y", data);
    return string(bufor);
}
void wylicz(int& czasTicka, queue<Zgloszenie>& lista, const Zegar zegar){//ale jestem śmieszny ha ha
    if(lista.front().czasRealizacji < czasTicka){
    czasTicka -= lista.front().czasRealizacji;
    cout << "Przesyłka o id: " << lista.front().id << " została nadana o " << getCzas(lista.front().czasRealizacji, zegar) << endl;
    lista.pop();
    wylicz(czasTicka, lista, zegar);
    }
    else lista.front().czasNadejscia -= czasTicka;
}
int main(){
    Config mojeDane = wczytajConfig("pliki/config.txt");
    Zegar zegar = mojeDane.data == "" ? Zegar(1) : Zegar(mojeDane.mnoznik, mojeDane.data);
    switch(mojeDane.rodzajObsl) {
    case 1:
        {  //FIFO, nie- nie muszę sprawdzać tutaj nawet wykonawców, poniewaz 1. ma być słaby dla kontrastu 2. nie chce mi sie
        queue<Zgloszenie> lista = zgrajZgloszenia("pliki/lista.txt");
        while(!lista.empty()){
            if (lista.front().czasNadejscia < zegar.Getczas()){
                wylicz(mojeDane.mnoznik, lista, zegar);
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
    cout << "Wszystko wysłane";
}
