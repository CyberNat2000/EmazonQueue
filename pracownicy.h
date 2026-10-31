#pragma once
#include <fstream>
#include <string>
#include <vector>
class Pracownicy{
    public:
        static int licznik;
        int Pid;
        bool zajetosc = false;
        Pracownicy(const int& specjal) : specjalizacja(specjal){
            licznik++;
        }
        int getSpecjalizacja(){
            return specjalizacja;
        }
    private:
        const int id = licznik;
        const int specjalizacja;
};
vector<Pracownicy> generujPracownikow(const string& nazwaPliku){
    vector<Pracownicy> osoby;
    Pracownicy::licznik = 0;
    ifstream plik(nazwaPliku);
    string osoba;
    getline(plik, osoba, ';');
    while(getline(plik, osoba)){
        osoby.push_back(Pracownicy(stoi(osoba)));
    }
    return osoby;
}