#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#include "redelogistica.hpp"
#include "evento.hpp"
#include "../datastructures/min-heap.hpp"



// A classe principal, o cérebro da simulação.
class Escalonador
{
private:
    MinHeap<Evento> fila_eventos;      // A agenda de eventos, ordenada pelo MinHeap.
    int relogio;                   // O relógio da simulação.
    RedeLogistica *rede_logistica_ptr; // Ponteiro para o "mapa" da rede.

    // Parâmetros da simulação lidos do arquivo.
    int esc_capacidade_transporte;
    int esc_latencia_transporte;
    int esc_intervalo_transportes;
    int esc_custo_remocao;
    int esc_tempo_inicial; // O tempo de chegada do primeiro pacote da simulação.

    // Controle de término da simulação.
    int m_total_pacotes;
    int m_pacotes_entregues;

    // Método que faz o trabalho de processar cada tipo de evento.
    void processarEvento(const Evento &evento);

public:
    // Construtor pra configurar tudo.
    Escalonador(RedeLogistica *rede, int cap, int lat, int interv, int custo, int total_pacotes, int tempo_primeiro_pacote);

    // Adiciona um novo evento na agenda.
    void agendarEvento(const Evento &evento);

    // Roda a simulação até acabar.
    void executar();
};

#endif // ESCALONADOR_HPP