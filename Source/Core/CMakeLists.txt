add_library(IVisCore STATIC
    Private/EngineInfo.cpp
)

target_include_directories(IVisCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Public
)
