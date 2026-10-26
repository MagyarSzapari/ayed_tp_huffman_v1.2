#ifndef FUNCIONES_FILES_HPP
#define FUNCIONES_FILES_HPP

#include <stdio.h>

// Funciones de la biblioteca de la cátedra para manejar archivos de registros.

// Graba el registro t en la posición actual del archivo.
template<typename T> void write(FILE* f, T t)
{
   fseek(f,0,SEEK_CUR); //debe anteponerse para evitar errores x implementacion de windows
   fwrite(&t,sizeof(T),1,f);
}

// Lee un registro desde la posición actual del archivo.
// Si no quedan datos retorna T() y feof(f) pasa a ser verdadero.
template<typename T> T read(FILE* f)
{
   fseek(f,0,SEEK_CUR); //debe anteponerse para evitar errores x implementacion de windows
   T t = T();
   fread(&t,sizeof(T),1,f);
   return t;
}

// Posiciona el archivo en el registro número n (contando desde 0).
template<typename T> void seek(FILE* f, int n)
{
   fseek(f,(long)(n*sizeof(T)),SEEK_SET);
}

// Retorna la cantidad de registros del archivo sin mover la posición actual.
template<typename T> int fileSize(FILE* f)
{
   long actual = ftell(f);

   fseek(f,0,SEEK_END);

   long nroUltByte = ftell(f);

   fseek(f,actual,SEEK_SET);

   return (int)(nroUltByte/sizeof(T));
}

#endif
