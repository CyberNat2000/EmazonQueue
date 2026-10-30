#pragma once
#include <iostream>
#include <fstream>
#include <queue>
#include <ctime>
#include <iomanip>
#include <string>
#include <sstream>
using namespace std;
struct Zgloszenie{
    static int licznik;
    const int id = licznik;
    int czasNadejscia;
    int czasRealizacji;
    int priorytet;
    int deadline;
    int wymaganyTypWykonawcy;
    Zgloszenie(){
        licznik++;
    }
};
int konwertujDate(const string& data){
    stringstream ss(data);
    tm t = {};
    ss >> get_time(&t, "%H:%M %d.%m.%Y");
    time_t sekundy = mktime(&t);
    return (int) sekundy;
}
int Zgloszenie::licznik = 0;
queue<Zgloszenie> zgrajZgloszenia(const string& nazwaPliku) {
    queue<Zgloszenie> zgloszenia;
    ifstream plik(nazwaPliku);

    if (!plik.is_open()) {
        cerr << "Nie udalo sie otworzyc pliku: "
             << nazwaPliku << endl;
        return zgloszenia;
    }

    string linia;

    while (getline(plik, linia)) {
        stringstream ss(linia);
        string wartosc;
        Zgloszenie z = Zgloszenie();
        if (!getline(ss, wartosc, ',')) continue;
        z.czasNadejscia = konwertujDate(wartosc);
        if (!getline(ss, wartosc, ',')) continue;
        z.czasRealizacji = stoi(wartosc) * 3600;
        if (!getline(ss, wartosc, ',')) continue;
        z.priorytet = stoi(wartosc);
        if (!getline(ss, wartosc, ',')) continue;
        z.deadline = konwertujDate(wartosc);
        if (!getline(ss, wartosc)) continue;
        z.wymaganyTypWykonawcy = stoi(wartosc);

        zgloszenia.push(z);
    }

    plik.close();
    return zgloszenia;
}