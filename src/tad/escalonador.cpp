#include "../../include/tad/escalonador.hpp"
#include "../../include/utils/logger.hpp"
#include "../../include/tad/armazem.hpp"
#include <sstream>

Escalonador::Escalonador(RedeLogistica *rede, int cap, int lat, int interv, int custo, int total_pacotes, int tempo_primeiro_pacote) : m_tempo_atual(0),
                                                                                                            m_rede_logistica_ptr(rede),
                                                                                                            m_capacidade_transporte(cap),
                                                                                                            m_latencia_transporte(lat),
                                                                                                            m_intervalo_transportes(interv),
                                                                                                            m_custo_remocao(custo),
                                                                                                            m_tempo_offset_primeiro_pacote(tempo_primeiro_pacote),
                                                                                                            m_total_pacotes(total_pacotes),
                                                                                                            m_pacotes_entregues(0)
{
}

void Escalonador::agendarEvento(const Evento &evento)
{
    m_fila_eventos.insere(evento);
}

void Escalonador::executar()
{
    while (!m_fila_eventos.estaVazia() && m_pacotes_entregues < m_total_pacotes)
    {
        Evento evento_atual = m_fila_eventos.extraiMin();
        if (evento_atual.timestamp < m_tempo_atual)
            continue;

        this->m_tempo_atual = evento_atual.timestamp;
        this->processarEvento(evento_atual);
    }
}

void Escalonador::processarEvento(const Evento &evento)
{
    if (evento.tipo == TipoEvento::CHEGADA_PACOTE)
    {
        Pacote *pacote = evento.pacote_ptr;
        int armazem_chegada_id = evento.armazem_id_1;
        if (!pacote)
            return;

        pacote->setEstado(EstadoPacote::ARMAZENADO, m_tempo_atual);

        if (armazem_chegada_id == pacote->getDestinoFinalID())
        {
            pacote->setEstado(EstadoPacote::ENTREGUE, m_tempo_atual);
            std::stringstream msg;
            msg << "entregue em " << formatarID(armazem_chegada_id, 3);
            logEvento(m_tempo_atual, pacote->getID(), msg.str());
            this->m_pacotes_entregues++;
        }
        else
        {
            // "Consome" a etapa atual da rota.
            if (pacote->getProximoDestinoNaRota() == armazem_chegada_id)
            {
                pacote->avancarNaRota();
            }
            int proximo_hop_id = pacote->getProximoDestinoNaRota();
            Armazem *armazem_obj = m_rede_logistica_ptr->getArmazemPorIndice(armazem_chegada_id);

            if (armazem_obj && proximo_hop_id != -1)
            {
                // Log de "armazenado" é gerado aqui.
                std::stringstream msg;
                msg << "armazenado em " << formatarID(armazem_chegada_id, 3) << " na secao " << formatarID(proximo_hop_id, 3);
                logEvento(m_tempo_atual, pacote->getID(), msg.str());
                // Chama o método silencioso do armazém.
                armazem_obj->armazenarPacote(pacote, proximo_hop_id, m_tempo_atual);
            }
        }
    }
    else if (evento.tipo == TipoEvento::TRANSPORTE_DIARIO)
    {
        int origem_id = evento.armazem_id_1;
        int destino_id = evento.armazem_id_2;
        Armazem *armazem_origem = m_rede_logistica_ptr->getArmazemPorIndice(origem_id);

        if (armazem_origem)
        {
            // A chamada ao armazém agora tem 4 argumentos.
            auto resultado = armazem_origem->recuperarPacotesParaTransporte(
                destino_id, m_capacidade_transporte, m_custo_remocao, m_tempo_atual);

            // LOGS para pacotes transportados
            auto &pacotes_a_enviar = resultado.pacotes_para_transporte;
            for (int i = 0; i < pacotes_a_enviar.GetTamanho(); ++i)
            {
                PacoteComAtraso &item = pacotes_a_enviar.BuscaElemento(i);
                int tempo_partida_real = m_tempo_atual + item.atraso_de_manipulacao + m_tempo_offset_primeiro_pacote;

                std::stringstream msg_removido, msg_transito;
                msg_removido << "removido de " << formatarID(origem_id, 3) << " na secao " << formatarID(destino_id, 3);
                logEvento(tempo_partida_real, item.pacote->getID(), msg_removido.str());

                msg_transito << "em transito de " << formatarID(origem_id, 3) << " para " << formatarID(destino_id, 3);
                logEvento(tempo_partida_real, item.pacote->getID(), msg_transito.str());

                int tempo_chegada = tempo_partida_real + m_latencia_transporte;
                item.pacote->setEstado(EstadoPacote::CHEGADA_ESCALONADA, tempo_partida_real);
                this->agendarEvento(Evento(tempo_chegada, item.pacote, destino_id));
            }

            // LOG para pacotes rearmazenados
            auto &pacotes_a_rearmazanar = resultado.pacotes_a_rearmazanar;
            for (int i = 0; i < pacotes_a_rearmazanar.GetTamanho(); ++i)
            {
                Pacote *p = pacotes_a_rearmazanar.BuscaElemento(i);
                std::stringstream msg;
                msg << "rearmazanado em " << formatarID(origem_id, 3) << " na secao " << formatarID(destino_id, 3);
                logEvento(m_tempo_atual, p->getID(), msg.str());
                armazem_origem->armazenarPacote(p, destino_id, m_tempo_atual);
            }
        }

        // Reagendar o próximo evento de transporte
        int proximo_transporte = evento.timestamp + m_intervalo_transportes;
        this->agendarEvento(Evento(proximo_transporte, origem_id, destino_id));
    }
}