// MinhaFilaDePrioridade.hpp
#ifndef MINHAFILADEPRIORIDADE_HPP
#define MINHAFILADEPRIORIDADE_HPP

#include <stdexcept>
#include "listadinamica.hpp"

template <typename T>
class MinHeap
{
private:
    ListaDinamica<T> heap; // Usa ListaDinamica como o contêiner base

    // Retorna o índice do pai de um nó i
    int getPai(int i) { return (i - 1) / 2; }
    // Retorna o índice do filho à esquerda do nó i
    int getFilhoEsquerda(int i) { return (2 * i) + 1; }
    // Retorna o índice do filho à direita do nó i
    int getFilhoDireita(int i) { return (2 * i) + 2; }

    // Troca dois elementos de posição no heap usando BuscaElemento,
    // que retorna uma referência (T&)
    void swap(int i, int j)
    {
        T temp = heap.BuscaElemento(i);
        heap.BuscaElemento(i) = heap.BuscaElemento(j);
        heap.BuscaElemento(j) = temp;
    }

    // "Peneira para cima": move um nó para cima na árvore para
    // restaurar a propriedade do heap após uma inserção.
    void siftUp(int i)
    {
        while (i > 0 && heap.BuscaElemento(i) < heap.BuscaElemento(getPai(i)))
        {
            swap(i, getPai(i));
            i = getPai(i);
        }
    }

    // "Peneira para baixo": move um nó para baixo na árvore para
    // restaurar a propriedade do heap após a remoção do mínimo.
    void siftDown(int i)
    {
        int minIndex = i;

        int esquerda = getFilhoEsquerda(i);
        if (esquerda < heap.GetTamanho() && heap.BuscaElemento(esquerda) < heap.BuscaElemento(minIndex))
        {
            minIndex = esquerda;
        }

        int direita = getFilhoDireita(i);
        if (direita < heap.GetTamanho() && heap.BuscaElemento(direita) < heap.BuscaElemento(minIndex))
        {
            minIndex = direita;
        }

        if (i != minIndex)
        {
            swap(i, minIndex);
            siftDown(minIndex);
        }
    }

public:
    MinHeap() {}

    int getTamanho() const
    {
        return heap.GetTamanho();
    }

    bool estaVazia() const
    {
        return heap.ListaVazia();
    }

    void insere(const T &item)
    {
        heap.InsereFinal(item); // Usa InsereFinal da lista
        int index = heap.GetTamanho() - 1;
        siftUp(index);
    }

    T getMin() const
    {
        if (estaVazia())
        {
            throw std::runtime_error("Fila de prioridade vazia ao tentar obter o minimo!");
        }
        // Retorna uma cópia do elemento no topo do heap (posição 0)
        return heap.BuscaElemento(0);
    }

    T extraiMin()
    {
        if (estaVazia())
        {
            throw std::runtime_error("Fila de prioridade vazia ao tentar extrair o minimo!");
        }

        T resultado = heap.BuscaElemento(0); // Guarda o mínimo para retornar

        if (heap.GetTamanho() > 1)
        {
            // Move o último elemento para a raiz.
            // BuscaElemento retorna uma referência, permitindo a atribuição.
            heap.BuscaElemento(0) = heap.BuscaElemento(heap.GetTamanho() - 1);
        }

        heap.RemoveFinal(); // Usa RemoveFinal da sua lista para diminuir o tamanho.
                            // Ignoramos o valor de retorno, pois já temos o que precisamos.

        if (!estaVazia())
        {
            siftDown(0);
        }

        return resultado;
    }
};

#endif /* MINHAFILADEPRIORIDADE_HPP */