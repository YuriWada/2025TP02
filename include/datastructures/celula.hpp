/**
 * Codigo adaptado das notas de aula
 * Principal adaptação: classe transformada em Template
 */

#ifndef CELULA_HPP
#define CELULA_HPP

template <typename U>
class ListaEncadeada;

template <typename V>
class PilhaEncadeada;

template <typename T>
class Celula
{
    public:
    Celula() : item(), prox(nullptr) {};
    Celula(const T& valorItem) : item(valorItem), prox(nullptr) {}; // funcao util

    private:
    T item;
    Celula<T>* prox;

    friend class ListaEncadeada<T>;
    friend class PilhaEncadeada<T>;
};

#endif /* CELULA_HPP */