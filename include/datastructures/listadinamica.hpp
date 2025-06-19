/**
 * Codigo adaptado das notas de aula
 * Principal adaptação: classe transformada em Template
 */

#ifndef LISTADINAMICA_HPP
#define LISTADINAMICA_HPP

#include <iostream>
#include <stdexcept> // Para exceções padrão

// classe para uma lista dinamica
// eh uma classe template para ser usada genericamente
template <typename T>
class ListaDinamica
{
public:
    // construtor
    ListaDinamica() : m_dados(nullptr), m_tamanho(0), m_capacidade(0) {};

    // destrutor
    ~ListaDinamica()
    {
        delete[] m_dados;
    }

    // Construtor de Cópia (Deep Copy)
    ListaDinamica(const ListaDinamica<T> &outra)
        : m_dados(nullptr), m_tamanho(0), m_capacidade(0)
    {
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

    // Operador de Atribuição por Cópia (=)
    ListaDinamica<T> &operator=(const ListaDinamica<T> &outra)
    {
        if (this == &outra)
        {
            return *this;
        }
        delete[] this->m_dados;
        this->m_dados = nullptr;
        this->m_tamanho = 0;
        this->m_capacidade = 0;
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
        return *this;
    }

    // redimensiona o vetor para uma nova capacidade
    void redimensionar(int nova_capacidade)
    {
        T *novosDados = new T[nova_capacidade];
        int itens_a_copiar = (m_tamanho < nova_capacidade) ? m_tamanho : nova_capacidade;
        for (int i = 0; i < itens_a_copiar; ++i)
        {
            novosDados[i] = m_dados[i];
        }
        delete[] m_dados;
        m_dados = novosDados;
        m_capacidade = nova_capacidade;
    }

    // insere elemento no final do vetor
    void InsereFinal(const T &element)
    {
        if (m_tamanho >= m_capacidade)
        {
            redimensionar(m_capacidade == 0 ? 2 : m_capacidade * 2);
        }
        m_dados[m_tamanho] = element;
        m_tamanho++;
    }

    // insere elemento no inicio do vetor
    void InsereInicio(const T &element)
    {
        if (m_tamanho >= m_capacidade)
        {
            redimensionar(m_capacidade == 0 ? 2 : m_capacidade * 2);
        }
        // passa todos os elementos para a posicao a frente
        for (int i = m_tamanho; i > 0; i--)
        {
            m_dados[i] = m_dados[i - 1];
        }
        m_dados[0] = element;
        m_tamanho++;
    }

    // Insere elemento em dada posicao
    void InsereElemento(const T &element, int pos)
    {
        if (pos < 0 || pos > m_tamanho)
        {
            throw std::out_of_range("Posicao de insercao invalida!");
        }
        if (m_tamanho >= m_capacidade)
        {
            redimensionar(m_capacidade == 0 ? 2 : m_capacidade * 2);
        }
        // Desloca elementos para a direita para abrir espaço
        for (int i = m_tamanho; i > pos; i--)
        {
            m_dados[i] = m_dados[i - 1];
        }
        m_dados[pos] = element;
        m_tamanho++;
    }

    // remove e retorna o ultimo elemento
    T RemoveFinal()
    {
        if (m_tamanho == 0)
        {
            throw std::runtime_error("Nao eh possivel remover de lista vazia!");
        }

        // 1. Guarda o elemento que está na última posição
        T elementoRemovido = m_dados[m_tamanho - 1];
        // 2. Decrementa o tamanho para "remover" logicamente o elemento
        m_tamanho--;
        // 3. Retorna o elemento que foi salvo
        return elementoRemovido;
    }

    // remove e retorna elemento da posicao pos
    T RemoveElemento(int pos)
    {
        if (pos < 0 || pos >= m_tamanho)
        {
            throw std::out_of_range("Posicao de remocao invalida!");
        }
        T placeHolder = m_dados[pos];
        // Desloca elementos para a esquerda para fechar o espaço
        for (int i = pos; i < m_tamanho - 1; i++)
        {
            m_dados[i] = m_dados[i + 1];
        }
        m_tamanho--;
        return placeHolder;
    }

    // retorna elemento da posicao
    T &BuscaElemento(int pos)
    {
        if (pos < 0 || pos >= m_tamanho)
        {
            throw std::out_of_range("BuscaElemento: Indice fora dos limites!");
        }
        return m_dados[pos];
    }

    const T &BuscaElemento(int pos) const
    {
        if (pos < 0 || pos >= m_tamanho)
        {
            throw std::out_of_range("BuscaElemento (const): Indice fora dos limites!");
        }
        return m_dados[pos];
    }

    // imprime elementos da lista
    void ImprimeElementos() const
    {
        for (int i = 0; i < m_tamanho; i++)
            std::cout << m_dados[i] << std::endl;
    }

    void LimparLista() { m_tamanho = 0; }
    bool ListaVazia() const { return m_tamanho == 0; }
    int GetTamanho() const { return m_tamanho; }
    int GetCapacidade() const { return m_capacidade; }

private:
    T *m_dados;       // armazena os dados do array dinamico
    int m_tamanho;    // numero atual de elementos no array
    int m_capacidade; // capacidade max de elementos no array
};

#endif /* ListaDinamica_HPP */