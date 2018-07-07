function(FLEX_GENERATE_CPP)
    cmake_parse_arguments(FLEX_ARGS "" "OUTPUT;INPUT" "" ${ARGN})

    add_custom_command(
        OUTPUT ${FLEX_ARGS_OUTPUT}
        COMMAND flex
        ARGS "-o" ${FLEX_ARGS_OUTPUT} ${FLEX_ARGS_INPUT}
        DEPENDS ${FLEX_ARGS_INPUT})

    set_source_files_properties(${FLEX_ARGS_OUTPUT} PROPERTIES GENERATED TRUE)
endfunction()

function(YACC_GENERATE_CPP)
    cmake_parse_arguments(YACC_ARGS "" "OUTPUT;INPUT" "" ${ARGN})

    string(REGEX REPLACE "\\.[^.]*$" "" YACC_ARGS_OUTPUT_NAME_WE ${YACC_ARGS_OUTPUT})
    set(YACC_ARGS_OUTPUT_HPP ${YACC_ARGS_OUTPUT_NAME_WE}.hpp)
    set(YACC_ARGS_OUTPUT_CPP ${YACC_ARGS_OUTPUT_NAME_WE}.cpp)

    add_custom_command(
        OUTPUT ${YACC_ARGS_OUTPUT_HPP} ${YACC_ARGS_OUTPUT_CPP}
        COMMAND bison
        ARGS "-o" ${YACC_ARGS_OUTPUT_CPP} ${YACC_ARGS_INPUT}
        DEPENDS ${YACC_ARGS_INPUT})

    set_source_files_properties(${YACC_ARGS_OUTPUT_HPP} PROPERTIES GENERATED TRUE)
    set_source_files_properties(${YACC_ARGS_OUTPUT_CPP} PROPERTIES GENERATED TRUE)
endfunction()

flex_generate_cpp(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tars.lex.cpp
    INPUT ${CMAKE_CURRENT_SOURCE_DIR}/tars.l)

yacc_generate_cpp(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tars.tab.cpp
    INPUT ${CMAKE_CURRENT_SOURCE_DIR}/tars.y)

file(GLOB_RECURSE TARSPARSE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
list(APPEND TARSPARSE_SRC ${CMAKE_CURRENT_BINARY_DIR}/tars.lex.cpp)
list(APPEND TARSPARSE_SRC ${CMAKE_CURRENT_BINARY_DIR}/tars.tab.cpp)

add_library(tarsparse STATIC ${TARSPARSE_SRC})
target_include_directories(tarsparse PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(tarsparse tarsutil)
target_include_directories(tarsparse PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
