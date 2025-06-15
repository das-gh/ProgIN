#include "Globalne.h"
#include "SUB01.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

/*  DZIAŁANIE
    Procedura wczytuje plik inicjalizacyjny - jeśli takowy istnieje, a program wywołano z wiersza polecenia 
        ze ścieżką do pliku inicjalizacyjnego jako pierwszym argumentem wywołania.
    Ponadto procedura konstruuje ObrazORY, co powoduje otwarcie odpowiedniego pliku.
*/
void SUB01::WczytajINIOrazObrazOrazMacierzFiltrow(void)
{
    ///     próba wczytania z pliku inicjalizacyjnego
    if(!Pliki::sPlikINI.empty())                    /// jest argument, być może to plik inicjalizacyjny
    {
        Pliki::fPlikINI.open(Pliki::sPlikINI.c_str(), ios::in);
        if(!Pliki::fPlikINI.good())
        {
            cout << "Blad: nie mozna odczytac pliku inicjalizacyjnego >" << Pliki::sPlikINI << "<";
            getchar();
            exit(0);
        }
        else                                        /// ekstrahowanie nazw plików ORY i WYN z pliku INI
        {
            { using namespace Pliki;
            fPlikINI >> sPlikORY >> sPlikWYN >> ZG::WymiarSiatkiKopii_n;
            #ifdef DEBUG
            cout << "Debug: Otwarto INI \t>" << sPlikINI << "<" << endl;
            cout << "Debug: sPlikORY \t>" << sPlikORY << "<" << endl;
            cout << "Debug: sPlikWYN \t>" << sPlikWYN << "<" << endl;
            cout << "Debug: WymiarSiatkiKopii_n \t>" << ZG::WymiarSiatkiKopii_n << "<" << endl;
            #endif // DEBUG
            }
            
            if(ZG::WymiarSiatkiKopii_n < 1)
            {
                cout << "Blad: nieprawidlowy wymiar siatki kopii: >" << ZG::WymiarSiatkiKopii_n << "<";
                getchar();
                exit(0);
            }
            
            /// wczytywanie macierzy filtrów
            WczytajMacierzFiltrowZINI();
        }
        Pliki::fPlikINI.close();
        Obrazy::ObrazORY = new cObraz(Pliki::sPlikORY);
        Obrazy::ObrazORY->ID = cObraz::IDs::ORY;
    }
    ///     próba wczytania nieudana, wczytywanie plików oraz macierzy filtrów poprzez prompt
    else 
    {
        cout << "Wprowadz nazwe pliku z obrazem do wczytania: " << endl << "> ";
        cin >> Pliki::sPlikORY;
        cout << "Wprowadz nazwe pliku obrazu do zapisania: " << endl << "> ";
        cin >> Pliki::sPlikWYN;
        
        /// obraz wczytuje się tu, bo jeśli coś jest nie tak, użytkownik będzie wiedział zanim wprowadzi macierz filtrów
        Obrazy::ObrazORY = new cObraz(Pliki::sPlikORY);
        Obrazy::ObrazORY->ID = cObraz::IDs::ORY;
        
        /// jest OK, można prosić użytkownika o macierz filtrów
        WczytajMacierzFiltrowRecznie();
    }
};





void SUB01::WczytajMacierzFiltrowZINI(void)
{
    ZG::MacierzFiltrow = new string [ZG::nn()]; /// zostanie usunięta po renderowaniu obrazków
    int IxKopii1 = 0;                                                           /// nr obrazka, którego filtry zapisujemy
    string Slowo("");
    Pliki::fPlikINI >> Slowo;
    
    ///     wczytanie następnego słowa z pliku
    while(!Pliki::fPlikINI.eof())                                               /// czy są jeszcze słowa?
    {
        if(Slowo[0] == '@')                                                     /// czy słowo zawiera numer obrazka?
        {
            if(atoi(Slowo.substr(1, Slowo.length()-1).c_str()) == IxKopii1+1)   /// czy to numer kolejny?
            {
                if(++IxKopii1 > ZG::nn())
                    goto Blad;
                ZG::MacierzFiltrow[IxKopii1-1] = Slowo;                         /// zapis numeru do nowego stringa
            }
            else if(atoi(Slowo.substr(1, Slowo.length()-1).c_str()) < 1)        /// czy to niepoprawny numer?
            {
                cout << "Blad: w macierzy filtrow wczytywanej z INI numery kopii powinny zaczynać się od 1, nie: >" << Slowo << "<";
                getchar();
                exit(0);
            }
            else
            {
                cout << "Blad: w macierzy filtrow wczytywanej z INI po obrazku nr @" << IxKopii1 << " nastepuje >" << Slowo << "<";
                getchar();
                exit(0);
            }
        }
        else
        {
            ZG::MacierzFiltrow[IxKopii1-1] += " ";
            ZG::MacierzFiltrow[IxKopii1-1] += Slowo;
        }
        Pliki::fPlikINI >> Slowo;
    }
    Blad:
    if(IxKopii1 != ZG::nn())
    {
        cout << "Blad: oczekiwano " << ZG::nn() << " ciagow transformacji; zakonczono wczytywanie na >" << IxKopii1 << "<";
        getchar();
        exit(0);
    }

    #ifdef DEBUG
    for(int i0 = 0 ; i0 < ZG::nn() ; i0++)
        cout << "Debug: ZG::MacierzFiltrow[" << i0 << "] == >" << ZG::MacierzFiltrow[i0] << "<" << endl;
    #endif // DEBUG
};




void SUB01::WczytajMacierzFiltrowRecznie(void)
{
    string sWybor("");
    do
    {
        cout << "Wybierz tryb procedowania: manualny lub automatyczny. " << endl << \
            "Wpisz \"koniec\", by zakonczyc program. [M/A/koniec]? " << endl << "> ";
        cin >> sWybor;
        if(sWybor == "koniec") exit(0);
    }
    while((sWybor != "A") && (sWybor != "a") && (sWybor != "M") && (sWybor != "m"));
    
    if((sWybor == "A") || (sWybor == "a"))                                        /// tryb automatyczny
    {
        ZG::WymiarSiatkiKopii_n = 2;
        ZG::MacierzFiltrow = new string [ZG::nn()]; /// zostanie usunięta po renderowaniu obrazków
        ZG::MacierzFiltrow[0] = "@1 Sz Neg";
        ZG::MacierzFiltrow[1] = "@2 Oy Kol r";
        ZG::MacierzFiltrow[2] = "@3 Kol g";
        ZG::MacierzFiltrow[3] = "@4 Oy Kol b Neg";
        return;
    }
    
    ///     TRYB MANUALNY
    
    ///     robienie pustej macierzy
    
    do
    {
        cout << "Wprowadz wymiar siatki kopii n x n: " << endl << "> ";
        cin >> ZG::WymiarSiatkiKopii_n;
        if(ZG::WymiarSiatkiKopii_n < 1)
            cout << "Nie moze byc mniejszy od 1!" << endl;
        if(ZG::WymiarSiatkiKopii_n > 4)
        {
            cout << "Zamierzasz zrobic " << ZG::nn() << " obrazkow! Wpisz 0, by sie wycofac.";
            cin >> sWybor;
            if(sWybor == "0") ZG::WymiarSiatkiKopii_n = 0;
        }
    }
    while(ZG::WymiarSiatkiKopii_n < 1);
    
    ZG::MacierzFiltrow = new string [ZG::nn()]; /// zostanie usunięta po renderowaniu obrazków
    for(int i0 = 0 ; i0 < ZG::nn() ; i0++)
    {
        ZG::MacierzFiltrow[i0] = "@";
        ZG::MacierzFiltrow[i0] += to_string(i0+1);
    }


    ///     część właściwa: wprowadzanie macierzy

    int IxKopii1 = 1;                                                            /// nr obrazka, którego filtry zapisujemy
    int iWybor;
    do
    {
        for(int i = 1 ; i <= 25 ; i++) cout << endl;
        ///     wyświetlanie macierzy, żeby wiedzieć, co już jest wpisane
        for(int i0 = 0 ; i0 < ZG::nn() ; i0++)
        {
            if(i0 == IxKopii1-1) cout << "!!!Kopia >" << ZG::MacierzFiltrow[i0] << "<" << endl;
            else cout << "   Kopia >" << ZG::MacierzFiltrow[i0] << "<" << endl;
        }

        cout << "DOKONYWANIE ZMIAN NA KOPII NR: " << IxKopii1 << " Z " << ZG::nn() << endl;
        cout << "Wprowadz numer z lewej strony, aby wprowadzic odpowiadajacy mu filtr: " << endl;
        cout << "====================================================================" << endl;
        cout << "[1]    Odbicie w pionie" << endl;
        cout << "[2]    Negatyw" << endl;
        cout << "[3]    Skala szarosci" << endl;
        cout << "[4]    Usuniecie koloru" << endl;
        cout << "[-6]   -> NADPISZ NASTEPNA KOPIE" << endl;
        cout << "[-4]   NADPISZ POPRZEDNIA KOPIE <-" << endl;
        cout << "[-2]   # WYJSCIE Z PROGRAMU #" << endl;
        cout << "> ";
        cin >> sWybor;                      /// "shock absorber"
        iWybor = atoi(sWybor.c_str());      /// jeśli nie wprowadzono liczby, iWybor = 0
        switch(iWybor)
        {
        case -6:
            IxKopii1++;
            if(IxKopii1 <= ZG::nn())
            {
                ZG::MacierzFiltrow[IxKopii1-1] = "@";
                ZG::MacierzFiltrow[IxKopii1-1] += to_string(IxKopii1);
            }
            break;
        case -4:
            if(IxKopii1 > 1)
            {
                IxKopii1--;
                ZG::MacierzFiltrow[IxKopii1-1] = "@";
                ZG::MacierzFiltrow[IxKopii1-1] += to_string(IxKopii1);
            }
            break;
        case -2:
            exit(0);
        case 1:
            ZG::MacierzFiltrow[IxKopii1-1] += " Oy";
            break;
        case 2:
            ZG::MacierzFiltrow[IxKopii1-1] += " Neg";
            break;
        case 3:
            ZG::MacierzFiltrow[IxKopii1-1] += " Sz";
            break;
        case 4:
            ZG::MacierzFiltrow[IxKopii1-1] += " Kol";
            cout << "Wpisz kolor/kolory do usuniecia: r = czerwony, g = zielony, b = niebieski" << endl << "> ";
            cin >> sWybor;
            ZG::MacierzFiltrow[IxKopii1-1] += " ";
            ZG::MacierzFiltrow[IxKopii1-1] += sWybor;
            break;
        default:
            break;
        }
    }
    while(IxKopii1 <= ZG::nn());
    
    
    
    
    ///     dodatek: znaki nowej linii
    
    do
    {
        cout << "Czy znaki nowej linii maja byc Unixowe (LF) czy Windowsowe (CRLF)? [U/W/koniec]" << endl << "> ";
        cin >> sWybor;
        if(sWybor == "koniec") exit(0);
    }
    while((sWybor != "U") && (sWybor != "u") && (sWybor != "W") && (sWybor != "w"));
    
    switch(sWybor[0])
    {
    case 'U':
    case 'u':
        ZG::CRLF = false;
        break;
    case 'W':
    case 'w':
        ZG::CRLF = true;
        break;
    default:
        cout << "Blad: wystapil niezidentyfikowany blad";
        getchar();
        exit(0);
    }
};
