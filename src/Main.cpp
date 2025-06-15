/*  PLIK INICJALIZACYJNY:
    =====================
    Można stworzyć plik, dzięki któremu nie będzie wymagana interakcja programu z użytkownikiem w czasie wykonania. W tym pliku należy 
    umieścić w podanej kolejności: 
        * nazwę pliku oryginalnego, 
        * nazwę pliku wynikowego, 
        * wymiar siatki n×n (jedna liczba int), 
        * macierz kodów transformacji (patrz poniżej).
    Program należy wtedy uruchamiać z nazwą pliku inicjalizacyjnego jako pierwszym argumentem wywołania.
*/

/*  NOMENKLATURA:
    =============
    c       class
    f       file / fstream
    p       pointer (wsk. ruchomy)
    s       string
    SUB     subroutine

    Zmienne plikowe z przedrostkiem oznaczają:
    ==========================================
    f       file / fstream      strumienie
    s       string              ścieżki dostępu
*/

/*  OBRAZY I PLIKI:
    ===============
    ORY Oryginalny      - obraz wyjściowy, którego kopie chcemy tworzyć
    WYN Wynikowy        - obraz powstały przez sklejenie kopii
    INI Inicjalizacyjny - plik zastępujący interakcję z użytkownikiem, zawierający:
            * ścieżkę dostępu do pliku wyjściowego
            * ścieżkę dostępu do pliku wynikowego
            * zestaw filtrów (transformacji) dla każdej kopii, zakodowanych w postaci słów (poniżej)
    
    Obrazy są typu cObraz i zawierają w sobie m.in. strumień pliku skojarzony z danym obrazem.
*/

/*  FILTRY (TRANSFORMACJE):
    =======================
    Można je zapisać w pliku inicjalizacyjnym w postaci słów oddzielonych białymi znakami. Każde słowo jest albo
    umowną nazwą filtra (wtedy wielką literą) albo parametrem tego filtra, jeśli filtr go wymaga (wtedy małą literą).
    
    NAZWA           SKRÓT       PARAMETRY
    OdbicieOy       Oy
    Negatyw         Neg
    Szarosc         Sz
    UsunKolor       Kol         Jeden par. zawierający nazwy kolorów do usunięcia: 
                                r - czerwony, g - zielony, b - niebieski, np. "rb" oznacza czerwony i niebieski.
                                
    Kolejność zastosowania filtrów jest istotna, np. "Neg Kol r" daje inny efekt niż "Kol r Neg".
    Każda sekwencja filtrów dla każdej kopii obrazu powinna się rozpoczynać od numeru tej kopii w siatce kopii poprzedzonego znakiem @. 
    Na przykład dla siatki domyślnej zdefiniowanej w treści zadania będzie to:
    
        "@1 Sz Neg @2 Oy Kol r @3 Kol g @4 Oy Kol b Neg"
        
    Oczywiście zamiast spacji mogą być dowolne białe znaki: \n, \t i inne.
*/


using namespace std;

#include <string>
#include <fstream>
#include <iostream>

/// Klasy
#include "Obraz.h"

/// Zmienne globalne
#include "Globalne.h"

/// Filtry (transformacje)
#include "Filtry.h"

/// Podprogramy
#include "SUB01.h"
#include "SUB02.h"
#include "SUB03.h"

int main(int argc, char *argv[])
{
    /// obsługa argumentów wywołania
    Pliki::sPlikINI = (argc > 1? argv[1] : "");
    
    /// skonstruowanie obiektu ObrazORY i MacierzFiltrow
    SUB01::WczytajINIOrazObrazOrazMacierzFiltrow();

    /// skonstruowanie tablicy obiektów ObrazKOP - renderowanie kopii
    SUB02::RenderujKopie();

    /// skonstruowanie obiektu ObrazWYN - sklejenie kopii i usuwanie śmieci
    SUB03::SklejKopie();

    return 0;
};
