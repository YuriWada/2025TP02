#ifndef LISTAENCADEADA_HPP
#define LISTAENCADEADA_HPP

#include "celula.hpp"

template <typename T>
class ListaEncadeada : public Lista
{
    public:
    // construtor
    ListaEncadeada() : Lista()
    {
        primeiro = new Celula();
        ultimo = primeiro;
    }

    // destrutor
    ~ListaEncadeada()
    {
        Limpa();
        delete primeiro;
    }

    // retorna Item
    virtual T GetItem(int pos)
    {
        Celula* p;
        p = Posiciona(pos);
        return p->item;
    }

    // posiciona item
    virtual void SetItem(const T& item, int pos)
    {
        Celula* p;
        p = Posiciona(pos);
        p->item = item;
    }

    // insere item no inicio da lista
    virtual void InsereInicio(const T& item)
    {
        Celula* nova;
        nova = new Celula();
        nova->item = item;
        nova->prox = primeiro->prox;
        primeiro->prox = nova;
        tamanho++;

        if (nova->prox == NULL)
            ultimo = nova;
    }

    // insere item no final da lista
    virtual void InsereFinal(const T& item)
    {
        Celula* nova;
        nova = new Celula();
        nova->item = item;
        ultimo->prox = nova;
        ultimo = nova;
        tamanho++;
    }

    // insere item em posicao pos
    virtual void InserePosicao(const T& item, int pos)
    {
        
    }
    virtual T RemoveInicio();
    virtual T RemoveFinal();
    virtual T RemovePosicao(int pos);
    virtual T Pesquisa(T chave);
    virtual void Imprime();
    virtual void Limpa();

    private:
    Celula primeiro;
    Celula ultimo;
    
    // funcao auxiliar para posicionar apontador
    // em uma determinada posicao (celula) da lista
    T* Posiciona(int pos, bool antes=false)
    {
        Celula* p; int i;
        if ((pos > tamanho) || (pos <= 0))
            throw "Erro: posicao invalida!";

        // posiciona na celula anterior a desejada
        p = primeiro;
        for (i = 1; i < pos; i++)
            p = p->prox;

        // vai para a proxima
        // se antes for false
        if (!antes)
            p = p->prox;

        return p;
    }
};

#endif /* LISTAENCADEADA_HPP */