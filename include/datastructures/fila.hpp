#ifndef FILA_HPP
#define FILA_HPP

#include <stdexcept>
#include "listaencadeada.hpp" // Usa a lista encadeada como base

template <typename T>
class Fila {
private:
    // CORREÇÃO: O nome da sua classe é ListaEncadeada, não MinhaListaEncadeada
    ListaEncadeada<T> lista;

public:
    Fila() {}
    ~Fila() {}

    bool estaVazia() const {
        return lista.Vazia();
    }

    int getTamanho() const {
        return lista.GetTamanho();
    }

    void enfileira(const T& item) {
        lista.InsereFinal(item);
    }

    T desenfileira() {
        if (estaVazia()) {
            throw std::runtime_error("Erro: Fila vazia ao tentar desenfileirar!");
        }
        return lista.RemoveInicio();
    }

    T frente() const {
        if (estaVazia()) {
            throw std::runtime_error("Erro: Fila vazia ao tentar ver a frente!");
        }
        return lista.GetItem(1);
    }

    void Limpa() {
        lista.Limpa();
    }
};

#endif // FILA_HPP