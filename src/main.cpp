//-----------------------------------------------------------//
//                        TP Huffman
//
// Uso: huffman <archivo> [<archivo> ...]
//   - Si el archivo NO termina en ".huf" se comprime y se genera <archivo>.huf
//   - Si el archivo termina en ".huf" se descomprime y se restaura el original
//-----------------------------------------------------------//

#include <iostream>
#include <string>
#include "funciones/strings.hpp"
#include "huffman/Compresor.hpp"
#include "huffman/Descompresor.hpp"

using std::string;
using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char** argv){
   if(argc < 2){
      cerr << "Uso: " << argv[0] << " <archivo> [<archivo> ...]" << endl;
      cerr << "  Comprime los archivos indicados (genera <archivo>.huf)." << endl;
      cerr << "  Los archivos que terminan en .huf se descomprimen." << endl;
      return 1;
   }

   int errores = 0;

   for(int i = 1; i<argc; i++){
      string fName = argv[i];

      if(!endsWith(fName, ".huf")){
         if(comprimir(fName)){
            cout << "Comprimido:    " << fName << " -> " << fName << ".huf" << endl;
         }
         else{
            errores++;
         }
      }
      else{
         if(descomprimir(fName)){
            cout << "Descomprimido: " << fName << " -> " << fName.substr(0, fName.length()-4) << endl;
         }
         else{
            errores++;
         }
      }
   }

   return errores == 0 ? 0 : 1;
}
