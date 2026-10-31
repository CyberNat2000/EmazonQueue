#include <iostream>
#include <queue>
#include <vector>
#include <fstream>
#include <ctime>
#include "zegar.h"
#include "konfig.h"
#include "zgloszenia.h"
#include "pracownicy.h"
string getCzas(const int& sekundy, const Zegar zegar) {
    time_t czas_t = static_cast<std::time_t>(zegar.getczas() + (zegar.getmnoznik()-sekundy));
    tm* data = localtime(&czas_t);
    char bufor[20];
    strftime(bufor, sizeof(bufor), "%H:%M %d.%m.%Y", data);
    return string(bufor);
}
void wylicz(int& czasTicka, queue<Zgloszenie>& lista, const Zegar zegar, ostream& wynik){//ale jestem śmieszny ha ha
    if(lista.empty())return;
    if(lista.front().czasRealizacji < czasTicka){
    czasTicka -= lista.front().czasRealizacji;
    wynik << "Przesyłka o id: " << lista.front().id << " została nadana o " << getCzas(czasTicka, zegar) << endl;
    cout << "Przesyłka o id: " << lista.front().id << " została nadana o " << getCzas(czasTicka, zegar) << endl;
    lista.pop();
    wylicz(czasTicka, lista, zegar, wynik);
    }
    else lista.front().czasRealizacji -= czasTicka;
}
queue<Zgloszenie> sortuj(queue<Zgloszenie> zgloszenia) {
    queue<Zgloszenie> wynik;
    while (!zgloszenia.empty()) {
        Zgloszenie z = zgloszenia.front();
        zgloszenia.pop();
        queue<Zgloszenie> temp;
        while (!wynik.empty() && wynik.front().deadline <= z.deadline) {
            temp.push(wynik.front());
            wynik.pop();
        }
        temp.push(z);
        while (!wynik.empty()) {
            temp.push(wynik.front());
            wynik.pop();
        }
        wynik = temp;
    }
    return wynik;
}
int wybierz(int& kat, queue<Zgloszenie> zgloszenia, const Zegar zegar){
    int wazne;
    bool GrubyNieZyje = true;
    while(GrubyNieZyje && !zgloszenia.empty()){
        if(zgloszenia.front().wymaganyTypWykonawcy != kat||zgloszenia.front().czasNadejscia>zegar.getczas())zgloszenia.pop();
        else{
            int = zgloszenia.front().id;
            GrubyNieZyje = false;
        }
    }
    if(wazne.empty()) return -1;
    return wazne;
}
void dosylaj(const int id, vector<Pracownicy>& osoba, queue<Zgloszenie>& paczki, const Zegar zegar, ostream& wynik, int& czasTicka = zegar.getmnoznik()){
    queue<Zgloszenie> tymczasowa;
    while true{
        paczki.front().id == id ? break; :{ tymczasowa.push(paczki.front()) paczki.pop();}
    }
    if(paczki.front().czasRealizacji < czasTicka){
    czasTicka -= paczki.front().czasRealizacji;
    wynik << "Przesyłka o id: " << paczki.front().id << " została nadana o " << getCzas(czasTicka, zegar) << endl;
    cout << "Przesyłka o id: " << paczki.front().id << " została nadana o " << getCzas(czasTicka, zegar) << endl;
    paczki.pop();
    if(wybierz(osoba.front().specjalizacja, paczki, zegar) != -1 ) dosylaj(wybierz(osoba.front().specjalizacja, paczki, zegar), paczki, zegar, wynik, czasTicka);
    }
    else{
        paczki.front().czasRealizacji -= czasTicka;
        if(czasTicka == 0){
        osoba.front().Pid = paczki.front().id;
        osoba.front().zajetosc = true;
        }
        else osoba.front().zajetosc = false;
    }
    while(!paczki.empty()){
        tymczasowa.push(paczki.front());
        paczki.pop();
    }
    while(!tymczasowa.empty()){
        paczki.push(tymczasowa.front());
        tymczasowa.pop();
    }
    return;
}
int main(){
    Config mojeDane = wczytajConfig("pliki/config.txt");
    Zegar zegar = mojeDane.data == "" ? Zegar(1) : Zegar(mojeDane.mnoznik, mojeDane.data);
    ofstream wynik("pliki/wynik.txt");
    switch(mojeDane.rodzajObsl) {
    case 1:
        {  //FIFO, nie- nie muszę sprawdzać tutaj nawet wykonawców, poniewaz 1. ma być słaby dla kontrastu 2. nie chce mi sie
        queue<Zgloszenie> lista = zgrajZgloszenia("pliki/lista.txt");
        while(!lista.empty()){
            if (lista.front().czasNadejscia < zegar.getczas()){
                wylicz(mojeDane.mnoznik, lista, zegar, wynik);
            }
            zegar.tick();
        }
        }
        break;
        
    case 2:
        {
        vector<Pracownicy> osoby = generujPracownikow("pliki/config.txt");
        queue<Zgloszenie> lista = sortuj(zgrajZgloszenia("pliki/lista.txt"));
        vector<Pracownicy> temp;
        int temp2;
        while(!lista.empty()){
            while(!osoby.empty()){
                if(osoby.front().zajetosc){
                    dosylaj(osoby.front().Pid, osoby, lista, zegar, wynik);
                }
                else{
                    temp2 = wybierz(osoby.front().getSpecjalizacja, zegar);
                    if(temp2 == -1){
                        temp.push(osoby.front());
                        osoby.pop();
                    }
                    else {
                        dosylaj(temp2, osoby, lista, zegar, wynik);
                    }
                }
                zegar.tick();
            }
        }
        }
        break;
    case 3:
        break;
    }
    cout << "Wszystko wysłane";
    wynik << "Wszystko wysłane";
}
