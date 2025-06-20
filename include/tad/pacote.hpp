#ifndef PACOTE_HPP
#define PACOTE_HPP

#include <iostream>
#include "../datastructures/listaencadeada.hpp"

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
    int ent_id;
    int ent_orig_id;
    int ent_dest_final_id;
    int ent_temp_chegada_inic; // Momento em que foi postado no sistema

    // Simulação
    EstadoPacote est_inicial;
    ListaEncadeada<int> sim_rota;    // Rota a ser seguida (lista de IDs de armazéns)
    int ult_evento; // Guarda o tempo do último evento/mudança de estado

    // Estatísticas
    int temp_armz_total;
    int tmp_transito_total;

public:
    Pacote(int id, int origem, int destino, int tempo_chegada)
        : ent_id(id),
          ent_orig_id(origem),
          ent_dest_final_id(destino),
          ent_temp_chegada_inic(tempo_chegada),
          est_inicial(EstadoPacote::NAO_POSTADO),
          ult_evento(tempo_chegada), // O primeiro evento é a própria postagem
          temp_armz_total(0.0),
          tmp_transito_total(0.0)
    {
    }

    // Getters
    int getID() const { return ent_id; }
    int getOrigemID() const { return ent_orig_id; }
    int getDestinoFinalID() const { return ent_dest_final_id; }
    int getTempoChegadaInicial() const { return ent_temp_chegada_inic; }
    EstadoPacote getEstadoAtual() const { return est_inicial; }
    int getTempoArmazenadoTotal() const { return temp_armz_total; }
    int getTempoEmTransitoTotal() const { return tmp_transito_total; }
    int getProximoDestinoNaRota() const;

    // Setters
    void setRota(const ListaEncadeada<int> &rota_calculada);

    void avancarNaRota();
    void setEstado(EstadoPacote novo_estado, int tempo_do_evento);
    bool operator<(const Pacote &outro) const;
};

#endif /* PACOTE_HPP */