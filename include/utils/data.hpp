#ifndef TIME_HPP
#define TIME_HPP

#include <string>

class Data
{
    public:
    Data();
    std::string GetData();
    void ConverteHora_Decimal_String();
    void ConverteHora_String_Decimal();
    void Formata_Data(); // formata a data para impressao de estatisticas
    double GetHora_Decimal();
    std::string GetHora_String();

    private:
    // variavel data
    std::string data;

    // variaveis tempo
    double hora; // horario em decimal
    std::string hora_s; // horario em string formato hh:mm:ss
};

#endif /* TIME_HPP */