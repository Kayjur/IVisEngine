#include "TestFramework.h"
#include "Container/Map.h"

TEST_GROUP(TestMap)

// 基本功能测试
TEST(Basic)
{
    TMap<std::string, int> Map;
    ASSERT(Map.IsEmpty() == true);
    
    Map.Add("Alice", 100);
    Map.Add("Bob", 85);
    
    ASSERT(Map.Num() == 2);
    ASSERT(Map.Contains("Alice") == true);
    ASSERT(Map["Bob"] == 85);
}

// 查找和删除测试
TEST(FindAndRemove)
{
    TMap<std::string, int> Map;
    Map["Charlie"] = 90;
    
    int* Score = Map.Find("Charlie");
    ASSERT(Score != nullptr);
    ASSERT(*Score == 90);
    
    bool Removed = Map.Remove("Charlie");
    ASSERT(Removed == true);
    ASSERT(Map.Num() == 0);
}