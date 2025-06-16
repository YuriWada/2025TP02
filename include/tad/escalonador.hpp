#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#include "../datastructures/min-heap.hpp"
#include "evento.hpp"
#include "redelogistica.hpp" // Precisa conhecer a rede

class Escalonador {
private:
    MinHeap<Evento> m_fila_eventos;
    double m_tempo_atual;

    // Ponteiro para o "mundo" da simulação
    RedeLogistica* m_rede_logistica_ptr;

    // Parâmetros globais da simulação
    int m_capacidade_transporte;
    int m_latencia_transporte;
    int m_intervalo_transportes;
    int m_custo_remocao;

    // Método privado para processar um único evento
    void processarEvento(const Evento& evento);

public:
    Escalonador(RedeLogistica* rede, int cap, int lat, int interv, int custo);
    
    // Adiciona um novo evento à fila
    void agendarEvento(const Evento& evento);
    
    // Inicia e executa o loop principal da simulação
    void executar();
};

#endif // ESCALONADOR_HPP