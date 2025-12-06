# ============================================================================
# IVisCore 模块 CMake 配置文件
# ============================================================================
# 此模块是 IVisEngine 的 Core 模块，提供引擎的基础功能
# 采用 UE 风格的模块化布局：Public 目录存放头文件，Private 目录存放实现文件
# ============================================================================
set(MODULE_NAME Core)

# GLOB_RECURSE 会递归搜索所有子目录中的 .cpp 文件
file(GLOB_RECURSE CORE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/*.cpp"
)

# 创建静态库目标
# STATIC 表示生成静态库（.a 或 .lib 文件）
add_library(IVisCore STATIC ${CORE_SOURCES}
        Public/HAL/Platform.h
        Public/Cell/CellType.cpp
        Public/Cell/CellType.h
        Public/Container/CellArray.cpp
        Public/Container/CellArray.h
)

# 设置公共头文件包含目录
# PUBLIC 表示使用此库的目标也会自动包含这些目录
target_include_directories(IVisCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Public
)
