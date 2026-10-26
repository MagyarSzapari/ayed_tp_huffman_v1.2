#ifndef HUFFMAN_BITWRITER_HPP
#define HUFFMAN_BITWRITER_HPP

#include <stdio.h>
#include <string>

// TAD BitWriter: acumula bits y graba un byte en el archivo cada vez que junta 8.

struct BitWriter{
   std::string cadena; // Bits pendientes de grabar ('0' y '1')
   FILE* f;
};

BitWriter bitWriter(FILE* fi);

// Agrega un bit (0 o 1).
void bitWriterWrite(BitWriter& bw, int bit);

// Agrega una secuencia de bits expresada como string, por ejemplo "0110".
void bitWriterWrite(BitWriter& bw, std::string bits);

// Completa con ceros el último byte (si quedó incompleto) y lo graba.
void bitWriterFlush(BitWriter& bw);

#endif
