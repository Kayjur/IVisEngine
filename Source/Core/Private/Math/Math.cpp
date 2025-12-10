#include "Math/Math.h"
#include <cmath>
#include <cstdlib>

// ============================================================================
// 三角函数
// ============================================================================

double FMath::Sin(double Value)
{
    return std::sin(Value);
}

double FMath::Cos(double Value)
{
    return std::cos(Value);
}

double FMath::Tan(double Value)
{
    return std::tan(Value);
}

double FMath::Asin(double Value)
{
    return std::asin(Value);
}

double FMath::Acos(double Value)
{
    return std::acos(Value);
}

double FMath::Atan(double Value)
{
    return std::atan(Value);
}

double FMath::Atan2(double Y, double X)
{
    return std::atan2(Y, X);
}

void FMath::SinCos(double* ScalarSin, double* ScalarCos, double Value)
{
    *ScalarSin = std::sin(Value);
    *ScalarCos = std::cos(Value);
}

// ============================================================================
// 幂与根
// ============================================================================

double FMath::Sqrt(double Value)
{
    return std::sqrt(Value);
}

double FMath::Pow(double A, double B)
{
    return std::pow(A, B);
}

double FMath::InvSqrt(double F)
{
    return 1.0 / std::sqrt(F);
}

// ============================================================================
// 浮点数比较
// ============================================================================

bool FMath::IsNearlyEqual(double A, double B, double ErrorTolerance)
{
    return Abs(A - B) <= ErrorTolerance;
}

bool FMath::IsNearlyZero(double Value, double ErrorTolerance)
{
    return Abs(Value) <= ErrorTolerance;
}

// ============================================================================
// 取整
// ============================================================================

int32 FMath::FloorToInt(double F)
{
    return static_cast<int32>(std::floor(F));
}

double FMath::Floor(double F)
{
    return std::floor(F);
}

int32 FMath::CeilToInt(double F)
{
    return static_cast<int32>(std::ceil(F));
}

double FMath::Ceil(double F)
{
    return std::ceil(F);
}

int32 FMath::RoundToInt(double F)
{
    return static_cast<int32>(std::round(F));
}

double FMath::Round(double F)
{
    return std::round(F);
}

double FMath::Fractional(double Value)
{
    return Value - Floor(Value);
}

double FMath::Fmod(double X, double Y)
{
    return std::fmod(X, Y);
}

// ============================================================================
// 随机数
// ============================================================================

int32 FMath::Rand()
{
    return std::rand();
}

void FMath::RandInit(int32 Seed)
{
    std::srand(Seed);
}

float FMath::FRand()
{
    return (float)std::rand() / (float)RAND_MAX;
}

double FMath::RandRange(double Min, double Max)
{
    return Min + (Max - Min) * FRand();
}

int32 FMath::RandRange(int32 Min, int32 Max)
{
    if (Min == Max) return Min;
    int32 Range = Max - Min + 1;
    return Min + (std::rand() % Range);
}

bool FMath::RandBool()
{
    return (std::rand() & 1) == 1;
}