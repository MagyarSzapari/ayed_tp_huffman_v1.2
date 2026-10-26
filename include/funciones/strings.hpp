#ifndef FUNCIONES_STRINGS_HPP
#define FUNCIONES_STRINGS_HPP

#include <string>

// Subconjunto de la biblioteca de strings de la cátedra que utiliza el TP.

int length(std::string s);
int charToInt(char c);
char intToChar(int i);
int digitCount(int n);
std::string reverseStr(std::string s);
std::string intToString(int i);
int stringToInt(std::string s, int b);
bool endsWith(std::string mainStr, std::string toMatch);

#endif
