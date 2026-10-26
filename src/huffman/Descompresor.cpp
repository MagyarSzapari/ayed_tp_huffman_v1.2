#include "huffman/Descompresor.hpp"

#include <iostream>
#include "funciones/files.hpp"
#include "funciones/strings.hpp"
#include "huffman/BitReader.hpp"

using std::string;
using std::cerr;
using std::endl;

static void addHijoDerecho(HuffmanTreeInfo*& aux, int registroC, int registroH){
   if(registroH == 1){ //Si el registroH == 1, entonces estamos por colocar la hoja.
      aux->der = huffmanTreeInfo(0, 0, NULL, NULL);
      aux->der->cByte = registroC; //Agregamos el Byte o carácter que le corresponde a la hoja.
      aux = aux->der; //Nos posicionamos en el siguiente nodo del árbol.
   }
   else if(aux->der == NULL){ //Si el registroH != 1 es porque todavía hay que seguir
                              //agregando nodos antes de llegar a la hoja.
      aux->der = huffmanTreeInfo(0, 0, NULL, NULL);
      aux = aux->der; //Nos posicionamos en el siguiente nodo del árbol.
   }
   else{ //Si aux->der != NULL quiere decir que el hijo derecho ya fue creado por otro código.
      aux = aux->der; //Nos posicionamos en el siguiente nodo del árbol.
   }
}

static void addHijoIzquierdo(HuffmanTreeInfo*& aux, int registroC, int registroH){
   if(registroH == 1){ //Si el registroH == 1, entonces estamos por colocar la hoja.
      aux->izq = huffmanTreeInfo(0, 0, NULL, NULL);
      aux->izq->cByte = registroC; //Agregamos el Byte o carácter que le corresponde a la hoja.
      aux = aux->izq; //Nos posicionamos en el siguiente nodo del árbol.
   }
   else if(aux->izq == NULL){ //Si el registroH != 1 es porque todavía hay que seguir
                              //agregando nodos antes de llegar a la hoja.
      aux->izq = huffmanTreeInfo(0, 0, NULL, NULL);
      aux = aux->izq; //Nos posicionamos en el siguiente nodo del árbol.
   }
   else{ //Si aux->izq != NULL quiere decir que el hijo izquierdo ya fue creado por otro código.
      aux = aux->izq; //Nos posicionamos en el siguiente nodo del árbol.
   }
}

HuffmanTreeInfo* armarArbolHuffman(FILE* fHuffman){
   seek<unsigned char>(fHuffman, 0); //Nos posicionamos al inicio del archivo comprimido.
   unsigned char cH = read<unsigned char>(fHuffman); //Obtengo el byte con la cantidad de hojas.
   int cantHojas = cH;

   //La cantidad de hojas se graba en un unsigned char (0..255). Cuando el archivo
   //original contiene los 256 bytes posibles, el valor grabado es 0 (256 no entra).
   if(cantHojas == 0){
      cantHojas = 256;
   }
   HuffmanTreeInfo* raiz = huffmanTreeInfo(0, 0, NULL, NULL);

   //Mientras queden registros (hojas) por leer...
   while(cantHojas > 0){
      HuffmanTreeInfo* aux = raiz;
      unsigned char rC = read<unsigned char>(fHuffman); //Leemos el byte (carácter) del registro.
      int registroC = rC;
      unsigned char rH = read<unsigned char>(fHuffman); //Leemos la longitud del código del registro.
      int registroH = rH;
      //El código m ocupa los bytes siguientes; un BitReader nuevo arranca en el primero de ellos.
      BitReader br = bitReader(fHuffman);

      //Recorremos (y creamos si hace falta) el camino que indica cada bit del código.
      while(registroH > 0){
         int bit = bitReaderRead(br); //Leemos el siguiente bit de m.

         if(bit == 1){ //Si el bit es 1...
            addHijoDerecho(aux, registroC, registroH); //Bajamos (o creamos) por la derecha.
         }
         if(bit == 0){ //Si el bit es 0...
            addHijoIzquierdo(aux, registroC, registroH); //Bajamos (o creamos) por la izquierda.
         }

         registroH--;
      }

      cantHojas--;
   }

   return raiz;
}

//Obtenemos el nombre del archivo original (quitamos la extensión ".huf").
static string crearArchivoDescomprimir(const string& fName){
   return fName.substr(0, length(fName)-4);
}

//Avanza un nivel en el árbol según el bit. Si llega a una hoja retorna su byte; si no, -1.
static int recorrerArbol(HuffmanTreeInfo*& aux, int bit){
   if(bit == 1){ //Si el bit es 1...
      aux = aux->der;
   }
   else{ //Si el bit es 0...
      aux = aux->izq;
   }

   if(aux->der == NULL && aux->izq == NULL){
      return aux->cByte;
   }

   return -1;
}

static void restaurarArchivoOriginal(FILE* fHuffman, FILE* fDescomprimido,
      HuffmanTreeInfo* raiz, unsigned int sizeOriginal){
   BitReader br = bitReader(fHuffman);
   HuffmanTreeInfo* aux = raiz;
   unsigned int escritos = 0;

   //Restauramos exactamente sizeOriginal bytes: así no se interpretan como datos
   //los ceros de relleno que bitWriterFlush agregó al último byte.
   while(escritos < sizeOriginal){
      int bit = bitReaderRead(br); //Leemos el siguiente bit.
      if(feof(fHuffman)){ //El archivo comprimido terminó antes de lo esperado.
         cerr << "El archivo comprimido esta incompleto." << endl;
         return;
      }

      int cByteEnAscii = recorrerArbol(aux, bit); //Recorremos el árbol.

      if(cByteEnAscii >= 0){ //Si llegamos a una hoja...
         unsigned char cByte = (unsigned char)cByteEnAscii;
         write<unsigned char>(fDescomprimido, cByte); //Escribimos el byte en el archivo restaurado.
         aux = raiz; //Volvemos a la raíz para decodificar el siguiente byte.
         escritos++;
      }
   }
}

bool descomprimir(const string& fName){
   FILE* fHuffman = fopen(fName.c_str(), "rb"); //Abrimos el archivo comprimido.
   if(fHuffman == NULL){
      cerr << "No se pudo abrir el archivo '" << fName << "'." << endl;
      return false;
   }

   string sNameOriginal = crearArchivoDescomprimir(fName); //Obtengo el nombre del archivo original.
   FILE* fDescomprimido = fopen(sNameOriginal.c_str(), "w+b"); //Creamos el archivo donde restauraremos el original.
   if(fDescomprimido == NULL){
      cerr << "No se pudo crear el archivo '" << sNameOriginal << "'." << endl;
      fclose(fHuffman);
      return false;
   }

   //Un .huf vacío corresponde a un archivo original vacío.
   if(fileSize<unsigned char>(fHuffman) > 0){
      HuffmanTreeInfo* raiz = armarArbolHuffman(fHuffman); //Armamos el árbol a partir del archivo comprimido.
      unsigned int sizeOriginal = read<unsigned int>(fHuffman); //Leemos el tamaño del archivo original.
      restaurarArchivoOriginal(fHuffman, fDescomprimido, raiz, sizeOriginal); //Restauramos el archivo original.
      huffmanTreeFree(raiz);
   }

   fclose(fHuffman);
   fclose(fDescomprimido);
   return true;
}
