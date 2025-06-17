#include "../../include/tad/armazem.hpp"
#include <iostream>
#include <stdexcept>

// construtor do struct auxiliar SecaoLIFO
SecaoLIFO::SecaoLIFO(int id_destino) : id_armazem_destino_vizinho(id_destino) {}

// construtor do Armazem
Armazem::Armazem(int id_unico) : m_id(id_unico) {}

// destrutor do Armazem
Armazem::~Armazem()
{
    // deleta cada secao que foi alocada com 'new'
    for (int i = 0; i < m_secoes_de_saida.GetTamanho(); ++i)
    {
        delete m_secoes_de_saida.BuscaElemento(i);
    }
}

// retorna o ID do armazem
int Armazem::getID() const
{
    return this->m_id;
}

// implementa a busca por uma secao especifica
SecaoLIFO *Armazem::getSecaoParaDestino(int id_destino_vizinho)
{
    for (int i = 0; i < m_secoes_de_saida.GetTamanho(); ++i)
    {
        if (m_secoes_de_saida.BuscaElemento(i)->id_armazem_destino_vizinho == id_destino_vizinho)
        {
            return m_secoes_de_saida.BuscaElemento(i);
        }
    }
    return nullptr;
}

// configura uma nova secao de saida para um vizinho
void Armazem::adicionarConexaoDeSaida(int id_armazem_vizinho_destino)
{
    if (getSecaoParaDestino(id_armazem_vizinho_destino) == nullptr)
    {
        // cria a secao no heap pra evitar problemas de copia
        m_secoes_de_saida.InsereFinal(new SecaoLIFO(id_armazem_vizinho_destino));
    }
}

// funcao pra ordenar a lista de pacotes usando bubble sort
void Armazem::ordenarPacotesPorPrioridade(ListaDinamica<Pacote *> &pacotes)
{
    int n = pacotes.GetTamanho();
    bool trocou;
    for (int i = 0; i < n - 1; i++)
    {
        trocou = false;
        for (int j = 0; j < n - i - 1; j++)
        {
            if (*(pacotes.BuscaElemento(j + 1)) < *(pacotes.BuscaElemento(j)))
            {
                Pacote *temp = pacotes.BuscaElemento(j);
                pacotes.BuscaElemento(j) = pacotes.BuscaElemento(j + 1);
                pacotes.BuscaElemento(j + 1) = temp;
                trocou = true;
            }
        }
        if (trocou == false)
            break;
    }
}

void Armazem::armazenarPacote(Pacote* pacote, int id_proximo_armazem_na_rota, double tempo_atual) {
    SecaoLIFO* secao = getSecaoParaDestino(id_proximo_armazem_na_rota);
    if (secao) {
        pacote->setEstado(EstadoPacote::ARMAZENADO, tempo_atual);
        secao->pilha_pacotes.Empilha(pacote);
    } else {
        throw std::runtime_error("Erro: Tentativa de armazenar pacote para destino sem secao configurada.");
    }
}

// Versão final e corrigida
ResultadoRecuperacao Armazem::recuperarPacotesParaTransporte(
    int id_secao_destino,
    int capacidade_transporte,
    double custoremocao,
    double tempo_atual // Recebe o tempo para gerar logs corretos
) {
    ResultadoRecuperacao resultado;
    SecaoLIFO* secao = getSecaoParaDestino(id_secao_destino);
    if (!secao || secao->pilha_pacotes.Vazia() || capacidade_transporte <= 0) {
        return resultado;
    }

    ListaDinamica<Pacote*> pacotes_desempilhados;
    while (!secao->pilha_pacotes.Vazia()) {
        pacotes_desempilhados.InsereFinal(secao->pilha_pacotes.Desempilha());
    }

    // ✨ GERA LOGS DE "REMOVIDO DE" AQUI ✨
    for (int i = 0; i < pacotes_desempilhados.GetTamanho(); ++i) {
        Pacote* p = pacotes_desempilhados.BuscaElemento(i);
        p->setEstado(EstadoPacote::REMOVIDO_PARA_TRANSPORTE, tempo_atual);
        double tempo_remocao = tempo_atual + ((double)i * custoremocao);
        std::stringstream msg;
        msg << "removido de " << formatarID(this->getID(), 3) << " na secao " << formatarID(id_secao_destino, 3);
        logEvento(tempo_remocao, p->getID(), msg.str());
    }

    ListaDinamica<Pacote*> pacotes_ordenados = pacotes_desempilhados;
    this->ordenarPacotesPorPrioridade(pacotes_ordenados);
    
    for (int i = 0; i < pacotes_ordenados.GetTamanho(); ++i) {
        Pacote* p_candidato = pacotes_ordenados.BuscaElemento(i);
        if (i < capacidade_transporte) {
            int pacotes_em_cima = 0;
            for (int j = 0; j < pacotes_desempilhados.GetTamanho(); ++j) {
                if (pacotes_desempilhados.BuscaElemento(j) == p_candidato) {
                    pacotes_em_cima = j;
                    break;
                }
            }
            double atraso = (double)pacotes_em_cima * custoremocao;
            resultado.pacotes_para_transporte.InsereFinal({p_candidato, atraso});
        } else {
            resultado.pacotes_a_rearmazanar.InsereFinal(p_candidato);
        }
    }
    return resultado;
}

// verifica se tem algum pacote em qualquer secao do armazem
bool Armazem::temPacotesArmazenados() const
{
    for (int i = 0; i < m_secoes_de_saida.GetTamanho(); ++i)
    {
        SecaoLIFO *secao = m_secoes_de_saida.BuscaElemento(i);
        if (secao && !secao->pilha_pacotes.Vazia())
        {
            return true;
        }
    }
    return false;
}