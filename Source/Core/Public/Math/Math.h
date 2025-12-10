#pragma once

#include <cmath>
#include <algorithm>
#include <limits>
#include <cstdlib>
#include "Vector.h"
#include "Vector2.h"
#include "Vector4.h"

// 类型别名（UE 风格）
using FVector = TVector<float>;
using FVector2D = TVector2<float>;
using FVector4D = TVector4<float>;

using FVector3f = TVector<float>;
using FVector2f = TVector2<float>;
using FVector4f = TVector4<float>;

using FVector3d = TVector<double>;
using FVector2d = TVector2<double>;
using FVector4d = TVector4<double>;


/**
 * @brief 数学工具库，提供常用的数学函数和常量
 *
 * 包含基础算术、三角函数、插值、随机数、几何计算等功能。
 * 为防止调用不明确的问题，统一使用 double 类型，但是类型转换会有一定的损耗
 */
struct FMath
{
public:
    // ============================================================================
    // 常量定义
    // ============================================================================
    static constexpr float PI = 3.1415926535897932f;
    static constexpr float InvPI = 0.31830988618379067f;
    static constexpr float HalfPI = 1.5707963267948966f;
    static constexpr float TwoPI = 6.2831853071795864f;
    static constexpr float EulersNumber = 2.71828182845904523536f;

    static constexpr float SmallNumber = 1.e-8f;
    static constexpr float KindaSmallNumber = 1.e-4f;
    static constexpr float BigNumber = 3.4e+38f;

    static constexpr double PId = 3.141592653589793238462643383279502884197169399;

    // ============================================================================
    // 基础算术
    // ============================================================================

    /** 计算绝对值 */
    template<typename T>
    static constexpr T Abs(const T A)
    {
        return (A >= (T)0) ? A : -A;
    }

    /** 计算符号：>0 返回 1，<0 返回 -1，=0 返回 0 */
    template<typename T>
    static constexpr T Sign(const T A)
    {
        return (A > (T)0) ? (T)1 : ((A < (T)0) ? (T)-1 : (T)0);
    }

    /** 返回最大值 */
    template<typename T>
    static constexpr T Max(const T A, const T B)
    {
        return (A >= B) ? A : B;
    }

    /** 返回最小值 */
    template<typename T>
    static constexpr T Min(const T A, const T B)
    {
        return (A <= B) ? A : B;
    }

    /** 将值限制在 [Min, Max] 范围内 */
    template<typename T>
    static constexpr T Clamp(const T X, const T Min, const T Max)
    {
        return (X < Min) ? Min : ((X > Max) ? Max : X);
    }

    /** 计算平方 */
    template<typename T>
    static constexpr T Square(const T A)
    {
        return A * A;
    }

    // ============================================================================
    // 三角函数
    // ============================================================================

    static double Sin(double Value);

    static double Cos(double Value);

    static double Tan(double Value);

    static double Asin(double Value);

    static double Acos(double Value);

    static double Atan(double Value);

    static double Atan2(double Y, double X);

    /** 同时计算正弦和余弦 */
    static void SinCos(double* ScalarSin, double* ScalarCos, double Value);

    // ============================================================================
    // 幂与根
    // ============================================================================

    static double Sqrt(double Value);

    static double Pow(double A, double B);

    /** 计算平方根倒数 (1/Sqrt(x)) */
    static double InvSqrt(double F);

    // ============================================================================
    // 插值
    // ============================================================================

    /** 线性插值 */
    template<typename T, typename U>
    static T Lerp(const T& A, const T& B, const U& Alpha)
    {
        return (T)(A + Alpha * (B - A));
    }

    // ============================================================================
    // 转换
    // ============================================================================

    /** 角度转弧度 */
    static constexpr float DegreesToRadians(float Degrees)
    {
        return Degrees * (PI / 180.0f);
    }

    /** 角度转弧度 */
    static constexpr double DegreesToRadians(double Degrees)
    {
        return Degrees * (PId / 180.0);
    }

    /** 弧度转角度 */
    static constexpr float RadiansToDegrees(float Radians)
    {
        return Radians * (180.0f / PI);
    }

    /** 弧度转角度 */
    static constexpr double RadiansToDegrees(double Radians)
    {
        return Radians * (180.0 / PId);
    }

    // ============================================================================
    // 浮点数比较
    // ============================================================================

    /** 判断两个浮点数是否近似相等 */
    static bool IsNearlyEqual(double A, double B, double ErrorTolerance = SmallNumber);

    /** 判断浮点数是否近似为零 */
    static bool IsNearlyZero(double Value, double ErrorTolerance = SmallNumber);

    // ============================================================================
    // 取整
    // ============================================================================

    /** 向下取整并转换为整数 */
    static int32 FloorToInt(double F);

    static double Floor(double F);

    /** 向上取整并转换为整数 */
    static int32 CeilToInt(double F);

    static double Ceil(double F);

    /** 四舍五入并转换为整数 */
    static int32 RoundToInt(double F);

    static double Round(double F);

    /** 获取小数部分 */
    static double Fractional(double Value);
    
    /** 浮点取模 */
    static double Fmod(double X, double Y);

    // ============================================================================
    // 随机数
    // ============================================================================

    /** 返回随机整数 */
    static int32 Rand();
    
    /** 初始化随机种子 */
    static void RandInit(int32 Seed);

    /** 返回 [0, 1] 之间的随机浮点数 */
    static float FRand();

    /** 返回 [Min, Max] 之间的随机浮点数 */
    static double RandRange(double Min, double Max);

    /** 返回 [Min, Max] 之间的随机整数 (包含 Min 和 Max) */
    static int32 RandRange(int32 Min, int32 Max);

    /** 返回随机布尔值 */
    static bool RandBool();
};