/**
 * Codigo adaptado das notas de aula
 * Principal adaptação: classe transformada em Template
 */

#ifndef LISTAENCADEADA_HPP
#define LISTAENCADEADA_HPP

#include <iostream>
#include "celula.hpp"

template <typename T>
class ListaEncadeada : public Lista
{
    public:
    // construtor
    ListaEncadeada() : Lista()<T>
    {
        primeiro = new Celula<T>();
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
        Celula<T>* p;
        p = Posiciona(pos);
        return p->item;
    }

    // posiciona item
    void SetItem(const T& item, int pos)
    {
        Celula<T>* p;
        p = Posiciona(pos);
        p->item = item;
    }

    // insere item no inicio da lista
    void InsereInicio(const T& item)
    {
        Celula<T>* nova;
        nova = new Celula<T>();
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
        Celula<T>* nova;
        nova = new Celula<T>();
        nova->item = item;
        ultimo->prox = nova;
        ultimo = nova;
        tamanho++;
    }

    // insere item em posicao pos
    void InserePosicao(const T& item, int pos)
    {
        Celula<T> *p, *nova;

        p = Posiciona(pos, true); // posiciona na celula<T> anterior

        nova = new Celula<T>();
        nova->item = item;
        nova->prox = p->prox;
        p->prox = nova;
        tamanho++;

        if(nova->prox == NULL)
            ultimo = nova;
    }

    // remove e retorna item do inicio
    T RemoveInicio()
    {
        Celula<T> *p;

        if (tamanho == 0)
            throw "Erro: lista vazia!";

        p = primeiro->prox;
        primeiro->prox = p->prox;
        tamanho--;

        if (primeiro->prox == NULL)
            ultimo = primeiro;

        T aux = p->item;
        delete p;

        return aux;
    }

    // remove e retorna item do final
    T RemoveFinal()
    {
        Celula<T> *p;

        if (tamanho == 0)
            throw "Erro: lista vazia!";

        // posiciona p na celula<T> anterior a ultima
        p = Posiciona(tamanho, true);

        p->prox = NULL;
        tamanho--;
        T aux = ultimo->item;
        delete ultimo;

        return aux;
    }

    // remove e retorna item da posicao pos
    T RemovePosicao(int pos)
    {
        Celula<T> *p, *q;

        if (tamanho == 0)
            throw "Erro: lista vazia";

        p = Posiciona(pos, true);
        q = p->prox;
        p->prox = q->prox;
        tamanho--;
        T aux = q->item;
        delete q;

        if (p->prox == NULL)
            ultimo = p;

        return aux;
    }

    // pesquisa item com uma determinada chave
    bool Pesquisa(const T& chave, T& itemEncontrado_out)
    {
        if (tamanho == 0)
            return false;
        
        Celula<T> *p;
        p = primeiro->prox;

        while (p != NULL)
        {
            if (p->item == chave)
            {
                itemEncontrado_out = p->item;
                return true;
            }
            p = p->prox;
        }

        return false;
    }

    // imprimir elementos da lista encadeada
    // para me ajudar a debugar
    void Imprime()
    {
        Celula<T> *p;
        p = primeiro->prox;
        
        while(p != NULL)
        {
            p->item.Imprime(); //o item da celula<T> tem que ter este metodo
            p = p->prox;
        }

        std::endl;
    }


    void Limpa()
    {
        Celula<T> *p;

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
    Celula<T> *primeiro;
    Celula<T> *ultimo;
    
    // funcao auxiliar para posicionar apontador
    // em uma determinada posicao (celula<T>) da lista
    Celula<T>* Posiciona(int pos, bool antes=false)
    {
        Celula<T>* p; int i;

        if ((pos > tamanho) || (pos <= 0))
            throw "Erro: posicao invalida!";

        // posiciona na celula<T> anterior a desejada
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