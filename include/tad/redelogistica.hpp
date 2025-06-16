#ifndef REDELOGISTICA_HPP
#define REDELOGISTICA_HPP

#include <string>

#include "../datastructures/listadinamica.hpp"
#include "../datastructures/listaencadeada.hpp"
#include "armazem.hpp"

class RedeLogistica
{
private:
    // Lista de ponteiros para os objetos Armazem
    ListaDinamica<Armazem *> m_armazens;

    // Lista de adjacência para representar as conexões
    ListaDinamica<ListaEncadeada<int>> m_lista_adj;

public:
    // Construtor: cria o grafo com um número definido de armazéns
    RedeLogistica(int num_armazens);

    // Destrutor: libera a memória alocada para os armazéns
    ~RedeLogistica();

    // Adiciona uma conexão não direcionada entre dois armazéns
    void adicionarConexao(int id_armazem1, int id_armazem2);

    // Retorna um ponteiro para o objeto Armazem dado seu índice
    Armazem *getArmazemPorIndice(int indice);

    // Retorna o número total de armazéns no grafo
    int getNumArmazens() const;

    // Encontra a rota mais curta usando Busca em Largura (BFS)
    ListaEncadeada<int> encontrarRotaBFS(int idx_origem, int idx_destino);
};

#endif // REDELOGISTICA_HPP