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
inline void logEvento(double tempo, int id_pacote, const std::string &mensagem)
{
    std::cout << formatarID(static_cast<int>(tempo), 7) << " pacote "
              << formatarID(id_pacote, 3) << " " << mensagem << std::endl;
}

#endif // LOGGER_HPP