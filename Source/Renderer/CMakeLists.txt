add_library(IVisRenderer STATIC
    Private/Renderer.cpp
)

target_include_directories(IVisRenderer PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Public
)

target_link_libraries(IVisRenderer PUBLIC
        IVisCore
)
