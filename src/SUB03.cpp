#include "Globalne.h"
#include "SUB03.h"

using namespace Obrazy;

/*  Funkcja tworzy n ruchomych wskaźników, każdy dla Bitmapy każdej Kopii.
    Wskaźniki przekazują po kolei daną linię z każdej kolejnej kopii. 
    Np. pBitmapaKOP[3] to wskaźnik przemierzający bitmapę Kopii nr 4.
*/
void SUB03::SklejKopie(void)
{
    ///     ustalanie optymalnego formatu
    bool JestObrazRGB = false;
    for(int IxKopii0 = 0 ; IxKopii0 < ZG::nn() && !JestObrazRGB ; IxKopii0++)
        if(ObrazKOP[IxKopii0]->FormatOptymalny == "P3")
            JestObrazRGB = true;
    
    ///     konwertowanie kopii, by miały ten sam format
    for(int IxKopii0 = 0 ; IxKopii0 < ZG::nn() ; IxKopii0++)
    {
        if(JestObrazRGB)
            ObrazKOP[IxKopii0]->KonwertujNaRGB();
        else
            ObrazKOP[IxKopii0]->KonwertujNaSzarosc();
    }
    
    ///     deklaracja tablicy wskaźników do bitmap
    int** pBitmapaKOPWWierszuSiatki = new int* [ZG::WymiarSiatkiKopii_n];   /// w wierszu jest tylko n obrazków
    
    ///     tworzenie obrazu wynikowego
    ObrazWYN = new cObraz(JestObrazRGB? "P3" : "P2", JestObrazRGB? "P3" : "P2", \
                                    ObrazKOP[0]->Get_Szerokosc() * ZG::WymiarSiatkiKopii_n, \
                                    ObrazKOP[0]->Get_Wysokosc() * ZG::WymiarSiatkiKopii_n, \
                                    ObrazKOP[0]->Get_Glebia(), Pliki::sPlikWYN);
    ObrazWYN->ID = cObraz::IDs::WYN;
    
    int* pBitmapaWYN = ObrazWYN->Bitmapa;
    /*      ALGORYTM
            Jak wszędzie indziej, zero na końcu oznacza "zero-based index", czyli to, że pierwszą wartością
                    indeksu jest 0 a nie 1. Analogicznie z jedynką na końcu.
            Są Wiersze Siatki i Wiersze Obrazu. Wiersz Siatki składa się z obrazów. Wiersz Obrazu składa się
                    z pikseli.
            * * *
            Przemierzamy siatkę obrazów Wierszami Siatki. 
            IxWierszaSiatki0 = 3 oznacza, że jesteśmy w 4 wierszu obrazków składających się na obraz wynikowy.
            Tych Wierszy Siatki jest n czyli ZG::WymiarSiatkiKopii_n, bo mamy siatkę n x n obrazów.
            W obrębie każdego Wiersza Siatki działają wskaźniki, po jednym dla każdego obrazu w Wierszu Siatki. 
            pBitmapaKOPWWierszuSiatki[5] to wskaźnik do szóstego obrazka w danym Wierszu Siatki (sic!!).
            Wskaźniki odczytują z obrazów kolejne Wiersze Obrazu, które zlicza IxWierszaObrazu0.
            Wskaźniki są "uruchamiane" po kolei, tzn. najpierw dla pierwszego obrazu w danym Wierszu Siatki,
                    potem dla drugiego itd.
            Kiedy wskaźniki przemierzą wszystkie Wiersze Obrazów w danym Wierszu Siatki, przenoszone są na kolejny
                    Wiersz Siatki.
            px_GREY_RGB0 to indeks danego piksela w Wierszu Obrazu. 
            "GREY_RGB" oznacza, że może to być zarówno szary piksel jak i subpiksel piksela kolorowego.
    */
/// dla każdego Wiersza Siatki
    for(int IxWierszaSiatki0 = 0 ; IxWierszaSiatki0 < ZG::WymiarSiatkiKopii_n ; IxWierszaSiatki0++)
    {
/// ustaw wskaźniki we właściwych obrazkach
        for(int IxKopiiWWierszu0 = 0 ; IxKopiiWWierszu0 < ZG::WymiarSiatkiKopii_n ; IxKopiiWWierszu0++)
            pBitmapaKOPWWierszuSiatki[IxKopiiWWierszu0] = ObrazKOP[(IxWierszaSiatki0*ZG::WymiarSiatkiKopii_n) + IxKopiiWWierszu0]->Bitmapa;

/// dla każdego Wiersza Obrazu            
        for(int IxWierszaObrazu0 = 0 ; IxWierszaObrazu0 < ObrazKOP[0]->Get_Wysokosc() ; IxWierszaObrazu0++)
/// dla każdego obrazka w Wierszu Siatki
            for(int IxKopiiWWierszu0 = 0 ; IxKopiiWWierszu0 < ZG::WymiarSiatkiKopii_n ; IxKopiiWWierszu0++)
/// dla każdego piksela w Wierszu Obrazu
                for(int px_GREY_RGB0 = 0 ; px_GREY_RGB0 < ObrazKOP[0]->Get_Szerokosc() * (ObrazWYN->FormatOptymalny == "P2"? 1 : 3) ; px_GREY_RGB0++)
/// kopiuj dany piksel
                    *(pBitmapaWYN++) = *(pBitmapaKOPWWierszuSiatki[IxKopiiWWierszu0]++);
/// wskaźniki same osiągają koniec Wiersza Obrazu i przechodzą do następnego
    }
        
    ObrazWYN->ZapiszDoPliku();
        
    ///     zwalnianie dynamicznie alokowanych obszarów z SUB; metody z plików Filtry i Obraz nie zostawiają za sobą takich obszarów
    delete ObrazORY;
    delete ObrazWYN;
    for(int i = 0 ; i < ZG::nn() ; i++)
        delete ObrazKOP[i];
    delete pBitmapaKOPWWierszuSiatki;
};
