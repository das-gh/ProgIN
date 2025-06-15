#pragma once /* żeby nie włączać pliku wielokrotnie przez przypadek */

#include <string>
#include <fstream>


class cObraz
{
public:
    enum IDs { NIEPRZYPISANY = -2, WYN, ORY };
    int ID = IDs::NIEPRZYPISANY; // -std=C++11 / -std=gnu++11
     /* Numer identyfikacyjny. Oryginał ma numer 0, plik wynikowy -1. Kopie są numerowane kolejno, więc
        (zakładając ruch w prawo i w dół) aby uzyskać numer kolumny - użyć:
            ID % n
        zaś aby uzyskać numer wiersza - użyć:
            ID / n
    */

    ///     INICJALIZACJA obiektu poprzez...
    
    cObraz(const std::string& SciezkaOdczytu);          /// odczyt z pliku
    cObraz(const cObraz* Oryginalny);                   /// skopiowanie innego obrazu
    cObraz(const std::string& FormatAktualny, const std::string& _FormatOptymalny, int Szerokosc, int Wysokosc, int Glebia, \
           const std::string& SciezkaZapisu);           /// zadeklarowanie właściwości obrazu - bitmapa wypełniona zerami
    ~cObraz();

    void WczytajZPliku(const std::string& Sciezka);
    void WczytajZPliku(void);                           /// użyj ścieżki zapisanej już w obiekcie
    void KopiujObraz(const cObraz* Oryginalny);
    void ZapiszDoPliku(const std::string& Sciezka, const bool CRLF);
    void ZapiszDoPliku(const std::string& Sciezka);
    void ZapiszDoPliku(void);                           /// j.w.

    ///     DOSTĘP do danych obiektu

    inline int          Get_Szerokosc()         const   { return Szerokosc; };
    inline int          Get_Wysokosc()          const   { return Wysokosc; };
    inline int          Get_Glebia()            const   { return Glebia; };
    inline std::string  Get_FormatAktualny()    const   { return FormatAktualny; };
    inline int          WielkoscBitmapy()       const   { return (Szerokosc * Wysokosc * (FormatAktualny=="P3"? 3 : 1)); };
    
    /// dzięki tym "const" można przekazywać do funkcji "const cObraz*", tzn. one gwarantują, że dane metody nie zmieniają obiektu, 
    /// zatem obiekt może być kwalifikowany jako "const"
    
    ///     KONWERSJE
    
    void KonwertujNaSzarosc(void);                      /// zmienia format na P2 (usuwa kolor tak jak filtr)
    void KonwertujNaRGB(void);                          /// zmienia format na P3 (duplikuje piksele)

    ///     DANE PUBLICZNE

    std::string FormatOptymalny;                        /// wyjaśnienie poniżej
    
    int* Bitmapa;                                       /// po prostu surowe dane liczbowe
    
    std::string sPlik;                                  /// ścieżka dostępu
    
private:
    /*  FORMAT
        Powyższe zmienne mają wartości "P2" lub "P3".
        F. aktualny (FA) to format bitmapy. Określa on, co oznaczają liczby w niej przechowywane - w szczególności, ile liczb określa jeden piksel.
        F. optymalny (FO) to najniższy format będący w stanie pomieścić całą informację o obrazie. Zatem jeśli obraz jest zapisany w formacie P3, ale
            wszystkie składowe każdego piksela są równe, np. 0 0 0, 25 25 25 - to taki obraz ma FA = "P3", a FO = "P2".
            
        =====================
        FA  FO  Interpretacja
        =====================
        P2  P2  Szary obrazek zapisany w optymalnym formacie.
        P2  P3  Sytuacja niemożliwa; nawet jeśli obrazek był kolorowy, dane o kolorze zostały utracone.
        P3  P2  Szary obrazek zapisany w zbyt pojemnym formacie.
        P3  P3  Kolorowy obrazek zapisany w optymalnym formacie.
        
        FA jest zmieniany automatycznie, dlatego jest atrybutem prywatnym. Niezgodność FA z bitmapą oznaczałaby utratę integralności danych.
        FA ustawiany jest:
            -   w obrębie konstruktorów
            -   za pomocą metod:
                KonwertujNaSzarosc
                KonwertujNaRGB
            
        FO powinien być zmieniany przez wszystkie funkcje, które wiedzą, czy obrazek jest szary czy kolorowy, zatem:
            -   na P2/P3    przy inicjalizacji
            -   na P2       przy utracie kolorów
            -   na P2       przy konwersji na format
            -   na P2/P3    przy manipulacji bitmapą (obecnie taki filtr nie istnieje)
        FO ustawiany jest:
            -   przez konstruktory
            -   przez metody konwersji:
                KonwertujNaSzarosc
            -   przez filtry

        Filtry nie zmieniają FA, jedynie FO (na mocy umowy).
    */
    std::string FormatAktualny;

    int Szerokosc;
    int Wysokosc;
    int Glebia;                                         /// maksymalne nasycenie, to nie jest głębia w bitach

    std::fstream fPlik;                                 /// strumień plikowy
    
/*  Funkcja ustawia odpowiednie rozszerzenie dla formatu pliku - modyfikuje zmienną sPlik w zależności od zmiennej FormatAktualny.
    W tym celu najpierw sprawdza, czy plik ma jedno ze znanych rozszerzeń: pgm i ppm. Jeśli nie, to nie ma możliwości odróżnienia faktycznego
    rozszerzenia od nazwy, w której występuje kropka, np. 
        2015.01.09          czy .09 to rozszerzenie?
        plik1.rvg           czy .rvg to rozszerzenie?
        wyslij.ten          czy .ten to rozszerzenie?
        plik2.dobry         czy .dobry to rozszerzenie?
    itp. Dlatego jeśli plik nie ma jednego ze znanych rozszerzeń, uznaje się, że nie ma w ogóle rozszerzenia i trzeba je dokleić.
*/
    void UstawRozszerzenie(void);
};
