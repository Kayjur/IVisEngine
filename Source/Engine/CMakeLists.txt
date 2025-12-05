add_library(IVisEngineRuntime STATIC
    Private/Engine.cpp
)

target_include_directories(IVisEngineRuntime PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Public
)

target_link_libraries(IVisEngineRuntime PUBLIC
        IVisCore
        IVisRenderer
)
