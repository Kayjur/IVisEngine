//
// TestMain.cpp - 测试模块入口文件
// Created by IVisEngine
//

#include <iostream>
#include <cstdint>
#include <exception>
#include "TestCore/TestFramework.h"

/**
 * 主函数 - 运行所有测试
 * 
 * 注意：所有使用 TEST() 宏定义的测试会自动注册并运行
 * 无需手动调用，只需包含相应的测试文件即可
 */
int main()
{
    try
    {
        // 运行所有注册的测试
        return ivis::test::TestRegistry::Get().RunAll();
    }
    catch (const std::exception& e)
    {
        std::cerr << "✗ 测试框架错误: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "✗ 测试框架错误: 未知错误" << std::endl;
        return 1;
    }
}

