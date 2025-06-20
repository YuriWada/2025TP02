#include "../../include/tad/escalonador.hpp"
#include "../../include/utils/logger.hpp"
#include "../../include/tad/armazem.hpp"
#include <sstream>

// Construtor do Escalonador.
// Recebe todos os parâmetros da simulação e ponteiros para os objetos principais.
// Usa a lista de inicialização para configurar todos os membros.
Escalonador::Escalonador(
    RedeLogistica *rede,
    int cap,
    int lat,
    int interv,
    int custo,
    int total_pacotes,
    int tempo_primeiro_pacote) : relogio(0),
                                 rede_logistica_ptr(rede),
                                 esc_capacidade_transporte(cap),
                                 esc_latencia_transporte(lat),
                                 esc_intervalo_transportes(interv),
                                 esc_custo_remocao(custo),
                                 esc_tempo_inicial(tempo_primeiro_pacote),
                                 m_total_pacotes(total_pacotes),
                                 m_pacotes_entregues(0)
{
}

// Adiciona um novo evento na agenda (a fila de prioridade).
void Escalonador::agendarEvento(const Evento &evento)
{
    fila_eventos.insere(evento);
}

// O motor principal da simulação.
void Escalonador::executar()
{
    // O loop continua enquanto houver eventos na fila E nem todos os pacotes foram entregues.
    // Esta é a condição de parada que evita o loop infinito.
    while (!fila_eventos.estaVazia() && m_pacotes_entregues < m_total_pacotes)
    {
        // Pega o próximo evento da fila (o com maior prioridade/menor chave).
        Evento evento_atual = fila_eventos.extraiMin();

        // Uma checagem de segurança para não deixar o tempo da simulação andar para trás.
        if (evento_atual.timestamp < relogio)
            continue;

        // O relógio da simulação SALTA para o tempo do evento atual.
        this->relogio = evento_atual.timestamp;

        // Delega o processamento do evento para a função especializada.
        this->processarEvento(evento_atual);
    }
}

// Processa um evento específico, agindo como um roteador baseado no tipo do evento.
void Escalonador::processarEvento(const Evento &evento)
{
    // Lógica para quando um pacote chega em um armazém.
    if (evento.tipo == TipoEvento::CHEGADA_PACOTE)
    {
        // Extrai os dados do objeto evento.
        Pacote *pacote = evento.pacote_ptr;
        int armazem_chegada_id = evento.armazeent_id_1;

        if (!pacote)
            return;

        // Atualiza o estado do pacote, o que também calcula as estatísticas de tempo.
        pacote->setEstado(EstadoPacote::ARMAZENADO, relogio);

        // Verifica se o armazém de chegada é o destino final do pacote.
        if (armazem_chegada_id == pacote->getDestinoFinalID())
        {
            // Se for, o pacote é entregue.
            pacote->setEstado(EstadoPacote::ENTREGUE, relogio);
            std::stringstream msg;
            msg << "entregue em " << formatarID(armazem_chegada_id, 3);
            logEvento(relogio, pacote->getID(), msg.str());
            // Incrementa o contador de entregas para a condição de parada.
            this->m_pacotes_entregues++;
        }
        else
        {
            // Se não, é uma parada intermediária.
            // "Consome" a etapa atual da rota.
            if (pacote->getProximoDestinoNaRota() == armazem_chegada_id)
            {
                pacote->avancarNaRota();
            }
            int proximo_hop_id = pacote->getProximoDestinoNaRota();
            Armazem *armazem_obj = rede_logistica_ptr->getArmazemPorIndice(armazem_chegada_id);

            // Se o pacote ainda tem um próximo destino, ele é armazenado.
            if (armazem_obj && proximo_hop_id != -1)
            {
                // Gera o log de "armazenado".
                std::stringstream msg;
                msg << "armazenado em " << formatarID(armazem_chegada_id, 3) << " na secao " << formatarID(proximo_hop_id, 3);
                logEvento(relogio, pacote->getID(), msg.str());
                // Manda o armazém guardar o pacote na seção correta.
                armazem_obj->armazenarPacote(pacote, proximo_hop_id, relogio);
            }
        }
    }
    // Lógica para o evento periódico de transporte entre dois armazéns.
    else if (evento.tipo == TipoEvento::TRANSPORTE_DIARIO)
    {
        int origeent_id = evento.armazeent_id_1;
        int destino_id = evento.armazeent_id_2;
        Armazem *armazem_origem = rede_logistica_ptr->getArmazemPorIndice(origeent_id);

        if (armazem_origem)
        {
            // Chama o Armazém para fazer a lógica complexa de seleção de pacotes.
            // O Armazém retorna quem vai viajar e quem vai ser guardado de volta.
            auto resultado = armazem_origem->recuperarPacotesParaTransporte(
                destino_id, esc_capacidade_transporte, esc_custo_remocao, relogio);

            // Processa os pacotes que serão rearmazenados.
            auto &pacotes_a_rearmazanar = resultado.pacotes_a_rearmazanar;
            for (int i = 0; i < pacotes_a_rearmazanar.GetTamanho(); ++i)
            {
                Pacote *p = pacotes_a_rearmazanar.BuscaElemento(i);
                std::stringstream msg;
                msg << "rearmazanado em " << formatarID(origeent_id, 3) << " na secao " << formatarID(destino_id, 3);
                logEvento(relogio, p->getID(), msg.str());
                armazem_origem->armazenarPacote(p, destino_id, relogio);
            }

            // Processa os pacotes que de fato serão transportados.
            auto &pacotes_a_enviar = resultado.pacotes_para_transporte;
            for (int i = 0; i < pacotes_a_enviar.GetTamanho(); ++i)
            {
                PacoteComAtraso &item = pacotes_a_enviar.BuscaElemento(i);

                // Calcula o tempo de partida real, aplicando o atraso de manipulação
                // e o offset do tempo do primeiro pacote (regra de negócio específica).
                int tempo_partida_real = relogio + (int)item.atraso_de_manipulacao + esc_tempo_inicial;

                // Gera os logs de "removido de" e "em transito" com o mesmo tempo de partida.
                std::stringstream msg_removido, msg_transito;
                msg_removido << "removido de " << formatarID(origeent_id, 3) << " na secao " << formatarID(destino_id, 3);
                logEvento(tempo_partida_real, item.pacote->getID(), msg_removido.str());

                msg_transito << "em transito de " << formatarID(origeent_id, 3) << " para " << formatarID(destino_id, 3);
                logEvento(tempo_partida_real, item.pacote->getID(), msg_transito.str());

                // Calcula o tempo de chegada e agenda o novo evento.
                int tempo_chegada = tempo_partida_real + esc_latencia_transporte;
                item.pacote->setEstado(EstadoPacote::CHEGADA_ESCALONADA, tempo_partida_real);
                this->agendarEvento(Evento(tempo_chegada, item.pacote, destino_id));
            }
        }

        // Reagenda o próprio evento de transporte para o próximo ciclo.
        // Usa o tempo original do evento para manter o intervalo regular.
        int proximo_transporte = evento.timestamp + esc_intervalo_transportes;
        this->agendarEvento(Evento(proximo_transporte, origeent_id, destino_id));
    }
}