# Configuración para Linux
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Iinclude
LDFLAGS = 
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
EXECUTABLE = $(BIN_DIR)/disco_simulador

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR)

run: $(EXECUTABLE)
	@./$(EXECUTABLE)

.PHONY: all clean run