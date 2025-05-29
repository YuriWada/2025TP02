#include "../include/datastructures/listadinamica.hpp"
#include <iostream>
#include <string>

int main()
{
    ListaDinamica<int> listaTeste;
    listaTeste.InsereFinal(10);
    listaTeste.InsereFinal(20);
    listaTeste.InsereFinal(30);
    listaTeste.ImprimeElementos();

    int ultimoElemento = listaTeste.RemoveFinal();
    std::cout << "Ultimo elemento:\n" << ultimoElemento << std::endl;
    
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