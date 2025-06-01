#ifndef CELULA_HPP
#define CELULA_HPP

#include "listaencadeada.hpp"

template <typename T>
class Celula
{
    public:
    Celula() : prox(nullptr) {};

    private:
    T item;
    T* prox;

    friend class ListaEncadeada;
};

#endif /* CELULA_HPP */