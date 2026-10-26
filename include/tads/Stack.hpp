#ifndef TADS_STACK_HPP
#define TADS_STACK_HPP

#include "funciones/lists.hpp"

// TAD Stack de la cátedra (pila implementada sobre una lista enlazada).

template<typename T>
struct Stack
{
   Node<T>* node;
};

template<typename T>
Stack<T> stack()
{
   Stack<T> st;
   st.node=NULL;
   return st;
}

template<typename T>
T* stackPush(Stack<T>& st,T e)
{
   Node<T>* aux = push(st.node,e);
   return &aux->info;
}

template<typename T>
T stackPop(Stack<T>& st)
{
   return pop<T>(st.node);
}

template<typename T>
bool stackIsEmpty(Stack<T> st)
{
   return isEmpty(st.node);
}

template<typename T>
int stackSize(Stack<T> st)
{
   return size(st.node);
}

#endif
