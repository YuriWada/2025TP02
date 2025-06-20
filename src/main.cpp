#include <iostream>
#include <string>
#include <stdexcept>

#include "../include/utils/leitorconfiguracao.hpp"
#include "../include/tad/redelogistica.hpp"
#include "../include/tad/armazem.hpp"
#include "../include/tad/pacote.hpp"
#include "../include/tad/escalonador.hpp"
#include "../include/utils/logger.hpp"

// Função principal do simulador.
int main(int argc, char *argv[])
{
    // Bloco try-catch pra pegar qualquer erro que possa acontecer
    // durante a execução e mostra uma mensagem amigável.
    try
    {
        // Validação básica pra ver se o usuário passou o nome do arquivo.
        if (argc < 2)
        {
            std::cerr << "Erro: Faltando nome do arquivo de entrada." << std::endl;
            std::cerr << "Uso: ./simulador <arquivo_de_entrada>" << std::endl;
            return 1;
        }

        LeitorConfiguracao leitor(argv[1]);

        RedeLogistica rede(leitor.getNumeroArmazens());
        const auto &matriz = leitor.getMatrizAdjacencia();
        for (int i = 0; i < leitor.getNumeroArmazens(); ++i)
        {
            // O j começa de i+1 pra não adicionar a mesma conexão duas vezes (ex: A->B e B->A).
            for (int j = i + 1; j < leitor.getNumeroArmazens(); ++j)
            {
                if (matriz.BuscaElemento(i).BuscaElemento(j) == 1)
                {
                    rede.adicionarConexao(i, j);
                }
            }
        }

        const auto &pacotes_data = leitor.getDadosPacotes();
        int tempo_primeiro_pacote = -1;
        if (pacotes_data.GetTamanho() > 0)
        {
            tempo_primeiro_pacote = pacotes_data.BuscaElemento(0).tempo_chegada;
            for (int i = 1; i < pacotes_data.GetTamanho(); ++i)
            {
                if (pacotes_data.BuscaElemento(i).tempo_chegada < tempo_primeiro_pacote)
                {
                    tempo_primeiro_pacote = pacotes_data.BuscaElemento(i).tempo_chegada;
                }
            }
        }

        Escalonador escalonador(&rede,
                                leitor.getCapacidadeTransporte(),
                                leitor.getLatenciaTransporte(),
                                leitor.getIntervaloTransportes(),
                                leitor.getCustoRemocao(),
                                leitor.getNumeroPacotes(),
                                tempo_primeiro_pacote);

        int primeiro_transporte_t = leitor.getIntervaloTransportes();
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

        ListaDinamica<Pacote *> pacotes_criados; // Cria uma lista pra guardar os ponteiros de todos os pacotes que eu aloquei com 'new'.
        for (int i = 0; i < pacotes_data.GetTamanho(); ++i)
        {
            const DadosPacote &dados_pacote = pacotes_data.BuscaElemento(i);

            // Cria o objeto Pacote no heap e passo o ID sequencial 'i'.
            Pacote *novo_pacote = new Pacote(
                i,
                dados_pacote.id_origem,
                dados_pacote.id_destino,
                dados_pacote.tempo_chegada);

            // Guarda o ponteiro pra poder deletar ele no final e não vazar memória.
            pacotes_criados.InsereFinal(novo_pacote);

            // Pra cada pacote, calcula a rota usando o BFS da RedeLogistica.
            ListaEncadeada<int> rota = rede.encontrarRotaBFS(dados_pacote.id_origem, dados_pacote.id_destino);
            novo_pacote->setRota(rota);

            // E finalmente, agenda o evento de chegada dele na origem.
            escalonador.agendarEvento(Evento(dados_pacote.tempo_chegada, novo_pacote, dados_pacote.id_origem));
        }

        escalonador.executar();

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