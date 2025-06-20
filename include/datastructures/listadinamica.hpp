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
    ListaDinamica() : m_dados(nullptr), m_tamanho(0), m_capacidade(0) {};

    // Destrutor, importante pra liberar a memória do array que eu aloquei com 'new'.
    ~ListaDinamica()
    {
        delete[] m_dados;
    }

    // Construtor de Cópia, uma das partes da "Regra dos Três".
    // Necessário pra fazer uma cópia de verdade (deep copy) e não só copiar o ponteiro.
    ListaDinamica(const ListaDinamica<T> &outra)
        : m_dados(nullptr), m_tamanho(0), m_capacidade(0)
    {
        // Inicia a nova lista como se fosse do zero.
        // Se a outra lista tiver elementos, aloca memória e copia um por um.
        if (outra.GetTamanho() > 0)
        {
            this->m_dados = new T[outra.GetCapacidade()];
            this->m_capacidade = outra.GetCapacidade();
            this->m_tamanho = outra.GetTamanho();
            for (int i = 0; i < this->m_tamanho; ++i)
            {
                this->m_dados[i] = outra.BuscaElemento(i);
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
        delete[] this->m_dados;
        this->m_dados = nullptr;
        this->m_tamanho = 0;
        this->m_capacidade = 0;

        // Aí sim, copio os dados da outra lista pra cá.
        if (outra.GetTamanho() > 0)
        {
            this->m_dados = new T[outra.GetCapacidade()];
            this->m_capacidade = outra.GetCapacidade();
            this->m_tamanho = outra.GetTamanho();
            for (int i = 0; i < this->m_tamanho; ++i)
            {
                this->m_dados[i] = outra.BuscaElemento(i);
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
        int itens_a_copiar = (m_tamanho < nova_capacidade) ? m_tamanho : nova_capacidade;

        // Copia os elementos do array antigo pro novo.
        for (int i = 0; i < itens_a_copiar; ++i)
        {
            novosDados[i] = m_dados[i];
        }

        // Libera a memória do array antigo e aponta para o novo.
        delete[] m_dados;
        m_dados = novosDados;
        m_capacidade = nova_capacidade;
    }

    // Método pra adicionar um item no fim da lista.
    void InsereFinal(const T &element)
    {
        // Checa se ainda tem espaço, se não tiver, dobra o tamanho.
        if (m_tamanho >= m_capacidade)
        {
            redimensionar(m_capacidade == 0 ? 2 : m_capacidade * 2);
        }
        // Coloca o novo elemento na primeira posição livre e incrementa o contador de tamanho.
        m_dados[m_tamanho] = element;
        m_tamanho++;
    }

    // Adiciona um item no comecinho da lista.
    void InsereInicio(const T &element)
    {
        // Garante que tem espaço.
        if (m_tamanho >= m_capacidade)
        {
            redimensionar(m_capacidade == 0 ? 2 : m_capacidade * 2);
        }
        // Empurra todo mundo uma posição pra frente pra abrir espaço no início.
        for (int i = m_tamanho; i > 0; i--)
        {
            m_dados[i] = m_dados[i - 1];
        }
        // Coloca o elemento novo na posição 0 e atualiza o tamanho.
        m_dados[0] = element;
        m_tamanho++;
    }

    // Insere um elemento numa posição qualquer no meio da lista.
    void InsereElemento(const T &element, int pos)
    {
        // Valida se a posição é possível (de 0 até o final da lista).
        if (pos < 0 || pos > m_tamanho)
        {
            throw std::out_of_range("Posicao de insercao invalida!");
        }
        // Garante que tem espaço.
        if (m_tamanho >= m_capacidade)
        {
            redimensionar(m_capacidade == 0 ? 2 : m_capacidade * 2);
        }
        // Desloca os elementos pra direita pra abrir o espaço na posição 'pos'.
        for (int i = m_tamanho; i > pos; i--)
        {
            m_dados[i] = m_dados[i - 1];
        }
        // Coloca o elemento e atualiza o tamanho.
        m_dados[pos] = element;
        m_tamanho++;
    }

    // Remove e retorna o ultimo elemento.
    T RemoveFinal()
    {
        if (m_tamanho == 0)
        {
            throw std::runtime_error("Nao eh possivel remover de lista vazia!");
        }
        // Salva o elemento que está na última posição.
        T elementoRemovido = m_dados[m_tamanho - 1];
        // Decrementa o tamanho para "remover" logicamente o elemento.
        m_tamanho--;
        // Retorna o elemento que foi salvo.
        return elementoRemovido;
    }

    // Remove um elemento de uma posição qualquer.
    T RemoveElemento(int pos)
    {
        if (pos < 0 || pos >= m_tamanho)
        {
            throw std::out_of_range("Posicao de remocao invalida!");
        }
        // Salva o elemento que vai ser removido pra poder retornar ele.
        T placeHolder = m_dados[pos];
        // Puxa todo mundo da direita pra esquerda pra fechar o buraco.
        for (int i = pos; i < m_tamanho - 1; i++)
        {
            m_dados[i] = m_dados[i + 1];
        }
        // Atualiza o tamanho e retorna o que foi salvo.
        m_tamanho--;
        return placeHolder;
    }

    // Retorna uma referência pra um elemento, permitindo modificação.
    // Ex: lista.BuscaElemento(i) = novo_valor;
    T &BuscaElemento(int pos)
    {
        if (pos < 0 || pos >= m_tamanho)
        {
            throw std::out_of_range("BuscaElemento: Indice fora dos limites!");
        }
        return m_dados[pos];
    }

    // Versão const de BuscaElemento, pra ser usada em métodos 'const'.
    // Retorna uma referência constante, que é só pra leitura.
    const T &BuscaElemento(int pos) const
    {
        if (pos < 0 || pos >= m_tamanho)
        {
            throw std::out_of_range("BuscaElemento (const): Indice fora dos limites!");
        }
        return m_dados[pos];
    }

    // Imprime todos os elementos pra eu poder debugar.
    void ImprimeElementos() const
    {
        for (int i = 0; i < m_tamanho; i++)
            std::cout << m_dados[i] << std::endl;
    }

    // Zera o tamanho da lista, mas não libera a memória alocada.
    void LimparLista() { m_tamanho = 0; }
    // Diz se a lista ta vazia.
    bool ListaVazia() const { return m_tamanho == 0; }
    // Retorna o número de elementos.
    int GetTamanho() const { return m_tamanho; }
    // Retorna a capacidade atual do array.
    int GetCapacidade() const { return m_capacidade; }

private:
    T *m_dados;       // Onde os dados ficam guardados mesmo, o array.
    int m_tamanho;    // Quantos elementos eu tenho agora.
    int m_capacidade; // Quantos elementos cabem no array antes de ter que redimensionar.
};

#endif /* ListaDinamica_HPP */