#include "TestFramework.h"
#include "Container/CellArray.h"

TEST_GROUP(TestCellArray)

namespace {
    FCellArray Array;
}


TEST(AddCell)
{
    return;
    FTimer Timer;

    Timer.Start();

    TArray<int32> VertexIndices;
    VertexIndices.Resize(3);
    for (int i = 0; i < 10000000; i++) {

        //VertexIndices.Add(i);
        VertexIndices[0] = (i * 3 + 0);
        VertexIndices[1] = (i * 3 + 1);
        VertexIndices[2] = (i * 3 + 2);

        Array.AddCell(ECellType::Triangle, VertexIndices);
    }
    // 一千万：160 ~ 200 ms

    Timer.Stop();
    //Timer.Print();
}

TEST(GetCell)
{
    return;
    FTimer Timer;
    Timer.Start();

    FCellInfo CellInfo;
    for (int i = 0; i < 10000000; i++) {
        Array.GetCell(i, CellInfo);

        for (int j = 0; j < CellInfo.Num(); j++) {
            int32 Index = CellInfo[j];
        }
    }

    Timer.Stop();
    //Timer.Print();
}

// ============================================================================
// 构造函数和基本属性测试
// ============================================================================

TEST(CellArray_Constructor)
{
    // 默认构造函数
    FCellArray Array1;
    ASSERT(Array1.IsEmpty());
    ASSERT(Array1.GetCellCount() == 0);
    ASSERT(Array1.GetVertexIndexCount() == 0);

    // 使用初始容量构造
    FCellArray Array2(100);
    ASSERT(Array2.IsEmpty());
    ASSERT(Array2.GetCellCount() == 0);

    // 拷贝构造函数
    FCellArray Array3;
    TArray<int32> Indices;
    Indices.Add(0);
    Indices.Add(1);
    Indices.Add(2);
    Array3.AddCell(ECellType::Triangle, Indices);

    FCellArray Array4(Array3);
    ASSERT(Array4.GetCellCount() == 1);
    ASSERT(Array4.GetCellType(0) == ECellType::Triangle);

    // 移动构造函数
    FCellArray Array5(std::move(Array3));
    ASSERT(Array5.GetCellCount() == 1);
    ASSERT(Array3.GetCellCount() == 0);  // 原对象应该被移动
}

TEST(CellArray_CopyAssignment)
{
    FCellArray Array1;
    TArray<int32> Indices1;
    Indices1.Add(0);
    Indices1.Add(1);
    Indices1.Add(2);
    Array1.AddCell(ECellType::Triangle, Indices1);

    FCellArray Array2;
    TArray<int32> Indices2;
    Indices2.Add(3);
    Indices2.Add(4);
    Indices2.Add(5);
    Indices2.Add(6);
    Array2.AddCell(ECellType::Quad, Indices2);

    Array2 = Array1;
    ASSERT(Array2.GetCellCount() == 1);
    ASSERT(Array2.GetCellType(0) == ECellType::Triangle);

    // 移动赋值
    FCellArray Array3;
    Array3 = std::move(Array1);
    ASSERT(Array3.GetCellCount() == 1);
    ASSERT(Array1.GetCellCount() == 0);
}

// ============================================================================
// 添加单元测试
// ============================================================================

TEST(CellArray_AddCell_Basic)
{
    FCellArray Array;

    // 添加三角形单元
    TArray<int32> TriangleIndices;
    TriangleIndices.Add(0);
    TriangleIndices.Add(1);
    TriangleIndices.Add(2);
    Array.AddCell(ECellType::Triangle, TriangleIndices);

    ASSERT(Array.GetCellCount() == 1);
    ASSERT(Array.GetCellType(0) == ECellType::Triangle);
    ASSERT(Array.GetCellVertexCount(0) == 3);
    ASSERT(Array.GetVertexIndexCount() == 3);

    // 添加四边形单元
    TArray<int32> QuadIndices;
    QuadIndices.Add(3);
    QuadIndices.Add(4);
    QuadIndices.Add(5);
    QuadIndices.Add(6);
    Array.AddCell(ECellType::Quad, QuadIndices);

    ASSERT(Array.GetCellCount() == 2);
    ASSERT(Array.GetCellType(1) == ECellType::Quad);
    ASSERT(Array.GetCellVertexCount(1) == 4);
    ASSERT(Array.GetVertexIndexCount() == 7);
}

TEST(CellArray_AddCell_CArray)
{
    FCellArray Array;

    // 使用C风格数组添加单元
    int32 TriangleIndices[3] = {0, 1, 2};
    Array.AddCell(ECellType::Triangle, TriangleIndices, 3);

    ASSERT(Array.GetCellCount() == 1);
    ASSERT(Array.GetCellVertexCount(0) == 3);

    // 添加四面体单元
    int32 TetraIndices[4] = {0, 1, 2, 3};
    Array.AddCell(ECellType::Tetra, TetraIndices, 4);

    ASSERT(Array.GetCellCount() == 2);
    ASSERT(Array.GetCellType(1) == ECellType::Tetra);
    ASSERT(Array.GetCellVertexCount(1) == 4);
}

TEST(CellArray_AddCells_Batch)
{
    FCellArray Array;

    TArray<FCellInfo> Cells;

    // 创建多个单元
    TArray<int32> Indices1;
    Indices1.Add(0);
    Indices1.Add(1);
    Indices1.Add(2);
    Cells.Add(FCellInfo(ECellType::Triangle, Indices1));

    TArray<int32> Indices2;
    Indices2.Add(3);
    Indices2.Add(4);
    Indices2.Add(5);
    Indices2.Add(6);
    Cells.Add(FCellInfo(ECellType::Quad, Indices2));

    TArray<int32> Indices3;
    Indices3.Add(7);
    Indices3.Add(8);
    Indices3.Add(9);
    Cells.Add(FCellInfo(ECellType::Triangle, Indices3));

    Array.AddCells(Cells);

    ASSERT(Array.GetCellCount() == 3);
    ASSERT(Array.GetCellType(0) == ECellType::Triangle);
    ASSERT(Array.GetCellType(1) == ECellType::Quad);
    ASSERT(Array.GetCellType(2) == ECellType::Triangle);
}

TEST(CellArray_AddCell_MixedTypes)
{
    FCellArray Array;

    // 添加不同类型的单元
    TArray<int32> LineIndices;
    LineIndices.Add(0);
    LineIndices.Add(1);
    Array.AddCell(ECellType::Line, LineIndices);

    TArray<int32> TriangleIndices;
    TriangleIndices.Add(2);
    TriangleIndices.Add(3);
    TriangleIndices.Add(4);
    Array.AddCell(ECellType::Triangle, TriangleIndices);

    TArray<int32> HexIndices;
    for (int i = 0; i < 8; ++i)
    {
        HexIndices.Add(10 + i);
    }
    Array.AddCell(ECellType::Hex, HexIndices);

    ASSERT(Array.GetCellCount() == 3);
    ASSERT(Array.GetCellType(0) == ECellType::Line);
    ASSERT(Array.GetCellType(1) == ECellType::Triangle);
    ASSERT(Array.GetCellType(2) == ECellType::Hex);
    ASSERT(Array.GetCellVertexCount(0) == 2);
    ASSERT(Array.GetCellVertexCount(1) == 3);
    ASSERT(Array.GetCellVertexCount(2) == 8);
}

// ============================================================================
// 获取单元测试
// ============================================================================

TEST(CellArray_GetCell)
{
    FCellArray Array;

    TArray<int32> Indices;
    Indices.Add(10);
    Indices.Add(20);
    Indices.Add(30);
    Array.AddCell(ECellType::Triangle, Indices);

    FCellInfo CellInfo;
    bool bSuccess = Array.GetCell(0, CellInfo);

    ASSERT(bSuccess);
    ASSERT(CellInfo.CellType == ECellType::Triangle);
    ASSERT(CellInfo.Num() == 3);
    ASSERT(CellInfo[0] == 10);
    ASSERT(CellInfo[1] == 20);
    ASSERT(CellInfo[2] == 30);

    // 测试无效索引
    bool bFail = Array.GetCell(100, CellInfo);
    ASSERT(!bFail);
}

TEST(CellArray_GetCellType)
{
    FCellArray Array;

    TArray<int32> TriangleIndices;
    TriangleIndices.Add(0);
    TriangleIndices.Add(1);
    TriangleIndices.Add(2);
    Array.AddCell(ECellType::Triangle, TriangleIndices);

    TArray<int32> QuadIndices;
    QuadIndices.Add(3);
    QuadIndices.Add(4);
    QuadIndices.Add(5);
    QuadIndices.Add(6);
    Array.AddCell(ECellType::Quad, QuadIndices);

    ASSERT(Array.GetCellType(0) == ECellType::Triangle);
    ASSERT(Array.GetCellType(1) == ECellType::Quad);
    ASSERT(Array.GetCellType(100) == ECellType::None);  // 无效索引
}

TEST(CellArray_GetCellVertexIndices)
{
    FCellArray Array;

    TArray<int32> Indices;
    Indices.Add(5);
    Indices.Add(6);
    Indices.Add(7);
    Indices.Add(8);
    Array.AddCell(ECellType::Quad, Indices);

    TArray<int32> OutIndices;
    bool bSuccess = Array.GetCellVertexIndices(0, OutIndices);

    ASSERT(bSuccess);
    ASSERT(OutIndices.Num() == 4);
    ASSERT(OutIndices[0] == 5);
    ASSERT(OutIndices[1] == 6);
    ASSERT(OutIndices[2] == 7);
    ASSERT(OutIndices[3] == 8);
}

TEST(CellArray_GetCellVertexIndicesPtr)
{
    FCellArray Array;

    TArray<int32> Indices;
    Indices.Add(10);
    Indices.Add(11);
    Indices.Add(12);
    Array.AddCell(ECellType::Triangle, Indices);

    uint32 VertexCount = 0;
    const int32* Ptr = Array.GetCellVertexIndicesPtr(0, VertexCount);

    ASSERT(Ptr != nullptr);
    ASSERT(VertexCount == 3);
    ASSERT(Ptr[0] == 10);
    ASSERT(Ptr[1] == 11);
    ASSERT(Ptr[2] == 12);

    // 测试无效索引
    const int32* NullPtr = Array.GetCellVertexIndicesPtr(100, VertexCount);
    ASSERT(NullPtr == nullptr);
    ASSERT(VertexCount == 0);
}

// ============================================================================
// 删除单元测试
// ============================================================================

TEST(CellArray_RemoveCell)
{
    FCellArray Array;

    // 添加3个单元
    for (int i = 0; i < 3; ++i)
    {
        TArray<int32> Indices;
        Indices.Add(i * 3 + 0);
        Indices.Add(i * 3 + 1);
        Indices.Add(i * 3 + 2);
        Array.AddCell(ECellType::Triangle, Indices);
    }

    ASSERT(Array.GetCellCount() == 3);

    // 删除中间的单元
    bool bSuccess = Array.RemoveCell(1);
    ASSERT(bSuccess);
    ASSERT(Array.GetCellCount() == 2);

    // 验证剩余单元
    ASSERT(Array.GetCellType(0) == ECellType::Triangle);
    ASSERT(Array.GetCellType(1) == ECellType::Triangle);

    // 测试无效索引
    bool bFail = Array.RemoveCell(100);
    ASSERT(!bFail);
}

TEST(CellArray_RemoveCells_Range)
{
    FCellArray Array;

    // 添加5个单元
    for (int i = 0; i < 5; ++i)
    {
        TArray<int32> Indices;
        Indices.Add(i * 3 + 0);
        Indices.Add(i * 3 + 1);
        Indices.Add(i * 3 + 2);
        Array.AddCell(ECellType::Triangle, Indices);
    }

    ASSERT(Array.GetCellCount() == 5);

    // 删除索引1-3的单元（3个）
    uint32 RemovedCount = Array.RemoveCells(1, 3);
    ASSERT(RemovedCount == 3);
    ASSERT(Array.GetCellCount() == 2);

    // 验证剩余单元
    ASSERT(Array.GetCellType(0) == ECellType::Triangle);
    ASSERT(Array.GetCellType(1) == ECellType::Triangle);
}

TEST(CellArray_RemoveCellsByType)
{
    FCellArray Array;

    // 添加不同类型的单元
    TArray<int32> TriangleIndices;
    TriangleIndices.Add(0);
    TriangleIndices.Add(1);
    TriangleIndices.Add(2);
    Array.AddCell(ECellType::Triangle, TriangleIndices);
    Array.AddCell(ECellType::Triangle, TriangleIndices);

    TArray<int32> QuadIndices;
    QuadIndices.Add(3);
    QuadIndices.Add(4);
    QuadIndices.Add(5);
    QuadIndices.Add(6);
    Array.AddCell(ECellType::Quad, QuadIndices);
    Array.AddCell(ECellType::Quad, QuadIndices);

    ASSERT(Array.GetCellCount() == 4);

    // 删除所有三角形单元
    uint32 RemovedCount = Array.RemoveCellsByType(ECellType::Triangle);
    ASSERT(RemovedCount == 2);
    ASSERT(Array.GetCellCount() == 2);
    ASSERT(Array.GetCellType(0) == ECellType::Quad);
    ASSERT(Array.GetCellType(1) == ECellType::Quad);
}

TEST(CellArray_Clear)
{
    FCellArray Array;

    // 添加一些单元
    for (int i = 0; i < 10; ++i)
    {
        TArray<int32> Indices;
        Indices.Add(i * 3 + 0);
        Indices.Add(i * 3 + 1);
        Indices.Add(i * 3 + 2);
        Array.AddCell(ECellType::Triangle, Indices);
    }

    ASSERT(Array.GetCellCount() == 10);

    Array.Clear();
    ASSERT(Array.GetCellCount() == 0);
    ASSERT(Array.IsEmpty());
    ASSERT(Array.GetVertexIndexCount() == 0);

    // 测试别名方法
    Array.AddCell(ECellType::Triangle, TArray<int32>{0, 1, 2});
    Array.Reset();
    ASSERT(Array.IsEmpty());

    Array.AddCell(ECellType::Triangle, TArray<int32>{0, 1, 2});
    Array.Empty();
    ASSERT(Array.IsEmpty());
}

// ============================================================================
// 查询和统计测试
// ============================================================================

TEST(CellArray_IsValidCellIndex)
{
    FCellArray Array;

    TArray<int32> Indices;
    Indices.Add(0);
    Indices.Add(1);
    Indices.Add(2);
    Array.AddCell(ECellType::Triangle, Indices);
    Array.AddCell(ECellType::Triangle, Indices);

    ASSERT(Array.IsValidCellIndex(0));
    ASSERT(Array.IsValidCellIndex(1));
    ASSERT(!Array.IsValidCellIndex(2));
    ASSERT(!Array.IsValidCellIndex(-1));
    ASSERT(!Array.IsValidCellIndex(100));
}

TEST(CellArray_GetCellCountByType)
{
    FCellArray Array;

    // 添加不同类型的单元
    TArray<int32> TriangleIndices{0, 1, 2};
    TArray<int32> QuadIndices{3, 4, 5, 6};
    TArray<int32> HexIndices;
    for (int i = 0; i < 8; ++i) HexIndices.Add(10 + i);

    Array.AddCell(ECellType::Triangle, TriangleIndices);
    Array.AddCell(ECellType::Triangle, TriangleIndices);
    Array.AddCell(ECellType::Quad, QuadIndices);
    Array.AddCell(ECellType::Hex, HexIndices);
    Array.AddCell(ECellType::Triangle, TriangleIndices);

    ASSERT(Array.GetCellCountByType(ECellType::Triangle) == 3);
    ASSERT(Array.GetCellCountByType(ECellType::Quad) == 1);
    ASSERT(Array.GetCellCountByType(ECellType::Hex) == 1);
    ASSERT(Array.GetCellCountByType(ECellType::Tetra) == 0);
}

TEST(CellArray_GetCellTypes)
{
    FCellArray Array;

    TArray<int32> TriangleIndices{0, 1, 2};
    TArray<int32> QuadIndices{3, 4, 5, 6};
    TArray<int32> HexIndices;
    for (int i = 0; i < 8; ++i) HexIndices.Add(10 + i);

    Array.AddCell(ECellType::Triangle, TriangleIndices);
    Array.AddCell(ECellType::Quad, QuadIndices);
    Array.AddCell(ECellType::Hex, HexIndices);
    Array.AddCell(ECellType::Triangle, TriangleIndices);  // 重复类型

    TArray<ECellType> Types;
    Array.GetCellTypes(Types);

    ASSERT(Types.Num() == 3);  // 应该去重
    // 验证包含的类型
    bool bHasTriangle = false;
    bool bHasQuad = false;
    bool bHasHex = false;
    for (const auto& Type : Types)
    {
        if (Type == ECellType::Triangle) bHasTriangle = true;
        if (Type == ECellType::Quad) bHasQuad = true;
        if (Type == ECellType::Hex) bHasHex = true;
    }
    ASSERT(bHasTriangle);
    ASSERT(bHasQuad);
    ASSERT(bHasHex);
}

TEST(CellArray_FindCellsContainingVertex)
{
    FCellArray Array;

    // 创建共享顶点的单元
    // 单元0: [0, 1, 2]
    Array.AddCell(ECellType::Triangle, TArray<int32>{0, 1, 2});
    // 单元1: [1, 2, 3]
    Array.AddCell(ECellType::Triangle, TArray<int32>{1, 2, 3});
    // 单元2: [2, 3, 4]
    Array.AddCell(ECellType::Triangle, TArray<int32>{2, 3, 4});

    // 查找包含顶点2的所有单元
    TArray<int32> CellIndices;
    uint32 Count = Array.FindCellsContainingVertex(2, CellIndices);

    ASSERT(Count == 3);
    ASSERT(CellIndices.Num() == 3);
    ASSERT(CellIndices.Contains(0));
    ASSERT(CellIndices.Contains(1));
    ASSERT(CellIndices.Contains(2));

    // 查找包含顶点0的单元（只在单元0中）
    CellIndices.Clear();
    Count = Array.FindCellsContainingVertex(0, CellIndices);
    ASSERT(Count == 1);
    ASSERT(CellIndices[0] == 0);
}

// ============================================================================
// 内存管理测试
// ============================================================================

TEST(CellArray_Reserve)
{
    FCellArray Array;

    Array.Reserve(100);

    // 添加单元应该更快（因为已经预留了空间）
    for (int i = 0; i < 50; ++i)
    {
        TArray<int32> Indices;
        Indices.Add(i * 3 + 0);
        Indices.Add(i * 3 + 1);
        Indices.Add(i * 3 + 2);
        Array.AddCell(ECellType::Triangle, Indices);
    }

    ASSERT(Array.GetCellCount() == 50);
}

TEST(CellArray_Shrink)
{
    FCellArray Array;

    // 添加然后删除一些单元
    for (int i = 0; i < 100; ++i)
    {
        TArray<int32> Indices;
        Indices.Add(i * 3 + 0);
        Indices.Add(i * 3 + 1);
        Indices.Add(i * 3 + 2);
        Array.AddCell(ECellType::Triangle, Indices);
    }

    // 删除大部分单元
    Array.RemoveCells(10, 80);
    ASSERT(Array.GetCellCount() == 20);

    // 收缩容量
    Array.Shrink();
    ASSERT(Array.GetCellCount() == 20);
}

TEST(CellArray_GetMemoryUsage)
{
    FCellArray Array;

    // 空数组的内存使用
    uint64 EmptyUsage = Array.GetMemoryUsage();
    ASSERT(EmptyUsage >= 0);

    TArray<int32> Indices;
    // 添加单元后的内存使用
    for (int i = 0; i < 100; ++i)
    {
        Indices.Reset();
        Indices.Add(i * 3 + 0);
        Indices.Add(i * 3 + 1);
        Indices.Add(i * 3 + 2);
        Array.AddCell(ECellType::Triangle, Indices);
    }

    uint64 Usage = Array.GetMemoryUsage();
    ASSERT(Usage > EmptyUsage);
}

// ============================================================================
// 容量和大小测试
// ============================================================================

TEST(CellArray_IsEmpty)
{
    FCellArray Array;

    ASSERT(Array.IsEmpty());
    ASSERT(Array.GetCellCount() == 0);

    TArray<int32> Indices{0, 1, 2};
    Array.AddCell(ECellType::Triangle, Indices);

    ASSERT(!Array.IsEmpty());
    ASSERT(Array.GetCellCount() == 1);
}