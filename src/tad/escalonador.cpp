#include "../../include/tad/escalonador.hpp"
#include "../../include/utils/logger.hpp"

Escalonador::Escalonador(RedeLogistica *rede,
                         int cap, double lat,
                         double interv,
                         double custo) : m_tempo_atual(0.0),
                                         m_rede_logistica_ptr(rede),
                                         m_capacidade_transporte(cap),
                                         m_latencia_transporte(lat),
                                         m_intervalo_transportes(interv),
                                         m_custo_remocao(custo)
{
}

void Escalonador::agendarEvento(const Evento &evento)
{
    m_fila_eventos.insere(evento);
}

void Escalonador::executar()
{
    while (!m_fila_eventos.estaVazia())
    {
        Evento evento_atual = m_fila_eventos.extraiMin();

        // Evita que o tempo ande para trás, uma boa prática de segurança
        if (evento_atual.timestamp < m_tempo_atual)
        {
            continue;
        }

        this->m_tempo_atual = evento_atual.timestamp;
        this->processarEvento(evento_atual);
    }
    std::cout << "Simulacao finalizada no tempo " << this->m_tempo_atual << std::endl;
}

void Escalonador::processarEvento(const Evento &evento)
{
    if (evento.tipo == TipoEvento::CHEGADA_PACOTE)
    {
        Pacote *pacote = evento.dados.pacote_ptr;
        int armazem_chegada_id = evento.dados.id_armazem_1;

        // A transição de estado calcula o tempo em trânsito
        pacote->setEstado(EstadoPacote::ARMAZENADO, m_tempo_atual);

        if (armazem_chegada_id == pacote->getDestinoFinalID())
        {
            // É UMA ENTREGA!
            pacote->setEstado(EstadoPacote::ENTREGUE, m_tempo_atual);
            std::stringstream msg;
            msg << "entregue em " << formatarID(armazem_chegada_id, 3);
            logEvento(m_tempo_atual, pacote->getID(), msg.str());
        }
        else
        {
            // É ARMAZENAMENTO INTERMEDIÁRIO
            pacote->avancarNaRota();
            int proximo_hop_id = pacote->getProximoDestinoNaRota();
            Armazem *armazem_obj = m_rede_logistica_ptr->getArmazemPorIndice(armazem_chegada_id);

            if (armazem_obj && proximo_hop_id != -1)
            {
                armazem_obj->armazenarPacote(pacote, proximo_hop_id, m_tempo_atual);
            }
        }
    }
    else if (evento.tipo == TipoEvento::TRANSPORTE_DIARIO)
    {
        int origem_id = evento.dados.id_armazem_1;
        int destino_id = evento.dados.id_armazem_2;
        Armazem *armazem_origem = m_rede_logistica_ptr->getArmazemPorIndice(origem_id);

        if (armazem_origem)
        {
            auto pacotes_a_enviar = armazem_origem->recuperarPacotesParaTransporte(
                destino_id, m_capacidade_transporte, m_custo_remocao, m_tempo_atual);

            for (int i = 0; i < pacotes_a_enviar.GetTamanho(); ++i)
            {
                PacoteComAtraso &item = pacotes_a_enviar.BuscaElemento(i);

                double tempo_partida_real = m_tempo_atual + item.atraso_de_manipulacao;

                std::stringstream msg;
                msg << "em transito de " << formatarID(origem_id, 3) << " para " << formatarID(destino_id, 3);
                logEvento(tempo_partida_real, item.pacote->getID(), msg.str());

                double tempo_chegada = tempo_partida_real + m_latencia_transporte;
                item.pacote->setEstado(EstadoPacote::CHEGADA_ESCALONADA, tempo_partida_real);
                this->agendarEvento(Evento(tempo_chegada, item.pacote, destino_id));
            }
        }

        double proximo_transporte = evento.timestamp + m_intervalo_transportes;
        this->agendarEvento(Evento(proximo_transporte, origem_id, destino_id));
    }
}