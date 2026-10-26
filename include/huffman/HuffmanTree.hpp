#ifndef HUFFMAN_HUFFMANTREE_HPP
#define HUFFMAN_HUFFMANTREE_HPP

#include <string>
#include "tads/Stack.hpp"

// Info de los nodos del árbol.
struct HuffmanTreeInfo{
   unsigned int cByte;      // Byte de la hoja (los nodos internos usan valores >= 256)
   unsigned int ocurrencia; // Apariciones del byte (en nodos internos: suma de los hijos)
   HuffmanTreeInfo* izq;    // Hijo izquierdo: se llega con el bit 0
   HuffmanTreeInfo* der;    // Hijo derecho: se llega con el bit 1
};

HuffmanTreeInfo* huffmanTreeInfo(int cByte, long ocurrencia, HuffmanTreeInfo* izq, HuffmanTreeInfo* der);

// Libera todos los nodos del árbol.
void huffmanTreeFree(HuffmanTreeInfo* raiz);

// Fila de la tabla de bytes: cantidad de ocurrencias y código Huffman asignado.
struct HuffmanTreeTable{
   int cont;
   std::string code;
};

// ---------------------
// -- TAD HuffmanTree --
// Iterador que recorre las hojas del árbol (en profundidad, usando pilas)
// y retorna, junto con cada hoja, el camino desde la raíz ("0" = izq, "1" = der).

struct HuffmanTree{
   Stack<HuffmanTreeInfo*> pila;
   Stack<std::string> pilaCod;
};

HuffmanTree huffmanTree(HuffmanTreeInfo* root);
bool huffmanTreeHasNext(HuffmanTree ht);
HuffmanTreeInfo* huffmanTreeNext(HuffmanTree& ht, std::string& cod);

#endif
