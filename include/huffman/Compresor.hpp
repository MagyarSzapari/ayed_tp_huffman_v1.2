#ifndef HUFFMAN_COMPRESOR_HPP
#define HUFFMAN_COMPRESOR_HPP

#include <stdio.h>
#include <string>
#include "huffman/HuffmanTree.hpp"
#include "tads/List.hpp"

// Paso 1: cuenta cuántas veces aparece cada byte (0..255) en el archivo.
void paso1(FILE* f, HuffmanTreeTable tabla[]);

// Paso 2: crea una lista con una hoja por cada byte presente, ordenada de
// menor a mayor por ocurrencia (y, a igual ocurrencia, por valor del byte).
List<HuffmanTreeInfo> paso2(HuffmanTreeTable tabla[]);

// Paso 3: arma el árbol de Huffman a partir de la lista y retorna su raíz.
HuffmanTreeInfo* paso3(List<HuffmanTreeInfo>& charList);

// Paso 4: recorre el árbol y guarda en la tabla el código Huffman de cada byte.
void cargarCodigoEnTabla(HuffmanTreeInfo* raiz, HuffmanTreeTable tabla[]);

// Comprime el archivo fName y genera fName + ".huf".
// Retorna false si no se pudo abrir alguno de los archivos.
bool comprimir(const std::string& fName);

#endif
