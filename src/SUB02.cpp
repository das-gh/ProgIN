#include "Globalne.h"
#include "SUB02.h"
#include "Filtry.h"

using namespace Obrazy;

void SUB02::RenderujKopie(void)
{
    /*      ObrazKOP jest tablicą wskaźników do obiektów cObraz.
            Każdy wskaźnik wskazuje na dynamicznie alokowany obiekt.
            Nie potrzeba tworzyć ruchomego wskaźnika do tablicy ObrazKOP, ponieważ kopii nie ma wiele,
                    więc nie zyska się na takim wskaźniku wiele czasu obliczeniowego.
            *ObrazKOP[0] to obiekt cObraz reprezentujący Kopię nr 1.
    */
    ObrazKOP = new cObraz* [ZG::nn()];      /// tablica zostanie usunięta po utworzeniu ObrazWYN
    cObraz** pObrazKOP = ObrazKOP;          /// wsk. ruchomy
    
    int IxKopii1;                           /// tzn. że indeks zaczyna się od 1
    for(IxKopii1 = 1 ; IxKopii1 <= ZG::nn() ; IxKopii1++, pObrazKOP++)
    {
        ObrazKOP[IxKopii1-1] = new cObraz(ObrazORY);
        ObrazKOP[IxKopii1-1]->ID = IxKopii1;
        Filtry::Parser(ObrazKOP[IxKopii1-1], ZG::MacierzFiltrow[IxKopii1-1]);
    }
    delete[] ZG::MacierzFiltrow;
};
