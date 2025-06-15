#pragma once /* żeby nie włączać pliku wielokrotnie przez przypadek */

#include <string>

/// elem = 0 to pierwszy element; zwrócenie pustego łańcucha oznacza koniec
std::string Split(const std::string& s, int elem);
