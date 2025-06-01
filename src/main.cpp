#include "../include/datastructures/listadinamica.hpp"
#include "../include/datastructures/lista.hpp"
#include <iostream>
#include <string>

int main()
{
    ListaDinamica<int> listaTeste;
    listaTeste.InsereFinal(10);
    listaTeste.InsereFinal(20);
    listaTeste.InsereFinal(30);
    listaTeste.ImprimeElementos();

    // int ultimoElemento = listaTeste.RemoveFinal();
    // std::cout << "Ultimo elemento:\n" << ultimoElemento << std::endl;

    std::cout << "Busca elemento 1: " << listaTeste.BuscaElemento(0) << std::endl;
    std::cout << "Busca elemento 2: " << listaTeste.BuscaElemento(1) << std::endl;
    std::cout << "Busca elemento 3: " << listaTeste.BuscaElemento(2) << std::endl;

    std::cout << "Tamanho: " << listaTeste.GetTamanho() << std::endl;
    std::cout << "Capacidade: " << listaTeste.GetCapacidade() << std::endl;

    listaTeste.InsereFinal(40);
    std::cout << "Tamanho: " << listaTeste.GetTamanho() << std::endl;
    std::cout << "Capacidade: " << listaTeste.GetCapacidade() << std::endl;
    
    listaTeste.InsereInicio(5);
    std::cout << "Tamanho: " << listaTeste.GetTamanho() << std::endl;
    std::cout << "Capacidade: " << listaTeste.GetCapacidade() << std::endl;
    std::cout << "\nElementos ate agora:" << std::endl;
    listaTeste.ImprimeElementos();

    listaTeste.InsereElemento(2, 1);
    std::cout << "\nInserindo novo elemento na pos 1:" << std::endl;
    listaTeste.ImprimeElementos();

    std::cout << "Tamanho: " << listaTeste.GetTamanho() << std::endl;
    std::cout << "Capacidade: " << listaTeste.GetCapacidade() << std::endl;
    
    listaTeste.InsereElemento(3, 0);
    std::cout << "\nInserindo novo elemento na pos 0:" << std::endl;
    listaTeste.ImprimeElementos();
    std::cout << "Tamanho: " << listaTeste.GetTamanho() << std::endl;
    std::cout << "Capacidade: " << listaTeste.GetCapacidade() << std::endl;
    
    int elemento1 = listaTeste.RemoveElemento(1);
    listaTeste.ImprimeElementos();
    std::cout << "Elemento pos1: " << elemento1 << std::endl;
    std::cout << "Tamanho: " << listaTeste.GetTamanho() << std::endl;
    std::cout << "Capacidade: " << listaTeste.GetCapacidade() << std::endl;

    // listaTeste.LimparLista();
    // std::cout << "Tamanho: " << listaTeste.GetTamanho() << std::endl;
    // std::cout << "Capacidade: " << listaTeste.GetCapacidade() << std::endl;
    // listaTeste.ImprimeElementos();

    std::cout << "Lista vazia? " << listaTeste.ListaVazia() << std::endl;
    
    // listaTeste.InsereInicio(1);
    // listaTeste.InsereInicio(2);
    // listaTeste.InsereInicio(3);
    // listaTeste.ImprimeElementos();

    // ListaDinamica<std::string> listaTeste;
    // listaTeste.InsereFinal("yuri");
    // listaTeste.InsereFinal("wada");
    // listaTeste.InsereFinal("hirono");
    // listaTeste.InsereFinal("imokuti");
    // listaTeste.ImprimeElementos();
}