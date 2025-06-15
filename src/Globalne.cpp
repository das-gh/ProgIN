#include <string>
#include <fstream>
#include "Obraz.h"

#include "Globalne.h"

using namespace std;

namespace Pliki
{
    string  sPlikORY(""), sPlikWYN(""), sPlikINI("");
    fstream fPlikINI;                                   /// pozostałe dwa strumienie są w klasie cObraz
};

///     OBRAZY ZOSTANĄ ZAINICJALIZOWANE W "SUB" W TYCH MIEJSCACH, GDZIE ZACZNĄ BYĆ POTRZEBNE!

namespace Obrazy
{
    cObraz*  ObrazORY;
    cObraz** ObrazKOP;                                  /// będzie to tablica; np. ObrazKOP[3] to wsk. do kopii nr 4
    cObraz*  ObrazWYN;
};

namespace ZG                                            /// zmienne globalne
{
    int WymiarSiatkiKopii_n = 2;                        /// stała "n" z treści zadania, określająca wymiary siatki kopii obrazu n × n
    bool CRLF = true;                                   /// znak końca linii; LF - Unix, CRLF - Windows
    string* MacierzFiltrow;
};

