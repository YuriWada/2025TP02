#ifndef LISTAENCADEADA_HPP
#define LISTAENCADEADA_HPP

#include <iostream>
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
    T GetItem(int pos)
    {
        Celula* p;
        p = Posiciona(pos);
        return p->item;
    }

    // posiciona item
    void SetItem(const T& item, int pos)
    {
        Celula* p;
        p = Posiciona(pos);
        p->item = item;
    }

    // insere item no inicio da lista
    void InsereInicio(const T& item)
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
    void InsereFinal(const T& item)
    {
        Celula* nova;
        nova = new Celula();
        nova->item = item;
        ultimo->prox = nova;
        ultimo = nova;
        tamanho++;
    }

    // insere item em posicao pos
    void InserePosicao(const T& item, int pos)
    {
        Celula *p, *nova;

        p = Posiciona(pos, true); // posiciona na celula anterior

        nova = new Celula();
        nova->item = item;
        nova->prox = nova;
        tamanho++;

        if(nova->prox == NULL)
            ultimo = nova;
    }

    // remove e retorna item do inicio
    T RemoveInicio()
    {
        T& aux;
        Celula *p;

        if (tamanho == 0)
            throw "Erro: lista vazia!";

        p = primeiro->prox;
        primeiro->prox = p->prox;
        tamanho--;

        if (primeiro->prox == NULL)
            ultimo = primeiro;

        aux = p->item;
        delete p;

        return aux;
    }

    // remove e retorna item do final
    T RemoveFinal()
    {
        T& aux;
        Celula *p;

        if (tamanho == 0)
            throw "Erro: lista vazia!";

        // posiciona p na celula anterior a ultima
        p = Posiciona(tamanho, true);

        p->prox = NULL;
        tamanho--;
        aux = ultimo->item;
        delete ultimo;

        return aux;
    }

    // remove e retorna item da posicao pos
    T RemovePosicao(int pos)
    {
        T& aux;
        Celula *p, *q;

        if (tamanho == 0)
            throw "Erro: lista vazia";

        p = Posiciona(pos, true);
        q = p->prox;
        p->prox = q->prox;
        tamanho--;
        aux = q->item;

        if (p->prox == NULL)
            ultimo = p;

        return aux;
    }

    // pesquisa item com uma determinada chave
    T Pesquisa(T chave)
    {
        T& aux;
        Celula *p;

        if (tamanho == 0)
            throw "Erro: lista vazia";

        p = primeiro->prox;
        aux.SetChave(-1);

        while (p != NULL)
        {
            if (p->item.GetChave() == chave)
            {
                aux = p->item;
                break;
            }
            p = p->prox;
        }

        return aux;
    }

    // imprimir elementos da lista encadeada
    // para me ajudar a debugar
    void Imprime()
    {
        Celula *p;
        p = primeiro->prox;
        
        while(p != NULL)
        {
            p->item.Imprime(); //o item da celula tem que ter este metodo
            p = p->prox;
        }

        std::endl;
    }


    void Limpa()
    {
        Celula *p;

        p = primeiro->prox;

        while(p != NULL)
        {
            primeiro->prox = p->prox;
            delete p;
            p = primeiro->prox;
        }

        ultimo = primeiro;
        tamanho = 0;
    }

    private:
    Celula *primeiro;
    Celula *ultimo;
    
    // funcao auxiliar para posicionar apontador
    // em uma determinada posicao (celula) da lista
    Celula* Posiciona(int pos, bool antes=false)
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