#include "huffman/HuffmanTree.hpp"

using std::string;

HuffmanTreeInfo* huffmanTreeInfo(int cByte, long ocurrencia, HuffmanTreeInfo* izq, HuffmanTreeInfo* der){
   HuffmanTreeInfo* node = new HuffmanTreeInfo();
   node->cByte = cByte;
   node->ocurrencia = ocurrencia;
   node->izq = izq;
   node->der = der;
   return node;
}

void huffmanTreeFree(HuffmanTreeInfo* raiz){
   if(raiz != NULL){
      huffmanTreeFree(raiz->izq);
      huffmanTreeFree(raiz->der);
      delete raiz;
   }
}

// init function
HuffmanTree huffmanTree(HuffmanTreeInfo* root){
   HuffmanTree tu;
   tu.pila = stack<HuffmanTreeInfo*>();
   tu.pilaCod = stack<string>();
   stackPush<HuffmanTreeInfo*>(tu.pila, root);
   stackPush<string>(tu.pilaCod, "");
   return tu;
}

bool huffmanTreeHasNext(HuffmanTree ht){
   return !stackIsEmpty<HuffmanTreeInfo*>(ht.pila);
}

HuffmanTreeInfo* huffmanTreeNext(HuffmanTree& ht, string& cod){
   bool hoja = false;
   HuffmanTreeInfo* p = NULL;

   while(!stackIsEmpty<HuffmanTreeInfo*>(ht.pila) && !hoja){
      p = stackPop<HuffmanTreeInfo*>(ht.pila);
      string zz = stackPop<string>(ht.pilaCod);

      // Se apila primero el hijo derecho para que el izquierdo se procese antes.
      if(p->der != NULL){
         stackPush<HuffmanTreeInfo*>(ht.pila, p->der);
         stackPush<string>(ht.pilaCod, zz+"1");
      }

      if(p->izq != NULL){
         stackPush<HuffmanTreeInfo*>(ht.pila, p->izq);
         stackPush<string>(ht.pilaCod, zz+"0");
      }

      if(p->izq == NULL && p->der == NULL){
         hoja = true;
         cod = zz;
      }
      else{
         p = NULL;
      }
   }

   return p;
}
