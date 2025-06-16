#include "../../include/tad/pacote.hpp"

// Retorna o próximo armazém na rota (o primeiro elemento da lista)
int Pacote::getProximoDestinoNaRota() const
{
    if (m_rota.Vazia())
    {
        return -1; // -1 indica que a rota acabou ou está vazia
    }
    return m_rota.GetItem(1); // GetItem(1) pega o primeiro elemento
}

// Define a rota calculada pelo Grafo
void Pacote::setRota(const ListaEncadeada<int> &rota_calculada)
{
    this->m_rota = rota_calculada;
    // O primeiro item na rota é o próprio armazém de origem, então podemos removê-lo.
    if (!this->m_rota.Vazia() && this->m_rota.GetItem(1) == this->m_origem_id)
    {
        this->m_rota.RemoveInicio();
    }
}

// Remove o destino atual da rota, "avançando" para o próximo passo
void Pacote::avancarNaRota()
{
    if (!m_rota.Vazia())
    {
        m_rota.RemoveInicio();
    }
}

// Método para atualizar o estado do pacote e calcular estatísticas
void Pacote::setEstado(EstadoPacote novo_estado, double tempo_do_evento)
{
    // Calcula a duração do estado anterior
    double duracao_no_estado_anterior = tempo_do_evento - this->m_timestamp_ultimo_evento;

    // Acumula estatísticas com base no estado em que o pacote ESTAVA
    if (this->m_estado_atual == EstadoPacote::ARMAZENADO)
    {
        this->m_tempo_armazenado_total += duracao_no_estado_anterior;
    }
    else if (this->m_estado_atual == EstadoPacote::CHEGADA_ESCALONADA)
    {
        // "Chegada escalonada" representa o tempo em que o pacote está em trânsito
        this->m_tempo_em_transito_total += duracao_no_estado_anterior;
    }

    // Atualiza para o novo estado e o novo tempo
    this->m_estado_atual = novo_estado;
    this->m_timestamp_ultimo_evento = tempo_do_evento;
}

// Sobrecarga de operador para comparações (útil para ordenar por prioridade)
// Um pacote é "menor" (mais prioritário) que outro se for mais antigo.
bool Pacote::operator<(const Pacote &outro) const
{
    return this->m_tempo_chegada_inicial < outro.m_tempo_chegada_inicial;
}