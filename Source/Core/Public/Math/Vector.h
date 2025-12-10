#pragma once

#include <cmath>
#include <limits>
#include "HAL/Platform.h"

/**
 * TVector - 3D 向量类（模板类）
 * 使用 UE 风格的接口命名
 * 
 * @tparam T 数值类型（如 float, double, int 等）
 */
template<typename T>
class TVector
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
        };
        /** Array access to X, Y, Z components. */
        T XYZ[3];
    };

    // ============================================================================
    // 构造函数
    // ============================================================================
    
    /** 默认构造函数，初始化为零向量 */
    TVector() : X(T(0)), Y(T(0)), Z(T(0)) {}
    
    /** 使用 X, Y, Z 分量构造 */
    TVector(T InX, T InY, T InZ) : X(InX), Y(InY), Z(InZ) {}
    
    /** 使用单个值构造（所有分量相同） */
    explicit TVector(T Value) : X(Value), Y(Value), Z(Value) {}
    
    /**
     * 从其他类型的向量构造（隐式转换）
     * @tparam U 其他数值类型
     */
    template<typename U>
    TVector(const TVector<U>& Other) : X(static_cast<T>(Other.X)), Y(static_cast<T>(Other.Y)), Z(static_cast<T>(Other.Z)) {}

    /** 拷贝构造函数 */
    TVector(const TVector& Other) = default;
    
    /** 移动构造函数 */
    TVector(TVector&& Other) noexcept = default;
    
    /** 析构函数 */
    ~TVector() = default;

    // ============================================================================
    // 赋值操作符
    // ============================================================================
    
    /** 拷贝赋值 */
    TVector& operator=(const TVector& Other) = default;
    
    /** 移动赋值 */
    TVector& operator=(TVector&& Other) noexcept = default;

    // ============================================================================
    // 算术运算
    // ============================================================================
    
    /** 向量加法 */
    TVector operator+(const TVector& Other) const
    {
        return TVector(X + Other.X, Y + Other.Y, Z + Other.Z);
    }
    
    /** 向量减法 */
    TVector operator-(const TVector& Other) const
    {
        return TVector(X - Other.X, Y - Other.Y, Z - Other.Z);
    }
    
    /** 向量数乘 */
    TVector operator*(T Scalar) const
    {
        return TVector(X * Scalar, Y * Scalar, Z * Scalar);
    }
    
    /** 向量数除 */
    TVector operator/(T Scalar) const
    {
        T InvScalar = T(1) / Scalar;
        return TVector(X * InvScalar, Y * InvScalar, Z * InvScalar);
    }
    
    /** 向量取反 */
    TVector operator-() const
    {
        return TVector(-X, -Y, -Z);
    }
    
    /** 向量加法赋值 */
    TVector& operator+=(const TVector& Other)
    {
        X += Other.X;
        Y += Other.Y;
        Z += Other.Z;
        return *this;
    }
    
    /** 向量减法赋值 */
    TVector& operator-=(const TVector& Other)
    {
        X -= Other.X;
        Y -= Other.Y;
        Z -= Other.Z;
        return *this;
    }
    
    /** 向量数乘赋值 */
    TVector& operator*=(T Scalar)
    {
        X *= Scalar;
        Y *= Scalar;
        Z *= Scalar;
        return *this;
    }
    
    /** 向量数除赋值 */
    TVector& operator/=(T Scalar)
    {
        T InvScalar = T(1) / Scalar;
        X *= InvScalar;
        Y *= InvScalar;
        Z *= InvScalar;
        return *this;
    }
    
    /** 向量相等比较 */
    bool operator==(const TVector& Other) const
    {
        return X == Other.X && Y == Other.Y && Z == Other.Z;
    }
    
    /** 向量不等比较 */
    bool operator!=(const TVector& Other) const
    {
        return !(*this == Other);
    }

    // ============================================================================
    // 向量运算
    // ============================================================================
    
    /** 计算向量长度（模长） */
    T Size() const
    {
        return static_cast<T>(std::sqrt(static_cast<double>(X * X + Y * Y + Z * Z)));
    }
    
    /** 计算向量长度的平方（避免开方运算） */
    T SizeSquared() const
    {
        return X * X + Y * Y + Z * Z;
    }
    
    /** 计算两点之间的距离 */
    T Distance(const TVector& Other) const
    {
        return (*this - Other).Size();
    }
    
    /** 计算两点之间距离的平方 */
    T DistanceSquared(const TVector& Other) const
    {
        return (*this - Other).SizeSquared();
    }
    
    /** 安全归一化向量（如果长度太小则返回零向量） */
    TVector GetSafeNormal(T Tolerance = std::numeric_limits<T>::epsilon()) const
    {
        const T VectorSize = Size();
        if (VectorSize > Tolerance)
        {
            const T Scale = T(1) / VectorSize;
            return TVector(X * Scale, Y * Scale, Z * Scale);
        }
        return TVector(T(0), T(0), T(0));
    }
    
    /** 归一化向量（单位化，返回新向量） */
    TVector GetNormalized() const
    {
        const T VectorSize = Size();
        if (VectorSize > T(0))
        {
            const T Scale = T(1) / VectorSize;
            return TVector(X * Scale, Y * Scale, Z * Scale);
        }
        return TVector(T(0), T(0), T(0));
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
        }
        else
        {
            X = T(0);
            Y = T(0);
            Z = T(0);
        }
    }
    
    /** 点积（内积） */
    T Dot(const TVector& Other) const
    {
        return X * Other.X + Y * Other.Y + Z * Other.Z;
    }
    
    /** 叉积（外积） */
    TVector Cross(const TVector& Other) const
    {
        return TVector(
            Y * Other.Z - Z * Other.Y,
            Z * Other.X - X * Other.Z,
            X * Other.Y - Y * Other.X
        );
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
    bool IsNearlyEqual(const TVector& Other, T Tolerance = std::numeric_limits<T>::epsilon()) const
    {
        return DistanceSquared(Other) < Tolerance * Tolerance;
    }

    // ============================================================================
    // 分量访问
    // ============================================================================
    
    /** 获取指定索引的分量（0=X, 1=Y, 2=Z） */
    T& operator[](int32 Index)
    {
        return XYZ[Index];
    }
    
    /** 获取指定索引的分量（常量版本） */
    const T& operator[](int32 Index) const
    {
        return XYZ[Index];
    }

    int Zero() const;

    // ============================================================================
    // 静态辅助函数
    // ============================================================================
    
    /** 获取零向量 */
    static TVector ZeroVector()
    {
        return TVector(T(0), T(0), T(0));
    }
    
    /** 获取单位向量 (1, 1, 1) */
    static TVector OneVector()
    {
        return TVector(T(1), T(1), T(1));
    }
    
    /** 获取 X 轴单位向量 (1, 0, 0) */
    static TVector XAxisVector()
    {
        return TVector(T(1), T(0), T(0));
    }
    
    /** 获取 Y 轴单位向量 (0, 1, 0) */
    static TVector YAxisVector()
    {
        return TVector(T(0), T(1), T(0));
    }
    
    /** 获取 Z 轴单位向量 (0, 0, 1) */
    static TVector ZAxisVector()
    {
        return TVector(T(0), T(0), T(1));
    }
    
    /** 获取向上向量 (0, 0, 1) */
    static TVector UpVector()
    {
        return TVector(T(0), T(0), T(1));
    }
    
    /** 获取向前向量 (1, 0, 0) */
    static TVector ForwardVector()
    {
        return TVector(T(1), T(0), T(0));
    }
    
    /** 获取向右向量 (0, 1, 0) */
    static TVector RightVector()
    {
        return TVector(T(0), T(1), T(0));
    }
};

// 全局运算符（数乘，支持 T * TVector<T>）
template<typename T>
inline TVector<T> operator*(T Scalar, const TVector<T>& Vector)
{
    return Vector * Scalar;
}
