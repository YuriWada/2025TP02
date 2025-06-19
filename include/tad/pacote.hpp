#ifndef PACOTE_HPP
#define PACOTE_HPP

#include <iostream>
#include "../datastructures/listaencadeada.hpp" // Para armazenar a rota

// Enumeração para os estados do pacote
enum class EstadoPacote
{
    NAO_POSTADO,              // Estado inicial antes do tempo de chegada
    CHEGADA_ESCALONADA,       // Em trânsito para um armazém (seja da postagem ou de outro armazém)
    ARMAZENADO,               // Parado em uma seção LIFO de um armazém
    REMOVIDO_PARA_TRANSPORTE, // Retirado da seção, aguardando início do trânsito
    ENTREGUE                  // Chegou ao destino final
};

class Pacote
{
private:
    // Dados de Entrada
    int m_id;
    int m_origem_id;
    int m_destino_final_id;
    int m_tempo_chegada_inicial; // Momento em que foi postado no sistema

    // Simulação
    EstadoPacote m_estado_atual;
    ListaEncadeada<int> m_rota;       // Rota a ser seguida (lista de IDs de armazéns)
    int m_timestamp_ultimo_evento; // Guarda o tempo do último evento/mudança de estado

    // Estatísticas
    int m_tempo_armazenado_total;
    int m_tempo_em_transito_total;

public:
    Pacote(int id, int origem, int destino, int tempo_chegada)
        : m_id(id),
          m_origem_id(origem),
          m_destino_final_id(destino),
          m_tempo_chegada_inicial(tempo_chegada),
          m_estado_atual(EstadoPacote::NAO_POSTADO),
          m_timestamp_ultimo_evento(tempo_chegada), // O primeiro evento é a própria postagem
          m_tempo_armazenado_total(0.0),
          m_tempo_em_transito_total(0.0) 
          {}

    // Getters
    int getID() const { return m_id; }
    int getOrigemID() const { return m_origem_id; }
    int getDestinoFinalID() const { return m_destino_final_id; }
    int getTempoChegadaInicial() const { return m_tempo_chegada_inicial; }
    EstadoPacote getEstadoAtual() const { return m_estado_atual; }
    int getTempoArmazenadoTotal() const { return m_tempo_armazenado_total; }
    int getTempoEmTransitoTotal() const { return m_tempo_em_transito_total; }
    int getProximoDestinoNaRota() const;

    // Setters
    void setRota(const ListaEncadeada<int> &rota_calculada);

    void avancarNaRota();
    void setEstado(EstadoPacote novo_estado, int tempo_do_evento);
    bool operator<(const Pacote &outro) const;
};

#endif /* PACOTE_HPP */