# ============================================================================
# IVisTest 模块 CMake 配置文件
# ============================================================================
# 此模块是 IVisEngine 的测试模块，用于运行单元测试和集成测试
# 采用 UE 风格的模块化布局：Public 目录存放头文件，Private 目录存放实现文件
# ============================================================================
set(MODULE_NAME Test)

# GLOB_RECURSE 会递归搜索所有子目录中的 .cpp 文件
file(GLOB_RECURSE TEST_SOURCES main.cpp
    "${CMAKE_CURRENT_SOURCE_DIR}/Tests/*.cpp"
)

# 创建测试可执行程序
add_executable(IVisTest ${TEST_SOURCES}
        Tests/Core/Delegates/TestDelegate.cpp
        Tests/Core/Container/TestCellArray.cpp
        TestCore/TestTimeUtil.h)

# 链接必要的库
target_link_libraries(IVisTest PRIVATE
    IVisCore
    IVisFramework
    IVisRenderer
)

# 设置包含目录
target_include_directories(IVisTest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/TestCore
    ${CMAKE_CURRENT_SOURCE_DIR}/../Core/Public
    ${CMAKE_CURRENT_SOURCE_DIR}/../Framework/Public
    ${CMAKE_CURRENT_SOURCE_DIR}/../Renderer/Public
)

