#pragma once

#include <vector>
#include <functional>
#include <string>
#include <iostream>
#include <cstring>
#include "TestTimeUtil.h"
#include "Exception/Exception.h"

namespace ivis::test {

/**
 * 测试函数类型
 */
using TestFunction = std::function<void()>;

/**
 * 测试用例结构
 */
struct TestCase
{
    std::string Name;
    TestFunction Function;
    
    TestCase(const std::string& InName, TestFunction InFunction)
        : Name(InName), Function(InFunction) {}
};

/**
 * 测试注册表（全局单例）
 */
class TestRegistry
{
public:
    static TestRegistry& Get()
    {
        static TestRegistry Instance;
        return Instance;
    }
    
    /**
     * 注册测试用例
     */
    void RegisterTest(const std::string& Name, TestFunction Func)
    {
        Tests.push_back(TestCase(Name, Func));
    }
    
    /**
     * 运行所有测试
     */
    int RunAll()
    {
        std::cout << "========================================" << std::endl;
        std::cout << "IVisEngine 测试模块" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "共 " << Tests.size() << " 个测试用例" << std::endl << std::endl;
        
        int PassedCount = 0;
        int FailedCount = 0;
        
        for (size_t i = 0; i < Tests.size(); ++i)
        {
            const auto& Test = Tests[i];
            std::cout << "[" << (i + 1) << "/" << Tests.size() << "] " << Test.Name << " ... ";
            
            try
            {
                Test.Function();
                std::cout << "✓ 通过" << std::endl;
                PassedCount++;
            }
            catch (const FException& e)
            {
                std::cerr << "✗ 失败: " << e.GetFullMessage() << std::endl;
                FailedCount++;
            }
            catch (const std::exception& e)
            {
                std::cerr << "✗ 失败: " << e.what() << std::endl;
                FailedCount++;
            }
            catch (...)
            {
                std::cerr << "✗ 失败: 未知错误" << std::endl;
                FailedCount++;
            }
        }
        
        std::cout << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "测试结果: " << PassedCount << " 通过, " << FailedCount << " 失败" << std::endl;
        std::cout << "========================================" << std::endl;
        
        return FailedCount > 0 ? 1 : 0;
    }
    
    /**
     * 获取测试数量
     */
    size_t GetTestCount() const { return Tests.size(); }
    
private:
    std::vector<TestCase> Tests;
};

} // namespace ivis::test

/**
 * 测试组辅助类
 * 使用文件作用域的静态变量来存储测试组名称
 */
namespace ivis::test::detail {
    // 每个文件可以有自己的测试组名称
    // 使用 inline 变量避免重复定义问题
    inline const char* g_CurrentTestGroup = nullptr;
    
    inline void SetTestGroup(const char* GroupName) {
        g_CurrentTestGroup = GroupName;
    }
    
    inline const char* GetTestGroup() {
        return g_CurrentTestGroup;
    }
}

/**
 * 测试组宏定义
 * 用于组织相关测试，会在测试名称前添加组名
 * 
 * 用法:
 *   TEST_GROUP("查找功能测试")
 *   TEST(TestArray_Find) { ... }
 *   TEST(TestArray_Contains) { ... }
 * 
 * 注意：TEST_GROUP 需要在同一文件中的测试之前定义
 */
#define TEST_GROUP(GroupName) \
    namespace { \
        struct TestGroup_Registrar { \
            TestGroup_Registrar() { \
                ivis::test::detail::SetTestGroup(#GroupName); \
            } \
        } TestGroup_Instance; \
    }

/**
 * 测试宏定义
 * 
 * 用法:
 *   TEST(TestArray_Find) {
 *       // 测试代码
 *   }
 * 
 * 这会自动：
 * 1. 创建一个测试函数
 * 2. 将测试注册到测试框架
 * 3. 在 main 中自动运行
 * 
 * 如果使用了 TEST_GROUP，测试名称会自动添加组名前缀
 *
 * 示例: TEST(TestArray_Find)宏展开
 * // 1. 声明实际的测试函数
 * static void TestArray_Find_Impl();
 *
 * // 2. 在匿名命名空间中创建一个注册器结构体
 * namespace { // 避免不同文件的注册器名称冲突
 *     struct TestArray_Find_Registrar {
 *         TestArray_Find_Registrar() {  // 构造函数在静态初始化时自动调用
 *             std::string TestNameWithGroup = "TestArray_Find";
 *             // 检查是否有测试组
 *             const char* GroupName = ivis::test::detail::GetTestGroup();
 *             if (GroupName && strlen(GroupName) > 0) {
 *                 TestNameWithGroup = std::string(GroupName) + "::TestArray_Find";
 *             }
 *             // 关键：在这里注册测试到全局注册表 <string, Function>
 *             ivis::test::TestRegistry::Get().RegisterTest(TestNameWithGroup, TestArray_Find_Impl);
 *         }
 *     } TestArray_Find_Instance;  // 静态变量，自动初始化
 * }
 *
 * // 3. 定义实际的测试函数
 * static void TestArray_Find_Impl()
 * {
 *     // 你的测试代码
 * }
 */
#define TEST(TestName) \
    static void TestName##_Impl(); \
    namespace { \
        struct TestName##_Registrar { \
            TestName##_Registrar() { \
                std::string TestNameWithGroup = #TestName; \
                /* 检查是否有测试组 */ \
                const char* GroupName = ivis::test::detail::GetTestGroup(); \
                if (GroupName && strlen(GroupName) > 0) { \
                    TestNameWithGroup = std::string(GroupName) + "::" + #TestName; \
                } \
                ivis::test::TestRegistry::Get().RegisterTest(TestNameWithGroup, TestName##_Impl); \
            } \
        } TestName##_Instance; \
    } \
    static void TestName##_Impl()

/**
 * 断言宏
 */
#define ASSERT(condition) \
    do { \
        if (!(condition)) { \
            std::cerr << "断言失败: " << #condition << " (文件: " << __FILE__ << ", 行: " << __LINE__ << ")" << std::endl; \
            throw std::runtime_error("测试断言失败"); \
        } \
    } while(0)

#define ASSERT_EQ(actual, expected) \
    do { \
        if ((actual) != (expected)) { \
            std::cerr << "断言失败: " << #actual << " == " << #expected << std::endl; \
            std::cerr << "  实际值: " << (actual) << ", 期望值: " << (expected) << std::endl; \
            std::cerr << "  (文件: " << __FILE__ << ", 行: " << __LINE__ << ")" << std::endl; \
            throw std::runtime_error("测试断言失败"); \
        } \
    } while(0)
