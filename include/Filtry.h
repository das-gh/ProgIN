/*  DZIAŁANIE
    =========
    Filtry otrzymują wskaźnik do obiektu typu cObraz, dzięki któremu pobierają potrzebne informacje - format, szerokość, 
    wysokość, głębię oraz oczywiście bitmapę.
    Aby dodać nowy filtr, należy ustalić jego nazwę kodową, zmodyfikować parser kodów, zmodyfikować funkcję ręcznego
    wprowadzania macierzy filtrów oraz umieścić deklarację funkcji filtra tutaj, a ciało tej funkcji w pliku .cpp.
*/
/*  OPIS FILTRÓW W PLIKU MAIN.CPP!
*/

#pragma once /* żeby nie włączać pliku wielokrotnie przez przypadek */

#include "Obraz.h"
#include <string>

namespace Filtry
{
    void Parser(cObraz* Obr, const std::string& KodFiltrow);
    /*  Parser kodów filtrów, wywołujący odpowiednie funkcje filtrów.
    
        Np.     
                KodFiltrow = "@3 Neg Sz Kol g";
                Parser(ObrazKOP, KodFiltrow);
                
        spowoduje wywołanie:
        
            Negatyw(ObrazKOP);
            Szarosc(ObrazKOP);
            UsunKolor(ObrazKOP, "g");
    */
    
    void OdbicieOy(cObraz* Obr);
    
    void Negatyw(cObraz* Obr);
    
    void Szarosc(cObraz* Obr);
    
    void UsunKolor(cObraz* Obr, const std::string& Kolory);
};
