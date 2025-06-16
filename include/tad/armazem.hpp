#ifndef ARMAZEM_HPP
#define ARMAZEM_HPP

#include <string>
#include "pacote.hpp"
#include "../datastructures/pilha.hpp"
#include "../datastructures/listadinamica.hpp"
#include "../utils/logger.hpp"

// struct pra guardar a pilha de uma secao do armazem
struct SecaoLIFO
{
    int id_armazem_destino_vizinho;
    PilhaEncadeada<Pacote *> pilha_pacotes;

    SecaoLIFO(int id_destino = -1);
};

// struct pra retornar o pacote junto com o atraso que ele teve
struct PacoteComAtraso
{
    Pacote *pacote;
    double atraso_de_manipulacao;
};

// classe Armazem, que eh um vertice do grafo
class Armazem
{
private:
    int m_id;
    ListaDinamica<SecaoLIFO *> m_secoes_de_saida;

    // funcao auxiliar pra achar a secao de um destino
    SecaoLIFO *getSecaoParaDestino(int id_destino_vizinho);

    // funcao auxiliar pra ordenar os pacotes pela prioridade
    void ordenarPacotesPorPrioridade(ListaDinamica<Pacote *> &pacotes);

public:
    // construtor
    Armazem(int id_unico = -1);

    // destrutor pra limpar a memoria das secoes
    ~Armazem();

    // retorna o ID do armazem
    int getID() const;

    // metodos principais de operacao do armazem
    void adicionarConexaoDeSaida(int id_armazem_vizinho_destino);
    void armazenarPacote(Pacote *pacote, int id_proximo_armazem_na_rota, double tempo_atual);
    ListaDinamica<PacoteComAtraso> recuperarPacotesParaTransporte(
        int id_secao_destino,
        int capacidade_transporte,
        double custoremocao,
        double tempo_atual);
    bool temPacotesArmazenados() const;
};

#endif // ARMAZEM_HPP