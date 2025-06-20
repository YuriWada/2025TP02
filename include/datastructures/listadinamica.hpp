#ifndef LISTADINAMICA_HPP
#define LISTADINAMICA_HPP

#include <iostream>
#include <stdexcept>

// classe de lista com array dinâmico, pra não ter tamanho fixo.
// Fiz como template pra poder usar com qualquer tipo (int, Pacote, etc).
template <typename T>
class ListaDinamica
{
public:
    // Construtor padrão.
    // Começa tudo zerado e com ponteiro nulo pra não dar problema.
    ListaDinamica() : ld_dados(nullptr), ld_tamanho(0), ld_capacidade(0) {};

    // Destrutor, importante pra liberar a memória do array que eu aloquei com 'new'.
    ~ListaDinamica()
    {
        delete[] ld_dados;
    }

    // Construtor de Cópia, uma das partes da "Regra dos Três".
    // Necessário pra fazer uma cópia de verdade (deep copy) e não só copiar o ponteiro.
    ListaDinamica(const ListaDinamica<T> &outra)
        : ld_dados(nullptr), ld_tamanho(0), ld_capacidade(0)
    {
        // Inicia a nova lista como se fosse do zero.
        // Se a outra lista tiver elementos, aloca memória e copia um por um.
        if (outra.GetTamanho() > 0)
        {
            this->ld_dados = new T[outra.GetCapacidade()];
            this->ld_capacidade = outra.GetCapacidade();
            this->ld_tamanho = outra.GetTamanho();
            for (int i = 0; i < this->ld_tamanho; ++i)
            {
                this->ld_dados[i] = outra.BuscaElemento(i);
            }
        }
    }

    // Operador de Atribuição por Cópia (=), outra parte da "Regra dos Três".
    ListaDinamica<T> &operator=(const ListaDinamica<T> &outra)
    {
        // Primeiro, checo se não estou fazendo algo tipo 'lista1 = lista1'.
        if (this == &outra)
        {
            return *this;
        }
        // Depois, limpo o que a lista atual tinha pra não vazar memória.
        delete[] this->ld_dados;
        this->ld_dados = nullptr;
        this->ld_tamanho = 0;
        this->ld_capacidade = 0;

        // Aí sim, copio os dados da outra lista pra cá.
        if (outra.GetTamanho() > 0)
        {
            this->ld_dados = new T[outra.GetCapacidade()];
            this->ld_capacidade = outra.GetCapacidade();
            this->ld_tamanho = outra.GetTamanho();
            for (int i = 0; i < this->ld_tamanho; ++i)
            {
                this->ld_dados[i] = outra.BuscaElemento(i);
            }
        }
        // Retorno *this pra permitir coisas como lista1 = lista2 = lista3.
        return *this;
    }

    // Função auxiliar pra quando o array enche e precisa de mais espaço.
    void redimensionar(int nova_capacidade)
    {
        // Aloca um novo array, maior.
        T *novosDados = new T[nova_capacidade];
        // Descobre quantos elementos precisam ser copiados (útil se o array diminuir).
        int itens_a_copiar = (ld_tamanho < nova_capacidade) ? ld_tamanho : nova_capacidade;

        // Copia os elementos do array antigo pro novo.
        for (int i = 0; i < itens_a_copiar; ++i)
        {
            novosDados[i] = ld_dados[i];
        }

        // Libera a memória do array antigo e aponta para o novo.
        delete[] ld_dados;
        ld_dados = novosDados;
        ld_capacidade = nova_capacidade;
    }

    // Método pra adicionar um item no fim da lista.
    void InsereFinal(const T &element)
    {
        // Checa se ainda tem espaço, se não tiver, dobra o tamanho.
        if (ld_tamanho >= ld_capacidade)
        {
            redimensionar(ld_capacidade == 0 ? 2 : ld_capacidade * 2);
        }
        // Coloca o novo elemento na primeira posição livre e incrementa o contador de tamanho.
        ld_dados[ld_tamanho] = element;
        ld_tamanho++;
    }

    // Adiciona um item no comecinho da lista.
    void InsereInicio(const T &element)
    {
        // Garante que tem espaço.
        if (ld_tamanho >= ld_capacidade)
        {
            redimensionar(ld_capacidade == 0 ? 2 : ld_capacidade * 2);
        }
        // Empurra todo mundo uma posição pra frente pra abrir espaço no início.
        for (int i = ld_tamanho; i > 0; i--)
        {
            ld_dados[i] = ld_dados[i - 1];
        }
        // Coloca o elemento novo na posição 0 e atualiza o tamanho.
        ld_dados[0] = element;
        ld_tamanho++;
    }

    // Insere um elemento numa posição qualquer no meio da lista.
    void InsereElemento(const T &element, int pos)
    {
        // Valida se a posição é possível (de 0 até o final da lista).
        if (pos < 0 || pos > ld_tamanho)
        {
            throw std::out_of_range("Posicao de insercao invalida!");
        }
        // Garante que tem espaço.
        if (ld_tamanho >= ld_capacidade)
        {
            redimensionar(ld_capacidade == 0 ? 2 : ld_capacidade * 2);
        }
        // Desloca os elementos pra direita pra abrir o espaço na posição 'pos'.
        for (int i = ld_tamanho; i > pos; i--)
        {
            ld_dados[i] = ld_dados[i - 1];
        }
        // Coloca o elemento e atualiza o tamanho.
        ld_dados[pos] = element;
        ld_tamanho++;
    }

    // Remove e retorna o ultimo elemento.
    T RemoveFinal()
    {
        if (ld_tamanho == 0)
        {
            throw std::runtime_error("Nao eh possivel remover de lista vazia!");
        }
        // Salva o elemento que está na última posição.
        T elementoRemovido = ld_dados[ld_tamanho - 1];
        // Decrementa o tamanho para "remover" logicamente o elemento.
        ld_tamanho--;
        // Retorna o elemento que foi salvo.
        return elementoRemovido;
    }

    // Remove um elemento de uma posição qualquer.
    T RemoveElemento(int pos)
    {
        if (pos < 0 || pos >= ld_tamanho)
        {
            throw std::out_of_range("Posicao de remocao invalida!");
        }
        // Salva o elemento que vai ser removido pra poder retornar ele.
        T placeHolder = ld_dados[pos];
        // Puxa todo mundo da direita pra esquerda pra fechar o buraco.
        for (int i = pos; i < ld_tamanho - 1; i++)
        {
            ld_dados[i] = ld_dados[i + 1];
        }
        // Atualiza o tamanho e retorna o que foi salvo.
        ld_tamanho--;
        return placeHolder;
    }

    // Retorna uma referência pra um elemento, permitindo modificação.
    // Ex: lista.BuscaElemento(i) = novo_valor;
    T &BuscaElemento(int pos)
    {
        if (pos < 0 || pos >= ld_tamanho)
        {
            throw std::out_of_range("BuscaElemento: Indice fora dos limites!");
        }
        return ld_dados[pos];
    }

    // Versão const de BuscaElemento, pra ser usada em métodos 'const'.
    // Retorna uma referência constante, que é só pra leitura.
    const T &BuscaElemento(int pos) const
    {
        if (pos < 0 || pos >= ld_tamanho)
        {
            throw std::out_of_range("BuscaElemento (const): Indice fora dos limites!");
        }
        return ld_dados[pos];
    }

    // Imprime todos os elementos pra eu poder debugar.
    void ImprimeElementos() const
    {
        for (int i = 0; i < ld_tamanho; i++)
            std::cout << ld_dados[i] << std::endl;
    }

    // Zera o tamanho da lista, mas não libera a memória alocada.
    void LimparLista() { ld_tamanho = 0; }
    // Diz se a lista ta vazia.
    bool ListaVazia() const { return ld_tamanho == 0; }
    // Retorna o número de elementos.
    int GetTamanho() const { return ld_tamanho; }
    // Retorna a capacidade atual do array.
    int GetCapacidade() const { return ld_capacidade; }

private:
    T *ld_dados;       // Onde os dados ficam guardados mesmo, o array.
    int ld_tamanho;    // Quantos elementos eu tenho agora.
    int ld_capacidade; // Quantos elementos cabem no array antes de ter que redimensionar.
};

#endif /* ListaDinamica_HPP */