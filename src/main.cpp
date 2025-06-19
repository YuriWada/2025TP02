#include <iostream>
#include <string>
#include <stdexcept>

// Inclua todas as suas classes TAD e de estruturas de dados
#include "../include/utils/leitorconfiguracao.hpp" // O leitor que carrega tudo
#include "../include/tad/redelogistica.hpp"
#include "../include/tad/armazem.hpp"
#include "../include/tad/pacote.hpp"
#include "../include/tad/evento.hpp"
#include "../include/tad/escalonador.hpp"

#include "../include/utils/logger.hpp"

int main(int argc, char *argv[])
{
    try
    {
        // --- FASE 1: LEITURA E CONFIGURAÇÃO ---

        // 1. Cria o objeto 'leitor'. Ele abre "entrada.txt" e carrega todos os dados.
        //    A partir daqui, a variável 'leitor' existe e pode ser usada.
        LeitorConfiguracao leitor(argv[1]);

        // 2. Cria a RedeLogistica usando o número de armazéns lido pelo 'leitor'.
        RedeLogistica rede(leitor.getNumeroArmazens());
        const auto &matriz = leitor.getMatrizAdjacencia();
        for (int i = 0; i < leitor.getNumeroArmazens(); ++i)
        {
            for (int j = i + 1; j < leitor.getNumeroArmazens(); ++j)
            {
                if (matriz.BuscaElemento(i).BuscaElemento(j) == 1)
                {
                    rede.adicionarConexao(i, j);
                }
            }
        }

        // 3. Cria o Escalonador. AGORA o 'leitor' existe para fornecer os parâmetros.
        Escalonador escalonador(&rede,
                                leitor.getCapacidadeTransporte(),
                                leitor.getLatenciaTransporte(),
                                leitor.getIntervaloTransportes(),
                                leitor.getCustoRemocao(),
                                leitor.getNumeroPacotes()); // Passa o total de pacotes

        // --- FASE 2: AGENDAMENTO DOS EVENTOS INICIAIS ---

        // 4. Agenda os primeiros eventos de transporte para cada conexão
        double primeiro_transporte_t = leitor.getIntervaloTransportes();
        for (int i = 0; i < leitor.getNumeroArmazens(); ++i)
        {
            for (int j = i + 1; j < leitor.getNumeroArmazens(); ++j)
            {
                if (matriz.BuscaElemento(i).BuscaElemento(j) == 1)
                {
                    escalonador.agendarEvento(Evento(primeiro_transporte_t, i, j));
                    escalonador.agendarEvento(Evento(primeiro_transporte_t, j, i));
                }
            }
        }

        // 5. Cria os Pacotes e agenda sua chegada na origem
        ListaDinamica<Pacote *> pacotes_criados; // Para gerenciar a memória
        const auto &pacotes_data = leitor.getDadosPacotes();
        for (int i = 0; i < pacotes_data.GetTamanho(); ++i)
        {
            const DadosPacote &dados_pacote = pacotes_data.BuscaElemento(i);

            Pacote *novo_pacote = new Pacote(
                i, // O novo ID sequencial (0, 1, 2...)
                dados_pacote.id_origem,
                dados_pacote.id_destino,
                dados_pacote.tempo_chegada);

            pacotes_criados.InsereFinal(novo_pacote);

            ListaEncadeada<int> rota = rede.encontrarRotaBFS(dados_pacote.id_origem, dados_pacote.id_destino);
            novo_pacote->setRota(rota);

            escalonador.agendarEvento(Evento(dados_pacote.tempo_chegada, novo_pacote, dados_pacote.id_origem));
        }

        // --- FASE 3: EXECUÇÃO E LIMPEZA ---

        // 6. Roda a simulação!
        escalonador.executar();

        // 7. Libera a memória alocada para os pacotes
        for (int i = 0; i < pacotes_criados.GetTamanho(); ++i)
        {
            delete pacotes_criados.BuscaElemento(i);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "ERRO FATAL: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}