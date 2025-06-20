#include "../../include/utils/leitorconfiguracao.hpp"
#include <fstream>
#include <stdexcept>

LeitorConfiguracao::LeitorConfiguracao(const std::string& nome_arquivo) {
    std::ifstream arquivo(nome_arquivo);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo de entrada: " + nome_arquivo);
    }

    // Lê os parâmetros globais da simulação
    arquivo >> esc_capacidade_transporte;
    arquivo >> esc_latencia_transporte;
    arquivo >> esc_intervalo_transportes;
    arquivo >> esc_custo_remocao;
    
    // Lê a topologia da rede
    arquivo >> m_numero_armazens;
    // Constrói a matriz de adjacência manualmente, linha por linha
    for (int i = 0; i < m_numero_armazens; ++i) {
        ListaDinamica<int> linha_atual; // Cria uma lista para a linha
        for (int j = 0; j < m_numero_armazens; ++j) {
            int valor_conexao;
            arquivo >> valor_conexao;
            linha_atual.InsereFinal(valor_conexao); // Adiciona cada coluna à linha
        }
        m_matriz_adjacencia.InsereFinal(linha_atual); // Adiciona a linha completa à matriz
    }

    // Lê as informações dos pacotes
    arquivo >> m_numero_pacotes;
    for (int i = 0; i < m_numero_pacotes; ++i) {
        DadosPacote dados_atuais;
        std::string placeholder; // Para ler e descartar as palavras "pac", "org", "dst"

        arquivo >> dados_atuais.tempo_chegada;
        arquivo >> placeholder; // Lê "pac"
        arquivo >> dados_atuais.id;
        arquivo >> placeholder; // Lê "org"
        arquivo >> dados_atuais.id_origem;
        arquivo >> placeholder; // Lê "dst"
        arquivo >> dados_atuais.id_destino;

        m_dados_pacotes.InsereFinal(dados_atuais);
    }

    arquivo.close();
}

// Implementação dos Getters
int LeitorConfiguracao::getCapacidadeTransporte() const { return esc_capacidade_transporte; }
int LeitorConfiguracao::getLatenciaTransporte() const { return esc_latencia_transporte; }
int LeitorConfiguracao::getIntervaloTransportes() const { return esc_intervalo_transportes; }
int LeitorConfiguracao::getCustoRemocao() const { return esc_custo_remocao; }
int LeitorConfiguracao::getNumeroArmazens() const { return m_numero_armazens; }
const ListaDinamica<ListaDinamica<int>>& LeitorConfiguracao::getMatrizAdjacencia() const { return m_matriz_adjacencia; }
int LeitorConfiguracao::getNumeroPacotes() const { return m_numero_pacotes; }
const ListaDinamica<DadosPacote>& LeitorConfiguracao::getDadosPacotes() const { return m_dados_pacotes; }