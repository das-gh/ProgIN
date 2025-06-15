//#define DEBUG

#pragma once /* żeby nie włączać pliku wielokrotnie przez przypadek */

#include <string>
#include <fstream>
#include "Obraz.h"

/*  Objaśnienia skrótów ORY, KOP, WYN są w pliku main.cpp. 
    Tu są tylko nazwy, żeby kompilator wiedział, że obiekty o takich nazwach istnieją. 
    Ich definicje są albo w pliku Globalne.cpp albo w odpowiednich miejscach w kodzie. 
*/

namespace Pliki
{
    extern std::string  sPlikORY, sPlikWYN, sPlikINI;
    extern std::fstream fPlikINI;               /// pozostałe dwa strumienie są w klasie cObraz
};

namespace Obrazy
{
    extern cObraz*  ObrazORY;
    extern cObraz** ObrazKOP;                   /// będzie to tablica; np. ObrazKOP[3] to wsk. do kopii nr 4
    extern cObraz*  ObrazWYN;
};

namespace ZG                                    /// zmienne globalne
{
    extern int WymiarSiatkiKopii_n;             /// stała "n" z treści zadania, określająca wymiary siatki kopii obrazu n × n
    inline int nn(void) { return WymiarSiatkiKopii_n * WymiarSiatkiKopii_n; };
                                                /// nn - tyle jest obrazków (kopii)
    extern bool CRLF;                           /// znak końca linii; LF - Unix, CRLF - Windows

    extern std::string* MacierzFiltrow;
};

/*  MACIERZ FILTRÓW
    ===============
    Poniżej znajduje się wskaźnik do macierzy, zawierającej kody filtrów, jakie należy zastosować na kolejnych kopiach obrazka.
    Każdy element tej macierzy odpowiada jednej kopii obrazka. Budowa takiego elementu została opisana w pliku main.cpp.
    Przykład dla macierzy domyślnej z treści zadania:
    ZG::MacierzFiltrow[0]   ==  "@1 Sz Neg"
    ZG::MacierzFiltrow[1]   ==  "@2 Oy Kol r"
    ZG::MacierzFiltrow[2]   ==  "@3 Kol g"
    ZG::MacierzFiltrow[3]   ==  "@4 Oy Kol b Neg"
*/
