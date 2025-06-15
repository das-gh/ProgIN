#include "Filtry.h"
#include "Split.h"

#include <iostream>
#include <stdlib.h>

using namespace std;

void Filtry::Parser(cObraz* Obr, const string& KodFiltrow)
{
    string Slowo(" ");
    int IxSlowa0 = 0;
    while(!Slowo.empty())
    {
        Slowo = Split(KodFiltrow, IxSlowa0++);
        if(Slowo[0] == '@') continue;                   /// pomiń słowo
        if(Slowo == "Oy")
            Filtry::OdbicieOy(Obr);
        else if(Slowo == "Neg")
            Filtry::Negatyw(Obr);
        else if(Slowo == "Sz")
            Filtry::Szarosc(Obr);
        else if(Slowo == "Kol")
        {
            Slowo = Split(KodFiltrow, IxSlowa0++);
            Filtry::UsunKolor(Obr, Slowo);
        }
    }
};

void Filtry::OdbicieOy(cObraz* Obr)
{
    #ifdef DEBUG
    cout << "Debug: " << Obr->ID << ": Oy" << endl;
    #endif // DEBUG
    
    if(Obr->Get_FormatAktualny() == "P2")
    {
        int* pBitmapa_GREY;
        int* TmpBitmapa_GREY = new int [Obr->WielkoscBitmapy()];
        int* pTmpBitmapa_GREY;      
                                    
        ///     obraz -> tmp
        for(int IxWiersza0 = 0 ; IxWiersza0 < Obr->Get_Wysokosc() ; IxWiersza0++)               /// kolejne wiersze
        {
            pBitmapa_GREY      = Obr->Bitmapa + (IxWiersza0+1) * Obr->Get_Szerokosc() - 1;      /// wsk. idzie od końca
            pTmpBitmapa_GREY   = TmpBitmapa_GREY + IxWiersza0 * Obr->Get_Szerokosc();           /// wsk. idzie od początku

            for(int px_GREY = 0 ; px_GREY < Obr->Get_Szerokosc() ; px_GREY++)
                *(pTmpBitmapa_GREY++) = *(pBitmapa_GREY--);
        }
        
        ///     tmp -> obraz
        pTmpBitmapa_GREY = TmpBitmapa_GREY;
        pBitmapa_GREY = Obr->Bitmapa;
        for(int px_GREY = 0 ; px_GREY < Obr->WielkoscBitmapy() ; px_GREY++)
            *(pBitmapa_GREY++) = *(pTmpBitmapa_GREY++);

        delete[] TmpBitmapa_GREY;
    }
    else if(Obr->Get_FormatAktualny() == "P3")
    {
        int* pBitmapa_RGB;
        int* TmpBitmapa_RGB = new int [Obr->WielkoscBitmapy()];
        #ifdef DEBUG
        cout << "Debug: WB" << Obr->WielkoscBitmapy();//debug
        #endif // DEBUG
        int* pTmpBitmapa_RGB;
        
        ///     obraz -> tmp
        /*  Załóżmy, że bitmapa ma 5 x 2 x 3 = 30 wartości int adresowanych od 0. Kopiujemy do tmp następująco:
            wrs0   offset       bmp     -> tmp
            0      -2     14    12      ->  0
            0      -1     14    13      ->  1
            0       0     14    14      ->  2
            0      -2     11     9      ->  3
            0      -1     11    10      ->  4
            0       0     11    11      ->  5
            itd.
            1      -2     29    27      -> 15
            1      -1     29    28      -> 16
            1       0     29    29      -> 17
            1      -2     26    24      -> 18
            1      -1     26    25      -> 19
            1       0     26    26      -> 20
        */
            
        for(int IxWiersza0 = 0 ; IxWiersza0 < Obr->Get_Wysokosc() ; IxWiersza0++)                   /// kolejne wiersze
        {
            pBitmapa_RGB = Obr->Bitmapa + (IxWiersza0+1) * Obr->Get_Szerokosc()*3 - 1;              /// wsk. idzie od końca
            pTmpBitmapa_RGB = TmpBitmapa_RGB + IxWiersza0 * Obr->Get_Szerokosc()*3;                 /// wsk. idzie od początku
            for(int px_RGB = 0 ; px_RGB < Obr->Get_Szerokosc()*3 ; px_RGB += 3, pBitmapa_RGB -= 3)
            {
                #ifdef DEBUG
                cout << "w" << IxWiersza0 << "bmp" << *pBitmapa_RGB;//debug
                #endif // DEBUG
                for(int sub_px_offset = 2 ; sub_px_offset >= 0 ; sub_px_offset--)                   /// bo tak wygodniej
                    *(pTmpBitmapa_RGB++) = *(pBitmapa_RGB - sub_px_offset);
            }
            cout << endl;//debug
        }
                
        ///     tmp -> obraz
        pTmpBitmapa_RGB = TmpBitmapa_RGB;
        pBitmapa_RGB = Obr->Bitmapa;
        
        for(int sub_px = 0 ; sub_px < Obr->WielkoscBitmapy() ; sub_px++)
            *(pBitmapa_RGB++) = *(pTmpBitmapa_RGB++);

        delete[] TmpBitmapa_RGB;
    }
    else
    {
        cout << "Blad: nieobslugiwany format";
        getchar();
        exit(0);
    }
};

void Filtry::Negatyw(cObraz* Obr)
{
    #ifdef DEBUG
    cout << "Debug: " << Obr->ID << ": Neg" << endl;
    #endif // DEBUG
    int* pBitmapa_GREY_RGB = Obr->Bitmapa;
    for(int sub_px_GREY_RGB = 0 ; sub_px_GREY_RGB < Obr->WielkoscBitmapy() ; sub_px_GREY_RGB++, pBitmapa_GREY_RGB++)
    {
        *pBitmapa_GREY_RGB *= -1;
        *pBitmapa_GREY_RGB += Obr->Get_Glebia();
    }
};

/*      Ten filtr działa wg tego samego algorytmu co funkcja cObraz.KonwertujNaSzarosc(), ale
            nie konwertuje formatu P3 na P2, tylko zapisuje obrazek w odcieniach szarości
            w formacie P3. Jest to przydatne, bo nie trzeba robić drugiej konwersji z powrotem
            na P3, jeśli się okaże, że inne kopie muszą być w formacie P3.
*/
void Filtry::Szarosc(cObraz* Obr)
{
    #ifdef DEBUG
    cout << "Debug: " << Obr->ID << ": Sz" << endl;
    #endif // DEBUG
    string frm(Obr->Get_FormatAktualny());
    if(frm == "P3")
    {
        Obr->FormatOptymalny = "P2";
        
        int* pBitmapa_RGB = Obr->Bitmapa;                                                       /// wsk. ruchomy
        int Srednia;
        for(int px_RGB = 0 ; px_RGB < Obr->WielkoscBitmapy() ; px_RGB += 3)
        {
            Srednia = (int)((pBitmapa_RGB[0] + pBitmapa_RGB[1] + pBitmapa_RGB[2]) / 3. + 0.5);  /// zaokrąglenie średniej
            for(int sub_px_offset = 0 ; sub_px_offset < 3 ; sub_px_offset++)
                *pBitmapa_RGB++ = Srednia;
        }
    }
    else cout << "Obrazek juz jest szary." << endl;
};

void Filtry::UsunKolor(cObraz* Obr, const string& Kolory)
{
    #ifdef DEBUG
    cout << "Debug: " << Obr->ID << ": Kol " << Kolory << endl;
    #endif // DEBUG
    if(Obr->Get_FormatAktualny() == "P2") return;                                               /// obrazek już jest szary
    
    
    ///     pobieżne sprawdzenie integralności danych
    if(Kolory.empty())
    {
        cout << "Blad: argument z kolorami do usuniecia jest pusty";
        getchar();
        exit(0);
    }
    for(int IxKoloru0 = 0 ; IxKoloru0 < Kolory.length() ; IxKoloru0++)
        switch(Kolory[IxKoloru0])
        {
        case 'R':
        case 'r':
        case 'G':
        case 'g':
        case 'B':
        case 'b':
            break;
        default:
            cout << "Blad: nieobslugiwany kolor >" << Kolory[IxKoloru0] << "<";
            getchar();
            exit(0);
        }
    
    ///     na mocy treści zadania, usunięcie 3 kolorów to wywołanie filtra Szarosc
    if(Kolory.length() > 2)                             /// można dodać obsługę błędów
        Szarosc(Obr);
    else
    {
        int OffsetKoloru = -1;
        for(int IxKoloru0 = 0 ; IxKoloru0 < Kolory.length() ; IxKoloru0++)
        {
            switch(Kolory[IxKoloru0])
            {
            case 'R':
            case 'r':
                OffsetKoloru = 0;
                break;
            case 'G':
            case 'g':
                OffsetKoloru = 1;
                break;
            case 'B':
            case 'b':
                OffsetKoloru = 2;
                break;
            default:
                continue;
            }
            int* pBitmapa_RGB = Obr->Bitmapa;
            for(int px_RGB = 0 ; px_RGB < Obr->WielkoscBitmapy() ; px_RGB += 3, pBitmapa_RGB += 3)
                pBitmapa_RGB[OffsetKoloru] = 0;
        }
    }
};
