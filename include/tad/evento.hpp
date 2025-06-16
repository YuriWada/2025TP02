#ifndef EVENTO_HPP
#define EVENTO_HPP

#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>

#include "pacote.hpp"

// Forward declarations para evitar inclusões circulares
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
    int id_armazem_origem = -1;
    int id_armazem_destino = -1;
};

class Evento
{
private:
    // Constrói a chave de 13 dígitos para ordenação
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
            ss << std::setw(2) << std::setfill('0') << dados.id_armazem_origem;
            ss << std::setw(3) << std::setfill('0') << dados.id_armazem_destino;
        }
        ss << static_cast<int>(tipo);
        return std::stoull(ss.str());
    }

public:
    uint64_t chave;
    double timestamp;
    TipoEvento tipo;
    DadosEvento dados;

    // Construtor para eventos de Pacote
    Evento(double tempo, Pacote *p)
    {
        this->timestamp = tempo;
        this->tipo = TipoEvento::CHEGADA_PACOTE;
        this->dados.pacote_ptr = p;
        this->chave = construirChave(tempo, this->tipo, this->dados);
    }

    // Construtor para eventos de Transporte
    Evento(double tempo, int origem, int destino)
    {
        this->timestamp = tempo;
        this->tipo = TipoEvento::TRANSPORTE_DIARIO;
        this->dados.id_armazem_origem = origem;
        this->dados.id_armazem_destino = destino;
        this->chave = construirChave(tempo, this->tipo, this->dados);
    }

    // Operador de comparação para o Min-Heap
    bool operator<(const Evento &outro) const
    {
        return this->chave < outro.chave;
    }
};

#endif // EVENTO_HPP