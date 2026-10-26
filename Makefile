# Makefile para g++ / clang++ (Linux, macOS, MSYS2 o MinGW con una shell POSIX).
#
#   make             compila build/huffman
#   make test        comprime, descomprime y compara cada archivo de prueba
#   make clean       borra build/
#   make DEBUG=1     compila con símbolos de depuración y sin optimizar

CXX      ?= g++
CXXFLAGS ?= -std=c++11 -Wall -Wextra -pedantic
CPPFLAGS += -Iinclude

ifeq ($(DEBUG),1)
  CXXFLAGS += -O0 -g
else
  CXXFLAGS += -O2
endif

BUILD_DIR := build
TARGET    := $(BUILD_DIR)/huffman

SRCS := $(wildcard src/*.cpp src/*/*.cpp)
OBJS := $(patsubst src/%.cpp,$(BUILD_DIR)/obj/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

CASOS := $(wildcard examples/* tests/casos_borde/*)

.PHONY: all test clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/obj/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

test: $(TARGET)
	@rm -rf $(BUILD_DIR)/pruebas
	@mkdir -p $(BUILD_DIR)/pruebas
	@for caso in $(CASOS); do \
	  nombre=$$(basename "$$caso"); \
	  copia="$(BUILD_DIR)/pruebas/$$nombre"; \
	  cp "$$caso" "$$copia"; \
	  ./$(TARGET) "$$copia" > /dev/null || exit 1; \
	  mv "$$copia" "$$copia.orig"; \
	  ./$(TARGET) "$$copia.huf" > /dev/null || exit 1; \
	  if cmp -s "$$copia" "$$copia.orig"; then \
	    echo "OK    $$nombre: $$(wc -c < "$$copia.orig") -> $$(wc -c < "$$copia.huf") bytes"; \
	  else \
	    echo "FALLO $$nombre"; exit 1; \
	  fi; \
	done

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)
