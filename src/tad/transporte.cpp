#include "../../include/tad/transporte.hpp"

// Implementação do Construtor
Transporte::Transporte(int origem, int destino, double partida, double chegada) : m_id_origem(origem),
                                                                                  m_id_destino(destino),
                                                                                  m_tempo_partida(partida),
                                                                                  m_tempo_chegada_previsto(chegada)
{
    // O construtor da lista de pacotes (m_pacotes_neste_transporte) é chamado automaticamente.
}

// Implementação para adicionar um pacote ao lote
void Transporte::adicionarPacote(Pacote *pacote)
{
    if (pacote)
    {
        m_pacotes_neste_transporte.InsereFinal(pacote);
    }
}

int Transporte::getOrigemID() const
{
    return m_id_origem;
}

int Transporte::getDestinoID() const
{
    return m_id_destino;
}

double Transporte::getTempoPartida() const
{
    return m_tempo_partida;
}

double Transporte::getTempoChegadaPrevisto() const
{
    return m_tempo_chegada_previsto;
}

const ListaDinamica<Pacote *> &Transporte::getPacotes() const
{
    return m_pacotes_neste_transporte;
}