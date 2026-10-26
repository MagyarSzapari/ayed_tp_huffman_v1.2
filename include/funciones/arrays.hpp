#ifndef FUNCIONES_ARRAYS_HPP
#define FUNCIONES_ARRAYS_HPP

// Funciones de la biblioteca de la cátedra para manejar arrays.

// Agrega e al final del array, incrementa len y retorna la posición donde quedó.
template <typename T>
int add(T arr[],int& len,T e)
{
   arr[len]=e;
   len++;
   return len-1;
}

#endif
