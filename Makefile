# Define o compilador C++ a ser usado
CXX = g++

# Define as flags de compilação:
# -std=c++11: Usa o padrão C++11
# -g: Inclui informações de depuração (para GDB)
# -Wall: Ativa todos os avisos (boa prática)
# -I include: Diz ao compilador para procurar arquivos de cabeçalho na pasta 'include'
CXXFLAGS = -std=c++11 -g -Wall -I include

# Onde o executável final será salvo e como ele se chamará
BIN_DIR = bin
EXEC = $(BIN_DIR)/tp2.out

# Pastas que contêm arquivos de código-fonte (.cpp)
SRC_DIRS = src src/tad src/utils

# Encontra automaticamente todos os arquivos .cpp nas pastas de código-fonte
SOURCES = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))

# Define onde os arquivos objeto (.o) serão salvos e cria seus nomes
OBJ_DIR = obj
OBJECTS = $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# Define argumentos para o comando 'run'
# Exemplo de uso: make run ARGS="meu_arquivo_de_teste.wkl"
ARGS = entrada.txt

# --- REGRAS DO MAKEFILE ---

# A regra padrão, chamada quando você digita apenas 'make'
all: $(EXEC)

# Regra para criar o executável final
# Ela depende de todos os arquivos objeto
$(EXEC): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Regra padrão para compilar um arquivo .cpp em um arquivo .o
# Ela cria os diretórios de objeto necessários automaticamente
$(OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Regra para executar o programa
# 'make run' irá compilar se necessário e depois rodar
run: all
	./$(EXEC) $(ARGS)

# Regra para limpar todos os arquivos gerados (executável e objetos)
clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR)

# Declara que 'all', 'clean', e 'run' não são nomes de arquivos
.PHONY: all clean run