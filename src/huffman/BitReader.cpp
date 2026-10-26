#include "huffman/BitReader.hpp"

#include "funciones/files.hpp"
#include "funciones/strings.hpp"

using std::string;

string binToString(unsigned char c){
   int desordenado[8];
   string cadena;

   // Obtenemos los bits del menos significativo al más significativo...
   for(int i = 0; i<8; i++){
      desordenado[i] = c%2;
      c = c/2;
   }

   // ...y los concatenamos en el orden inverso.
   for(int k = 7; k>=0; k--){
      cadena += intToString(desordenado[k]);
   }

   return cadena;
}

BitReader bitReader(FILE* fi){
   BitReader br;
   br.idxBit = 8;
   br.f = fi;
   br.cadena = "";
   return br;
}

int bitReaderRead(BitReader& br){
   // Si ya entregamos los 8 bits del byte actual, leemos el siguiente.
   if(br.idxBit >= 8){
      br.idxBit = 0;
      unsigned char a = read<unsigned char>(br.f);
      br.cadena = binToString(a);
   }

   int ret = charToInt(br.cadena[br.idxBit]);
   br.idxBit++;

   return ret;
}
