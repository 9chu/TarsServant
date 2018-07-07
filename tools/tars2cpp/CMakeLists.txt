file(GLOB_RECURSE TARS2CPP_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(tars2cpp ${TARS2CPP_SRC})
target_link_libraries(tars2cpp tarsparse tarsutil)
