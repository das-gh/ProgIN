#include "Globalne.h"
#include <stdlib.h>
#include <iostream>
#include "Split.h"
#include <string>

#include "Obraz.h"

using namespace std;

/// KONSTRUKTOR WCZYTUJĄCY

cObraz::cObraz(const string& SciezkaOdczytu) { WczytajZPliku(SciezkaOdczytu); };

/// WCZYTYWANIE Z PLIKU

// hardcode: liczba właściwości formatu = 4
void cObraz::WczytajZPliku(const string& Sciezka)
{
    fPlik.open(Sciezka.c_str(), ios::in);
    if(!fPlik.good())
    {
        cout << "Blad: nie mozna odczytac pliku z obrazem >" << Sciezka << "<";
        getchar();
        exit(0);
    }
    sPlik = "K" + Sciezka;              /// K = kopia, bo nie chcemy przypadkiem nadpisać

    
    ///  wczytywanie danych linia po linii, słowo po słowie
    string linia;
    int Ix_slowa0;                      /// potrzebne do Split()
    int l_wczytanych_danych = 0;        /// potrzebne, by wczytać właściwości po kolei
    int* Piksel;                        /// piksel lub jego składowa; jest to wskaźnik ruchomy
    while(!fPlik.eof())
    {
        getline(fPlik, linia);          /// nowa linia, trzeba wyekstrahować z niej słowa
        if(linia[0] != '#')             /// czyli: nie komentarz
        {
            string s(Split(linia, Ix_slowa0 = 0)); /// słowo; jeśli nie ma więcej słów, Split zwraca ""
            while(!s.empty())
            {
                /// poniższa instrukcja wczytuje parametry zgodnie z kolejnością w definicji formatu Netpbm
                switch(++l_wczytanych_danych) /// dopiero tu wiadomo, że coś wczytano
                {
                case 1:
                    if((s != "P2") && (s != "P3"))
                    {
                        cout << "Blad: plik >" << sPlik << "< ma nieobslugiwany format: >" << s << "<";
                        getchar();
                        exit(0);
                    }
                    FormatAktualny = s;
                    FormatOptymalny = s;
                    break;
                case 2:
                    Szerokosc = atoi(s.c_str());
                    break;
                case 3:
                    Wysokosc = atoi(s.c_str());
                    break;
                case 4:
                    Glebia = atoi(s.c_str());
                    break;
                default:                    /// wczytano właściwości, teraz bitmapa
                    int p = atoi(s.c_str());
                    if((p < 0) || (p > Glebia))
                    {
                        cout << "Blad: token (slowo) nr >" << l_wczytanych_danych << "< zawiera niedozwolona wartosc nasycenia: >" << p << "<";
                        getchar();
                        exit(0);
                    }
                    *(Piksel++) = p;
                    break;
                }
                
                ///     przygotowanie do wczytania bitmapy
                if(((FormatAktualny == "P2") && (l_wczytanych_danych == 4)) || \
                   ((FormatAktualny == "P3") && (l_wczytanych_danych == 4))) /// dla ogólności, aby móc dodawać obsługę kolejnych formatów
                {
                    Bitmapa = new int [WielkoscBitmapy()];
                    Piksel = Bitmapa;
                }
                
                s = Split(linia, ++Ix_slowa0);   /// następne słowo
            }
        }
        /// koniec linii, wczytujemy następną
    }
    if(l_wczytanych_danych != 4 + WielkoscBitmapy())
    {
            cout << "Blad: wczytano: >" << l_wczytanych_danych << "< danych, oczekiwano: >" << 4 + WielkoscBitmapy() << "<";
            getchar();
            exit(0);
    }
    fPlik.close();
};

void cObraz::WczytajZPliku(void) { WczytajZPliku(sPlik); };


/// KONSTRUKTOR KOPIUJĄCY

cObraz::cObraz(const cObraz* Oryginalny) { KopiujObraz(Oryginalny); };

void cObraz::KopiujObraz(const cObraz* Oryginalny)
{
    FormatAktualny  = Oryginalny->Get_FormatAktualny();
    FormatOptymalny = Oryginalny->FormatOptymalny;
    Szerokosc       = Oryginalny->Get_Szerokosc();
    Wysokosc        = Oryginalny->Get_Wysokosc();
    Glebia          = Oryginalny->Get_Glebia();
    
    Bitmapa     = new int [WielkoscBitmapy()];
    int* pBitmapa1; pBitmapa1 = Oryginalny->Bitmapa;
    int* pBitmapa2; pBitmapa2 = Bitmapa;
    for(int sub_px = 0 ; sub_px < WielkoscBitmapy() ; sub_px++, pBitmapa1++, pBitmapa2++) /// sub_px może być albo subpikselem RGB albo pikselem GREY
        *pBitmapa2 = *pBitmapa1;                    /// kopiowanie bitmapy z jednego obrazka do drugiego
    
    sPlik = "";
};


/// KONSTRUKTOR OTRZYMUJĄCY WSZYSTKIE DANE OBIEKTU

cObraz::cObraz(const string& _FormatAktualny, const string& _FormatOptymalny, int _Szerokosc, int _Wysokosc, int _Glebia, const string& SciezkaZapisu)
{
    FormatAktualny  = _FormatAktualny;
    FormatOptymalny = _FormatOptymalny;
    Szerokosc       = _Szerokosc;
    Wysokosc        = _Wysokosc;
    Glebia          = _Glebia;
    
    Bitmapa     = new int [WielkoscBitmapy()];
    fill_n(Bitmapa, WielkoscBitmapy(), 0);          /// zapełnianie zerami, tworzenie czarnego obrazka
    
    sPlik = SciezkaZapisu;
};

/// DESTRUKTOR

cObraz::~cObraz() { delete[] Bitmapa; };


/// ZAPISYWANIE DO PLIKU

void cObraz::ZapiszDoPliku(const std::string& Sciezka, const bool CRLF)
{
    sPlik = Sciezka;
    UstawRozszerzenie();
    
    fPlik.open(sPlik.c_str(), ios::binary|ios::out|ios::trunc);             /// binarnie, żeby można było zapisać LF
    if(!fPlik.good())
    {
        cout << "Blad: nie mozna nadpisac pliku z obrazem >" << sPlik << "<";
        getchar();
        exit(0);
    }
    
    /// UNIXowe znaki końca linii 0xA, czyli LF 
    /// lub Windowsowe znaki końca linii 0xD 0xA, czyli CRLF
    std::string KLin(CRLF? "\xD\xA" : "\xA");                               /// KLin - koniec linii
    fPlik << FormatAktualny << KLin << "# Wygenerowane za pomocą ProgIN" << KLin << Szerokosc << " " << Wysokosc << KLin << Glebia << KLin;
    int* pBitmapa = Bitmapa;
    for(int sub_px = 0 ; sub_px < WielkoscBitmapy() ; sub_px++, pBitmapa++)
        fPlik << *pBitmapa << KLin;
    fPlik.close();
};

void cObraz::ZapiszDoPliku(const std::string& Sciezka) { ZapiszDoPliku(Sciezka, ZG::CRLF); };

void cObraz::ZapiszDoPliku(void) { ZapiszDoPliku(sPlik, ZG::CRLF); };



/*  Funkcja dzieli sPlik na dwie części: nazwę (N) oraz rozszerzenie poprzedzone kropką (R); następnie skleja je. Opis założeń merytorycznych w pliku .h!!
*/
void cObraz::UstawRozszerzenie(void)
{
    string N, R;
    
    ///     ustalanie, co jest nazwą a co rozszerzeniem
    
    if(sPlik.length() < 4)                              /// za krótki, by można było mówić o rozszerzeniu
    {
        N = sPlik;
        R = "";
    }
    else 
    {
        R = sPlik.substr(sPlik.length()-4, 4);          /// badamy 4 ostatnie znaki
        if((R == ".pgm") || (R == ".ppm"))
            N = sPlik.substr(0, sPlik.length()-4);      /// mamy rozszerzenie, reszta jest nazwą
        else
            N = sPlik;                                  /// 4 ost. znaki mogły być rozszerzeniem, ale nim nie są; wszystko jest nazwą
    }
    
    ///     ustalanie właściwego rozszerzenia
    
    if(FormatAktualny == "P2")
        R = ".pgm";
    else if(FormatAktualny == "P3") 
        R = ".ppm";
    else
    {
        cout << "Blad: nieobslugiwany format pliku " << sPlik << ": >" << FormatAktualny << "<";
        getchar();
        exit(0);
    }
    
    ///     sklejanie
    
    sPlik = N + R;
};



/// KONWERSJE

/// GREY - greyscale; RGB - red, green, blue

void cObraz::KonwertujNaSzarosc(void)
{
    if(FormatAktualny == "P2") return;
    FormatAktualny = "P2";
    FormatOptymalny = "P2";
    
    int* TmpBitmapa_GREY = new int [WielkoscBitmapy()];
    int* pTmpBitmapa_GREY = TmpBitmapa_GREY;    /// wsk. ruchomy
    int* pBitmapa_RGB = Bitmapa;                /// wsk. ruchomy
    
    /// kopiowanie bitmapy do tmp
    for(int px_GREY = 0 ; px_GREY < WielkoscBitmapy() ; px_GREY++, pTmpBitmapa_GREY++, pBitmapa_RGB+=3)
        *pTmpBitmapa_GREY = (int)((pBitmapa_RGB[0] + pBitmapa_RGB[1] + pBitmapa_RGB[2]) / 3. + 0.5); /// zaokrąglenie średniej

    /// kopiowanie tmp do nowej bitmapy
    delete[] Bitmapa;   pBitmapa_RGB = NULL;
    Bitmapa = new int [WielkoscBitmapy()];
    int* pBitmapa_GREY = Bitmapa;               /// wsk. ruchomy
    
    pTmpBitmapa_GREY = TmpBitmapa_GREY;         /// ustawienie wsk. na początek
    for(int px_GREY = 0 ; px_GREY < WielkoscBitmapy() ; px_GREY++)
        *(pBitmapa_GREY++) = *(pTmpBitmapa_GREY++);
    
    delete[] TmpBitmapa_GREY;
    pBitmapa_GREY = NULL;
};

void cObraz::KonwertujNaRGB(void)
{
    if(FormatAktualny == "P3") return;
    FormatAktualny = "P3";
    FormatOptymalny = "P2";
    
    int* TmpBitmapa_RGB = new int [WielkoscBitmapy()];
    int* pTmpBitmapa_RGB = TmpBitmapa_RGB;      /// wsk. ruchomy
    int* pBitmapa_GREY = Bitmapa;               /// wsk. ruchomy
    
    /// kopiowanie bitmapy do tmp
    for(int px_RGB = 0 ; px_RGB < WielkoscBitmapy() / 3 ; px_RGB++, pBitmapa_GREY++)
        for(int sub_px_offset = 0 ; sub_px_offset < 3 ; sub_px_offset++, pTmpBitmapa_RGB++)
            *pTmpBitmapa_RGB = *pBitmapa_GREY;
    
    /// kopiowanie tmp do nowej bitmapy
    delete[] Bitmapa;   pBitmapa_GREY = NULL;
    Bitmapa = new int [WielkoscBitmapy()];
    int* pBitmapa_RGB = Bitmapa;
    
    pTmpBitmapa_RGB = TmpBitmapa_RGB;           /// ustawienie wsk. na początek
    for(int px_RGB = 0 ; px_RGB < WielkoscBitmapy() ; px_RGB++)
        *(pBitmapa_RGB++) = *(pTmpBitmapa_RGB++);
    
    delete[] TmpBitmapa_RGB;
    pBitmapa_RGB = NULL;
};
