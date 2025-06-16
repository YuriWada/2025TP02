#ifndef PILHAENCADEADA_HPP
#define PILHAENCADEADA_HPP

#include <stdexcept>

#include "celula.hpp"

// classe interface
template <typename T>
class Pilha
{
public:
    Pilha() { this->tamanho = 0; }
    int GetTamanho() { return this->tamanho; }
    bool Vazia() const
    {
        std::cout << "--- DEBUG Pilha::Vazia() chamada. Tamanho = " << this->tamanho << " ---" << std::endl;
        return this->tamanho == 0;
    }

    virtual void Empilha(const T &item) = 0;
    virtual T Desempilha() = 0;
    virtual void Limpa() = 0;

protected:
    int tamanho;
};

template <typename T>
class PilhaEncadeada : public Pilha<T>
{
public:
    // Construtor: inicializa a pilha como vazia.
    PilhaEncadeada() : Pilha<T>()
    {
        this->topo = nullptr;
    }

    // Construtor de Cópia
    PilhaEncadeada(const PilhaEncadeada<T> &outra) : Pilha<T>()
    {
        this->topo = nullptr; // Começa como uma pilha vazia

        // Se a outra pilha não for vazia, copia os elementos
        if (outra.topo == nullptr)
        {
            return;
        }

        // Copia o primeiro item
        this->topo = new Celula<T>(outra.topo->item);
        this->tamanho++;

        // Ponteiros para percorrer as duas pilhas
        Celula<T> *p_original = outra.topo->prox;
        Celula<T> *p_copia = this->topo;

        // Copia o resto dos itens
        while (p_original != nullptr)
        {
            p_copia->prox = new Celula<T>(p_original->item);
            p_copia = p_copia->prox;
            p_original = p_original->prox;
            this->tamanho++;
        }
    }

    // Operador de Atribuição por Cópia (=)
    PilhaEncadeada<T> &operator=(const PilhaEncadeada<T> &outra)
    {
        // 1. Proteção contra auto-atribuição
        if (this == &outra)
        {
            return *this;
        }

        // 2. Limpa a pilha atual
        this->Limpa();

        // 3. Copia os elementos da outra pilha (mesma lógica do construtor de cópia)
        if (outra.topo != nullptr)
        {
            this->topo = new Celula<T>(outra.topo->item);
            this->tamanho++;
            Celula<T> *p_original = outra.topo->prox;
            Celula<T> *p_copia = this->topo;
            while (p_original != nullptr)
            {
                p_copia->prox = new Celula<T>(p_original->item);
                p_copia = p_copia->prox;
                p_original = p_original->prox;
                this->tamanho++;
            }
        }

        // 4. Retorna referência para o objeto atual
        return *this;
    }

    // Destrutor: libera toda a memória alocada.
    ~PilhaEncadeada()
    {
        this->Limpa();
    }

    // Adiciona um item no topo da pilha.
    void Empilha(const T &item)
    {
        Celula<T> *nova_celula = new Celula<T>(item); // Cria novo nó com o item
        nova_celula->prox = this->topo;               // O novo nó aponta para o antigo topo
        this->topo = nova_celula;                     // O novo nó se torna o novo topo
        this->tamanho++;
    }

    // Remove e retorna o item do topo da pilha.
    T Desempilha()
    {
        if (this->Vazia())
            throw std::runtime_error("Erro: Pilha vazia ao tentar desempilhar!");

        T item_removido = this->topo->item;
        Celula<T> *no_a_deletar = this->topo;
        this->topo = this->topo->prox;

        delete no_a_deletar;
        this->tamanho--; // <-- O bug deve estar relacionado a esta linha ou à sua interação com algo.

        return item_removido;
    }

    // Retorna o item do topo sem removê-lo.
    T GetTopo() const
    {
        if (this->Vazia())
            throw std::runtime_error("Erro: Pilha vazia ao tentar ver o topo!");

        return this->topo->item;
    }

    // Limpa todos os elementos da pilha.
    void Limpa()
    {
        while (!this->Vazia())
            this->Desempilha();
    }

private:
    Celula<T> *topo; // Ponteiro para o nó no topo da pilha
};

#endif /* PILHAENCADEADA_HPP */