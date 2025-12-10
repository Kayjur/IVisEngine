#include "TestFramework.h"
#include "Math/Math.h"

TEST_GROUP(TestVector)

// TVector 基本功能测试
TEST(TVector_Basic)
{
    FVector V1(1.0f, 2.0f, 3.0f);
    FVector V2(4.0f, 5.0f, 6.0f);
    
    ASSERT(V1.X == 1.0f);
    ASSERT(V1.Y == 2.0f);
    ASSERT(V1.Z == 3.0f);
    
    // 测试数组访问
    ASSERT(V1.XYZ[0] == 1.0f);
    ASSERT(V1.XYZ[1] == 2.0f);
    ASSERT(V1.XYZ[2] == 3.0f);
    
    // 测试索引访问
    ASSERT(V1[0] == 1.0f);
    ASSERT(V1[1] == 2.0f);
    ASSERT(V1[2] == 3.0f);
    
    // 测试向量运算
    FVector Sum = V1 + V2;
    ASSERT(Sum.X == 5.0f);
    ASSERT(Sum.Y == 7.0f);
    ASSERT(Sum.Z == 9.0f);
}

// TVector 长度和归一化测试
TEST(TVector_SizeAndNormalize)
{
    FVector V(3.0f, 4.0f, 0.0f);
    
    float Size = V.Size();
    ASSERT_EQ(Size, 5.0f);
    
    float SizeSquared = V.SizeSquared();
    ASSERT_EQ(SizeSquared, 25.0f);
    
    FVector Normalized = V.GetNormalized();
    ASSERT_EQ(Normalized.Size(), 1.0f);
}

// TVector 类型转换测试
TEST(TVector_Conversion)
{
    FVector3d DoubleVec(1.0, 2.0, 3.0);
    FVector FloatVec = DoubleVec; // 隐式转换
    
    ASSERT_EQ(FloatVec.X, 1.0f);
    ASSERT_EQ(FloatVec.Y, 2.0f);
    ASSERT_EQ(FloatVec.Z, 3.0f);
    
    FVector3d DoubleVec2 = FloatVec; // 隐式转换
    ASSERT_EQ(DoubleVec2.X, 1.0);
    ASSERT_EQ(DoubleVec2.Y, 2.0);
    ASSERT_EQ(DoubleVec2.Z, 3.0);
}

// TVector2 基本功能测试
TEST(TVector2_Basic)
{
    TVector2<float> V1(1.0f, 2.0f);
    TVector2<float> V2(3.0f, 4.0f);
    
    ASSERT(V1.X == 1.0f);
    ASSERT(V1.Y == 2.0f);
    
    // 测试数组访问
    ASSERT(V1.XY[0] == 1.0f);
    ASSERT(V1.XY[1] == 2.0f);
    
    // 测试向量运算
    TVector2<float> Sum = V1 + V2;
    ASSERT(Sum.X == 4.0f);
    ASSERT(Sum.Y == 6.0f);
    
    float Size = V1.Size();
    ASSERT(Size > 2.2f && Size < 2.3f); // sqrt(1^2 + 2^2) ≈ 2.236
}

// TVector2 类型转换测试
TEST(TVector2_Conversion)
{
    TVector2<double> DoubleVec(1.0, 2.0);
    TVector2<float> FloatVec = DoubleVec;
    
    ASSERT_EQ(FloatVec.X, 1.0f);
    ASSERT_EQ(FloatVec.Y, 2.0f);
}

// TVector4 基本功能测试
TEST(TVector4_Basic)
{
    TVector4<float> V1(1.0f, 2.0f, 3.0f, 4.0f);
    TVector4<float> V2(5.0f, 6.0f, 7.0f, 8.0f);
    
    ASSERT(V1.X == 1.0f);
    ASSERT(V1.Y == 2.0f);
    ASSERT(V1.Z == 3.0f);
    ASSERT(V1.W == 4.0f);
    
    // 测试数组访问
    ASSERT(V1.XYZW[0] == 1.0f);
    ASSERT(V1.XYZW[3] == 4.0f);
    
    // 测试向量运算
    TVector4<float> Sum = V1 + V2;
    ASSERT(Sum.X == 6.0f);
    ASSERT(Sum.W == 12.0f);
    
    // 测试从 TVector 构造
    TVector<float> V3(10.0f, 20.0f, 30.0f);
    TVector4<float> V4FromV3(V3, 1.0f);
    ASSERT(V4FromV3.X == 10.0f);
    ASSERT(V4FromV3.Z == 30.0f);
    ASSERT(V4FromV3.W == 1.0f);
}

// TVector4 类型转换测试
TEST(TVector4_Conversion)
{
    TVector4<double> DoubleVec(1.0, 2.0, 3.0, 4.0);
    TVector4<float> FloatVec = DoubleVec;
    
    ASSERT_EQ(FloatVec.X, 1.0f);
    ASSERT_EQ(FloatVec.Y, 2.0f);
    ASSERT_EQ(FloatVec.Z, 3.0f);
    ASSERT_EQ(FloatVec.W, 4.0f);
}