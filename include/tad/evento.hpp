#ifndef EVENTO_HPP
#define EVENTO_HPP

#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include "pacote.hpp"

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
    int armazeent_id_1 = -1; // CHEGADA: armazem_chegada | TRANSPORTE: origem
    int armazeent_id_2 = -1; // TRANSPORTE: destino

    // Construtor padrão, importante pro array na ListaDinamica do heap.
    Evento() : chave(0), timestamp(0), tipo(TipoEvento::CHEGADA_PACOTE) {}

    // Construtor pra quando um pacote chega.
    Evento(int tempo, Pacote *p, int armazem_chegada_id)
    {
        this->timestamp = tempo;
        this->tipo = TipoEvento::CHEGADA_PACOTE;
        this->pacote_ptr = p;
        this->armazeent_id_1 = armazem_chegada_id;
        this->chave = construirChave(tempo, this->tipo, p, -1, -1);
    }

    // Construtor pra quando um transporte acontece.
    Evento(int tempo, int origem_transporte, int destino_transporte)
    {
        this->timestamp = tempo;
        this->tipo = TipoEvento::TRANSPORTE_DIARIO;
        this->armazeent_id_1 = origem_transporte;
        this->armazeent_id_2 = destino_transporte;
        this->chave = construirChave(tempo, this->tipo, nullptr, origem_transporte, destino_transporte);
    }

    // O operador que o MinHeap usa pra comparar dois eventos. So compara as chaves.
    bool operator<(const Evento &outro) const
    {
        return this->chave < outro.chave;
    }
};

#endif