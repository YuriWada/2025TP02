/**
 * Classe utilitaria para imprimir
 */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

// Função auxiliar para formatar um número com zeros à esquerda
inline std::string formatarID(int id, int largura)
{
    std::stringstream ss;
    ss << std::setw(largura) << std::setfill('0') << id;
    return ss.str();
}

// Função principal de log que formata a linha de saída completa
// Esta versão garante que não haja espaços extras no final.
inline void logEvento(int tempo, int id_pacote, const std::string &mensagem)
{
    std::cout << formatarID(tempo, 7)
              << " pacote "
              << formatarID(id_pacote, 3)
              << " "
              << mensagem   // A mensagem já contém os espaços que precisa
              << std::endl; // std::endl finaliza a linha sem adicionar mais espaços.
}

#endif // LOGGER_HPP