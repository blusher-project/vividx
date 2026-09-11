cmake_minimum_required(VERSION 3.10)
project(vividx
    VERSION 0.152001.0
    LANGUAGES C CXX
)

set(VIVIDX_CORE_HEADERS
    include/vividx/core/point.h
    include/vividx/gpu/backend-surface.h
    include/vividx/gpu/direct-context.h
    include/vividx/gpu/gl-interface.h
)

set(VIVIDX_CORE_SOURCES
    src/core/point.c
    src/gpu/backend-surface.cpp
    src/gpu/direct-context.cpp
    src/gpu/gl-interface.cpp
)

set(VIVIDX_CORE_PARTIAL_SOURCES
    src/core/SkMD5.cpp
    src/core/SkBase64.cpp
)

set(VIVIDX_GPU_PARTIAL_SOURCES
    src/gpu/ganesh/gl/GrGLBackendSurface.cpp
    src/gpu/ganesh/gl/GrGLDirectContext.cpp
)

add_library(vividx-core-partial STATIC
    ${VIVIDX_CORE_SOURCES}
    ${VIVIDX_CORE_PARTIAL_SOURCES}
    ${VIVIDX_GPU_PARTIAL_SOURCES}
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
