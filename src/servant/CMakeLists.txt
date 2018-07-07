function(TARS_GENERATE_CPP)
    cmake_parse_arguments(TARS_ARGS "WITH_TARS" "INPUT;WORKDIR;OUTDIR" "" ${ARGN})

    if("${TARS_ARGS_WORKDIR}" STREQUAL "")
        get_filename_component(TARS_ARGS_WORKDIR ${TARS_ARGS_INPUT} DIRECTORY)
    endif()
    if("${TARS_ARGS_OUTDIR}" STREQUAL "")
        set(TARS_ARGS_OUTDIR ${CMAKE_CURRENT_SOURCE_DIR})
    endif()

    string(REGEX REPLACE "\\.[^.]*$" "" TARS_ARGS_INPUT_NAME_WE ${TARS_ARGS_INPUT})
    get_filename_component(TARS_ARGS_OUTPUT_NAME ${TARS_ARGS_INPUT_NAME_WE} NAME)
    set(TARS_ARGS_OUTPUT_NAME ${TARS_ARGS_OUTPUT_NAME}.h)
    set(TARS_ARGS_OUTPUT ${TARS_ARGS_OUTDIR}/${TARS_ARGS_OUTPUT_NAME})

    if(TARS_ARGS_WITH_TARS)
        set(TARS_COMMAND_ARGS "--with-tars")
    else()
        set(TARS_COMMAND_ARGS)
    endif()

    add_custom_command(
        OUTPUT ${TARS_ARGS_OUTPUT}
        WORKING_DIRECTORY "${TARS_ARGS_WORKDIR}"
        COMMAND tars2cpp
        ARGS ${TARS_COMMAND_ARGS} "--dir=${TARS_ARGS_OUTDIR}" ${TARS_ARGS_INPUT}
        DEPENDS ${TARS_ARGS_INPUT} tars2cpp)
    set_source_files_properties(${TARS_ARGS_OUTPUT} PROPERTIES GENERATED TRUE)
endfunction()

set(TARS_SERVANT_PROTOCOLS_OUTPUT)
set(TARS_SERVANT_PROTOCOLS AdminF Auth BaseF ConfigF EndpointF LogF NodeF NotifyF PropertyF StatF QueryF)

# 生成Servant协议
foreach(TARS_SERVANT_PROTOCOL ${TARS_SERVANT_PROTOCOLS})
    tars_generate_cpp(
        OUTDIR "${CMAKE_CURRENT_BINARY_DIR}/servant" WITH_TARS
        INPUT "${CMAKE_CURRENT_SOURCE_DIR}/../../protocol/${TARS_SERVANT_PROTOCOL}.tars")
    list(APPEND TARS_SERVANT_PROTOCOLS_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/servant/${TARS_SERVANT_PROTOCOL}.h")
endforeach()

# 生成Wup协议
tars_generate_cpp(
    OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/tup WITH_TARS
    INPUT ${CMAKE_CURRENT_SOURCE_DIR}/../../protocol/RequestF.tars)
list(APPEND TARS_SERVANT_PROTOCOLS_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/tup/RequestF.h")

file(GLOB_RECURSE TARSSERVANT_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/servant/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/promise/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/jmem/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/tup/*.h)

add_library(tarsservant STATIC ${TARSSERVANT_SRC} ${TARS_SERVANT_PROTOCOLS_OUTPUT})
target_include_directories(tarsservant PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(tarsservant PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
target_link_libraries(tarsservant tarsutil)
