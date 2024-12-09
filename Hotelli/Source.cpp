// Author: Timi Uski <timi.uski@tuni.fi>
// Date: 2024-12-09

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <cctype>

using namespace std;

const int MIN_HUONEITA = 40;
const int MAX_HUONEITA = 300;
const int MIN_HINTA = 80;
const int MAX_HINTA = 100;
const int VARAUSNUMERO_MIN = 10000;
const int VARAUSNUMERO_MAX = 99999;

string trim(const string& str) {

    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");

    return (start == string::npos || end == string::npos) ? "" : str.substr(start, end - start + 1);
}

string toLower(const string& str) {

    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
        [](unsigned char c) { return tolower(c); });

    return lowerStr;
}

struct Huone {

    int numero;
    int koko;
    double hinta;
    bool varattu;

};

struct Varaus {

    int varausNumero{};
    string varaajanNimi;
    int huoneNumero{};
    int oidenMaara{};
    double loppuSumma{};

};


vector<Huone> alustaHuoneet();

bool onkoHuoneVapaa(vector<Huone>& huoneet, int huoneNumero);
int arvoVarausNumero();
double laskeAlennus(double loppuSumma);
void teeVaraus(vector<Huone>& huoneet, vector<Varaus>& varaukset);
void haeVaraus(const vector<Varaus>& varaukset);
void naytaValikko();


int main() {

    srand(static_cast<unsigned>(time(0)));

    vector<Huone> huoneet = alustaHuoneet();
    vector<Varaus> varaukset;

    int valinta;

    do {

        naytaValikko();
        cin >> valinta;
        cin.ignore();

        switch (valinta) {

        case 1:
            teeVaraus(huoneet, varaukset);
            break;

        case 2:
            haeVaraus(varaukset);
            break;

        case 3:
            cout << "Kiitos, ettA kAytit varausjArjestelmAA. NAkemiin!" << endl;
            break;

        default:

            cout << "Virheellinen valinta! YritA uudelleen." << endl;

        }

    } while (valinta != 3);

    return 0;
}


vector<Huone> alustaHuoneet() {

    int huoneidenMaara = MIN_HUONEITA + rand() % ((MAX_HUONEITA - MIN_HUONEITA) / 2) * 2;

    vector<Huone> huoneet;

    for (int i = 1; i <= huoneidenMaara; ++i) {

        Huone huone;
        huone.numero = i;
        huone.koko = (i <= huoneidenMaara / 2) ? 1 : 2;
        huone.hinta = MIN_HINTA + rand() % (MAX_HINTA - MIN_HINTA + 1);
        huone.varattu = false;
        huoneet.push_back(huone);
    }

    return huoneet;
}


int arvoVarausNumero() {

    return VARAUSNUMERO_MIN + rand() % (VARAUSNUMERO_MAX - VARAUSNUMERO_MIN + 1);

}


double laskeAlennus(double loppuSumma) {

    int alennusProsentti = (rand() % 3) * 10;
    return loppuSumma * (1 - alennusProsentti / 100.0);
}


void teeVaraus(vector<Huone>& huoneet, vector<Varaus>& varaukset) {

    cout << "Anna varaajan nimi: ";
    cin.ignore();
    string nimi;
    getline(cin, nimi);

    string siistiNimi = trim(nimi);

    cout << "Valitse huoneen koko (1 = yhden hengen, 2 = kahden hengen): ";

    int koko;
    cin >> koko;

    cout << "Anna oiden maara: ";

    int yot;
    cin >> yot;

    int huoneNumero = -1;

    for (auto& huone : huoneet) {

        if (!huone.varattu && huone.koko == koko) {
            huoneNumero = huone.numero;
            huone.varattu = true;

            break;
        }
    }

    if (huoneNumero == -1) {

        cout << "Valitettavasti ei loytynyt vapaita huoneita kyseiselle huonetyypille." << endl;

        return;
    }

    double loppuSumma = huoneet[huoneNumero - 1].hinta * yot;
    double alennettuSumma = laskeAlennus(loppuSumma);

    Varaus varaus;
    varaus.varausNumero = arvoVarausNumero();
    varaus.varaajanNimi = siistiNimi;
    varaus.huoneNumero = huoneNumero;
    varaus.oidenMaara = yot;
    varaus.loppuSumma = alennettuSumma;

    varaukset.push_back(varaus);

    cout << "Huone varattu onnistuneesti!" << endl;
    cout << "Varausnumero: " << varaus.varausNumero << endl;
    cout << "Huone: " << huoneNumero << ", Oiden maara: " << yot << endl;
    cout << "Loppusumma: " << fixed << setprecision(2) << alennettuSumma << " euroa" << endl;
}

bool onkoHuoneVapaa(vector<Huone>& huoneet, int huoneNumero) {

    if (huoneNumero < 1 || huoneNumero > static_cast<int>(huoneet.size()))
        return false;

    return !huoneet[huoneNumero - 1].varattu;
}


void haeVaraus(const vector<Varaus>& varaukset) {
    cout << "Haluatko hakea (1) varausnumerolla vai (2) nimella? ";

    int valinta;
    cin >> valinta;
    cin.ignore();

    if (valinta == 1) {

        cout << "Anna varausnumero: ";
        int numero;
        cin >> numero;

        for (const auto& varaus : varaukset) {

            if (varaus.varausNumero == numero) {

                cout << "Varaus loytyi!" << endl;
                cout << "Nimi: " << varaus.varaajanNimi << ", Huone: " << varaus.huoneNumero
                    << ", Oita: " << varaus.oidenMaara << ", Hinta: " << fixed << setprecision(2)
                    << varaus.loppuSumma << " euroa" << endl;

                return;
            }
        }

        cout << "Varausta ei loytynyt varausnumerolla." << endl;
    }
    else if (valinta == 2) {

        cout << "Anna varaajan nimi: ";
        cin.ignore();
        string nimi;
        getline(cin, nimi);

        string hakunimi = toLower(trim(nimi));

        for (const auto& varaus : varaukset) {

            string tallennettuNimi = toLower(trim(varaus.varaajanNimi));

            if (tallennettuNimi == hakunimi) {

                cout << "Varaus loytyi!" << endl;
                cout << "Nimi: " << varaus.varaajanNimi << ", Huone: " << varaus.huoneNumero
                    << ", Oita: " << varaus.oidenMaara << ", Hinta: " << fixed << setprecision(2)
                    << varaus.loppuSumma << " euroa" << endl;

                return;
            }
        }

        cout << "Varausta ei loytynyt nimella." << endl;
    }

    else {

        cout << "Virheellinen valinta." << endl;
    }
}


void naytaValikko() {

    cout << "\n********************************************" << endl;
    cout << "*         HOTELLIN VARAUSJARJESTELMA       *" << endl;
    cout << "********************************************" << endl;
    cout << "1. Tee uusi varaus" << endl;
    cout << "2. Hae varaus" << endl;
    cout << "3. Poistu ohjelmasta" << endl;
    cout << "********************************************" << endl;
    cout << "Valitse toiminto (1-3): ";
}
