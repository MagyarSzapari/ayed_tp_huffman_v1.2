#include "huffman/Compresor.hpp"

#include <iostream>
#include "funciones/arrays.hpp"
#include "funciones/files.hpp"
#include "funciones/strings.hpp"
#include "huffman/BitWriter.hpp"

using std::string;
using std::cerr;
using std::endl;

// Registro que se graba en el encabezado del .huf por cada hoja del árbol.
struct Registros{
   unsigned int c; // Byte (carácter) que representa la hoja
   unsigned int h; // Longitud en bits de su código Huffman
   string m;       // Código Huffman completado con ceros hasta un múltiplo de 8
};

static int cmpInt(int a, int b){
   return a-b;
}

static int cmpHuffmanTreeInfo(HuffmanTreeInfo c1, HuffmanTreeInfo c2){
   //Comparo las ocurrencias de cada caracter.
   int a = cmpInt(c1.ocurrencia, c2.ocurrencia);

   //Si tienen la misma ocurrencia...
   if(a == 0){
      //Comparo los caracteres que representan.
      a = cmpInt(c1.cByte, c2.cByte);
   }

   return a;
}

void paso1(FILE* f, HuffmanTreeTable tabla[]){
   //Creo la tabla del árbol vacía.
   for(int i = 0; i<256; i++){
      tabla[i].cont = 0;
      tabla[i].code = "";
   }

   //Leemos el primer byte antes de preguntar por feof: feof recién es verdadero
   //después de un intento de lectura fallido.
   unsigned char c = read<unsigned char>(f);

   //Mientras el archivo tenga elementos...
   while(!feof(f)){
      tabla[c].cont++; //Aumento la cantidad de ocurrencias del carácter en su respectiva posición.
      c = read<unsigned char>(f); //Leemos el siguiente byte.
   }
}

List<HuffmanTreeInfo> paso2(HuffmanTreeTable tabla[]){
   List<HuffmanTreeInfo> charList = list<HuffmanTreeInfo>(); //Creamos una lista.

   for(int i = 0; i<256; i++){
      if(tabla[i].cont > 0){ //Si el Byte tiene ocurrencia, entra; sino, pasamos al siguiente.
         //Creamos un HuffmanTreeInfo con los datos de cada Byte y sus ocurrencias.
         HuffmanTreeInfo carac;
         carac.cByte = i;
         carac.ocurrencia = tabla[i].cont;
         carac.izq = NULL;
         carac.der = NULL;
         listAdd<HuffmanTreeInfo>(charList, carac); //Agregamos el caracter a la lista.
      }
   }

   listSort<HuffmanTreeInfo>(charList, cmpHuffmanTreeInfo); //Ordenamos la lista.
   return charList;
}

HuffmanTreeInfo* paso3(List<HuffmanTreeInfo>& charList){
   //Los nodos internos se numeran desde 256 para no confundirse con un byte real.
   int i = 256;

   //Mientras queden al menos dos elementos en la lista...
   while(listSize<HuffmanTreeInfo>(charList) > 1){
      //Removemos el primer elemento de la lista (el de menor ocurrencia): será el hijo derecho.
      HuffmanTreeInfo* huffInfoDer = huffmanTreeInfo(0, 0, NULL, NULL);
      *huffInfoDer = listRemoveFirst<HuffmanTreeInfo>(charList);
      //Removemos el siguiente elemento de la lista: será el hijo izquierdo.
      HuffmanTreeInfo* huffInfoIzq = huffmanTreeInfo(0, 0, NULL, NULL);
      *huffInfoIzq = listRemoveFirst<HuffmanTreeInfo>(charList);
      //El nuevo nodo tiene como ocurrencia la suma de las ocurrencias de sus hijos.
      unsigned int ocurrencia = huffInfoIzq->ocurrencia+huffInfoDer->ocurrencia;
      HuffmanTreeInfo huffInfoNuevo;
      huffInfoNuevo.cByte = i;
      huffInfoNuevo.ocurrencia = ocurrencia;
      huffInfoNuevo.izq = huffInfoIzq;
      huffInfoNuevo.der = huffInfoDer;
      listOrderedInsert<HuffmanTreeInfo>(charList, huffInfoNuevo, cmpHuffmanTreeInfo); //Lo insertamos en orden.
      i++;
   }

   //El único elemento que queda en la lista es la raíz del árbol.
   HuffmanTreeInfo* raiz = huffmanTreeInfo(0, 0, NULL, NULL);
   *raiz = listRemoveFirst<HuffmanTreeInfo>(charList);

   return raiz;
}

void cargarCodigoEnTabla(HuffmanTreeInfo* raiz, HuffmanTreeTable tabla[]){
   //Caso borde: si el archivo tiene un único byte distinto, la raíz es una hoja y su
   //código quedaría vacío. Le asignamos "0" para poder codificarlo.
   if(raiz->izq == NULL && raiz->der == NULL){
      tabla[raiz->cByte].code = "0";
      return;
   }

   //Obtengo el árbol a partir de su raíz.
   HuffmanTree arbol = huffmanTree(raiz);

   while(huffmanTreeHasNext(arbol)){ //Recorremos el arbol hasta que no queden hojas.
      string x = "";
      HuffmanTreeInfo* t = huffmanTreeNext(arbol, x); //Obtenemos un puntero a la siguiente hoja.
      int pos = t->cByte; //Usaremos el numero en ASCII del Byte para acceder a la posicion del array de tablas.
      tabla[pos].code = x; //Guardamos el código en la fila de la tabla del Byte.
   }
}

static Registros registros(){
   Registros regis;
   regis.c = 0;
   regis.h = 0;
   regis.m = "";
   return regis;
}

static FILE* crearFile(const string& fName){
   //Creamos el archivo sobre el cual escribiremos la compresión: <nombre original>.huf
   string nuevo = fName+".huf";
   return fopen(nuevo.c_str(), "w+b");
}

static FILE* obtenerOriginalFile(const string& fName){
   //Abrimos para leer el archivo original.
   return fopen(fName.c_str(), "rb");
}

static string escribirM(string code){
   //Completamos el código con ceros a la derecha hasta que su longitud sea múltiplo de 8.
   while(length(code)%8 != 0){
      code += "0";
   }

   return code;
}

static void bytesToChar(string code, unsigned char bytes[]){
   //Convertimos cada grupo de 8 caracteres '0'/'1' en el byte que representan.
   string aux = "";

   for(int i = 0, j = 0; i<length(code); i++){
      aux += code[i];
      if((i+1)%8 == 0){
         bytes[j] = (unsigned char)stringToInt(aux, 2);
         aux = "";
         j++;
      }
   }
}

static void comprimirArchivoOriginal(FILE* fComprimido, FILE* fOriginal, HuffmanTreeTable arrTabla[]){
   //Nos posicionamos al inicio del archivo original para recorrerlo por segunda vez.
   seek<unsigned char>(fOriginal, 0);
   //El BitWriter junta los bits de los códigos y graba un byte cada vez que completa 8.
   BitWriter bw = bitWriter(fComprimido);
   //Leo el byte correspondiente.
   unsigned char cByteLeido = read<unsigned char>(fOriginal);

   //Mientras el archivo tenga elementos...
   while(!feof(fOriginal)){
      //Obtengo el código Huffman correspondiente al byte.
      string codeHuffman = arrTabla[cByteLeido].code;
      //Escribimos sus bits en el archivo comprimido.
      bitWriterWrite(bw, codeHuffman);
      //Leo el siguiente byte del archivo.
      cByteLeido = read<unsigned char>(fOriginal);
   }

   //Completamos con ceros el último byte.
   bitWriterFlush(bw);
}

static void escribirArchivoComprimido(FILE* fComprimido, FILE* fOriginal, int contHojas,
      Registros arrRegistros[], HuffmanTreeTable arrTabla[], int lenRegistro){
   //La cantidad de hojas se graba en 1 byte. Un unsigned char va de 0 a 255, así que
   //el caso de 256 hojas (aparecen todos los bytes posibles) queda grabado como 0.
   unsigned char hojas = (unsigned char)contHojas;
   write<unsigned char>(fComprimido, hojas);

   //Grabamos un registro (c, h, m) por cada hoja del árbol.
   for(int pos = 0; pos<lenRegistro; pos++){
      Registros reg = arrRegistros[pos];
      unsigned char c = (unsigned char)reg.c;
      unsigned char h = (unsigned char)reg.h;
      unsigned char m[32]; //Un código tiene a lo sumo 255 bits (32 bytes).
      bytesToChar(reg.m, m); //Transformamos el code de binario a bytes.
      write<unsigned char>(fComprimido, c);
      write<unsigned char>(fComprimido, h);
      for(int j = 0, hCont = reg.h; j<32 && hCont>0; j++, hCont -= 8){
         write<unsigned char>(fComprimido, m[j]);
      }
   }

   //Obtenemos el tamaño del archivo original y lo grabamos (4 bytes).
   unsigned int tamanio = fileSize<unsigned char>(fOriginal);
   write<unsigned int>(fComprimido, tamanio);
   comprimirArchivoOriginal(fComprimido, fOriginal, arrTabla); //Comprimimos el archivo original.
}

static void grabarArchivoComprimido(HuffmanTreeTable arrTabla[], FILE* fComprimido, FILE* fOriginal){
   Registros arrRegistros[256]; //Array que va a guardar un registro por cada hoja del árbol.
   int contHojas = 0;
   int len = 0;

   for(int i = 0; i<256; i++){
      if(arrTabla[i].cont > 0){ //Si el Byte tiene ocurrencia, entra; sino, pasamos al siguiente.
         Registros registro = registros();
         registro.c = i;
         registro.h = length(arrTabla[i].code);
         registro.m = escribirM(arrTabla[i].code);
         add<Registros>(arrRegistros, len, registro); //Agregamos el registro al array.
         contHojas++;
      }
   }

   escribirArchivoComprimido(fComprimido, fOriginal, contHojas, arrRegistros, arrTabla, len);
}

bool comprimir(const string& fName){
   FILE* fOriginal = obtenerOriginalFile(fName); //Obtenemos el archivo original.
   if(fOriginal == NULL){
      cerr << "No se pudo abrir el archivo '" << fName << "'." << endl;
      return false;
   }

   FILE* fComprimido = crearFile(fName); //Creamos un archivo para escribir la compresión.
   if(fComprimido == NULL){
      cerr << "No se pudo crear el archivo '" << fName << ".huf'." << endl;
      fclose(fOriginal);
      return false;
   }

   HuffmanTreeTable tabla[256]; //Tabla con las ocurrencias y el código de cada byte.
   paso1(fOriginal, tabla);

   //Un archivo vacío no tiene bytes que codificar: el .huf también queda vacío.
   if(fileSize<unsigned char>(fOriginal) > 0){
      List<HuffmanTreeInfo> charList = paso2(tabla); //Obtenemos la lista ordenada de hojas.
      HuffmanTreeInfo* raiz = paso3(charList); //Obtenemos el árbol (particularmente su raíz) a partir de la lista.
      cargarCodigoEnTabla(raiz, tabla); //Cargamos el código Huffman del árbol en la tabla.
      grabarArchivoComprimido(tabla, fComprimido, fOriginal); //Comprimimos el archivo original.
      huffmanTreeFree(raiz);
   }

   fclose(fOriginal);
   fclose(fComprimido);
   return true;
}
