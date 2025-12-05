add_library(IVisRender STATIC
    Private/Renderer.cpp
)

target_include_directories(IVisRender
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Public
)

target_link_libraries(IVisRender
    PUBLIC
        IVisCore
)
