#include "../../include/tad/pacote.hpp"

// Retorna o próximo armazém na rota (o primeiro elemento da lista)
int Pacote::getProximoDestinoNaRota() const
{
    if (sim_rota.Vazia())
    {
        return -1; // -1 indica que a rota acabou ou está vazia
    }
    return sim_rota.GetItem(1); // GetItem(1) pega o primeiro elemento
}

// Define a rota calculada pelo Grafo
void Pacote::setRota(const ListaEncadeada<int> &rota_calculada)
{
    this->sim_rota = rota_calculada;
    // O primeiro item na rota é o próprio armazém de origem, então podemos removê-lo.
    if (!this->sim_rota.Vazia() && this->sim_rota.GetItem(1) == this->ent_orig_id) {
        this->sim_rota.RemoveInicio();
    }
}

// Remove o destino atual da rota, "avançando" para o próximo passo
void Pacote::avancarNaRota()
{
    if (!sim_rota.Vazia())
    {
        sim_rota.RemoveInicio();
    }
}

// Método para atualizar o estado do pacote e calcular estatísticas
void Pacote::setEstado(EstadoPacote novo_estado, int tempo_do_evento)
{
    // Calcula a duração do estado anterior
    int duracao_no_estado_anterior = tempo_do_evento - this->ult_evento;

    // Acumula estatísticas com base no estado em que o pacote ESTAVA
    if (this->est_inicial == EstadoPacote::ARMAZENADO)
    {
        this->temp_armz_total += duracao_no_estado_anterior;
    }
    else if (this->est_inicial == EstadoPacote::CHEGADA_ESCALONADA)
    {
        // "Chegada escalonada" representa o tempo em que o pacote está em trânsito
        this->tmp_transito_total += duracao_no_estado_anterior;
    }

    // Atualiza para o novo estado e o novo tempo
    this->est_inicial = novo_estado;
    this->ult_evento = tempo_do_evento;
}

// Sobrecarga de operador para comparações (útil para ordenar por prioridade)
// Um pacote é "menor" (mais prioritário) que outro se for mais antigo.
bool Pacote::operator<(const Pacote &outro) const
{
    return this->ent_temp_chegada_inic < outro.ent_temp_chegada_inic;
}