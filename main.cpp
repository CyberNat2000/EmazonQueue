#include <iostream>
#include <queue>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>
#include "zegar.h"
#include "konfig.h"
#include "zgloszenia.h"
#include "pracownicy.h"
using namespace std;
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
bool sprawdz(const int id, vector<Pracownicy> osoba){
    while(!osoba.empty()){
        if(osoba.front().Pid == id)return true;
        osoba.erase(osoba.begin());
    }
    return false;
}
int wybierz(int kat, queue<Zgloszenie> zgloszenia, const Zegar zegar, vector<Pracownicy>& osoba){
    int wazne = -1;
    bool GrubyNieZyje = true;
    while(GrubyNieZyje && !zgloszenia.empty()){
        if(zgloszenia.front().wymaganyTypWykonawcy != kat||zgloszenia.front().czasNadejscia>zegar.getczas()||sprawdz(zgloszenia.front().id, osoba))zgloszenia.pop();
        else{
            wazne = zgloszenia.front().id;
            GrubyNieZyje = false;
        }
    }
    return wazne;
}
void dosylaj(const int id, vector<Pracownicy>& osoba, queue<Zgloszenie>& paczki, const Zegar zegar, ostream& wynik, int& czasTicka){
    queue<Zgloszenie> tymczasowa;
    while(true){
        if(paczki.front().id == id)break; 
        else{ 
            tymczasowa.push(paczki.front()); 
            paczki.pop();
        }
    }
    if(paczki.front().czasRealizacji < czasTicka){
    czasTicka -= paczki.front().czasRealizacji;
    wynik << "Przesyłka o id: " << paczki.front().id << " została nadana o " << getCzas(czasTicka, zegar) << endl;
    cout << "Przesyłka o id: " << paczki.front().id << " została nadana o " << getCzas(czasTicka, zegar) << endl;
    paczki.pop();
    if(wybierz(osoba.front().getSpecjalizacja(), paczki, zegar, osoba) != -1 ) dosylaj(wybierz(osoba.front().getSpecjalizacja(), paczki, zegar, osoba), osoba, paczki, zegar, wynik, czasTicka);
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
        int mnoznik = zegar.getmnoznik();
        int temp2;
        while(!lista.empty()){
            while(!osoby.empty()){
                if(osoby.front().zajetosc){
                    dosylaj(osoby.front().Pid, osoby, lista, zegar, wynik,  mnoznik);
                }
                else{
                    temp2 = wybierz(osoby.front().getSpecjalizacja(), lista, zegar, osoby);
                    if(temp2 == -1){
                        temp.push_back(osoby.front());
                        osoby.erase(osoby.begin());
                    }
                    else {
                        dosylaj(temp2, osoby, lista, zegar, wynik, mnoznik);
                    }
                }
            }
            zegar.tick();
            while(!temp.empty()){
                osoby.push_back(temp.front());
                temp.erase(temp.begin());
            }
        }
        }
        break;
    }
    cout << "Wszystko wysłane";
    wynik << "Wszystko wysłane";
}
