#pragma once

#include <cmath>
#include <cstdint>
#include <limits>
#include "Vector.h"

/**
 * TVector4 - 4D 向量类（模板类）
 * 使用 UE 风格的接口命名
 * 
 * @tparam T 数值类型（如 float, double, int 等）
 */
template<typename T>
class TVector4
{
public:
    // ============================================================================
    // 类型定义
    // ============================================================================
    using ElementType = T;

    // ============================================================================
    // 成员变量（使用 union 同时支持成员变量和数组访问）
    // ============================================================================
    
    union
    {
        struct
        {
            /** Vector's X component. */
            T X;
            /** Vector's Y component. */
            T Y;
            /** Vector's Z component. */
            T Z;
            /** Vector's W component. */
            T W;
        };
        /** Array access to X, Y, Z, W components. */
        T XYZW[4];
    };

    // ============================================================================
    // 构造函数
    // ============================================================================
    
    /** 默认构造函数，初始化为零向量 */
    TVector4() : X(T(0)), Y(T(0)), Z(T(0)), W(T(0)) {}
    
    /** 使用 X, Y, Z, W 分量构造 */
    TVector4(T InX, T InY, T InZ, T InW) : X(InX), Y(InY), Z(InZ), W(InW) {}
    
    /** 使用单个值构造（所有分量相同） */
    explicit TVector4(T Value) : X(Value), Y(Value), Z(Value), W(Value) {}
    
    /** 从 3D 向量和 W 分量构造 */
    template<typename U>
    TVector4(const TVector<U>& InVector, T InW) : X(static_cast<T>(InVector.X)), Y(static_cast<T>(InVector.Y)), Z(static_cast<T>(InVector.Z)), W(InW) {}
    
    /** 拷贝构造函数 */
    TVector4(const TVector4& Other) = default;
    
    /** 移动构造函数 */
    TVector4(TVector4&& Other) noexcept = default;
    
    /** 析构函数 */
    ~TVector4() = default;

    // ============================================================================
    // 赋值操作符
    // ============================================================================
    
    /** 拷贝赋值 */
    TVector4& operator=(const TVector4& Other) = default;
    
    /** 移动赋值 */
    TVector4& operator=(TVector4&& Other) noexcept = default;

    // ============================================================================
    // 算术运算
    // ============================================================================
    
    /** 向量加法 */
    TVector4 operator+(const TVector4& Other) const
    {
        return TVector4(X + Other.X, Y + Other.Y, Z + Other.Z, W + Other.W);
    }
    
    /** 向量减法 */
    TVector4 operator-(const TVector4& Other) const
    {
        return TVector4(X - Other.X, Y - Other.Y, Z - Other.Z, W - Other.W);
    }
    
    /** 向量数乘 */
    TVector4 operator*(T Scalar) const
    {
        return TVector4(X * Scalar, Y * Scalar, Z * Scalar, W * Scalar);
    }
    
    /** 向量数除 */
    TVector4 operator/(T Scalar) const
    {
        T InvScalar = T(1) / Scalar;
        return TVector4(X * InvScalar, Y * InvScalar, Z * InvScalar, W * InvScalar);
    }
    
    /** 向量取反 */
    TVector4 operator-() const
    {
        return TVector4(-X, -Y, -Z, -W);
    }
    
    /** 向量加法赋值 */
    TVector4& operator+=(const TVector4& Other)
    {
        X += Other.X;
        Y += Other.Y;
        Z += Other.Z;
        W += Other.W;
        return *this;
    }
    
    /** 向量减法赋值 */
    TVector4& operator-=(const TVector4& Other)
    {
        X -= Other.X;
        Y -= Other.Y;
        Z -= Other.Z;
        W -= Other.W;
        return *this;
    }
    
    /** 向量数乘赋值 */
    TVector4& operator*=(T Scalar)
    {
        X *= Scalar;
        Y *= Scalar;
        Z *= Scalar;
        W *= Scalar;
        return *this;
    }
    
    /** 向量数除赋值 */
    TVector4& operator/=(T Scalar)
    {
        T InvScalar = T(1) / Scalar;
        X *= InvScalar;
        Y *= InvScalar;
        Z *= InvScalar;
        W *= InvScalar;
        return *this;
    }
    
    /** 向量相等比较 */
    bool operator==(const TVector4& Other) const
    {
        return X == Other.X && Y == Other.Y && Z == Other.Z && W == Other.W;
    }
    
    /** 向量不等比较 */
    bool operator!=(const TVector4& Other) const
    {
        return !(*this == Other);
    }

    // ============================================================================
    // 向量运算
    // ============================================================================
    
    /** 计算向量长度（模长） */
    T Size() const
    {
        return static_cast<T>(std::sqrt(static_cast<double>(X * X + Y * Y + Z * Z + W * W)));
    }
    
    /** 计算向量长度的平方（避免开方运算） */
    T SizeSquared() const
    {
        return X * X + Y * Y + Z * Z + W * W;
    }
    
    /** 计算两点之间的距离 */
    T Distance(const TVector4& Other) const
    {
        return (*this - Other).Size();
    }
    
    /** 计算两点之间距离的平方 */
    T DistanceSquared(const TVector4& Other) const
    {
        return (*this - Other).SizeSquared();
    }
    
    /** 安全归一化向量（如果长度太小则返回零向量） */
    TVector4 GetSafeNormal(T Tolerance = std::numeric_limits<T>::epsilon()) const
    {
        const T VectorSize = Size();
        if (VectorSize > Tolerance)
        {
            const T Scale = T(1) / VectorSize;
            return TVector4(X * Scale, Y * Scale, Z * Scale, W * Scale);
        }
        return TVector4(T(0), T(0), T(0), T(0));
    }
    
    /** 归一化向量（单位化，返回新向量） */
    TVector4 GetNormalized() const
    {
        const T VectorSize = Size();
        if (VectorSize > T(0))
        {
            const T Scale = T(1) / VectorSize;
            return TVector4(X * Scale, Y * Scale, Z * Scale, W * Scale);
        }
        return TVector4(T(0), T(0), T(0), T(0));
    }
    
    /** 归一化向量（原地修改） */
    void Normalize(T Tolerance = std::numeric_limits<T>::epsilon())
    {
        const T VectorSize = Size();
        if (VectorSize > Tolerance)
        {
            const T Scale = T(1) / VectorSize;
            X *= Scale;
            Y *= Scale;
            Z *= Scale;
            W *= Scale;
        }
        else
        {
            X = T(0);
            Y = T(0);
            Z = T(0);
            W = T(0);
        }
    }
    
    /** 点积（内积） */
    T Dot(const TVector4& Other) const
    {
        return X * Other.X + Y * Other.Y + Z * Other.Z + W * Other.W;
    }
    
    /** 检查向量是否为零向量 */
    bool IsZero(T Tolerance = std::numeric_limits<T>::epsilon()) const
    {
        return SizeSquared() < Tolerance * Tolerance;
    }
    
    /** 检查向量是否接近零（在误差范围内） */
    bool IsNearlyZero(T Tolerance = std::numeric_limits<T>::epsilon()) const
    {
        return IsZero(Tolerance);
    }
    
    /** 检查向量是否接近另一个向量（在误差范围内） */
    bool IsNearlyEqual(const TVector4& Other, T Tolerance = std::numeric_limits<T>::epsilon()) const
    {
        return DistanceSquared(Other) < Tolerance * Tolerance;
    }

    // ============================================================================
    // 分量访问
    // ============================================================================
    
    /** 获取指定索引的分量（0=X, 1=Y, 2=Z, 3=W） */
    T& operator[](int32_t Index)
    {
        return XYZW[Index];
    }
    
    /** 获取指定索引的分量（常量版本） */
    const T& operator[](int32_t Index) const
    {
        return XYZW[Index];
    }

    // ============================================================================
    // 静态辅助函数
    // ============================================================================
    
    /** 获取零向量 */
    static TVector4 ZeroVector()
    {
        return TVector4(T(0), T(0), T(0), T(0));
    }
    
    /** 获取单位向量 (1, 1, 1, 1) */
    static TVector4 OneVector()
    {
        return TVector4(T(1), T(1), T(1), T(1));
    }
    
    /** 获取 X 轴单位向量 (1, 0, 0, 0) */
    static TVector4 XAxisVector()
    {
        return TVector4(T(1), T(0), T(0), T(0));
    }
    
    /** 获取 Y 轴单位向量 (0, 1, 0, 0) */
    static TVector4 YAxisVector()
    {
        return TVector4(T(0), T(1), T(0), T(0));
    }
    
    /** 获取 Z 轴单位向量 (0, 0, 1, 0) */
    static TVector4 ZAxisVector()
    {
        return TVector4(T(0), T(0), T(1), T(0));
    }
    
    /** 获取 W 轴单位向量 (0, 0, 0, 1) */
    static TVector4 WAxisVector()
    {
        return TVector4(T(0), T(0), T(0), T(1));
    }
};

// 全局运算符（数乘，支持 T * TVector4<T>）
template<typename T>
inline TVector4<T> operator*(T Scalar, const TVector4<T>& Vector)
{
    return Vector * Scalar;
}
