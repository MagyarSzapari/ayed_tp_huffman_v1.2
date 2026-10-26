cmake_minimum_required(VERSION 3.15)

# Si no se indica el tipo de compilación, se usa Release. Tiene que definirse
# antes de project(): con MSVC, project() inicializa CMAKE_BUILD_TYPE en Debug.
if(NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Tipo de compilación")
endif()

project(HuffmanTP
  VERSION 1.0.0
  DESCRIPTION "Compresor y descompresor de archivos con el algoritmo de Huffman"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(huffman
  src/main.cpp
  src/funciones/strings.cpp
  src/huffman/HuffmanTree.cpp
  src/huffman/BitWriter.cpp
  src/huffman/BitReader.cpp
  src/huffman/Compresor.cpp
  src/huffman/Descompresor.cpp
)

target_include_directories(huffman PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(MSVC)
  target_compile_options(huffman PRIVATE /W4 /utf-8)
  target_compile_definitions(huffman PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
  target_compile_options(huffman PRIVATE -Wall -Wextra -pedantic)
endif()

# ---------------------------------------------------------------------------
# Pruebas: cada archivo de examples/ y de tests/casos_borde/ se comprime,
# se descomprime y se compara byte a byte con el original.
#   cmake --build build && ctest --test-dir build --output-on-failure
# ---------------------------------------------------------------------------
enable_testing()

file(GLOB CASOS_DE_PRUEBA
  ${CMAKE_CURRENT_SOURCE_DIR}/examples/*
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/casos_borde/*)

foreach(caso ${CASOS_DE_PRUEBA})
  get_filename_component(nombre ${caso} NAME)
  add_test(NAME roundtrip_${nombre}
    COMMAND ${CMAKE_COMMAND}
      -DHUFFMAN=$<TARGET_FILE:huffman>
      -DENTRADA=${caso}
      -DDIRECTORIO=${CMAKE_CURRENT_BINARY_DIR}/pruebas
      -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/roundtrip.cmake)
endforeach()
