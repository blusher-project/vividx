cmake_minimum_required(VERSION 3.10)
project(vividx
    VERSION 0.152001.0
    LANGUAGES C CXX
)

set(VIVIDX_PUBLIC_HEADERS
    include/vividx/common.h
    include/vividx/core/canvas.h
    include/vividx/core/color-type.h
    include/vividx/core/point.h
    include/vividx/core/surface.h
    include/vividx/gpu/backend-render-target.h
    include/vividx/gpu/backend-surface.h
    include/vividx/gpu/direct-context.h
    include/vividx/gpu/gl-interface.h
    include/vividx/gpu/gpu-surfaces.h
    include/vividx/gpu/types.h
)

set(VIVIDX_CORE_SOURCES
    src/core/canvas.cpp
    src/core/converts.h
    src/core/point.c
    src/core/surface.cpp
    src/gpu/backend-render-target.cpp
    src/gpu/backend-surface.cpp
    src/gpu/converts.h
    src/gpu/direct-context.cpp
    src/gpu/gl-interface.cpp
    src/gpu/gpu-surfaces.cpp
)

set(VIVIDX_CORE_PARTIAL_SOURCES
    src/core/SkMD5.cpp
    src/core/SkBase64.cpp
    src/core/SkCPURecorder.cpp
    src/core/SkStream.cpp
)

set(VIVIDX_GPU_PARTIAL_SOURCES
    # src/gpu/GlobalResourceStats.cpp
    # src/gpu/ganesh/GrBackendSurface.cpp
    # src/gpu/ganesh/GrContext_Base.cpp
    # src/gpu/ganesh/GrDirectContext.cpp
    # src/gpu/ganesh/GrImageContext.cpp
    # src/gpu/ganesh/GrRecordingContext.cpp
    # src/gpu/ganesh/gl/GrGLBackendSurface.cpp
    # src/gpu/ganesh/gl/GrGLDirectContext.cpp

    # DEBUG
    # src/gpu/ganesh/gl/egl/GrGLMakeEGLInterface.cpp

    # src/image/SkSurface_Base.cpp
    # src/image/SkSurface_Raster.cpp
)

add_library(vividx-partial STATIC
    ${VIVIDX_CORE_SOURCES}
    ${VIVIDX_CORE_PARTIAL_SOURCES}
    ${VIVIDX_GPU_PARTIAL_SOURCES}
)

target_compile_definitions(vividx-partial PRIVATE
    SK_GL
    SK_GANESH
    SKIA_DLL
    SKIA_IMPLEMENTATION=1
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_definitions(vividx-partial PRIVATE
        SK_R32_SHIFT=16
    )
endif()

target_include_directories(vividx-partial
    PRIVATE ./include
)

target_include_directories(vividx-partial
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_options(vividx-partial
    PRIVATE -fno-rtti
)

set_target_properties(vividx-partial PROPERTIES
    POSITION_INDEPENDENT_CODE ON    # -fPIC
)
