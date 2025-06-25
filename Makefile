# Compilador e flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Iinclude

# Diretórios
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Executável
TARGET = $(BIN_DIR)/tp2.out

# Lista de arquivos fonte
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Lista de arquivos objeto (em obj/)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Regra padrão
all: $(TARGET)

# Linka os objetos para formar o executável
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJS) -o $@

# Compila cada .cpp em um .o dentro de obj/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza de arquivos objeto e binário
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
