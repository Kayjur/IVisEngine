add_executable(IVisEngine
        main.cpp
)

target_link_libraries(IVisEngine PRIVATE
        IVisFramework
)

target_include_directories(IVisEngine
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../Framework/Public
)
