#ifndef FUNCIONES_LISTS_HPP
#define FUNCIONES_LISTS_HPP

#include <stddef.h>

// Funciones de la biblioteca de la cátedra para manejar listas enlazadas.

template <typename T>
struct Node
{
   T info;
   Node<T>* sig;
};

// Retorna la cantidad de nodos de la lista.
template <typename T>
int size(Node<T>* p)
{
   int ret = 0;
   Node<T>* aux = p;
   while( aux!=NULL )
   {
      aux = aux->sig;
      ret++;
   }
   return ret;
}

// Agrega e al final de la lista.
template <typename T>
Node<T>* add(Node<T>*& p, T e)
{
   Node<T>* nuevo = new Node<T>();
   nuevo->info = e;
   nuevo->sig = NULL;
   if( p==NULL )
   {
      p = nuevo;
   }
   else
   {
      Node<T>* aux = p;
      while( aux->sig!=NULL )
      {
         aux = aux->sig;
      }
      aux->sig = nuevo;
   }
   return nuevo;
}

// Agrega e al principio de la lista.
template <typename T>
Node<T>* addFirst(Node<T>*& p, T e)
{
   Node<T>* nuevo = new Node<T>();
   nuevo->info = e;
   nuevo->sig = p;
   p = nuevo;
   return nuevo;
}

// Remueve el primer nodo de la lista y retorna su contenido.
template <typename T>
T removeFirst(Node<T>*& p)
{
   T result = T();
   if( p!=NULL )
   {
      Node<T>* temp = p;
      result = temp->info;
      p = p->sig;
      delete temp;
   }
   return result;
}

// Inserta e respetando el orden que define cmpTT. Si hay elementos iguales,
// e queda después de ellos.
template <typename T>
Node<T>* orderedInsert(Node<T>*& p, T e, int cmpTT(T, T))
{
   Node<T>* nuevo = new Node<T>();
   nuevo->info = e;
   nuevo->sig = NULL;

   Node<T>* ant = NULL;
   Node<T>* aux = p;

   // Avanzamos mientras los elementos de la lista sean menores o iguales a e.
   while( aux!=NULL && cmpTT(aux->info,e)<=0 )
   {
      ant = aux;
      aux = aux->sig;
   }

   nuevo->sig = aux;
   if( ant==NULL )
   {
      p = nuevo;
   }
   else
   {
      ant->sig = nuevo;
   }
   return nuevo;
}

// Ordena la lista según cmpTT.
template <typename T>
void sort(Node<T>*& p, int cmpTT(T, T))
{
   Node<T>* nuevo = NULL;
   Node<T>* aux = p;
   while( aux!=NULL )
   {
      orderedInsert<T>(nuevo,aux->info,cmpTT);
      aux = aux->sig;
   }
   while( p!=NULL )
   {
      Node<T>* temp = p;
      p = p->sig;
      delete temp;
   }
   p = nuevo;
}

template <typename T>
bool isEmpty(Node<T>* p)
{
   return p==NULL;
}

// Apila e (lo agrega al principio de la lista).
template <typename T>
Node<T>* push(Node<T>*& p, T e)
{
   return addFirst(p,e);
}

// Desapila y retorna el primer elemento de la lista.
template <typename T>
T pop(Node<T>*& p)
{
   return removeFirst<T>(p);
}

#endif
