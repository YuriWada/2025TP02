#ifndef ARMAZEM_HPP
#define ARMAZEM_HPP

#include <string>
#include "pacote.hpp"
#include "../datastructures/pilha.hpp"
#include "../datastructures/listadinamica.hpp"

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
    int atraso_de_manipulacao;
};

// struct para agrupar os resultados da operação de recuperação
struct ResultadoRecuperacao
{
    ListaDinamica<PacoteComAtraso> pacotes_para_transporte;
    ListaDinamica<Pacote *> pacotes_a_rearmazanar;
};

// classe Armazem, que eh um vertice do grafo
class Armazem
{
private:
    int ent_id;
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
    void armazenarPacote(Pacote *pacote, int id_proximo_armazem_na_rota, int tempo_atual);
    
    ResultadoRecuperacao recuperarPacotesParaTransporte(
        int id_secao_destino,
        int capacidade_transporte,
        int custoremocao,
        int tempo_atual
    );
    
    bool temPacotesArmazenados() const;
};

#endif // ARMAZEM_HPP