#include <iostream>
#include <string>  
#include <stdexcept> 

#include "../include/datastructures/listaencadeada.hpp"

// funcao auxiliar para imprimir a lista
template <typename T>
void imprimeLista(ListaEncadeada<T>& lista, const std::string& titulo) {
    std::cout << "\n--- " << titulo << " ---" << std::endl;
    if (lista.Vazia()) {
        std::cout << "Lista esta vazia." << std::endl;
        std::cout << "Tamanho: " << lista.GetTamanho() << std::endl;
        return;
    }
    std::cout << "Elementos: ";
    for (int i = 1; i <= lista.GetTamanho(); ++i) { // Indexação baseada em 1 para GetItem
        try {
            std::cout << lista.GetItem(i) << " ";
        } catch (const char* msg) {
            std::cerr << "\nErro ao obter item na pos " << i << ": " << msg << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "\nErro ao obter item na pos " << i << ": " << e.what() << std::endl;
        }
    }
    std::cout << std::endl;
    std::cout << "Tamanho: " << lista.GetTamanho() << std::endl;
}

int main() {
    std::cout << "****** INICIANDO TESTES COM ListaEncadeada<int> ******" << std::endl;
    ListaEncadeada<int> listaTesteInt;

    // Teste Vazia e GetTamanho
    std::cout << "Lista inicialmente vazia? " << (listaTesteInt.Vazia() ? "Sim" : "Nao") << std::endl;
    imprimeLista(listaTesteInt, "Lista Inicial");

    // Teste InsereFinal
    std::cout << "\n-> Testando InsereFinal..." << std::endl;
    listaTesteInt.InsereFinal(10);
    listaTesteInt.InsereFinal(20);
    listaTesteInt.InsereFinal(30);
    imprimeLista(listaTesteInt, "Apos InsereFinal(10, 20, 30)");

    // Teste InsereInicio
    std::cout << "\n-> Testando InsereInicio..." << std::endl;
    listaTesteInt.InsereInicio(5);
    imprimeLista(listaTesteInt, "Apos InsereInicio(5)"); // Esperado: 5 10 20 30

    listaTesteInt.InsereInicio(1);
    imprimeLista(listaTesteInt, "Apos InsereInicio(1)"); // Esperado: 1 5 10 20 30

    // Teste GetItem
    std::cout << "\n-> Testando GetItem..." << std::endl;
    try {
        std::cout << "Elemento na posicao 1: " << listaTesteInt.GetItem(1) << std::endl; // Deve ser 1
        std::cout << "Elemento na posicao 3: " << listaTesteInt.GetItem(3) << std::endl; // Deve ser 10
        std::cout << "Elemento na posicao 5: " << listaTesteInt.GetItem(5) << std::endl; // Deve ser 30
    } catch (const char* msg) {
        std::cerr << "Erro no GetItem: " << msg << std::endl;
    }

    // Teste InserePosicao (indexação 1-based)
    std::cout << "\n-> Testando InserePosicao..." << std::endl;
    try {
        // Lista: 1 5 10 20 30 (Tamanho 5)
        listaTesteInt.InserePosicao(0, 1); // Insere 0 na pos 1. Lista: 0 1 5 10 20 30
        imprimeLista(listaTesteInt, "Apos InserePosicao(0, 1)");

        // Lista: 0 1 5 10 20 30 (Tamanho 6)
        listaTesteInt.InserePosicao(15, 5); // Insere 15 na pos 5 (entre 10 e 20). Lista: 0 1 5 10 15 20 30
        imprimeLista(listaTesteInt, "Apos InserePosicao(15, 5)");

        // Lista: 0 1 5 10 15 20 30 (Tamanho 7)
        listaTesteInt.InserePosicao(40, 8); // Insere 40 na pos 8 (final). Lista: 0 1 5 10 15 20 30 40
        imprimeLista(listaTesteInt, "Apos InserePosicao(40, 8)");
    } catch (const char* msg) {
        std::cerr << "Erro no InserePosicao: " << msg << std::endl;
    }

    // Teste SetItem
    std::cout << "\n-> Testando SetItem..." << std::endl;
    try {
        // Lista: 0 1 5 10 15 20 30 40
        listaTesteInt.SetItem(99, 4); // Muda o valor na pos 4 (onde está 10) para 99
        imprimeLista(listaTesteInt, "Apos SetItem(99, 4)"); // Esperado: 0 1 5 99 15 20 30 40
    } catch (const char* msg) {
        std::cerr << "Erro no SetItem: " << msg << std::endl;
    }

    // Teste Pesquisa
    std::cout << "\n-> Testando Pesquisa..." << std::endl;
    int itemEncontrado;
    if (listaTesteInt.Pesquisa(15, itemEncontrado)) {
        std::cout << "Pesquisa por 15: Encontrado, valor: " << itemEncontrado << std::endl;
    } else {
        std::cout << "Pesquisa por 15: Nao encontrado." << std::endl;
    }
    if (listaTesteInt.Pesquisa(100, itemEncontrado)) {
        std::cout << "Pesquisa por 100: Encontrado, valor: " << itemEncontrado << std::endl;
    } else {
        std::cout << "Pesquisa por 100: Nao encontrado." << std::endl;
    }


    // Teste RemoveInicio
    std::cout << "\n-> Testando RemoveInicio..." << std::endl;
    try {
        // Lista: 0 1 5 99 15 20 30 40
        int removidoInicio = listaTesteInt.RemoveInicio();
        std::cout << "Removido do inicio: " << removidoInicio << std::endl; // Deve ser 0
        imprimeLista(listaTesteInt, "Apos RemoveInicio"); // Esperado: 1 5 99 15 20 30 40
    } catch (const char* msg) {
        std::cerr << "Erro no RemoveInicio: " << msg << std::endl;
    }

    // Teste RemoveFinal
    std::cout << "\n-> Testando RemoveFinal..." << std::endl;
    try {
        // Lista: 1 5 99 15 20 30 40
        int removidoFinal = listaTesteInt.RemoveFinal();
        std::cout << "Removido do final: " << removidoFinal << std::endl; // Deve ser 40
        imprimeLista(listaTesteInt, "Apos RemoveFinal"); // Esperado: 1 5 99 15 20 30
    } catch (const char* msg) {
        std::cerr << "Erro no RemoveFinal: " << msg << std::endl;
    }

    // Teste RemovePosicao (indexação 1-based)
    std::cout << "\n-> Testando RemovePosicao..." << std::endl;
    try {
        // Lista: 1 5 99 15 20 30 (Tamanho 6)
        int removidoPos = listaTesteInt.RemovePosicao(3); // Remove da pos 3 (onde está 99)
        std::cout << "Removido da posicao 3: " << removidoPos << std::endl; // Deve ser 99
        imprimeLista(listaTesteInt, "Apos RemovePosicao(3)"); // Esperado: 1 5 15 20 30

        // Lista: 1 5 15 20 30 (Tamanho 5)
        removidoPos = listaTesteInt.RemovePosicao(1); // Remove da pos 1 (onde está 1)
        std::cout << "Removido da posicao 1: " << removidoPos << std::endl; // Deve ser 1
        imprimeLista(listaTesteInt, "Apos RemovePosicao(1)"); // Esperado: 5 15 20 30

        // Lista: 5 15 20 30 (Tamanho 4)
        removidoPos = listaTesteInt.RemovePosicao(4); // Remove da pos 4 (onde está 30, o último)
        std::cout << "Removido da posicao 4: " << removidoPos << std::endl; // Deve ser 30
        imprimeLista(listaTesteInt, "Apos RemovePosicao(4)"); // Esperado: 5 15 20
    } catch (const char* msg) {
        std::cerr << "Erro no RemovePosicao: " << msg << std::endl;
    }

    // Teste Limpa
    std::cout << "\n-> Testando Limpa..." << std::endl;
    listaTesteInt.Limpa();
    imprimeLista(listaTesteInt, "Apos Limpa");
    std::cout << "Lista vazia apos Limpa? " << (listaTesteInt.Vazia() ? "Sim" : "Nao") << std::endl;

    // Testar operações em lista vazia (devem lançar exceção ou tratar)
    std::cout << "\n-> Testando operacoes em lista vazia..." << std::endl;
    try {
        listaTesteInt.RemoveInicio();
    } catch (const char* msg) {
        std::cout << "Tentativa de RemoveInicio em lista vazia: " << msg << std::endl;
    }
     try {
        listaTesteInt.GetItem(1);
    } catch (const char* msg) {
        std::cout << "Tentativa de GetItem(1) em lista vazia: " << msg << std::endl;
    }


    std::cout << "\n\n****** INICIANDO TESTES COM ListaEncadeada<std::string> ******" << std::endl;
    ListaEncadeada<std::string> listaTesteString;

    listaTesteString.InsereFinal("yuri");
    listaTesteString.InsereFinal("wada");
    listaTesteString.InsereFinal("hirono");
    imprimeLista(listaTesteString, "Nomes Iniciais");

    listaTesteString.InsereInicio("primeiro");
    imprimeLista(listaTesteString, "Apos InsereInicio(\"primeiro\")");

    try {
        listaTesteString.InserePosicao("meio", 3);
        imprimeLista(listaTesteString, "Apos InserePosicao(\"meio\", 3)");
    } catch (const char* msg) {
        std::cerr << "Erro no InserePosicao (string): " << msg << std::endl;
    }

    std::string strEncontrada;
    if(listaTesteString.Pesquisa("wada", strEncontrada)) {
        std::cout << "Pesquisa por \"wada\": Encontrado - " << strEncontrada << std::endl;
    } else {
        std::cout << "Pesquisa por \"wada\": Nao encontrado" << std::endl;
    }
    if(listaTesteString.Pesquisa("nao_existe", strEncontrada)) {
        std::cout << "Pesquisa por \"nao_existe\": Encontrado - " << strEncontrada << std::endl;
    } else {
        std::cout << "Pesquisa por \"nao_existe\": Nao encontrado" << std::endl;
    }

    try {
        std::string strRemovida = listaTesteString.RemoveFinal();
        std::cout << "Removido do Final (string): " << strRemovida << std::endl;
        imprimeLista(listaTesteString, "Apos RemoveFinal (string)");
    } catch (const char* msg) {
        std::cerr << "Erro no RemoveFinal (string): " << msg << std::endl;
    }


    std::cout << "\n****** FIM DOS TESTES ******" << std::endl;

    return 0;
}