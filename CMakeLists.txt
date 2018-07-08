cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

set(CMAKE_CXX_STANDARD 11)

set(TARS_SSL 0)
set(TARS_HTTP2 0)
set(TARS_VERSION "1.1.0")
add_definitions(-DTARS_SSL=${TARS_SSL})
add_definitions(-DTARS_HTTP2=${TARS_HTTP2})
add_definitions(-DTARS_VERSION="${TARS_VERSION}")

add_subdirectory(src/util)
add_subdirectory(tools/tarsparse)
add_subdirectory(tools/tars2cpp)
add_subdirectory(src/servant)

function(TARS_GENERATE_TAR TARGET)
    add_custom_target(${TARGET}-tar COMMAND
        tar "-czvf" "${TARGET}.tgz" "--transform" "'s,^.*/\\|^,${TARGET}/,'" $<TARGET_FILE:${TARGET}>
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS ${TARGET})
endfunction()

function(TARS_UPLOAD_TAR URL APP TARGET)
    add_custom_target(${TARGET}-upload COMMAND
        curl -X POST -F "file=@${TARGET}.tgz"
        "${URL}/pages/server/api/upload_patch_package?application=${APP}\\&module_name=${TARGET}\\&comment=AutoUpload"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS ${TARGET}-tar)
endfunction()

function(TARS_AUTO_DEPLOY_TAR URL APP TARGET)
    add_custom_target(${TARGET}-auto-deploy COMMAND
        bash -c "$<TARGET_PROPERTY:tarsutil,SOURCE_DIR>/../../tools/auto_deploy.sh '${URL}' '${APP}' '${TARGET}'"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS ${TARGET}-tar)
endfunction()
