/**
 * Codigo adaptado das notas de aula
 * Principal adaptação: classe transformada em Template
 */

#ifndef LISTAENCADEADA_HPP
#define LISTAENCADEADA_HPP

#include <iostream>
#include <stdexcept> // Para exceções padrão
#include "celula.hpp"

// interface Lista
// sera implementada pelas classes herdeiras
template <typename T>
class Lista
{
public:
    Lista() { tamanho = 0; }
    virtual T GetItem(int pos) const = 0;
    virtual void SetItem(const T &item, int pos) = 0;
    virtual void InsereInicio(const T &item) = 0;
    virtual void InsereFinal(const T &item) = 0;
    virtual void InserePosicao(const T &item, int pos) = 0;
    virtual T RemoveInicio() = 0;
    virtual T RemoveFinal() = 0;
    virtual T RemovePosicao(int pos) = 0;
    virtual bool Pesquisa(const T &chave, T &itemEncontrado_out) const = 0;
    virtual void Imprime() const = 0;
    virtual void Limpa() = 0;

    int GetTamanho() const { return tamanho; }
    bool Vazia() const { return tamanho == 0; }

protected:
    int tamanho;
};

template <typename T>
class ListaEncadeada : public Lista<T>
{
public:
    // Construtor
    ListaEncadeada() : Lista<T>()
    {
        this->primeiro = new Celula<T>();
        this->ultimo = this->primeiro;
    }
    
    // Construtor de Cópia
    ListaEncadeada(const ListaEncadeada<T> &outra) : Lista<T>()
    {
        // Inicializa esta lista como vazia
        this->primeiro = new Celula<T>();
        this->ultimo = this->primeiro;

        // Percorre a 'outra' lista e insere cada item nesta lista,
        // criando uma cópia profunda dos dados.
        Celula<T> *p = outra.primeiro->prox;
        while (p != nullptr)
        {
            this->InsereFinal(p->item);
            p = p->prox;
        }
    }

    // Destrutor
    ~ListaEncadeada()
    {
        this->Limpa();
        delete this->primeiro;
    }

    // Operador de Atribuição por Cópia (=)
    ListaEncadeada<T> &operator=(const ListaEncadeada<T> &outra)
    {
        if (this == &outra)
        {
            return *this;
        }

        // Limpa a lista atual para evitar vazamento de memória
        this->Limpa();

        // Copia os elementos da 'outra' lista para esta,
        //assim como o construtor de cópia faz.
        Celula<T> *p = outra.primeiro->prox;
        while (p != nullptr)
        {
            this->InsereFinal(p->item);
            p = p->prox;
        }

        //Retorna uma referência para esta lista para permitir encadeamento
        return *this;
    }

    // Retorna Item
    T GetItem(int pos) const override
    {
        if (pos <= 0 || pos > this->tamanho)
        { // Validação explícita para GetItem
            throw std::out_of_range("GetItem: Posicao invalida!");
        }
        const Celula<T> *p;
        p = this->Posiciona(pos); // antes=false por padrão
        return p->item;
    }

    // Posiciona item
    void SetItem(const T &item, int pos) override
    {
        if (pos <= 0 || pos > this->tamanho)
        { // Validação explícita para SetItem
            throw std::out_of_range("SetItem: Posicao invalida!");
        }
        Celula<T> *p;
        p = this->Posiciona(pos); // antes=false por padrão
        p->item = item;
    }

    // Insere item no inicio da lista
    void InsereInicio(const T &item) override
    {
        Celula<T> *nova = new Celula<T>(item); // Usando construtor de Celula com item
        nova->prox = this->primeiro->prox;
        this->primeiro->prox = nova;
        this->tamanho++;

        if (this->tamanho == 1)
        { // Se a lista estava vazia, o novo nó também é o último
            this->ultimo = nova;
        }
    }

    // Insere item no final da lista
    void InsereFinal(const T &item) override
    {
        Celula<T> *nova = new Celula<T>(item); // Usando construtor de Celula com item
        this->ultimo->prox = nova;
        this->ultimo = nova;
        this->tamanho++;
    }

    // Insere item em posicao pos
    void InserePosicao(const T &item, int pos) override
    {
        // Validação de 'pos' para inserção: 1 a tamanho+1
        if (pos <= 0 || pos > this->tamanho + 1)
        {
            throw std::out_of_range("InserePosicao: Posicao invalida!");
        }

        if (pos == 1)
        {
            this->InsereInicio(item);
            return;
        }
        if (pos == this->tamanho + 1)
        {
            this->InsereFinal(item);
            return;
        }

        // Inserção no meio
        Celula<T> *p_anterior, *nova;
        p_anterior = this->Posiciona(pos, true); // Posiciona na celula anterior à 'pos'

        nova = new Celula<T>(item); // Usando construtor de Celula com item
        nova->prox = p_anterior->prox;
        p_anterior->prox = nova;
        this->tamanho++;
        // 'ultimo' não precisa ser atualizado aqui, pois não é inserção no final
    }

    // Remove e retorna item do inicio
    T RemoveInicio() override
    {
        if (this->tamanho == 0)
            throw std::runtime_error("RemoveInicio: Lista vazia!");

        Celula<T> *p_remover = this->primeiro->prox;
        T itemCopiado = p_remover->item;

        this->primeiro->prox = p_remover->prox;
        this->tamanho--;

        if (this->tamanho == 0)
        { // Se a lista ficou vazia
            this->ultimo = this->primeiro;
        }
        // Se p_remover era o ultimo, e a lista não ficou vazia,
        // o novo ultimo é agora this->primeiro->prox se tamanho == 1,
        // ou o último elemento real. Mas o 'ultimo' só muda se o *nó último* for removido.
        // Se p_remover == this->ultimo, isso foi tratado por tamanho == 0 ou será o caso do RemoveFinal.

        delete p_remover;
        return itemCopiado;
    }

    // Remove e retorna item do final
    T RemoveFinal() override
    {
        if (this->tamanho == 0)
            throw std::runtime_error("RemoveFinal: Lista vazia!");

        T itemRemovido;
        Celula<T> *noADeletar = this->ultimo;
        itemRemovido = noADeletar->item;

        if (this->tamanho == 1)
        {// ou this->primeiro->prox == this->ultimo
            delete this->ultimo; // Deleta o único nó de dados
            this->primeiro->prox = nullptr;
            this->ultimo = this->primeiro; // 'ultimo' volta a ser o sentinela
        }
        else
        {
            Celula<T> *penultimoNo = this->primeiro;
            // Encontra o nó ANTES de 'ultimo'
            while (penultimoNo->prox != this->ultimo)
            {
                penultimoNo = penultimoNo->prox;
            }
            delete this->ultimo;  // Deleta o nó que era o último
            this->ultimo = penultimoNo; // O penúltimo nó agora é o último
            this->ultimo->prox = nullptr; // Garante que o novo último nó aponte para NULL
        }

        this->tamanho--;
        return itemRemovido;
    }

    // Remove e retorna item da posicao pos
    T RemovePosicao(int pos) override
    {
        if (pos <= 0 || pos > this->tamanho)
        {
            throw std::out_of_range("RemovePosicao: Posicao invalida!");
        }
        if (this->tamanho == 0)
        { // seguro
            throw std::runtime_error("RemovePosicao: Lista vazia!");
        }

        if (pos == 1)
        {
            return this->RemoveInicio();
        }
        if (pos == this->tamanho)
        {
            return this->RemoveFinal();
        }

        // Remoção do meio
        Celula<T> *p_anterior, *q_remover;
        p_anterior = this->Posiciona(pos, true); // p_anterior é o nó ANTES da posição 'pos'
        q_remover = p_anterior->prox; // q_remover é o nó na posição 'pos'

        T itemCopiado = q_remover->item;
        p_anterior->prox = q_remover->prox; // Nó p_anterior agora aponta para o sucessor de q_remover

        delete q_remover;
        this->tamanho--;
        // 'ultimo' não muda a menos que 'pos' fosse o último (tratado por RemoveFinal)
        return itemCopiado;
    }

    // Pesquisa item com uma determinada chave
    bool Pesquisa(const T &chave, T &itemEncontrado_out) const override
    {
        if (this->tamanho == 0)
            return false;

        const Celula<T> *p = this->primeiro->prox;

        while (p != nullptr)
        {
            if (p->item == chave)
            { // Requer que T tenha operator==
                itemEncontrado_out = p->item; // Faz cópia
                return true;
            }
            p = p->prox;
        }
        return false;
    }

    // Imprimir elementos da lista encadeada
    void Imprime() const override
    {
        if (this->Vazia())
        { // Usa o método Vazia() herdado
            std::cout << "Lista esta vazia." << std::endl;
            return;
        }

        const Celula<T> *p = this->primeiro->prox;

        std::cout << "Conteudo da lista: [ ";
        while (p != nullptr)
        {
            std::cout << p->item; // Requer que T tenha operator<< com std::ostream
            if (p->prox != nullptr)
            {
                std::cout << ", ";
            }
            p = p->prox;
        }
        std::cout << " ]" << std::endl;
    }

    void Limpa() override
    {
        Celula<T> *p_atual = this->primeiro->prox;
        Celula<T> *p_proximo;

        while (p_atual != nullptr)
        {
            p_proximo = p_atual->prox; // Guarda o próximo
            delete p_atual;// Deleta o atual
            p_atual = p_proximo; // Avança para o próximo
        }
        this->primeiro->prox = nullptr; // Importante
        this->ultimo = this->primeiro;
        this->tamanho = 0;
    }

private:
    Celula<T> *primeiro; // Nó sentinela (cabeçalho)
    Celula<T> *ultimo;// Ponteiro para o último nó real da lista (ou para 'primeiro' se vazia)

    // Funcao auxiliar para posicionar apontador (Esta permite modificação)
    // Retorna o nó na 'pos' (se antes=false) ou o nó ANTERIOR à 'pos' (se antes=true)
    // 'pos' é 1-indexada.
    Celula<T> *Posiciona(int pos, bool antes = false)
    {
        if (pos <= 0)
        {
            throw std::out_of_range("Posiciona: Posicao deve ser > 0");
        }

        Celula<T> *p = this->primeiro;
        int iteracoes = antes ? (pos - 1) : pos;

        for (int i = 0; i < iteracoes; ++i)
        {
            if (p == nullptr || p->prox == nullptr)
            {
                throw std::out_of_range("Posiciona: Posicao fora dos limites da lista (muito grande)");
            }
            p = p->prox;
        }
        return p;
    }

    // esta função é constante, não permite modificações, serve apenas para leitura
    const Celula<T> *Posiciona(int pos, bool antes = false) const
    {
        if (pos <= 0)
        {
            throw std::out_of_range("Posiciona: Posicao deve ser > 0");
        }

        const Celula<T> *p = this->primeiro;
        int iteracoes = antes ? (pos - 1) : pos;

        for (int i = 0; i < iteracoes; ++i)
        {
            if (p == nullptr || p->prox == nullptr)
            {
                throw std::out_of_range("Posiciona: Posicao fora dos limites da lista (muito grande)");
            }
            p = p->prox;
        }
        return p;
    }
};

#endif /* LISTAENCADEADA_HPP */