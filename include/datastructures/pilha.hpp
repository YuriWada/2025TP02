#ifndef PILHAENCADEADA_HPP
#define PILHAENCADEADA_HPP

#include "celula.hpp"

// classe interface
template <typename U>
class Pilha
{
    public:
    Pilha() { tamanho = 0; }
    int GetTamanho() { return tamanho; }
    bool Vazia() { return tamanho == 0; }

    virtual void Empilha(const U& item) = 0;
    virtual U Desempilha() = 0;
    virtual void Limpa() = 0;

    private:
    int tamanho;
};

template <typename T>
class PilhaEncadeada
{
    public:
    PilhaEncadeada() : Pilha<T> 
    {
        this->topo = NULL;
    }

    private:
    Celula<T>* topo;
};

#endif /* PILHAENCADEADA_HPP */