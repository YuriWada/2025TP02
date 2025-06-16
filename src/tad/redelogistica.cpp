#include "../../include/tad/redelogistica.hpp"
#include "../../include/datastructures/fila.hpp" // Inclui a Fila necessária para o BFS
#include <stdexcept>

// implementa o construtor, que cria os armazéns
RedeLogistica::RedeLogistica(int num_armazens)
{
    if (num_armazens > 0)
    {
        for (int i = 0; i < num_armazens; ++i)
        {
            // cria o objeto Armazem no heap e guarda o ponteiro
            m_armazens.InsereFinal(new Armazem(i));
            // adiciona uma lista de adjacência vazia para este novo armazém
            m_lista_adj.InsereFinal(ListaEncadeada<int>());
        }
    }
}

// implementa o destrutor, que libera a memória de cada armazém
RedeLogistica::~RedeLogistica()
{
    for (int i = 0; i < m_armazens.GetTamanho(); ++i)
    {
        delete m_armazens.BuscaElemento(i);
    }
}

// implementa a adição de uma conexão (aresta) no grafo
void RedeLogistica::adicionarConexao(int id_armazem1, int id_armazem2)
{
    if (id_armazem1 >= getNumArmazens() || id_armazem2 >= getNumArmazens() || id_armazem1 < 0 || id_armazem2 < 0)
    {
        throw std::out_of_range("Erro: ID de armazem invalido ao adicionar conexao.");
    }

    // adiciona a aresta nos dois sentidos (grafo não direcionado)
    m_lista_adj.BuscaElemento(id_armazem1).InsereFinal(id_armazem2);
    m_lista_adj.BuscaElemento(id_armazem2).InsereFinal(id_armazem1);

    // avisa os objetos Armazem para eles criarem suas seções LIFO internas
    Armazem *armazem1 = getArmazemPorIndice(id_armazem1);
    Armazem *armazem2 = getArmazemPorIndice(id_armazem2);
    if (armazem1 && armazem2)
    {
        armazem1->adicionarConexaoDeSaida(id_armazem2);
        armazem2->adicionarConexaoDeSaida(id_armazem1);
    }
}

// implementa o getter para um armazém específico
Armazem *RedeLogistica::getArmazemPorIndice(int indice)
{
    if (indice >= 0 && indice < m_armazens.GetTamanho())
    {
        return m_armazens.BuscaElemento(indice);
    }
    return nullptr;
}

// implementa o getter para o número total de armazéns
int RedeLogistica::getNumArmazens() const
{
    return m_armazens.GetTamanho();
}

// implementa o algoritmo de Busca em Largura para achar a rota mais curta
ListaEncadeada<int> RedeLogistica::encontrarRotaBFS(int idx_origem, int idx_destino)
{
    ListaEncadeada<int> rota;
    if (idx_origem < 0 || idx_destino < 0 || idx_origem >= getNumArmazens() || idx_destino >= getNumArmazens())
    {
        return rota; // Retorna rota vazia se IDs forem inválidos
    }

    // Estruturas de dados auxiliares para o BFS
    Fila<int> fila_bfs;
    ListaDinamica<bool> visitado;
    ListaDinamica<int> predecessor;

    // Inicializa os vetores auxiliares
    for (int i = 0; i < getNumArmazens(); ++i)
    {
        visitado.InsereFinal(false);
        predecessor.InsereFinal(-1); // -1 indica que não há predecessor
    }

    // Inicia o BFS a partir da origem
    fila_bfs.enfileira(idx_origem);
    visitado.BuscaElemento(idx_origem) = true;

    while (!fila_bfs.estaVazia())
    {
        int u = fila_bfs.desenfileira();

        if (u == idx_destino)
            break; // Encontrou o destino, pode parar a busca

        // Itera sobre os vizinhos de 'u'
        ListaEncadeada<int> &vizinhos = m_lista_adj.BuscaElemento(u);
        for (int i = 1; i <= vizinhos.GetTamanho(); ++i)
        {
            int v = vizinhos.GetItem(i);
            if (!visitado.BuscaElemento(v))
            {
                visitado.BuscaElemento(v) = true;
                predecessor.BuscaElemento(v) = u;
                fila_bfs.enfileira(v);
            }
        }
    }

    // Se o destino foi alcançado, reconstrói o caminho de trás para frente
    if (predecessor.BuscaElemento(idx_destino) != -1 || idx_origem == idx_destino)
    {
        int atual = idx_destino;
        while (atual != -1)
        {
            rota.InsereInicio(atual); // Insere no início para já obter a ordem correta
            atual = predecessor.BuscaElemento(atual);
        }
    }

    return rota; // Retorna a rota [origem, ..., destino] ou uma lista vazia se não houver caminho
}