#ifndef PILHAENCADEADA_HPP
#define PILHAENCADEADA_HPP

#include <stdexcept>

#include "celula.hpp"

// classe interface
template <typename T>
class Pilha
{
    public:
    Pilha() { tamanho = 0; }
    int GetTamanho() { return tamanho; }
    bool Vazia() const { return tamanho == 0; }

    virtual void Empilha(const T& item) = 0;
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

    // Destrutor: libera toda a memória alocada.
    ~PilhaEncadeada() 
    {
        this->Limpa();
    }

    // Adiciona um item no topo da pilha.
    void Empilha(const T& item) 
    {
        Celula<T>* nova_celula = new Celula<T>(item); // Cria novo nó com o item
        nova_celula->prox = this->topo; // O novo nó aponta para o antigo topo
        this->topo = nova_celula; // O novo nó se torna o novo topo
        this->tamanho++;
    }

    // Remove e retorna o item do topo da pilha.
    T Desempilha() 
    {
        if (this->Vazia()) 
            throw std::runtime_error("Erro: Pilha vazia ao tentar desempilhar!");

        T item_removido = this->topo->item; // Pega uma cópia do item do topo
        Celula<T>* no_a_deletar = this->topo; // Guarda o ponteiro para o topo atual
        this->topo = this->topo->prox; // O topo agora é o próximo elemento da lista

        delete no_a_deletar; // Deleta o nó do antigo topo
        this->tamanho--;

        return item_removido; // Retorna a cópia do item
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
    Celula<T>* topo; // Ponteiro para o nó no topo da pilha
};

#endif /* PILHAENCADEADA_HPP */