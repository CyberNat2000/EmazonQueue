#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
struct Config {
    string data;
    int mnoznik;
    int rodzajObsl;
};

Config wczytajConfig(const string& nazwaPliku) {
    Config cfg = {"", 1, 1};
    ifstream plik(nazwaPliku);
    if (!plik.is_open()) {
        cerr << "Nie udalo sie otworzyc pliku: " << nazwaPliku << endl;
        return cfg;
    }
    getline(plik, cfg.data);
    plik >> cfg.mnoznik;
    plik >> cfg.rodzajObsl;
    plik.close();
    return cfg;
}