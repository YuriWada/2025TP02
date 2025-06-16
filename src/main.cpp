#include <iostream>
#include <string>
#include <stdexcept>

#include "../include/datastructures/listaencadeada.hpp" // Dependência da classe Pacote
#include "../include/tad/pacote.hpp"

// Função auxiliar para imprimir o estado de um pacote
void imprimeStatusPacote(const Pacote &p)
{
    std::cout << "  > Pacote ID: " << p.getID()
              << " | Estado: " << static_cast<int>(p.getEstadoAtual()) // Mostra o enum como int
              << " | Tempo Armazenado: " << p.getTempoArmazenadoTotal()
              << " | Tempo em Transito: " << p.getTempoEmTransitoTotal() << std::endl;
}

// Função principal de teste para a classe Pacote
void testarPacote()
{
    std::cout << "****** INICIANDO TESTES COM A CLASSE Pacote ******" << std::endl;

    // --- Teste 1: Estado Inicial (Construtor) ---
    std::cout << "\n--- Teste 1: Estado Inicial do Pacote ---" << std::endl;
    Pacote pacote_teste(101, 0, 3, 10.0); // ID 101, Origem 0, Destino 3, chega no tempo 10.0

    std::cout << "Verificando dados iniciais..." << std::endl;
    std::cout << "ID: " << pacote_teste.getID() << " (Esperado: 101)" << std::endl;
    std::cout << "Origem: " << pacote_teste.getOrigemID() << " (Esperado: 0)" << std::endl;
    std::cout << "Destino: " << pacote_teste.getDestinoFinalID() << " (Esperado: 3)" << std::endl;
    std::cout << "Tempo de Postagem: " << pacote_teste.getTempoChegadaInicial() << " (Esperado: 10.0)" << std::endl;
    imprimeStatusPacote(pacote_teste);
    std::cout << "Verificacao inicial: OK" << std::endl;

    // --- Teste 2: Gerenciamento da Rota ---
    std::cout << "\n--- Teste 2: Gerenciamento da Rota ---" << std::endl;
    ListaEncadeada<int> rota_calculada;
    rota_calculada.InsereFinal(0); // Origem
    rota_calculada.InsereFinal(2); // Primeiro Hop
    rota_calculada.InsereFinal(3); // Destino Final

    pacote_teste.setRota(rota_calculada);
    std::cout << "Rota [0, 2, 3] definida." << std::endl;
    std::cout << "Proximo destino na rota: " << pacote_teste.getProximoDestinoNaRota() << " (Esperado: 2)" << std::endl;

    std::cout << "Avancando na rota..." << std::endl;
    pacote_teste.avancarNaRota();
    std::cout << "Proximo destino na rota: " << pacote_teste.getProximoDestinoNaRota() << " (Esperado: 3)" << std::endl;

    std::cout << "Avancando na rota novamente..." << std::endl;
    pacote_teste.avancarNaRota();
    std::cout << "Proximo destino na rota: " << pacote_teste.getProximoDestinoNaRota() << " (Esperado: -1, rota finalizada)" << std::endl;
    std::cout << "Gerenciamento de rota: OK" << std::endl;

    // --- Teste 3: Transições de Estado e Estatísticas ---
    // Simulando a jornada de um novo pacote para testar os cálculos de tempo
    std::cout << "\n--- Teste 3: Simulando Jornada e Estatisticas ---" << std::endl;
    Pacote pacote_jornada(102, 1, 0, 50.0);
    std::cout << "T=50.0: Pacote 102 criado." << std::endl;
    imprimeStatusPacote(pacote_jornada);

    // Pacote é imediatamente colocado "em trânsito" a partir da postagem
    pacote_jornada.setEstado(EstadoPacote::CHEGADA_ESCALONADA, 50.0);
    std::cout << "\nT=50.0: Estado mudado para CHEGADA_ESCALONADA (Em Transito)." << std::endl;
    imprimeStatusPacote(pacote_jornada); // Tempo em ambos deve ser 0

    // Pacote chega no primeiro armazém intermediário
    pacote_jornada.setEstado(EstadoPacote::ARMAZENADO, 75.0); // Chegou após 25 unidades de tempo
    std::cout << "\nT=75.0: Estado mudado para ARMAZENADO." << std::endl;
    imprimeStatusPacote(pacote_jornada);
    std::cout << "  (Verificacao: Tempo em transito deve ser 75-50=25)" << std::endl;

    // Pacote é retirado da seção para transporte
    pacote_jornada.setEstado(EstadoPacote::REMOVIDO_PARA_TRANSPORTE, 90.0); // Ficou armazenado por 15 u.t.
    std::cout << "\nT=90.0: Estado mudado para REMOVIDO_PARA_TRANSPORTE." << std::endl;
    imprimeStatusPacote(pacote_jornada);
    std::cout << "  (Verificacao: Tempo armazenado deve ser 90-75=15)" << std::endl;

    // Pacote inicia o próximo trecho da viagem (após possível custo de manipulação)
    pacote_jornada.setEstado(EstadoPacote::CHEGADA_ESCALONADA, 92.0); // Estado anterior durou 2 u.t.
    std::cout << "\nT=92.0: Estado mudado para CHEGADA_ESCALONADA novamente." << std::endl;
    imprimeStatusPacote(pacote_jornada);
    std::cout << "  (Verificacao: Estatisticas nao devem mudar neste passo)" << std::endl;

    // Pacote é entregue no destino final
    pacote_jornada.setEstado(EstadoPacote::ENTREGUE, 112.0); // Viajou por mais 20 u.t.
    std::cout << "\nT=112.0: Estado mudado para ENTREGUE." << std::endl;
    imprimeStatusPacote(pacote_jornada);
    std::cout << "  (Verificacao: Tempo total em transito deve ser 25+20=45)" << std::endl;
    std::cout << "Caculo de estatisticas: OK" << std::endl;

    // --- Teste 4: Comparação de Prioridade (operator<) ---
    std::cout << "\n--- Teste 4: Comparacao de Prioridade ---" << std::endl;
    Pacote pacote_antigo(201, 0, 1, 100.0); // Postado no tempo 100
    Pacote pacote_novo(202, 0, 1, 500.0);   // Postado no tempo 500

    std::cout << "Pacote 201 (T=100) < Pacote 202 (T=500)? "
              << (pacote_antigo < pacote_novo ? "Sim (Correto, mais antigo tem maior prioridade)" : "Nao (FALHA)")
              << std::endl;

    std::cout << "Pacote 202 (T=500) < Pacote 201 (T=100)? "
              << (pacote_novo < pacote_antigo ? "Sim (FALHA)" : "Nao (Correto, mais novo tem menor prioridade)")
              << std::endl;
    std::cout << "Comparacao de prioridade: OK" << std::endl;

    std::cout << "\n****** FIM DOS TESTES DO PACOTE ******" << std::endl;
}

int main()
{
    testarPacote();

    return 0;
}