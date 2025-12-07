#include "TestFramework.h"
#include "Mesh/Mesh.h"
#include "Field/Field.h"
#include "Container/CellArray.h"
#include "Math/MathType.h"

TEST_GROUP(TestMesh)

// ============================================================================
// 测试用例1: 基本构造和顶点操作
// ============================================================================

TEST(Mesh_BasicConstructionAndVertices)
{
    // 测试默认构造函数
    IMesh DefaultMesh;
    ASSERT(DefaultMesh.GetVertexCount() == 0);
    ASSERT(DefaultMesh.GetCellCount() == 0);
    ASSERT(DefaultMesh.GetMeshName() == "UnnamedMesh");
    ASSERT(!DefaultMesh.IsValid());
    
    // 测试带名称的构造函数
    IMesh NamedMesh("TestMesh");
    ASSERT(NamedMesh.GetMeshName() == "TestMesh");
    ASSERT(NamedMesh.GetVertexCount() == 0);
    
    // 测试添加顶点
    NamedMesh.AddVertex(1.0f, 2.0f, 3.0f);
    NamedMesh.AddVertex(4.0f, 5.0f, 6.0f);
    NamedMesh.AddVertex(7.0f, 8.0f, 9.0f);
    
    ASSERT(NamedMesh.GetVertexCount() == 3);
    ASSERT(NamedMesh.IsValidVertexIndex(0));
    ASSERT(NamedMesh.IsValidVertexIndex(2));
    ASSERT(!NamedMesh.IsValidVertexIndex(3));
    
    // 测试获取顶点
    FVector V0 = NamedMesh.GetVertex(0);
    ASSERT_EQ(V0.X, 1.0f);
    ASSERT_EQ(V0.Y, 2.0f);
    ASSERT_EQ(V0.Z, 3.0f);
    
    FVector V1 = NamedMesh.GetVertex(1);
    ASSERT_EQ(V1.X, 4.0f);
    ASSERT_EQ(V1.Y, 5.0f);
    ASSERT_EQ(V1.Z, 6.0f);
    
    // 测试设置顶点
    NamedMesh.SetVertex(1, 10.0f, 11.0f, 12.0f);
    V1 = NamedMesh.GetVertex(1);
    ASSERT_EQ(V1.X, 10.0f);
    ASSERT_EQ(V1.Y, 11.0f);
    ASSERT_EQ(V1.Z, 12.0f);
    
    // 测试批量添加顶点
    TArray<FVector> MoreVertices;
    MoreVertices.Emplace(13.0f, 14.0f, 15.0f);
    MoreVertices.Emplace(16.0f, 17.0f, 18.0f);
    NamedMesh.AddVertices(MoreVertices);
    
    ASSERT(NamedMesh.GetVertexCount() == 5);
    FVector V4 = NamedMesh.GetVertex(4);
    ASSERT_EQ(V4.X, 16.0f);
    ASSERT_EQ(V4.Y, 17.0f);
    ASSERT_EQ(V4.Z, 18.0f);
    
    // 测试无效索引返回零向量
    FVector InvalidVertex = NamedMesh.GetVertex(100);
    ASSERT(InvalidVertex.IsZero());
}

// ============================================================================
// 测试用例2: 单元操作
// ============================================================================

TEST(Mesh_CellOperations)
{
    IMesh Mesh("CellTestMesh");
    
    // 添加顶点
    Mesh.AddVertex(0.0f, 0.0f, 0.0f);
    Mesh.AddVertex(1.0f, 0.0f, 0.0f);
    Mesh.AddVertex(0.0f, 1.0f, 0.0f);
    Mesh.AddVertex(1.0f, 1.0f, 0.0f);
    
    ASSERT(Mesh.GetVertexCount() == 4);
    
    // 获取单元数组并添加单元
    FCellArray& Cells = Mesh.GetCells();
    
    // 添加三角形单元
    TArray<int32> TriangleIndices;
    TriangleIndices.Add(0);
    TriangleIndices.Add(1);
    TriangleIndices.Add(2);
    Cells.AddCell(ECellType::Triangle, TriangleIndices);
    
    // 添加四边形单元
    TArray<int32> QuadIndices;
    QuadIndices.Add(0);
    QuadIndices.Add(1);
    QuadIndices.Add(3);
    QuadIndices.Add(2);
    Cells.AddCell(ECellType::Quad, QuadIndices);
    
    ASSERT(Mesh.GetCellCount() == 2);
    ASSERT(Mesh.IsValidCellIndex(0));
    ASSERT(Mesh.IsValidCellIndex(1));
    ASSERT(!Mesh.IsValidCellIndex(2));
    
    // 测试获取单元信息
    FCellInfo CellInfo;
    ASSERT(Cells.GetCell(0, CellInfo));
    ASSERT(CellInfo.CellType == ECellType::Triangle);
    ASSERT(CellInfo.Num() == 3);
    ASSERT_EQ(CellInfo[0], 0);
    ASSERT_EQ(CellInfo[1], 1);
    ASSERT_EQ(CellInfo[2], 2);
    
    ASSERT(Cells.GetCell(1, CellInfo));
    ASSERT(CellInfo.CellType == ECellType::Quad);
    ASSERT(CellInfo.Num() == 4);
    
    // 测试常量引用
    const FCellArray& ConstCells = Mesh.GetCells();
    ASSERT(ConstCells.GetCellCount() == 2);
}

// ============================================================================
// 测试用例3: 场数据操作
// ============================================================================

TEST(Mesh_FieldOperations)
{
    IMesh Mesh("FieldTestMesh");
    
    // 添加顶点
    for (int i = 0; i < 5; ++i)
    {
        Mesh.AddVertex(static_cast<float>(i), 0.0f, 0.0f);
    }
    
    // 创建顶点场（标量场）
    TUniquePtr<FField> TemperatureField = MakeUnique<FField>("Temperature", EFieldType::Scalar, EFieldAttachment::Vertex);
    
    TArray<float> TemperatureData;
    TemperatureData.Add(10.0f);
    TemperatureData.Add(20.0f);
    TemperatureData.Add(30.0f);
    TemperatureData.Add(40.0f);
    TemperatureData.Add(50.0f);
    TemperatureField->SetScalarData(TemperatureData);
    
    // 添加顶点场
    ASSERT(Mesh.AddField(std::move(TemperatureField)));
    ASSERT(Mesh.HasVertexField("Temperature"));
    ASSERT(Mesh.HasField("Temperature"));
    
    // 获取顶点场
    FField* TempField = Mesh.GetVertexField("Temperature");
    ASSERT(TempField != nullptr);
    ASSERT(TempField->GetFieldName() == "Temperature");
    ASSERT(TempField->GetDataCount() == 5);
    ASSERT_EQ(TempField->GetScalar(0), 10.0f);
    ASSERT_EQ(TempField->GetScalar(4), 50.0f);
    
    // 添加单元
    FCellArray& Cells = Mesh.GetCells();
    TArray<int32> TriangleIndices;
    TriangleIndices.Add(0);
    TriangleIndices.Add(1);
    TriangleIndices.Add(2);
    Cells.AddCell(ECellType::Triangle, TriangleIndices);
    
    // 创建单元场（向量场）
    TUniquePtr<FField> StressField = MakeUnique<FField>("Stress", EFieldType::Vector, EFieldAttachment::Cell);
    
    TArray<float> StressData;
    StressData.Add(1.0f);
    StressData.Add(2.0f);
    StressData.Add(3.0f); // 第一个单元
    StressField->SetVectorData(StressData);
    
    ASSERT(Mesh.AddField(std::move(StressField)));
    ASSERT(Mesh.HasCellField("Stress"));
    ASSERT(Mesh.HasField("Stress"));
    
    // 获取单元场
    FField* StressFieldPtr = Mesh.GetCellField("Stress");
    ASSERT(StressFieldPtr != nullptr);
    ASSERT(StressFieldPtr->GetFieldName() == "Stress");
    ASSERT(StressFieldPtr->GetDataCount() == 1);
    
    FVector StressVec = StressFieldPtr->GetVector(0);
    ASSERT_EQ(StressVec.X, 1.0f);
    ASSERT_EQ(StressVec.Y, 2.0f);
    ASSERT_EQ(StressVec.Z, 3.0f);
    
    // 测试 GetField 自动查找
    FField* AutoField = Mesh.GetField("Temperature");
    ASSERT(AutoField != nullptr);
    ASSERT(AutoField->GetFieldName() == "Temperature");
    
    AutoField = Mesh.GetField("Stress");
    ASSERT(AutoField != nullptr);
    ASSERT(AutoField->GetFieldName() == "Stress");
    
    // 测试删除场
    ASSERT(Mesh.RemoveVertexField("Temperature"));
    ASSERT(!Mesh.HasVertexField("Temperature"));
    ASSERT(!Mesh.HasField("Temperature"));
    
    // 测试获取场名称列表
    TArray<std::string> VertexFieldNames;
    Mesh.GetVertexFieldNames(VertexFieldNames);
    ASSERT(VertexFieldNames.Num() == 0);
    
    TArray<std::string> CellFieldNames;
    Mesh.GetCellFieldNames(CellFieldNames);
    ASSERT(CellFieldNames.Num() == 1);
    ASSERT(CellFieldNames[0] == "Stress");
}

// ============================================================================
// 测试用例4: 拷贝、移动和验证
// ============================================================================

TEST(Mesh_CopyMoveAndValidation)
{
    // 创建原始网格
    IMesh OriginalMesh("OriginalMesh");
    
    // 添加顶点
    OriginalMesh.AddVertex(0.0f, 0.0f, 0.0f);
    OriginalMesh.AddVertex(1.0f, 0.0f, 0.0f);
    OriginalMesh.AddVertex(0.0f, 1.0f, 0.0f);
    
    // 添加单元
    FCellArray& Cells = OriginalMesh.GetCells();
    TArray<int32> TriangleIndices;
    TriangleIndices.Add(0);
    TriangleIndices.Add(1);
    TriangleIndices.Add(2);
    Cells.AddCell(ECellType::Triangle, TriangleIndices);
    
    // 添加场数据
    TUniquePtr<FField> Field = MakeUnique<FField>("TestField", EFieldType::Scalar, EFieldAttachment::Vertex);
    TArray<float> FieldData;
    FieldData.Add(1.0f);
    FieldData.Add(2.0f);
    FieldData.Add(3.0f);
    Field->SetScalarData(FieldData);
    OriginalMesh.AddField(std::move(Field));
    
    // 测试拷贝构造
    IMesh CopiedMesh(OriginalMesh);
    ASSERT(CopiedMesh.GetMeshName() == "OriginalMesh");
    ASSERT(CopiedMesh.GetVertexCount() == 3);
    ASSERT(CopiedMesh.GetCellCount() == 1);
    ASSERT(CopiedMesh.HasField("TestField"));
    
    // 验证拷贝是独立的
    CopiedMesh.SetMeshName("CopiedMesh");
    ASSERT(CopiedMesh.GetMeshName() == "CopiedMesh");
    ASSERT(OriginalMesh.GetMeshName() == "OriginalMesh");
    
    CopiedMesh.AddVertex(FVector(4.0f, 4.0f, 4.0f));
    ASSERT(CopiedMesh.GetVertexCount() == 4);
    ASSERT(OriginalMesh.GetVertexCount() == 3);
    
    // 测试移动构造
    IMesh MovedMesh(std::move(CopiedMesh));
    ASSERT(MovedMesh.GetMeshName() == "CopiedMesh");
    ASSERT(MovedMesh.GetVertexCount() == 4);
    ASSERT(MovedMesh.GetCellCount() == 1);
    
    // 测试拷贝赋值
    IMesh AssignedMesh;
    AssignedMesh = OriginalMesh;
    ASSERT(AssignedMesh.GetMeshName() == "OriginalMesh");
    ASSERT(AssignedMesh.GetVertexCount() == 3);
    ASSERT(AssignedMesh.GetCellCount() == 1);
    
    // 测试移动赋值
    IMesh MoveAssignedMesh;
    MoveAssignedMesh = std::move(MovedMesh);
    ASSERT(MoveAssignedMesh.GetMeshName() == "CopiedMesh");
    ASSERT(MoveAssignedMesh.GetVertexCount() == 4);
    
    // 测试验证功能
    ASSERT(OriginalMesh.Validate());
    ASSERT(AssignedMesh.Validate());
    
    // 测试清空
    OriginalMesh.Clear();
    ASSERT(OriginalMesh.GetVertexCount() == 0);
    ASSERT(OriginalMesh.GetCellCount() == 0);
    ASSERT(!OriginalMesh.HasField("TestField"));
    
    // 测试重置
    OriginalMesh.Reset();
    ASSERT(OriginalMesh.GetMeshName() == "UnnamedMesh");
    ASSERT(OriginalMesh.GetVertexCount() == 0);
    ASSERT(OriginalMesh.GetCellCount() == 0);
    
    // 测试内存管理
    IMesh MemoryMesh;
    MemoryMesh.ReserveVertices(100);
    MemoryMesh.ReserveCells(50);
    
    for (int i = 0; i < 10; ++i)
    {
        MemoryMesh.AddVertex(static_cast<float>(i), 0.0f, 0.0f);
    }
    
    MemoryMesh.Shrink();
    ASSERT(MemoryMesh.GetVertexCount() == 10);
}

