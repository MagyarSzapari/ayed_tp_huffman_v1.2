#ifndef TADS_LIST_HPP
#define TADS_LIST_HPP

#include "funciones/lists.hpp"

// TAD List de la cátedra (lista enlazada genérica).

template<typename T>
struct List
{
   Node<T>* node;
};

template<typename T>
List<T> list()
{
   List<T> lst;
   lst.node=NULL;
   return lst;
}

template<typename T>
T* listAdd(List<T>& lst,T e)
{
   Node<T>* aux = add(lst.node,e);
   return &aux->info;
}

template<typename T>
T listRemoveFirst(List<T>& lst)
{
   return removeFirst<T>(lst.node);
}

template<typename T>
bool listIsEmpty(List<T> lst)
{
   return isEmpty(lst.node);
}

template<typename T>
int listSize(List<T> lst)
{
   return size(lst.node);
}

template<typename T>
T* listOrderedInsert(List<T>& lst,T t,int cmpTT(T,T))
{
   Node<T>* aux = orderedInsert<T>(lst.node,t,cmpTT);
   return &aux->info;
}

template<typename T>
void listSort(List<T>& lst,int cmpTT(T,T))
{
   sort<T>(lst.node,cmpTT);
}

#endif
