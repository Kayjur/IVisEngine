#include "TestFramework.h"
#include "Container/Array.h"

TEST_GROUP(TestArray)

// 基本功能测试
TEST(Basic)
{
    TArray<int> Array;
    ASSERT(Array.IsEmpty() == true);
    
    Array.Add(1);
    Array.Add(2);
    Array.Add(3);
    
    ASSERT(Array.Num() == 3);
    ASSERT(Array[0] == 1);
    ASSERT(Array[2] == 3);
    ASSERT(Array.Contains(2) == true);
}

// 查找和删除测试
TEST(FindAndRemove)
{
    TArray<int> Array{1, 2, 3, 4, 5};
    
    int32 Index = Array.Find(3);
    ASSERT(Index == 2);
    
    Array.RemoveAt(1);
    ASSERT(Array.Num() == 4);
    ASSERT(Array[1] == 3);
}

