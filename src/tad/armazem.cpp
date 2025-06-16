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

// poe o pacote na pilha (secao) certa
void Armazem::armazenarPacote(Pacote *pacote, int id_proximo_armazem_na_rota, double tempo_atual)
{
    if (!pacote)
        return;
    SecaoLIFO *secao = getSecaoParaDestino(id_proximo_armazem_na_rota);
    if (secao)
    {
        pacote->setEstado(EstadoPacote::ARMAZENADO, tempo_atual);
        secao->pilha_pacotes.Empilha(pacote);

        std::cout << tempo_atual << " pac " << pacote->getID() << " armazenado em "
                  << this->getID() << " " << id_proximo_armazem_na_rota << std::endl;
    }
    else
    {
        throw std::runtime_error("Erro: Tentativa de armazenar pacote para destino sem secao configurada.");
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

// metodo principal, decide quem vai ser transportado
ListaDinamica<PacoteComAtraso> Armazem::recuperarPacotesParaTransporte(
    int id_secao_destino,
    int capacidade_transporte,
    double custoremocao,
    double tempo_atual)
{
    SecaoLIFO *secao = getSecaoParaDestino(id_secao_destino);
    if (!secao || secao->pilha_pacotes.Vazia() || capacidade_transporte <= 0)
    {
        return ListaDinamica<PacoteComAtraso>();
    }

    // 1. tira tudo da pilha e poe numa lista temporaria pra poder olhar
    ListaDinamica<Pacote *> pacotes_ordem_lifo;
    while (!secao->pilha_pacotes.Vazia())
    {
        pacotes_ordem_lifo.InsereFinal(secao->pilha_pacotes.Desempilha());
    }

    // 2. log pra dizer que os pacotes foram removidos pra triagem
    for (int i = 0; i < pacotes_ordem_lifo.GetTamanho(); ++i)
    {
        Pacote *p = pacotes_ordem_lifo.BuscaElemento(i);
        p->setEstado(EstadoPacote::REMOVIDO_PARA_TRANSPORTE, tempo_atual);
        std::cout << tempo_atual << " pac " << p->getID() << " removido de "
                  << this->getID() << " " << id_secao_destino << std::endl;
    }

    // 3. cria uma copia pra ordenar por prioridade (pacote mais antigo primeiro)
    ListaDinamica<Pacote *> pacotes_ordenados = pacotes_ordem_lifo;
    this->ordenarPacotesPorPrioridade(pacotes_ordenados);

    // 4. separa quem vai viajar e quem vai voltar pra pilha
    ListaDinamica<PacoteComAtraso> pacotes_para_transporte;
    ListaDinamica<Pacote *> pacotes_a_rearmazanar;

    for (int i = 0; i < pacotes_ordenados.GetTamanho(); ++i)
    {
        Pacote *p_candidato = pacotes_ordenados.BuscaElemento(i);

        if (i < capacidade_transporte)
        {
            // esse aqui vai ser transportado, calcula o custo
            int pacotes_em_cima = 0;
            for (int j = 0; j < pacotes_ordem_lifo.GetTamanho(); ++j)
            {
                if (pacotes_ordem_lifo.BuscaElemento(j) == p_candidato)
                {
                    pacotes_em_cima = j;
                    break;
                }
            }
            double atraso = (double)pacotes_em_cima * custoremocao;
            pacotes_para_transporte.InsereFinal({p_candidato, atraso});
        }
        else
        {
            // esse aqui nao coube e vai ser rearmazenado
            pacotes_a_rearmazanar.InsereFinal(p_candidato);
        }
    }

    // 5. quem nao foi selecionado volta pra pilha
    for (int i = pacotes_a_rearmazanar.GetTamanho() - 1; i >= 0; --i)
    {
        Pacote *p = pacotes_a_rearmazanar.BuscaElemento(i);
        secao->pilha_pacotes.Empilha(p);
        p->setEstado(EstadoPacote::ARMAZENADO, tempo_atual);
        std::cout << tempo_atual << " pac " << p->getID() << " rearmazenado em "
                  << this->getID() << " " << id_secao_destino << std::endl;
    }

    // retorna a lista de pacotes pro transporte
    return pacotes_para_transporte;
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