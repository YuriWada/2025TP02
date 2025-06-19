#!/bin/bash

# ===================================================================
# Script de Teste de Integração para o Simulador de Armazéns Hanoi
# Autor: Seu Nome
# Data:  17 de Junho de 2025
# ===================================================================

# --- CONFIGURAÇÃO ---

# O nome que daremos ao programa compilado
EXECUTAVEL="simulador"
# O arquivo de entrada que o seu programa lê
ARQUIVO_ENTRADA="entrada.txt"
# O arquivo onde a saída do SEU programa será salva temporariamente
ARQUIVO_SAIDA_GERADA="minha_saida.txt"
# O arquivo com a saída CORRETA que o professor forneceu
ARQUIVO_GABARITO="saida1.txt"

# --- CORES PARA O TERMINAL (Deixa a saída mais bonita) ---
VERDE='\033[0;32m'
VERMELHO='\033[0;31m'
AMARELO='\033[1;33m'
NC='\033[0m' # Sem Cor

# --- PASSO 1: COMPILAÇÃO ---
echo -e "${AMARELO}--- 1. Compilando o projeto... ---${NC}"
g++ -std=c++11 -g -Wall -I include src/main.cpp src/tad/*.cpp src/utils/*.cpp -o $EXECUTAVEL

# Verifica se o comando anterior (g++) teve sucesso. '$?' guarda o código de saída.
if [ $? -ne 0 ]; then
    echo -e "\n${VERMELHO}ERRO DE COMPILACAO! Teste abortado.${NC}"
    exit 1
fi
echo "Compilacao bem-sucedida."

# --- PASSO 2: EXECUÇÃO ---
echo -e "\n${AMARELO}--- 2. Executando o simulador para gerar a saida... ---${NC}"

# Verifica se o arquivo de entrada necessário existe
if [ ! -f $ARQUIVO_ENTRADA ]; then
    echo -e "${VERMELHO}ERRO: Arquivo de entrada '$ARQUIVO_ENTRADA' nao encontrado!${NC}"
    exit 1
fi

# Executa o programa e redireciona a saida padrao (stdout) para o arquivo de saida
./$EXECUTAVEL $ARQUIVO_ENTRADA > $ARQUIVO_SAIDA_GERADA
echo "Arquivo '$ARQUIVO_SAIDA_GERADA' gerado."

# --- PASSO 3: COMPARAÇÃO E VEREDITO ---
echo -e "\n${AMARELO}--- 3. Comparando sua saida com o gabarito... ---${NC}"

# Verifica se o arquivo de gabarito existe
if [ ! -f $ARQUIVO_GABARITO ]; then
    echo -e "${VERMELHO}ERRO: Arquivo de gabarito '$ARQUIVO_GABARITO' nao encontrado! Coloque-o na pasta raiz.${NC}"
    exit 1
fi

# Usa o comando 'diff' para comparar os dois arquivos.
# A flag '-q' (quiet) faz com que ele não imprima nada se os arquivos forem iguais.
# A flag '--strip-trailing-cr' ajuda a ignorar problemas de final de linha entre Windows/Linux.
diff -q --strip-trailing-cr $ARQUIVO_SAIDA_GERADA $ARQUIVO_GABARITO

# Verifica o código de saída do diff: 0 para arquivos iguais, 1 para diferentes.
if [ $? -eq 0 ]; then
    echo -e "\n${VERDE}===================================="
    echo -e "          TESTE PASSOU! ✅"
    echo -e "====================================${NC}"
    echo "Sua saida e identica ao gabarito."
else
    echo -e "\n${VERMELHO}===================================="
    echo -e "          TESTE FALHOU! ❌"
    echo -e "====================================${NC}"
    echo "Foram encontradas diferencas entre a sua saida e o gabarito."
    echo "Veja a comparacao detalhada abaixo:"
    echo "------------------------------------------------------------"
    # Mostra as diferenças de forma legível (lado a lado) para você poder depurar
    diff -y --suppress-common-lines $ARQUIVO_SAIDA_GERADA $ARQUIVO_GABARITO
    echo "------------------------------------------------------------"
fi