#ifndef HUFFMAN_DESCOMPRESOR_HPP
#define HUFFMAN_DESCOMPRESOR_HPP

#include <stdio.h>
#include <string>
#include "huffman/HuffmanTree.hpp"

// Reconstruye el árbol de Huffman a partir del encabezado del archivo comprimido.
// Al terminar, el archivo queda posicionado justo después de los registros (c, h, m).
HuffmanTreeInfo* armarArbolHuffman(FILE* fHuffman);

// Descomprime fName (que debe terminar en ".huf") y restaura el archivo original
// con el mismo nombre sin la extensión. Si ese archivo existe, se sobrescribe.
// Retorna false si no se pudo abrir alguno de los archivos.
bool descomprimir(const std::string& fName);

#endif
