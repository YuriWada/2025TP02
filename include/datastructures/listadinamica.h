#ifndef ListaDinamica_H
#define ListaDinamica_H

// classe para uma lista dinamica
// eh uma classe template para ser usada genericamente
template <typename T>
class ListaDinamica
{
    public:
    // construtor
    void ListaDinamica() : m_dados(nullptr), m_tamanho(0), m_capacidade(0) {};

    // destrutor
    ~ListaDinamica() { delete[] m_dados; }

    // redimensiona o vetor para uma nova capacidade
    void redimensionar(int nova_capacidade)
    {
        T* novosDados = new T[nova_capacidade];
        
        for (int i = 0; i < m_tamanho; ++i)
            novosDados[i] = m_dados[i]; // lembrar que aqui eh necessario ter construtor de copia 

        delete[] m_dados;
        m_dados = novosDados;
        m_capacidade = nova_capacidade;
    }

    // insere elemento no final do vetor
    void InsereFinal(const T& element)
    {
        if (m_tamanho >= m_capacidade) 
            redimensionar(m_capacidade == 0 ? 1 : m_capacidade*2); // dobra capacidade do vetor

        m_dados[m_tamanho] = element; // armazena o elemento tipo T (generico)
        m_tamanho++;
    }

    // insere elemento no inicio do vetor
    void InsereInicio(const T& element)
    {
        if (m_tamanho >= m_capacidade)
            redimensionar(m_capacidade == 0 ? 1 : m_capacidade*2);

        // se o vetor nao tiver nenhum elemento
        if (m_tamanho == 0)
        {
            m_dados[0] = element; 
            return;
        }

        // passa todos os elementos para a posicao a frente
        for (int i = m_tamanho-1; i > 0; i--)
            m_dados[i+1] = m_dados[i];
        
        m_dados[0] = element;
    }

    // remove e retorna o ultimo elemento
    T RemoveFinal()
    {
        // apenas diminui tamanho do array, ignorando ultimo elemento
        if (m_tamanho > 0)
            m_tamanho--;
        else 
            throw "Nao eh possivel remover ultimo elemento!";
    
        T ultimoElemento = m_dados[m_tamanho+1];
        return ultimoElemento;    
    }

    // retorna elemento da posicao
    T BuscaElemento(int pos)
    {
        return m_dados[pos];
    }

    private:
    T* m_dados; // armazena os dados do array dinamico
    int m_tamanho; // numero atual de elementos no array
    int m_capacidade; // capacidade max de elementos no array
};

#endif /* ListaDinamica_H */