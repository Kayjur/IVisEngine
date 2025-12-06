#pragma once

#include <memory>
#include <type_traits>

/**
 * TSharedPtr - 共享指针
 * 包装std::shared_ptr，提供UE风格的接口
 * 
 * 使用示例：
 *   TSharedPtr<MyClass> SharedObj = MakeShared<MyClass>(Arg1, Arg2);
 *   if (SharedObj.IsValid())
 *   {
 *       SharedObj->DoSomething();
 *   }
 */
template<typename T>
class TSharedPtr {
public:
    using ElementType = T;
    using PointerType = T*;

    /**
     * 默认构造函数
     */
    TSharedPtr() = default;

    /**
     * 从std::shared_ptr构造
     */
    explicit TSharedPtr(std::shared_ptr<T> InPtr)
        : Ptr(std::move(InPtr))
    {
    }

    /**
     * 从原始指针构造（不推荐，使用MakeShared代替）
     */
    explicit TSharedPtr(T* InPtr)
        : Ptr(InPtr)
    {
    }

    /**
     * 拷贝构造函数
     */
    TSharedPtr(const TSharedPtr& Other) = default;

    /**
     * 移动构造函数
     */
    TSharedPtr(TSharedPtr&& Other) noexcept = default;

    /**
     * 从其他类型的TSharedPtr构造（支持向上类型转换）
     * 允许从派生类智能指针隐式转换为基类智能指针
     * @tparam U 源类型，必须是T的派生类或可转换为T，且不等于T
     */
    template<typename U>
    TSharedPtr(const TSharedPtr<U>& Other,
               typename std::enable_if<std::is_convertible<U*, T*>::value && !std::is_same<U, T>::value>::type* = nullptr)
        : Ptr(Other.GetSharedPtr())
    {
    }

    /**
     * 从其他类型的TSharedPtr移动构造（支持向上类型转换）
     * @tparam U 源类型，必须是T的派生类或可转换为T，且不等于T
     */
    template<typename U>
    TSharedPtr(TSharedPtr<U>&& Other,
               typename std::enable_if<std::is_convertible<U*, T*>::value && !std::is_same<U, T>::value>::type* = nullptr)
        : Ptr(std::move(Other.GetSharedPtr()))
    {
    }

    /**
     * 拷贝赋值
     */
    TSharedPtr& operator=(const TSharedPtr& Other) = default;

    /**
     * 移动赋值
     */
    TSharedPtr& operator=(TSharedPtr&& Other) noexcept = default;

    /**
     * 从其他类型的TSharedPtr赋值（支持向上类型转换）
     * @tparam U 源类型，必须是T的派生类或可转换为T，且不等于T
     */
    template<typename U>
    typename std::enable_if<std::is_convertible<U*, T*>::value && !std::is_same<U, T>::value, TSharedPtr&>::type
    operator=(const TSharedPtr<U>& Other)
    {
        Ptr = Other.GetSharedPtr();
        return *this;
    }

    /**
     * 从其他类型的TSharedPtr移动赋值（支持向上类型转换）
     * @tparam U 源类型，必须是T的派生类或可转换为T，且不等于T
     */
    template<typename U>
    typename std::enable_if<std::is_convertible<U*, T*>::value && !std::is_same<U, T>::value, TSharedPtr&>::type
    operator=(TSharedPtr<U>&& Other)
    {
        Ptr = std::move(Other.GetSharedPtr());
        return *this;
    }

    /**
     * 从std::shared_ptr赋值
     */
    TSharedPtr& operator=(std::shared_ptr<T> InPtr)
    {
        Ptr = std::move(InPtr);
        return *this;
    }

    /**
     * 重置指针
     */
    void Reset()
    {
        Ptr.reset();
    }

    /**
     * 重置为新的指针
     */
    void Reset(T* InPtr)
    {
        Ptr.reset(InPtr);
    }

    /**
     * 获取原始指针
     * @return 原始指针，如果为空则返回nullptr
     */
    T* Get() const
    {
        return Ptr.get();
    }

    /**
     * 解引用操作符
     * @return 对象的引用
     */
    T& operator*() const
    {
        return *Ptr;
    }

    /**
     * 成员访问操作符
     * @return 原始指针
     */
    T* operator->() const
    {
        return Ptr.get();
    }

    /**
     * 布尔转换（检查是否有效）
     * @return 如果指针有效返回true
     */
    explicit operator bool() const
    {
        return Ptr != nullptr;
    }

    /**
     * 检查指针是否有效
     * @return 如果指针有效返回true
     */
    bool IsValid() const
    {
        return Ptr != nullptr;
    }

    /**
     * 获取引用计数
     * @return 引用计数，如果指针为空返回0
     */
    long UseCount() const
    {
        return Ptr.use_count();
    }

    /**
     * 检查是否唯一拥有
     * @return 如果引用计数为1返回true
     */
    bool IsUnique() const
    {
        return Ptr.use_count() == 1;
    }

    /**
     * 交换两个指针
     */
    void Swap(TSharedPtr& Other)
    {
        Ptr.swap(Other.Ptr);
    }

    /**
     * 获取底层的std::shared_ptr
     * @return std::shared_ptr引用
     */
    std::shared_ptr<T>& GetSharedPtr()
    {
        return Ptr;
    }

    /**
     * 获取底层的std::shared_ptr（const版本）
     * @return const std::shared_ptr引用
     */
    const std::shared_ptr<T>& GetSharedPtr() const
    {
        return Ptr;
    }

    /**
     * 比较操作符
     */
    bool operator==(const TSharedPtr& Other) const
    {
        return Ptr == Other.Ptr;
    }

    bool operator!=(const TSharedPtr& Other) const
    {
        return Ptr != Other.Ptr;
    }

    bool operator==(std::nullptr_t) const
    {
        return Ptr == nullptr;
    }

    bool operator!=(std::nullptr_t) const
    {
        return Ptr != nullptr;
    }

private:
    std::shared_ptr<T> Ptr;
};

/**
 * MakeShared - 创建TSharedPtr的辅助函数
 * 
 * 使用示例：
 *   auto SharedObj = MakeShared<MyClass>(Arg1, Arg2);
 */
template<typename T, typename... Args>
TSharedPtr<T> MakeShared(Args&&... InArgs)
{
    return TSharedPtr<T>(std::make_shared<T>(std::forward<Args>(InArgs)...));
}

/**
 * StaticCastSharedPtr - 静态转换TSharedPtr
 */
template<typename To, typename From>
TSharedPtr<To> StaticCastSharedPtr(const TSharedPtr<From>& FromPtr)
{
    return TSharedPtr<To>(std::static_pointer_cast<To>(FromPtr.GetSharedPtr()));
}

/**
 * DynamicCastSharedPtr - 动态转换TSharedPtr
 */
template<typename To, typename From>
TSharedPtr<To> DynamicCastSharedPtr(const TSharedPtr<From>& FromPtr)
{
    return TSharedPtr<To>(std::dynamic_pointer_cast<To>(FromPtr.GetSharedPtr()));
}

/**
 * ConstCastSharedPtr - const转换TSharedPtr
 */
template<typename To, typename From>
TSharedPtr<To> ConstCastSharedPtr(const TSharedPtr<From>& FromPtr)
{
    return TSharedPtr<To>(std::const_pointer_cast<To>(FromPtr.GetSharedPtr()));
}

