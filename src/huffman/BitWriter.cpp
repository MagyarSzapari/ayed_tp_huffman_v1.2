#include "huffman/BitWriter.hpp"

#include "funciones/files.hpp"
#include "funciones/strings.hpp"

using std::string;

BitWriter bitWriter(FILE* fi){
   BitWriter bw;
   bw.cadena = "";
   bw.f = fi;
   return bw;
}

void bitWriterWrite(BitWriter& bw, int bit){
   bw.cadena += intToString(bit);

   // Cuando juntamos 8 bits los convertimos a un byte y lo grabamos.
   if(length(bw.cadena) == 8){
      unsigned char b = (unsigned char)stringToInt(bw.cadena, 2);
      write<unsigned char>(bw.f, b);
      bw.cadena = "";
   }
}

void bitWriterWrite(BitWriter& bw, string bits){
   int i = 0;
   while(i<length(bits)){
      int bit = charToInt(bits[i]);
      bitWriterWrite(bw, bit);
      i++;
   }
}

void bitWriterFlush(BitWriter& bw){
   int largo = length(bw.cadena);

   if(largo%8 != 0){
      int ceros = 8-largo%8;

      while(ceros != 0){
         bitWriterWrite(bw, 0);
         ceros--;
      }
   }
}
