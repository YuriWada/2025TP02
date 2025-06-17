#ifndef EVENTO_HPP
#define EVENTO_HPP

#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include "pacote.hpp"

enum class TipoEvento {
    CHEGADA_PACOTE = 1,
    TRANSPORTE_DIARIO = 2
};

class Evento {
private:
    uint64_t construirChave(double tempo, TipoEvento tipo, Pacote* p, int origem, int destino) {
        std::stringstream ss;
        // Tempo: 6 dígitos
        ss << std::setw(6) << std::setfill('0') << static_cast<long long>(tempo);

        if (tipo == TipoEvento::CHEGADA_PACOTE && p != nullptr) {
            // ID do Pacote: 6 dígitos
            ss << std::setw(6) << std::setfill('0') << p->getID();
        } else { // TRANSPORTE
            // Origem: 3 dígitos, Destino: 3 dígitos
            ss << std::setw(3) << std::setfill('0') << origem;
            ss << std::setw(3) << std::setfill('0') << destino;
        }
        // Tipo do Evento: 1 dígito
        ss << static_cast<int>(tipo);
        return std::stoull(ss.str());
    }

public:
    uint64_t chave;
    double timestamp;
    TipoEvento tipo;
    Pacote* pacote_ptr = nullptr;
    int armazem_id_1 = -1; // Para CHEGADA: armazem_chegada. Para TRANSPORTE: origem.
    int armazem_id_2 = -1; // Para TRANSPORTE: destino.

    Evento() : chave(0), timestamp(0), tipo(TipoEvento::CHEGADA_PACOTE) {}

    Evento(double tempo, Pacote* p, int armazem_chegada_id) {
        this->timestamp = tempo;
        this->tipo = TipoEvento::CHEGADA_PACOTE;
        this->pacote_ptr = p;
        this->armazem_id_1 = armazem_chegada_id;
        this->chave = construirChave(tempo, this->tipo, p, -1, -1);
    }

    Evento(double tempo, int origem_transporte, int destino_transporte) {
        this->timestamp = tempo;
        this->tipo = TipoEvento::TRANSPORTE_DIARIO;
        this->armazem_id_1 = origem_transporte;
        this->armazem_id_2 = destino_transporte;
        this->chave = construirChave(tempo, this->tipo, nullptr, origem_transporte, destino_transporte);
    }
    
    bool operator<(const Evento& outro) const {
        return this->chave < outro.chave;
    }
};

#endif // EVENTO_HPP