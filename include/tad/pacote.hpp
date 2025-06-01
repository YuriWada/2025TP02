#ifndef PACOTE_HPP
#define PACOTE_HPP

#include <string>
#include "../utils/estados.hpp"
#include "../utils/time.hpp"

class Pacote
{
    public:
    void LerDadosEntrada();
    void ImprimirEstatisticas();

    private:
    /* variaveis gerais */
    Data data_hora(); // data-hora da postagem;
    std::string nome_remetente;
    std::string nome_destinatario;
    std::string tipo;
    int id_pacote;
    

    /* variaveis de estado do pacote */
    int estado_atual;
    double tempo_armazenado;
    double tempo_transportado;

};

#endif /* PACOTE_HPP */