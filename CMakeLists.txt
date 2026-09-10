cmake_minimum_required(VERSION 3.10)
project(vividx
    VERSION 0.152001.0
    LANGUAGES C CXX
)

set(VIVIDX_CORE_HEADERS
    include/vividx/core/point.h
)

set(VIVIDX_CORE_SOURCES
    src/core/point.c
)

set(VIVIDX_CORE_PARTIAL_SOURCES
    src/core/SkMD5.cpp
    src/core/SkBase64.cpp
)

add_library(vividx-core-partial STATIC
    ${VIVIDX_CORE_SOURCES}
    ${VIVIDX_CORE_PARTIAL_SOURCES}
)

target_include_directories(vividx-core-partial
    PRIVATE ./include
)

target_include_directories(vividx-core-partial
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(vividx-core-partial PROPERTIES
    POSITION_INDEPENDENT_CODE ON    # -fPIC
)
