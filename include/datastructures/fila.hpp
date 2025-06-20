#ifndef FILA_HPP
#define FILA_HPP

#include <stdexcept>
#include "listaencadeada.hpp" // Usa a lista encadeada como base

/**
 * Fila usando a lista encadeada que já tava pronta.
 * A lógica é FIFO (First-In, First-Out), o primeiro a entrar é o primeiro a sair.
 * A frente da fila é o início da lista e o fim da fila é o final da lista.
 */
template <typename T>
class Fila
{
private:
    // a fila eh basicamente uma lista encadeada por baixo dos panos
    ListaEncadeada<T> lista;

public:
    // construtor padrao, so cria a fila vazia
    Fila() {}

    // o destrutor da lista ja limpa tudo, entao o da fila fica simples
    ~Fila() {}

    // ve se a fila ta vazia, so repasso a pergunta pra lista
    bool estaVazia() const
    {
        return lista.Vazia();
    }

    // retorna o tamanho atual da fila
    int getTamanho() const
    {
        return lista.GetTamanho();
    }

    // pra enfileirar, so insiro no final da lista
    void enfileira(const T &item)
    {
        lista.InsereFinal(item);
    }

    // pra desenfileirar, so removo do inicio da lista
    T desenfileira()
    {
        if (estaVazia())
        {
            throw std::runtime_error("Erro: Fila vazia ao tentar desenfileirar!");
        }
        return lista.RemoveInicio();
    }

    // espia o primeiro da fila sem tirar ele
    T frente() const
    {
        if (estaVazia())
        {
            throw std::runtime_error("Erro: Fila vazia ao tentar ver a frente!");
        }
        // GetItem(1) da lista eh o da frente da fila
        return lista.GetItem(1);
    }

    // limpa a fila toda chamando o Limpa da lista
    void Limpa()
    {
        lista.Limpa();
    }
};

#endif // FILA_HPP