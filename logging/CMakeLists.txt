cmake_minimum_required(VERSION 3.17)
project(logging)

add_library(logging STATIC src/logging.c)

set(EXPORT_DIR exports)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${EXPORT_DIR}
    PRIVATE ${PROJECT_SOURCE_DIR}
)
