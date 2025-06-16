#ifndef FILA_HPP
#define FILA_HPP

#include <stdexcept>
#include "listaencadeada.hpp"

template <typename T>
class Fila
{
private:
    ListaEncadeada<T> lista;

public:
    Fila() {}

    // Verifica se a fila está vazia
    bool estaVazia() const
    {
        return lista.Vazia();
    }

    // Retorna o tamanho da fila
    int getTamanho() const
    {
        return lista.GetTamanho();
    }

    // Adiciona um item no final da fila (enqueue)
    void enfileira(const T &item)
    {
        lista.InsereFinal(item);
    }

    // Remove e retorna o item do início da fila (dequeue)
    T desenfileira()
    {
        if (estaVazia())
        {
            throw std::runtime_error("Erro: Fila vazia ao tentar desenfileirar!");
        }
        return lista.RemoveInicio();
    }

    // Retorna o item no início da fila sem removê-lo (front/peek)
    T frente() const
    {
        if (estaVazia())
        {
            throw std::runtime_error("Erro: Fila vazia ao tentar ver a frente!");
        }
        return lista.GetItem(1); // GetItem(1) pega o primeiro elemento
    }
};

#endif /* FILA_HPP */