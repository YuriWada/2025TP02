#ifndef MINHEAP_HPP
#define MINHEAP_HPP

#include <stdexcept>
#include "listadinamica.hpp"

// implementação de Fila de Prioridade usando um Min-Heap.
// O menor elemento sempre fica no topo, por isso "Min-Heap".
// Fiz usando a minha ListaDinamica como base, tratando ela como se fosse uma árvore.
template <typename T>
class MinHeap
{
private:
    // A lista dinâmica que guarda os elementos do heap.
    ListaDinamica<T> heap;

    // --- Funções auxiliares pra navegar na "árvore" dentro do array ---

    // A matemática pra achar o pai de um nó no índice i.
    int getPai(int i) { return (i - 1) / 2; }
    // A matemática pra achar o filho da esquerda.
    int getFilhoEsquerda(int i) { return (2 * i) + 1; }
    // A matemática pra achar o filho da direita.
    int getFilhoDireita(int i) { return (2 * i) + 2; }

    // Função pra trocar dois elementos de lugar na minha lista.
    // Como o BuscaElemento retorna uma referência (T&), a troca funciona direto.
    void swap(int i, int j)
    {
        T temp = heap.BuscaElemento(i);
        heap.BuscaElemento(i) = heap.BuscaElemento(j);
        heap.BuscaElemento(j) = temp;
    }

    // A lógica pra "subir" um elemento na árvore (peneirar para cima).
    // Uso isso depois de inserir um item novo no final da lista.
    void siftUp(int i)
    {
        // O loop continua enquanto o item não for a raiz (i > 0) e
        // for menor que o pai dele (quebrando a propriedade do heap).
        // A comparação 'heap.BuscaElemento(i) < heap.BuscaElemento(getPai(i))'
        // depende do operator< do tipo T. Para os Eventos, ele compara as chaves.
        while (i > 0 && heap.BuscaElemento(i) < heap.BuscaElemento(getPai(i)))
        {
            // Se for menor, eu troco ele com o pai e continuo subindo a partir da nova posição.
            swap(i, getPai(i));
            i = getPai(i);
        }
    }

    // A lógica pra "afundar" um elemento na árvore (peneirar para baixo).
    // Uso isso depois de tirar o topo do heap e colocar o último elemento no lugar.
    void siftDown(int i)
    {
        // Começo achando que o nó atual (i) é o menor.
        int minIndex = i;

        // Vejo se o filho da esquerda existe e se ele é menor que o nó atual.
        int esquerda = getFilhoEsquerda(i);
        if (esquerda < heap.GetTamanho() && heap.BuscaElemento(esquerda) < heap.BuscaElemento(minIndex))
        {
            minIndex = esquerda;
        }

        // Vejo se o filho da direita existe e se ele é menor que o menor que eu já achei até agora.
        int direita = getFilhoDireita(i);
        if (direita < heap.GetTamanho() && heap.BuscaElemento(direita) < heap.BuscaElemento(minIndex))
        {
            minIndex = direita;
        }

        // Se o menor dos três (pai, filho esquerdo, filho direito) não for o pai...
        if (i != minIndex)
        {
            // ... eu troco o pai com o menor filho.
            swap(i, minIndex);
            // E chamo a função de novo pro lugar pra onde o nó foi, pra continuar afundando se precisar.
            siftDown(minIndex);
        }
    }

public:
    // Construtor, não faz nada demais, a lista já começa vazia.
    MinHeap() {}

    // Retorna o número de elementos na fila.
    int getTamanho() const
    {
        return heap.GetTamanho();
    }

    // Verifica se a fila está vazia.
    bool estaVazia() const
    {
        return heap.ListaVazia();
    }

    // Método público pra inserir um novo item.
    void insere(const T &item)
    {
        // 1. Adiciono o item no final da lista.
        heap.InsereFinal(item);
        // 2. Chamo o siftUp pra corrigir a posição dele e manter a propriedade do heap.
        int index = heap.GetTamanho() - 1;
        siftUp(index);
    }

    // Só espia o menor elemento (o do topo) sem tirar ele da fila.
    T getMin() const
    {
        if (estaVazia())
        {
            throw std::runtime_error("Fila de prioridade vazia ao tentar obter o minimo!");
        }
        // O menor tá sempre na raiz (posição 0).
        return heap.BuscaElemento(0);
    }

    // O método mais importante: tira o menor elemento e retorna ele.
    T extraiMin()
    {
        if (estaVazia())
        {
            throw std::runtime_error("Fila de prioridade vazia ao tentar extrair o minimo!");
        }

        T resultado = heap.BuscaElemento(0);

        if (heap.GetTamanho() > 1)
        {
            heap.BuscaElemento(0) = heap.BuscaElemento(heap.GetTamanho() - 1);
        }

        heap.RemoveFinal();

        if (!estaVazia())
        {
            siftDown(0);
        }

        return resultado;
    }
};

#endif /* MINHEAP_HPP */