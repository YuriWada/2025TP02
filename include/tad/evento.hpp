#ifndef EVENTO_HPP
#define EVENTO_HPP

#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include "pacote.hpp"

class Armazem;

enum class TipoEvento
{
    CHEGADA_PACOTE = 1,
    TRANSPORTE_DIARIO = 2
};

// Agrupa os dados necessários para processar um evento
struct DadosEvento
{
    Pacote *pacote_ptr = nullptr;
    int id_armazem_1 = -1; // Para CHEGADA: armazem_chegada. Para TRANSPORTE: origem.
    int id_armazem_2 = -1; // Para TRANSPORTE: destino.
};

class Evento
{
private:
    uint64_t construirChave(double tempo, TipoEvento tipo, const DadosEvento &dados)
    {
        std::stringstream ss;
        ss << std::setw(7) << std::setfill('0') << static_cast<long long>(tempo);

        if (tipo == TipoEvento::CHEGADA_PACOTE)
        {
            ss << std::setw(5) << std::setfill('0') << dados.pacote_ptr->getID();
        }
        else
        { // TRANSPORTE
            ss << std::setw(2) << std::setfill('0') << dados.id_armazem_1;
            ss << std::setw(3) << std::setfill('0') << dados.id_armazem_2;
        }
        ss << static_cast<int>(tipo);
        return std::stoull(ss.str());
    }

public:
    uint64_t chave;
    double timestamp;
    TipoEvento tipo;
    DadosEvento dados;

    Evento(double tempo, Pacote *p, int armazem_chegada_id)
    {
        this->timestamp = tempo;
        this->tipo = TipoEvento::CHEGADA_PACOTE;
        this->dados.pacote_ptr = p;
        this->dados.id_armazem_1 = armazem_chegada_id; // Guarda o ID do armazém de chegada
        this->chave = construirChave(tempo, this->tipo, this->dados);
    }

    // Construtor para eventos de Transporte
    Evento(double tempo, int origem, int destino)
    {
        this->timestamp = tempo;
        this->tipo = TipoEvento::TRANSPORTE_DIARIO;
        this->dados.id_armazem_1 = origem;
        this->dados.id_armazem_2 = destino;
        this->chave = construirChave(tempo, this->tipo, this->dados);
    }

    // Operador de comparação para o Min-Heap
    bool operator<(const Evento &outro) const
    {
        return this->chave < outro.chave;
    }
};

#endif // EVENTO_HPP