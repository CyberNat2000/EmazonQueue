#pragma once
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <cstdint>
using namespace std;
class Zegar {
    public:
        Zegar(const int& mno, const string& data) : mnoznik(mno){
            stringstream ss(data);
            tm t = {};
            ss >> get_time(&t, "%H:%M %d.%m.%Y");
            time_t sekundy = mktime(&t);
            czas = sekundy;
        }
        Zegar(const int& mno) : mnoznik(mno){
            time_t sekundy = time(nullptr);;
            czas = sekundy;
        }
        void tick(){
            czas += mnoznik;
        }
        int Getczas() const{
            return czas;
        }
    private:
        const int mnoznik;
        int czas;
};