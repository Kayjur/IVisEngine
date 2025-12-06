add_executable(IVisEngine
        main.cpp
)

target_link_libraries(IVisEngine PRIVATE
        IVisFramework
        IVisRenderer
        IVisCore
)

target_include_directories(IVisEngine
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../Framework/Public
        ${CMAKE_CURRENT_SOURCE_DIR}/../Renderer/Public
)
