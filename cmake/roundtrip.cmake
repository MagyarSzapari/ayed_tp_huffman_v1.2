# Prueba de ida y vuelta para un archivo:
#   1. copia ENTRADA a un directorio de trabajo,
#   2. la comprime (genera <archivo>.huf),
#   3. renombra la copia a <archivo>.orig,
#   4. descomprime el .huf (restaura <archivo>),
#   5. compara el archivo restaurado con <archivo>.orig.
#
# Variables: HUFFMAN (ejecutable), ENTRADA (archivo de prueba), DIRECTORIO (trabajo).

get_filename_component(nombre "${ENTRADA}" NAME)
set(dir "${DIRECTORIO}/${nombre}.d")
set(copia "${dir}/${nombre}")

file(REMOVE_RECURSE "${dir}")
file(MAKE_DIRECTORY "${dir}")
file(COPY "${ENTRADA}" DESTINATION "${dir}")

execute_process(COMMAND "${HUFFMAN}" "${copia}" RESULT_VARIABLE rc)
if(NOT rc EQUAL 0)
  message(FATAL_ERROR "Falló la compresión de ${nombre} (código ${rc})")
endif()

file(RENAME "${copia}" "${copia}.orig")

execute_process(COMMAND "${HUFFMAN}" "${copia}.huf" RESULT_VARIABLE rc)
if(NOT rc EQUAL 0)
  message(FATAL_ERROR "Falló la descompresión de ${nombre}.huf (código ${rc})")
endif()

execute_process(
  COMMAND "${CMAKE_COMMAND}" -E compare_files "${copia}" "${copia}.orig"
  RESULT_VARIABLE rc)
if(NOT rc EQUAL 0)
  message(FATAL_ERROR "El archivo restaurado no coincide con ${nombre}")
endif()

file(SIZE "${copia}.orig" original)
file(SIZE "${copia}.huf" comprimido)
message(STATUS "OK ${nombre}: ${original} -> ${comprimido} bytes")
