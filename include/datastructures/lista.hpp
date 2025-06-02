/**
 * Codigo adaptado das notas de aula
 * Principal adaptação: classe transformada em Template
 */

#ifndef LISTA_HPP
#define LISTA_HPP

// interface Lista
// sera implementada pelas classes herdeiras
template <typename T>
class Lista
{
    public:
    Lista() { tamanho = 0; }
    virtual T GetItem(int pos) = 0;
    virtual void SetItem(const T& item, int pos) = 0;
    virtual void InsereInicio(const T& item) = 0;
    virtual void InsereFinal(const T& item) = 0;
    virtual void InserePosicao(const T& item, int pos) = 0;
    virtual T RemoveInicio() = 0;
    virtual T RemoveFinal() = 0;
    virtual T RemovePosicao(int pos) = 0;
    virtual bool Pesquisa(const T& chave, T& itemEncontrado_out) = 0;
    virtual void Imprime() = 0;
    virtual void Limpa() = 0;

    int GetTamanho() { return tamanho; }
    bool Vazia() { return tamanho == 0; }

    protected:
    int tamanho;
};

#endif /* LISTA_HPP */