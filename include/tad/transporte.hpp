#ifndef TRANSPORTE_HPP
#define TRANSPORTE_HPP

#include "pacote.hpp"
#include "../datastructures/listadinamica.hpp"

class Transporte {
private:
    // Dados que identificam este transporte específico
    int m_id_origem;
    int m_id_destino;
    double m_tempo_partida;
    double m_tempo_chegada_previsto;

    // Lista de ponteiros para todos os pacotes neste "lote" de transporte
    ListaDinamica<Pacote*> m_pacotes_neste_transporte;

public:
    // Construtor para inicializar as informações do transporte
    Transporte(int origem, int destino, double partida, double chegada);

    // Adiciona um ponteiro de pacote a este transporte
    void adicionarPacote(Pacote* pacote);

    // Getters para acessar as informações do transporte
    int getOrigemID() const;
    int getDestinoID() const;
    double getTempoPartida() const;
    double getTempoChegadaPrevisto() const;

    // Retorna uma referência constante à lista de pacotes
    const ListaDinamica<Pacote*>& getPacotes() const;
};

#endif // TRANSPORTE_HPP