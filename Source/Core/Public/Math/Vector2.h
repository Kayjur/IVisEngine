#pragma once

#include <cmath>
#include <cstdint>
#include <limits>

/**
 * TVector2 - 2D 向量类（模板类）
 * 使用 UE 风格的接口命名
 * 
 * @tparam T 数值类型（如 float, double, int 等）
 */
template<typename T>
class TVector2
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
        };
        /** Array access to X, Y components. */
        T XY[2];
    };

    // ============================================================================
    // 构造函数
    // ============================================================================
    
    /** 默认构造函数，初始化为零向量 */
    TVector2() : X(T(0)), Y(T(0)) {}
    
    /** 使用 X, Y 分量构造 */
    TVector2(T InX, T InY) : X(InX), Y(InY) {}
    
    /** 使用单个值构造（所有分量相同） */
    explicit TVector2(T Value) : X(Value), Y(Value) {}
    
    /**
     * 从其他类型的向量构造（隐式转换）
     * @tparam U 其他数值类型
     */
    template<typename U>
    TVector2(const TVector2<U>& Other) : X(static_cast<T>(Other.X)), Y(static_cast<T>(Other.Y)) {}

    /** 拷贝构造函数 */
    TVector2(const TVector2& Other) = default;
    
    /** 移动构造函数 */
    TVector2(TVector2&& Other) noexcept = default;
    
    /** 析构函数 */
    ~TVector2() = default;

    // ============================================================================
    // 赋值操作符
    // ============================================================================
    
    /** 拷贝赋值 */
    TVector2& operator=(const TVector2& Other) = default;
    
    /** 移动赋值 */
    TVector2& operator=(TVector2&& Other) noexcept = default;

    // ============================================================================
    // 算术运算
    // ============================================================================
    
    /** 向量加法 */
    TVector2 operator+(const TVector2& Other) const
    {
        return TVector2(X + Other.X, Y + Other.Y);
    }
    
    /** 向量减法 */
    TVector2 operator-(const TVector2& Other) const
    {
        return TVector2(X - Other.X, Y - Other.Y);
    }
    
    /** 向量数乘 */
    TVector2 operator*(T Scalar) const
    {
        return TVector2(X * Scalar, Y * Scalar);
    }
    
    /** 向量数除 */
    TVector2 operator/(T Scalar) const
    {
        T InvScalar = T(1) / Scalar;
        return TVector2(X * InvScalar, Y * InvScalar);
    }
    
    /** 向量取反 */
    TVector2 operator-() const
    {
        return TVector2(-X, -Y);
    }
    
    /** 向量加法赋值 */
    TVector2& operator+=(const TVector2& Other)
    {
        X += Other.X;
        Y += Other.Y;
        return *this;
    }
    
    /** 向量减法赋值 */
    TVector2& operator-=(const TVector2& Other)
    {
        X -= Other.X;
        Y -= Other.Y;
        return *this;
    }
    
    /** 向量数乘赋值 */
    TVector2& operator*=(T Scalar)
    {
        X *= Scalar;
        Y *= Scalar;
        return *this;
    }
    
    /** 向量数除赋值 */
    TVector2& operator/=(T Scalar)
    {
        T InvScalar = T(1) / Scalar;
        X *= InvScalar;
        Y *= InvScalar;
        return *this;
    }
    
    /** 向量相等比较 */
    bool operator==(const TVector2& Other) const
    {
        return X == Other.X && Y == Other.Y;
    }
    
    /** 向量不等比较 */
    bool operator!=(const TVector2& Other) const
    {
        return !(*this == Other);
    }

    // ============================================================================
    // 向量运算
    // ============================================================================
    
    /** 计算向量长度（模长） */
    T Size() const
    {
        return static_cast<T>(std::sqrt(static_cast<double>(X * X + Y * Y)));
    }
    
    /** 计算向量长度的平方（避免开方运算） */
    T SizeSquared() const
    {
        return X * X + Y * Y;
    }
    
    /** 计算两点之间的距离 */
    T Distance(const TVector2& Other) const
    {
        return (*this - Other).Size();
    }
    
    /** 计算两点之间距离的平方 */
    T DistanceSquared(const TVector2& Other) const
    {
        return (*this - Other).SizeSquared();
    }
    
    /** 安全归一化向量（如果长度太小则返回零向量） */
    TVector2 GetSafeNormal(T Tolerance = std::numeric_limits<T>::epsilon()) const
    {
        const T VectorSize = Size();
        if (VectorSize > Tolerance)
        {
            const T Scale = T(1) / VectorSize;
            return TVector2(X * Scale, Y * Scale);
        }
        return TVector2(T(0), T(0));
    }
    
    /** 归一化向量（单位化，返回新向量） */
    TVector2 GetNormalized() const
    {
        const T VectorSize = Size();
        if (VectorSize > T(0))
        {
            const T Scale = T(1) / VectorSize;
            return TVector2(X * Scale, Y * Scale);
        }
        return TVector2(T(0), T(0));
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
        }
        else
        {
            X = T(0);
            Y = T(0);
        }
    }
    
    /** 点积（内积） */
    T Dot(const TVector2& Other) const
    {
        return X * Other.X + Y * Other.Y;
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
    bool IsNearlyEqual(const TVector2& Other, T Tolerance = std::numeric_limits<T>::epsilon()) const
    {
        return DistanceSquared(Other) < Tolerance * Tolerance;
    }

    // ============================================================================
    // 分量访问
    // ============================================================================
    
    /** 获取指定索引的分量（0=X, 1=Y） */
    T& operator[](int32_t Index)
    {
        return XY[Index];
    }
    
    /** 获取指定索引的分量（常量版本） */
    const T& operator[](int32_t Index) const
    {
        return XY[Index];
    }

    // ============================================================================
    // 静态辅助函数
    // ============================================================================
    
    /** 获取零向量 */
    static TVector2 ZeroVector()
    {
        return TVector2(T(0), T(0));
    }
    
    /** 获取单位向量 (1, 1) */
    static TVector2 OneVector()
    {
        return TVector2(T(1), T(1));
    }
    
    /** 获取 X 轴单位向量 (1, 0) */
    static TVector2 XAxisVector()
    {
        return TVector2(T(1), T(0));
    }
    
    /** 获取 Y 轴单位向量 (0, 1) */
    static TVector2 YAxisVector()
    {
        return TVector2(T(0), T(1));
    }
};

// 全局运算符（数乘，支持 T * TVector2<T>）
template<typename T>
inline TVector2<T> operator*(T Scalar, const TVector2<T>& Vector)
{
    return Vector * Scalar;
}
