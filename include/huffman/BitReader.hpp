#ifndef HUFFMAN_BITREADER_HPP
#define HUFFMAN_BITREADER_HPP

#include <stdio.h>
#include <string>

// TAD BitReader: lee un archivo bit a bit (del bit más significativo al menos significativo).

struct BitReader{
   FILE* f;
   int idxBit;         // Próximo bit a entregar dentro de cadena (8 = hay que leer otro byte)
   std::string cadena; // Último byte leído expresado en binario, por ejemplo "01000001"
};

// Retorna los 8 bits de c como string, por ejemplo 65 -> "01000001".
std::string binToString(unsigned char c);

BitReader bitReader(FILE* fi);

// Retorna el siguiente bit del archivo (0 o 1).
int bitReaderRead(BitReader& br);

#endif
