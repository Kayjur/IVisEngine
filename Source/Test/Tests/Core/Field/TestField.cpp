#include "TestFramework.h"
#include "Field/Field.h"
#include "Math/Math.h"

TEST_GROUP(TestField)

// ============================================================================
// 构造函数和基本属性测试
// ============================================================================

TEST(Field_Constructor)
{
    // 默认构造函数
    FField DefaultField;
    ASSERT(!DefaultField.IsValid());
    ASSERT(DefaultField.GetFieldDimension() == 1);
    ASSERT(DefaultField.GetDataCount() == 0);
    
    // 标量场构造函数
    FField ScalarField("Temperature", EFieldType::Scalar, EFieldAttachment::Vertex);
    ASSERT(ScalarField.IsValid());
    ASSERT(ScalarField.GetFieldName() == "Temperature");
    ASSERT(ScalarField.GetFieldType() == EFieldType::Scalar);
    ASSERT(ScalarField.GetAttachment() == EFieldAttachment::Vertex);
    ASSERT(ScalarField.GetFieldDimension() == 1);
    
    // 向量场构造函数
    FField VectorField("Displacement", EFieldType::Vector, EFieldAttachment::Vertex);
    ASSERT(VectorField.GetFieldType() == EFieldType::Vector);
    ASSERT(VectorField.GetFieldDimension() == 3);
    
    // 张量场构造函数
    FField TensorField("Stress", EFieldType::Tensor, EFieldAttachment::Cell);
    ASSERT(TensorField.GetFieldType() == EFieldType::Tensor);
    ASSERT(TensorField.GetAttachment() == EFieldAttachment::Cell);
    ASSERT(TensorField.GetFieldDimension() == 9);
    
    // 自定义场构造函数
    FField CustomField("Custom", EFieldType::Custom, EFieldAttachment::Vertex, 5);
    ASSERT(CustomField.GetFieldType() == EFieldType::Custom);
    ASSERT(CustomField.GetFieldDimension() == 5);
}

// ============================================================================
// 标量场测试
// ============================================================================

TEST(Field_Scalar_Basic)
{
    FField ScalarField("Temperature", EFieldType::Scalar, EFieldAttachment::Vertex);
    
    // 测试添加标量值
    ScalarField.AddScalar(10.0);
    ScalarField.AddScalar(20.0);
    ScalarField.AddScalar(30.0);
    
    ASSERT(ScalarField.GetDataCount() == 3);
    ASSERT(ScalarField.GetScalar(0) == 10.0);
    ASSERT(ScalarField.GetScalar(1) == 20.0);
    ASSERT(ScalarField.GetScalar(2) == 30.0);
    
    // 测试设置标量值
    ScalarField.SetScalar(1, 25.0);
    ASSERT(ScalarField.GetScalar(1) == 25.0);
}

TEST(Field_Scalar_SetData)
{
    FField ScalarField("Temperature", EFieldType::Scalar, EFieldAttachment::Vertex);
    
    TArray<float> Data;
    Data.Add(1.0);
    Data.Add(2.0);
    Data.Add(3.0);
    Data.Add(4.0);
    Data.Add(5.0);
    
    ScalarField.SetScalarData(Data);
    ASSERT(ScalarField.GetDataCount() == 5);
    ASSERT(ScalarField.GetScalar(0) == 1.0);
    ASSERT(ScalarField.GetScalar(4) == 5.0);
    
    // 测试移动语义
    TArray<float> MoveData;
    MoveData.Add(10.0);
    MoveData.Add(20.0);
    ScalarField.SetScalarData(std::move(MoveData));
    ASSERT(ScalarField.GetDataCount() == 2);
    ASSERT(ScalarField.GetScalar(0) == 10.0);
}

// ============================================================================
// 向量场测试
// ============================================================================


TEST(Field_Vector_Basic)
{
    FField VectorField("Displacement", EFieldType::Vector, EFieldAttachment::Vertex);
    
    // 测试添加向量
    FVector V1(1.0, 2.0, 3.0);
    FVector V2(4.0, 5.0, 6.0);
    FVector V3(7.0, 8.0, 9.0);
    
    VectorField.AddVector(V1);
    VectorField.AddVector(V2);
    VectorField.AddVector(V3);
    
    ASSERT(VectorField.GetDataCount() == 3);
    
    FVector Result1 = VectorField.GetVector(0);
    ASSERT(Result1.X == 1.0);
    ASSERT(Result1.Y == 2.0);
    ASSERT(Result1.Z == 3.0);
    
    FVector Result2 = VectorField.GetVector(1);
    ASSERT(Result2.X == 4.0);
    ASSERT(Result2.Y == 5.0);
    ASSERT(Result2.Z == 6.0);
    
    // 测试设置向量
    FVector NewVec(10.0, 11.0, 12.0);
    VectorField.SetVector(1, NewVec);
    FVector Result3 = VectorField.GetVector(1);
    ASSERT(Result3.X == 10.0);
    ASSERT(Result3.Y == 11.0);
    ASSERT(Result3.Z == 12.0);
}

TEST(Field_Vector_SetData)
{
    FField VectorField("Velocity", EFieldType::Vector, EFieldAttachment::Vertex);
    
    // 设置向量数据（连续存储：x0, y0, z0, x1, y1, z1, ...）
    TArray<float> VectorData;
    VectorData.Add(1.0);  // V0.X
    VectorData.Add(2.0);  // V0.Y
    VectorData.Add(3.0);  // V0.Z
    VectorData.Add(4.0);  // V1.X
    VectorData.Add(5.0);  // V1.Y
    VectorData.Add(6.0);  // V1.Z
    
    VectorField.SetVectorData(VectorData);
    ASSERT(VectorField.GetDataCount() == 2);
    
    FVector V0 = VectorField.GetVector(0);
    ASSERT(V0.X == 1.0);
    ASSERT(V0.Y == 2.0);
    ASSERT(V0.Z == 3.0);
    
    FVector V1 = VectorField.GetVector(1);
    ASSERT(V1.X == 4.0);
    ASSERT(V1.Y == 5.0);
    ASSERT(V1.Z == 6.0);
}

// ============================================================================
// 张量场测试
// ============================================================================

TEST(Field_Tensor_Basic)
{
    FField TensorField("Stress", EFieldType::Tensor, EFieldAttachment::Cell);
    
    // 创建3x3矩阵（按行优先存储为9个float）
    TArray<float> Tensor1;
    for (int i = 0; i < 9; ++i)
    {
        Tensor1.Add(static_cast<float>(i + 1));  // [1, 2, 3, 4, 5, 6, 7, 8, 9]
    }
    
    TArray<float> Tensor2;
    for (int i = 0; i < 9; ++i)
    {
        Tensor2.Add(static_cast<float>(i + 10));  // [10, 11, 12, 13, 14, 15, 16, 17, 18]
    }
    
    TensorField.AddTensor(Tensor1);
    TensorField.AddTensor(Tensor2);
    
    ASSERT(TensorField.GetDataCount() == 2);
    
    // 测试获取张量
    TArray<float> OutTensor1;
    TensorField.GetTensor(0, OutTensor1);
    ASSERT(OutTensor1.Num() == 9);
    ASSERT(OutTensor1[0] == 1.0);
    ASSERT(OutTensor1[8] == 9.0);
    
    TArray<float> OutTensor2;
    TensorField.GetTensor(1, OutTensor2);
    ASSERT(OutTensor2.Num() == 9);
    ASSERT(OutTensor2[0] == 10.0);
    ASSERT(OutTensor2[8] == 18.0);
    
    // 测试设置张量
    TArray<float> NewTensor;
    for (int i = 0; i < 9; ++i)
    {
        NewTensor.Add(static_cast<float>(i + 100));
    }
    TensorField.SetTensor(0, NewTensor);
    
    TArray<float> OutTensor3;
    TensorField.GetTensor(0, OutTensor3);
    ASSERT(OutTensor3[0] == 100.0);
    ASSERT(OutTensor3[8] == 108.0);
}

TEST(Field_Tensor_SetData)
{
    FField TensorField("Strain", EFieldType::Tensor, EFieldAttachment::Cell);
    
    // 设置张量数据（连续存储：t0_9elems, t1_9elems, ...）
    TArray<float> TensorData;
    // 第一个张量
    for (int i = 0; i < 9; ++i)
    {
        TensorData.Add(static_cast<float>(i + 1));
    }
    // 第二个张量
    for (int i = 0; i < 9; ++i)
    {
        TensorData.Add(static_cast<float>(i + 10));
    }
    
    TensorField.SetTensorData(TensorData);
    ASSERT(TensorField.GetDataCount() == 2);
    
    TArray<float> OutTensor;
    TensorField.GetTensor(1, OutTensor);
    ASSERT(OutTensor[0] == 10.0);
    ASSERT(OutTensor[8] == 18.0);
}

// ============================================================================
// 通用操作测试
// ============================================================================

TEST(Field_Generic_Operations)
{
    // 测试自定义维度场
    FField CustomField("Custom", EFieldType::Custom, EFieldAttachment::Vertex, 4);
    
    TArray<float> Data1;
    Data1.Add(1.0);
    Data1.Add(2.0);
    Data1.Add(3.0);
    Data1.Add(4.0);
    
    TArray<float> Data2;
    Data2.Add(5.0);
    Data2.Add(6.0);
    Data2.Add(7.0);
    Data2.Add(8.0);
    
    CustomField.AddData(Data1);
    CustomField.AddData(Data2);
    
    ASSERT(CustomField.GetDataCount() == 2);
    
    // 测试获取数据
    TArray<float> OutData;
    CustomField.GetData(0, OutData);
    ASSERT(OutData.Num() == 4);
    ASSERT(OutData[0] == 1.0);
    ASSERT(OutData[3] == 4.0);
    
    // 测试设置数据
    TArray<float> NewData;
    NewData.Add(10.0);
    NewData.Add(20.0);
    NewData.Add(30.0);
    NewData.Add(40.0);
    CustomField.SetData(1, NewData);
    
    TArray<float> OutData2;
    CustomField.GetData(1, OutData2);
    ASSERT(OutData2[0] == 10.0);
    ASSERT(OutData2[3] == 40.0);
}

TEST(Field_SetFieldData)
{
    FField Field("Test", EFieldType::Custom, EFieldAttachment::Vertex, 2);

    TArray<float> Data;
    Data.Add(1.0);
    Data.Add(2.0);
    Data.Add(3.0);
    Data.Add(4.0);
    Data.Add(5.0);
    Data.Add(6.0);
    
    Field.SetFieldData(Data);
    ASSERT(Field.GetDataCount() == 3);
    ASSERT(Field.GetFieldDimension() == 2);
    
    TArray<float> OutData;
    Field.GetData(2, OutData);
    ASSERT(OutData[0] == 5.0);
    ASSERT(OutData[1] == 6.0);

    // 测试移动语义
    TArray<float> MoveData;
    MoveData.Add(10.0);
    MoveData.Add(20.0);
    Field.SetFieldData(std::move(MoveData));
    ASSERT(Field.GetDataCount() == 1);
}

TEST(Field_ClearAndReserve)
{
    FField ScalarField("Temp", EFieldType::Scalar, EFieldAttachment::Vertex);
    
    ScalarField.AddScalar(1.0);
    ScalarField.AddScalar(2.0);
    ScalarField.AddScalar(3.0);
    
    ASSERT(ScalarField.GetDataCount() == 3);
    
    ScalarField.Clear();
    ASSERT(ScalarField.GetDataCount() == 0);
    
    ScalarField.Reserve(100);
    ScalarField.AddScalar(1.0);
    ASSERT(ScalarField.GetDataCount() == 1);
}

TEST(Field_Resize)
{
    FField ScalarField("Temp", EFieldType::Scalar, EFieldAttachment::Vertex);
    
    ScalarField.Resize(5);
    ASSERT(ScalarField.GetDataCount() == 5);
    ASSERT(ScalarField.GetRawDataSize() == 5 * sizeof(float));
    
    // 向量场 Resize
    FField VectorField("Vel", EFieldType::Vector, EFieldAttachment::Vertex);
    VectorField.Resize(3);
    ASSERT(VectorField.GetDataCount() == 3);
    ASSERT(VectorField.GetRawDataSize() == 3 * 3 * sizeof(float));
}

// ============================================================================
// 底层数据访问测试
// ============================================================================

TEST(Field_RawDataAccess)
{
    FField ScalarField("Temp", EFieldType::Scalar, EFieldAttachment::Vertex);
    
    ScalarField.AddScalar(10.0);
    ScalarField.AddScalar(20.0);
    ScalarField.AddScalar(30.0);
    
    const float* RawPtr = ScalarField.GetRawDataPtr();
    ASSERT(RawPtr != nullptr);
    ASSERT(RawPtr[0] == 10.0);
    ASSERT(RawPtr[1] == 20.0);
    ASSERT(RawPtr[2] == 30.0);
    
    size_t DataSize = ScalarField.GetRawDataSize();
    ASSERT(DataSize == 3 * sizeof(float));
    
    // 测试向量场的底层数据访问
    FField VectorField("Vel", EFieldType::Vector, EFieldAttachment::Vertex);
    VectorField.AddVector(FVector(1.0, 2.0, 3.0));
    
    const float* VectorPtr = VectorField.GetRawDataPtr();
    ASSERT(VectorPtr[0] == 1.0);
    ASSERT(VectorPtr[1] == 2.0);
    ASSERT(VectorPtr[2] == 3.0);
}

TEST(Field_GetFieldData)
{
    FField ScalarField("Temp", EFieldType::Scalar, EFieldAttachment::Vertex);
    ScalarField.AddScalar(1.0);
    ScalarField.AddScalar(2.0);
    
    const TArray<float>& ConstData = ScalarField.GetFieldData();
    ASSERT(ConstData.Num() == 2);
    ASSERT(ConstData[0] == 1.0);
    
    TArray<float>& MutableData = ScalarField.GetFieldData();
    MutableData[0] = 10.0;
    ASSERT(ScalarField.GetScalar(0) == 10.0);
}

// ============================================================================
// 边界检查测试
// ============================================================================

TEST(Field_IndexValidation)
{
    FField ScalarField("Temp", EFieldType::Scalar, EFieldAttachment::Vertex);
    ScalarField.AddScalar(1.0);
    ScalarField.AddScalar(2.0);
    
    ASSERT(ScalarField.IsValidIndex(0));
    ASSERT(ScalarField.IsValidIndex(1));
    ASSERT(!ScalarField.IsValidIndex(2));
    ASSERT(!ScalarField.IsValidIndex(100));
}

// ============================================================================
// 性能测试
// ============================================================================

TEST(Field_Performance_AddScalar)
{
    FTimer Timer;
    Timer.Start();
    
    FField ScalarField("Temp", EFieldType::Scalar, EFieldAttachment::Vertex);
    for (int i = 0; i < 1000000; ++i)
    {
        ScalarField.AddScalar(static_cast<float>(i));
    }
    
    Timer.Stop();
    ASSERT(ScalarField.GetDataCount() == 1000000);
    //Timer.Print();
}

TEST(Field_Performance_GetVector)
{
    FField VectorField("Vel", EFieldType::Vector, EFieldAttachment::Vertex);
    
    // 添加1000个向量
    for (int i = 0; i < 1000; ++i)
    {
        VectorField.AddVector(FVector(static_cast<float>(i), static_cast<float>(i + 1), static_cast<float>(i + 2)));
    }
    
    FTimer Timer;
    Timer.Start();
    
    // 读取1000万次
    for (int i = 0; i < 10000000; ++i)
    {
        uint32 Index = static_cast<uint32>(i % 1000);
        FVector V = VectorField.GetVector(Index);
        (void)V;  // 避免优化
    }
    
    Timer.Stop();
    //Timer.Print();
}

