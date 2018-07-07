find_package(MySQL REQUIRED)

enable_language(ASM)

file(GLOB_RECURSE TARSUTIL_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../../include/util/*.h)

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    list(APPEND TARSUTIL_SRC "tc_jump_x86_64_sysv_elf_gas.s")
    list(APPEND TARSUTIL_SRC "tc_make_x86_64_sysv_elf_gas.s")
else ()
    list(APPEND TARSUTIL_SRC "tc_jump_i386_sysv_elf_gas.s")
    list(APPEND TARSUTIL_SRC "tc_make_i386_sysv_elf_gas.s")
endif ()

#link_directories(${MYSQL_LIB_DIR})

add_library(tarsutil STATIC ${TARSUTIL_SRC})
target_include_directories(tarsutil PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
target_include_directories(tarsutil PUBLIC ${MYSQL_INCLUDE_DIR})
#target_link_libraries(tarsutil ${MYSQL_LIBRARIES})

if("${TARS_SSL}" EQUAL 1)
    target_link_libraries(tarsutil crypto ssl)
else()
    target_link_libraries(tarsutil)
endif()

if("${TARS_HTTP2}" EQUAL 1)
    target_link_libraries(tarsutil nghttp2)
else()
    target_link_libraries(tarsutil)
endif()
