#include "TestFramework.h"
#include "Math/Math.h"

TEST_GROUP(TestFMath)

TEST(FMath_Constants)
{
    ASSERT(FMath::Abs(FMath::PI - 3.14159265f) < 0.0001f);
    ASSERT(FMath::SmallNumber == 1.e-8f);
}

TEST(FMath_Basic)
{
    ASSERT(FMath::Abs(-5.0f) == 5.0f);
    ASSERT(FMath::Max(10, 20) == 20);
    ASSERT(FMath::Min(10, 20) == 10);
    ASSERT(FMath::Clamp(15, 0, 10) == 10);
    ASSERT(FMath::Clamp(-5, 0, 10) == 0);
    ASSERT(FMath::Square(3.0f) == 9.0f);
    ASSERT(FMath::Sign(-10.0f) == -1.0f);
    ASSERT(FMath::Sign(10.0f) == 1.0f);
}

TEST(FMath_Trig)
{
    ASSERT(FMath::IsNearlyZero(FMath::Sin(0.0f)));
    ASSERT(FMath::IsNearlyEqual(FMath::Cos(0.0f), 1.0f));
    ASSERT(FMath::IsNearlyEqual(FMath::Tan(FMath::PI / 4.0f), 1.0f, 1.e-4f));
}

TEST(FMath_Power)
{
    ASSERT(FMath::IsNearlyEqual(FMath::Sqrt(4.0f), 2.0f));
    ASSERT(FMath::IsNearlyEqual(FMath::Pow(2.0f, 3.0f), 8.0f));
}

TEST(FMath_Interpolation)
{
    ASSERT(FMath::IsNearlyEqual(FMath::Lerp(0.0f, 10.0f, 0.5f), 5.0f));
    ASSERT(FMath::IsNearlyEqual(FMath::Lerp(0.0f, 10.0f, 0.0f), 0.0f));
    ASSERT(FMath::IsNearlyEqual(FMath::Lerp(0.0f, 10.0f, 1.0f), 10.0f));
}

TEST(FMath_Rounding)
{
    ASSERT(FMath::FloorToInt(1.9f) == 1);
    ASSERT(FMath::CeilToInt(1.1f) == 2);
    ASSERT(FMath::RoundToInt(1.5f) == 2);
    ASSERT(FMath::RoundToInt(1.4f) == 1);
}

TEST(FMath_Random)
{
    FMath::RandInit(12345);
    float R = FMath::FRand();
    ASSERT(R >= 0.0f && R <= 1.0f);
    
    int32 I = FMath::RandRange(10, 20);
    ASSERT(I >= 10 && I <= 20);
}