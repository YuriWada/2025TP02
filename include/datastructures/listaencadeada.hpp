#ifndef LISTAENCADEADA_HPP
#define LISTAENCADEADA_HPP

#include <iostream>
#include <stdexcept>
#include "celula.hpp"

// Essa é a interface base para todas as listas.
// Ela define o "contrato", ou seja, quais métodos uma lista precisa ter.
// Usei virtual... = 0; pra fazer dela uma classe abstrata.
template <typename T>
class Lista
{
public:
    // O construtor da base já inicializa o tamanho.
    Lista() { tamanho = 0; }

    // Métodos que toda classe de Lista que eu criar DEVE implementar.
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

    // Os getters de tamanho e vazia já implemento aqui porque são iguais pra todas.
    // Marquei como 'const' porque eles não alteram a lista.
    int GetTamanho() const { return tamanho; }
    bool Vazia() const { return tamanho == 0; }

protected:
    // O 'tamanho' fica protegido pra só as classes filhas acessarem.
    int tamanho;
};

// Minha implementação da Lista usando nós encadeados (células).
// Herda da interface Lista pra garantir que eu implementei tudo que precisava.
template <typename T>
class ListaEncadeada : public Lista<T>
{
public:
    // Construtor padrão da lista.
    // Crio um nó sentinela (cabeçalho) pra facilitar as operações.
    // No começo, a lista tá vazia, então o primeiro e o último apontam pro sentinela.
    ListaEncadeada() : Lista<T>()
    {
        this->primeiro = new Celula<T>();
        this->ultimo = this->primeiro;
    }

    // Construtor de cópia, essencial da "Regra dos Três".
    // Garante que ao copiar uma lista, eu crio uma cópia de verdade (deep copy).
    ListaEncadeada(const ListaEncadeada<T> &outra) : Lista<T>()
    {
        // Começo criando uma lista nova e vazia com seu próprio sentinela.
        this->primeiro = new Celula<T>();
        this->ultimo = this->primeiro;

        // Depois, percorro a 'outra' lista e vou inserindo cada item na minha lista nova.
        Celula<T> *p = outra.primeiro->prox;
        while (p != nullptr)
        {
            this->InsereFinal(p->item);
            p = p->prox;
        }
    }

    // Destrutor. Preciso liberar toda a memória que aloquei com 'new'.
    // Chamo o Limpa() pra deletar os nós de dados, e depois deleto o nó sentinela.
    ~ListaEncadeada()
    {
        this->Limpa();
        delete this->primeiro;
    }

    // Operador de atribuição (=), outra parte da "Regra dos Três".
    ListaEncadeada<T> &operator=(const ListaEncadeada<T> &outra)
    {
        // Primeiro, a checagem pra não fazer algo tipo 'lista = lista'.
        if (this == &outra)
        {
            return *this;
        }

        // Depois, limpo tudo que minha lista tinha antes pra não vazar memória.
        this->Limpa();

        // E por fim, copio os elementos da outra lista, igual no construtor de cópia.
        Celula<T> *p = outra.primeiro->prox;
        while (p != nullptr)
        {
            this->InsereFinal(p->item);
            p = p->prox;
        }

        // Retorno *this pra permitir coisas como lista1 = lista2 = lista3.
        return *this;
    }

    // Pega o item numa posição. A validação impede de acessar posição que não existe.
    T GetItem(int pos) const override
    {
        if (pos <= 0 || pos > this->tamanho)
        {
            throw std::out_of_range("GetItem: Posicao invalida!");
        }
        const Celula<T> *p = this->Posiciona(pos);
        return p->item;
    }

    // Muda o item de uma posição.
    void SetItem(const T &item, int pos) override
    {
        if (pos <= 0 || pos > this->tamanho)
        {
            throw std::out_of_range("SetItem: Posicao invalida!");
        }
        Celula<T> *p = this->Posiciona(pos);
        p->item = item;
    }

    // Insere um nó logo depois do sentinela, tornando-o o primeiro da lista.
    void InsereInicio(const T &item) override
    {
        Celula<T> *nova = new Celula<T>(item);
        nova->prox = this->primeiro->prox;
        this->primeiro->prox = nova;
        this->tamanho++;

        // Se a lista estava vazia, o novo nó também é o último.
        if (this->tamanho == 1)
        {
            this->ultimo = nova;
        }
    }

    // Usa o ponteiro 'ultimo' pra inserir no final bem rápido (O(1)).
    void InsereFinal(const T &item) override
    {
        Celula<T> *nova = new Celula<T>(item);
        this->ultimo->prox = nova;
        this->ultimo = nova;
        this->tamanho++;
    }

    // Insere no meio. Reutiliza os outros Insere... pros casos das pontas.
    void InserePosicao(const T &item, int pos) override
    {
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

        // Acha a posição anterior e encaixa o novo nó ali.
        Celula<T> *p_anterior = this->Posiciona(pos, true);
        Celula<T> *nova = new Celula<T>(item);
        nova->prox = p_anterior->prox;
        p_anterior->prox = nova;
        this->tamanho++;
    }

    // Tira o primeiro item (o que vem depois do sentinela).
    T RemoveInicio() override
    {
        if (this->tamanho == 0)
            throw std::runtime_error("RemoveInicio: Lista vazia!");

        Celula<T> *p_remover = this->primeiro->prox;
        T itemCopiado = p_remover->item;
        this->primeiro->prox = p_remover->prox;
        this->tamanho--;

        // Se a lista ficou vazia, o 'ultimo' precisa voltar a ser o sentinela.
        if (this->tamanho == 0)
        {
            this->ultimo = this->primeiro;
        }

        delete p_remover;
        return itemCopiado;
    }

    // Remove o último. Preciso achar o penúltimo pra fazer ele ser o novo último.
    T RemoveFinal() override
    {
        if (this->tamanho == 0)
            throw std::runtime_error("RemoveFinal: Lista vazia!");

        T itemRemovido = this->ultimo->item;

        // Trata o caso especial de ter só um elemento.
        if (this->tamanho == 1)
        {
            delete this->ultimo;
            this->primeiro->prox = nullptr;
            this->ultimo = this->primeiro;
        }
        else
        {
            // Acha o penúltimo nó pra poder ligar os ponteiros.
            Celula<T> *penultimoNo = this->primeiro;
            while (penultimoNo->prox != this->ultimo)
            {
                penultimoNo = penultimoNo->prox;
            }
            delete this->ultimo;
            this->ultimo = penultimoNo;
            this->ultimo->prox = nullptr;
        }

        this->tamanho--;
        return itemRemovido;
    }

    // Tira um item do meio. Reutiliza os outros Remove... pros casos das pontas.
    T RemovePosicao(int pos) override
    {
        if (pos <= 0 || pos > this->tamanho)
        {
            throw std::out_of_range("RemovePosicao: Posicao invalida!");
        }
        if (this->tamanho == 0)
        {
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

        // Acha o anterior, religa os ponteiros e deleta o nó do meio.
        Celula<T> *p_anterior = this->Posiciona(pos, true);
        Celula<T> *q_remover = p_anterior->prox;
        T itemCopiado = q_remover->item;
        p_anterior->prox = q_remover->prox;

        delete q_remover;
        this->tamanho--;
        return itemCopiado;
    }

    // Procura um item na lista. Retorna true e o item (copiado) se achar.
    bool Pesquisa(const T &chave, T &itemEncontrado_out) const override
    {
        if (this->tamanho == 0)
            return false;

        // Começa a procurar a partir do primeiro nó com dados.
        const Celula<T> *p = this->primeiro->prox;
        while (p != nullptr)
        {
            // Requer que o tipo T tenha o '==' pra funcionar.
            if (p->item == chave)
            {
                itemEncontrado_out = p->item;
                return true;
            }
            p = p->prox;
        }
        return false;
    }

    // Imprime a lista formatada pra eu poder debugar.
    void Imprime() const override
    {
        if (this->Vazia())
        {
            std::cout << "Lista esta vazia." << std::endl;
            return;
        }

        const Celula<T> *p = this->primeiro->prox;
        std::cout << "Conteudo da lista: [ ";
        while (p != nullptr)
        {
            // Requer que o tipo T tenha o '<<' com cout pra funcionar.
            std::cout << p->item;
            if (p->prox != nullptr)
            {
                std::cout << ", ";
            }
            p = p->prox;
        }
        std::cout << " ]" << std::endl;
    }

    // Esvazia a lista, deletando todos os nós de dados um por um.
    void Limpa() override
    {
        Celula<T> *p_atual = this->primeiro->prox;
        Celula<T> *p_proximo;
        while (p_atual != nullptr)
        {
            p_proximo = p_atual->prox;
            delete p_atual;
            p_atual = p_proximo;
        }
        this->primeiro->prox = nullptr;
        this->ultimo = this->primeiro;
        this->tamanho = 0;
    }

private:
    Celula<T> *primeiro; // Ponteiro pro nó sentinela. Ele nunca guarda um item de verdade.
    Celula<T> *ultimo;   // Ponteiro pro último nó com item. Ajuda a inserir no final mais rápido.

    // Função auxiliar pra achar um nó. Retorna um ponteiro que me deixa modificar o nó.
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
                throw std::out_of_range("Posiciona: Posicao fora dos limites");
            }
            p = p->prox;
        }
        return p;
    }

    // Versão const do Posiciona. Chamada por métodos const (só de leitura).
    // Retorna um ponteiro que não me deixa modificar o nó, pra mais segurança.
    const Celula<T> *Posiciona(int pos, bool antes = false) const
    {
        if (pos <= 0)
        {
            throw std::out_of_range("Posiciona (const): Posicao deve ser > 0");
        }
        const Celula<T> *p = this->primeiro;
        int iteracoes = antes ? (pos - 1) : pos;
        for (int i = 0; i < iteracoes; ++i)
        {
            if (p == nullptr || p->prox == nullptr)
            {
                throw std::out_of_range("Posiciona (const): Posicao fora dos limites");
            }
            p = p->prox;
        }
        return p;
    }
};

#endif /* LISTAENCADEADA_HPP */