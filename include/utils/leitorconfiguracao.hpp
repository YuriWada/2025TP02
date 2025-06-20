#ifndef LEITORCONFIGURACAO_HPP
#define LEITORCONFIGURACAO_HPP

#include <string>
#include "../datastructures/listadinamica.hpp"

// Struct para agrupar os dados de um pacote lido do arquivo
struct DadosPacote
{
    int tempo_chegada;
    int id;
    int id_origem;
    int id_destino;
};

class LeitorConfiguracao
{
private:
    // Membros para armazenar todos os dados lidos do arquivo
    int esc_capacidade_transporte;
    int esc_latencia_transporte;
    int esc_intervalo_transportes;
    int esc_custo_remocao;
    int m_numero_armazens;
    ListaDinamica<ListaDinamica<int>> m_matriz_adjacencia;
    int m_numero_pacotes;
    ListaDinamica<DadosPacote> m_dados_pacotes;

public:
    // O construtor lê o arquivo e popula os membros da classe
    LeitorConfiguracao(const std::string &nome_arquivo);

    // Métodos "getter" para acessar os dados lidos
    int getCapacidadeTransporte() const;
    int getLatenciaTransporte() const;
    int getIntervaloTransportes() const;
    int getCustoRemocao() const;
    int getNumeroArmazens() const;
    // Retorna uma referência constante para a matriz
    const ListaDinamica<ListaDinamica<int>> &getMatrizAdjacencia() const;
    int getNumeroPacotes() const;
    // Retorna uma referência constante para a lista de dados de pacotes
    const ListaDinamica<DadosPacote> &getDadosPacotes() const;
};

#endif // LEITORCONFIGURACAO_HPP