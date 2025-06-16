#include <iostream>
#include <string>
#include <stdexcept>

// Incluindo a classe que vamos testar e suas dependências
#include "../include/tad/redelogistica.hpp"
#include "../include/tad/armazem.hpp"
#include "../include/datastructures/listaencadeada.hpp"

// Função auxiliar para imprimir uma rota de forma legível
void imprimirRota(const std::string &titulo, ListaEncadeada<int> &rota)
{
    std::cout << titulo;
    if (rota.Vazia())
    {
        std::cout << " (Rota Vazia ou Inexistente)" << std::endl;
        return;
    }

    // Imprime a rota no formato: 0 -> 1 -> 3 -> 4
    for (int i = 1; i <= rota.GetTamanho(); ++i)
    {
        std::cout << rota.GetItem(i) << (i == rota.GetTamanho() ? "" : " -> ");
    }
    std::cout << std::endl;
}

// Função principal de teste para a RedeLogistica
void testarRedeLogistica()
{
    std::cout << "****** INICIANDO TESTES COM A CLASSE RedeLogistica ******" << std::endl;

    // --- Teste 1: Construção do Grafo ---
    std::cout << "\n--- Teste 1: Criando uma rede com 6 armazens ---" << std::endl;
    // O grafo terá armazéns com IDs de 0 a 5
    RedeLogistica rede(6);
    std::cout << "Numero de armazens na rede: " << rede.getNumArmazens() << " (Esperado: 6)" << std::endl;

    std::cout << "\n--- Adicionando conexoes (arestas) ---" << std::endl;
    // Vamos criar o seguinte mapa:
    // 0 -- 1 -- 2
    //      |
    //      3 -- 4
    // 5 (isolado)
    rede.adicionarConexao(0, 1);
    rede.adicionarConexao(1, 2);
    rede.adicionarConexao(1, 3);
    rede.adicionarConexao(3, 4);

    // Supondo que você tem um método para imprimir a lista de adjacência
    // rede.imprimirGrafo(); // Descomente se você implementou este método para debug

    // --- Teste 2: Encontrando Rotas com BFS ---
    std::cout << "\n--- Teste 2: Testando o calculo de rotas (BFS) ---" << std::endl;

    // Cenário A: Caminho com múltiplos saltos
    ListaEncadeada<int> rota_0_para_4 = rede.encontrarRotaBFS(0, 4);
    imprimirRota("Rota de 0 para 4: ", rota_0_para_4);
    std::cout << "(Esperado: 0 -> 1 -> 3 -> 4)" << std::endl;

    // Cenário B: Caminho reverso
    ListaEncadeada<int> rota_4_para_0 = rede.encontrarRotaBFS(4, 0);
    imprimirRota("Rota de 4 para 0: ", rota_4_para_0);
    std::cout << "(Esperado: 4 -> 3 -> 1 -> 0)" << std::endl;

    // Cenário C: Caminho direto (vizinhos)
    ListaEncadeada<int> rota_1_para_2 = rede.encontrarRotaBFS(1, 2);
    imprimirRota("Rota de 1 para 2: ", rota_1_para_2);
    std::cout << "(Esperado: 1 -> 2)" << std::endl;

    // Cenário D: Caminho de um nó para ele mesmo
    ListaEncadeada<int> rota_3_para_3 = rede.encontrarRotaBFS(3, 3);
    imprimirRota("Rota de 3 para 3: ", rota_3_para_3);
    std::cout << "(Esperado: 3)" << std::endl;

    // Cenário E: Sem caminho possível (nó isolado)
    ListaEncadeada<int> rota_0_para_5 = rede.encontrarRotaBFS(0, 5);
    imprimirRota("Rota de 0 para 5: ", rota_0_para_5);
    std::cout << "(Esperado: Rota Vazia ou Inexistente)" << std::endl;

    // Cenário F: Nó de destino inválido
    ListaEncadeada<int> rota_0_para_99 = rede.encontrarRotaBFS(0, 99);
    imprimirRota("Rota de 0 para 99 (invalido): ", rota_0_para_99);
    std::cout << "(Esperado: Rota Vazia ou Inexistente)" << std::endl;

    // --- Teste 3: Acessando os Objetos Armazem ---
    std::cout << "\n--- Teste 3: Testando acesso aos armazens ---" << std::endl;
    Armazem *armazem_ptr = rede.getArmazemPorIndice(3);
    if (armazem_ptr != nullptr)
    {
        std::cout << "getArmazemPorIndice(3) retornou um armazem com ID: " << armazem_ptr->getID() << " (OK)" << std::endl;
    }
    else
    {
        std::cout << "getArmazemPorIndice(3) retornou nullptr (FALHA)" << std::endl;
    }

    armazem_ptr = rede.getArmazemPorIndice(10);
    if (armazem_ptr == nullptr)
    {
        std::cout << "getArmazemPorIndice(10) retornou nullptr como esperado (OK)" << std::endl;
    }
    else
    {
        std::cout << "getArmazemPorIndice(10) nao retornou nullptr (FALHA)" << std::endl;
    }

    std::cout << "\n****** FIM DOS TESTES DA REDE LOGISTICA ******" << std::endl;
}

int main()
{
    testarRedeLogistica();

    // Seus outros testes...
    // testarArmazem();
    // testarPacote();

    return 0;
}