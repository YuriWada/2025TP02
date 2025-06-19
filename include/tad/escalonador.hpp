#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#include "../datastructures/min-heap.hpp"
#include "evento.hpp"
#include "redelogistica.hpp"

class Escalonador {
private:
    MinHeap<Evento> m_fila_eventos;
    int m_tempo_atual;
    RedeLogistica* m_rede_logistica_ptr;

    int m_capacidade_transporte;
    int m_latencia_transporte;
    int m_intervalo_transportes;
    int m_custo_remocao;

    int m_total_pacotes;
    int m_pacotes_entregues;

    void processarEvento(const Evento& evento);

public:
    Escalonador(RedeLogistica* rede, int cap, int lat, int interv, int custo, int total_pacotes);
    void agendarEvento(const Evento& evento);
    void executar();
};

#endif // ESCALONADOR_HPP