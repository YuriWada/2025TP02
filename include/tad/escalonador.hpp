#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include "pacote.hpp"
#include "redelogistica.hpp"
#include "../datastructures/min-heap.hpp"

// Um enum pra deixar o código mais legível. Ajuda a saber qual tipo de evento é.
// O número define a prioridade de desempate: 1 (PACOTE) vem antes de 2 (TRANSPORTE).
enum class TipoEvento
{
    CHEGADA_PACOTE = 1,
    TRANSPORTE_DIARIO = 2
};

// A classe que representa cada "coisa" que vai acontecer na simulação.
// Ela é o item que vai na minha fila de prioridade (MinHeap).
class Evento
{
private:
    // O método mais importante do Evento. Ele monta a chave de 13 dígitos.
    // A chave garante a ordem total dos eventos: primeiro por tempo, depois por dados e tipo.
    uint64_t construirChave(int tempo, TipoEvento tipo, Pacote *p, int origem, int destino)
    {
        std::stringstream ss;
        // Tempo: 6 dígitos, preenchido com zeros à esquerda
        ss << std::setw(6) << std::setfill('0') << static_cast<long long>(tempo);

        // Se for evento de pacote, o desempate é pelo ID do pacote
        if (tipo == TipoEvento::CHEGADA_PACOTE && p != nullptr)
        {
            // ID do Pacote: 6 dígitos
            ss << std::setw(6) << std::setfill('0') << p->getID();
        }
        else
        { // Se for transporte, o desempate é pela origem e destino
            ss << std::setw(3) << std::setfill('0') << origem;
            ss << std::setw(3) << std::setfill('0') << destino;
        }
        // Tipo do Evento: 1 dígito, no final, para o último desempate
        ss << static_cast<int>(tipo);
        return std::stoull(ss.str());
    }

public:
    // Deixei os membros públicos pra facilitar o acesso pelo Escalonador.
    uint64_t chave;
    int timestamp;
    TipoEvento tipo;
    Pacote *pacote_ptr = nullptr;
    int armazem_id_1 = -1; // CHEGADA: armazem_chegada | TRANSPORTE: origem
    int armazem_id_2 = -1; // TRANSPORTE: destino

    // Construtor padrão, importante pro array na ListaDinamica do heap.
    Evento() : chave(0), timestamp(0), tipo(TipoEvento::CHEGADA_PACOTE) {}

    // Construtor pra quando um pacote chega.
    Evento(int tempo, Pacote *p, int armazem_chegada_id)
    {
        this->timestamp = tempo;
        this->tipo = TipoEvento::CHEGADA_PACOTE;
        this->pacote_ptr = p;
        this->armazem_id_1 = armazem_chegada_id;
        this->chave = construirChave(tempo, this->tipo, p, -1, -1);
    }

    // Construtor pra quando um transporte acontece.
    Evento(int tempo, int origem_transporte, int destino_transporte)
    {
        this->timestamp = tempo;
        this->tipo = TipoEvento::TRANSPORTE_DIARIO;
        this->armazem_id_1 = origem_transporte;
        this->armazem_id_2 = destino_transporte;
        this->chave = construirChave(tempo, this->tipo, nullptr, origem_transporte, destino_transporte);
    }

    // O operador que o MinHeap usa pra comparar dois eventos. So compara as chaves.
    bool operator<(const Evento &outro) const
    {
        return this->chave < outro.chave;
    }
};

// A classe principal, o cérebro da simulação.
class Escalonador
{
private:
    MinHeap<Evento> m_fila_eventos;      // A agenda de eventos, ordenada pelo MinHeap.
    int m_tempo_atual;                   // O relógio da simulação.
    RedeLogistica *m_rede_logistica_ptr; // Ponteiro para o "mapa" da rede.

    // Parâmetros da simulação lidos do arquivo.
    int m_capacidade_transporte;
    int m_latencia_transporte;
    int m_intervalo_transportes;
    int m_custo_remocao;
    int m_tempo_offset_primeiro_pacote; // O tempo de chegada do primeiro pacote da simulação.

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