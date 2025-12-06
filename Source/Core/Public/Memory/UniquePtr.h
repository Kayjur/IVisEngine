#pragma once

#include <memory>
#include <type_traits>

/**
 * TUniquePtr - 独占指针
 * 包装std::unique_ptr，提供UE风格的接口
 * 
 * 使用示例：
 *   TUniquePtr<MyClass> UniqueObj = MakeUnique<MyClass>(Arg1, Arg2);
 *   if (UniqueObj.IsValid())
 *   {
 *       UniqueObj->DoSomething();
 *   }
 */
template<typename T>
class TUniquePtr {
public:
    using ElementType = T;
    using PointerType = T*;

    /**
     * 默认构造函数
     */
    TUniquePtr() = default;

    /**
     * 从std::unique_ptr构造
     */
    explicit TUniquePtr(std::unique_ptr<T> InPtr)
        : Ptr(std::move(InPtr))
    {
    }

    /**
     * 从原始指针构造（不推荐，使用MakeUnique代替）
     */
    explicit TUniquePtr(T* InPtr)
        : Ptr(InPtr)
    {
    }

    /**
     * 拷贝构造函数（删除，unique_ptr不可拷贝）
     */
    TUniquePtr(const TUniquePtr&) = delete;

    /**
     * 移动构造函数
     */
    TUniquePtr(TUniquePtr&& Other) noexcept = default;

    /**
     * 从其他类型的TUniquePtr移动构造（支持向上类型转换）
     * 允许从派生类智能指针隐式转换为基类智能指针
     * @tparam U 源类型，必须是T的派生类或可转换为T，且不等于T
     */
    template<typename U>
    TUniquePtr(TUniquePtr<U>&& Other,
               typename std::enable_if<std::is_convertible<U*, T*>::value && !std::is_same<U, T>::value>::type* = nullptr)
        : Ptr(std::move(Other.GetUniquePtr()))
    {
    }

    /**
     * 拷贝赋值（删除，unique_ptr不可拷贝）
     */
    TUniquePtr& operator=(const TUniquePtr&) = delete;

    /**
     * 移动赋值
     */
    TUniquePtr& operator=(TUniquePtr&& Other) noexcept = default;

    /**
     * 从其他类型的TUniquePtr移动赋值（支持向上类型转换）
     * @tparam U 源类型，必须是T的派生类或可转换为T，且不等于T
     */
    template<typename U>
    typename std::enable_if<std::is_convertible<U*, T*>::value && !std::is_same<U, T>::value, TUniquePtr&>::type
    operator=(TUniquePtr<U>&& Other)
    {
        Ptr = std::move(Other.GetUniquePtr());
        return *this;
    }

    /**
     * 从std::unique_ptr赋值
     */
    TUniquePtr& operator=(std::unique_ptr<T> InPtr)
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
     * 释放所有权并返回原始指针
     * 调用后，TUniquePtr将不再拥有该对象
     * @return 原始指针
     */
    T* Release()
    {
        return Ptr.release();
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
     * 交换两个指针
     */
    void Swap(TUniquePtr& Other)
    {
        Ptr.swap(Other.Ptr);
    }

    /**
     * 获取底层的std::unique_ptr
     * @return std::unique_ptr引用
     */
    std::unique_ptr<T>& GetUniquePtr()
    {
        return Ptr;
    }

    /**
     * 获取底层的std::unique_ptr（const版本）
     * @return const std::unique_ptr引用
     */
    const std::unique_ptr<T>& GetUniquePtr() const
    {
        return Ptr;
    }

    /**
     * 比较操作符
     */
    bool operator==(const TUniquePtr& Other) const
    {
        return Ptr == Other.Ptr;
    }

    bool operator!=(const TUniquePtr& Other) const
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
    std::unique_ptr<T> Ptr;
};

/**
 * MakeUnique - 创建TUniquePtr的辅助函数
 * 
 * 使用示例：
 *   auto UniqueObj = MakeUnique<MyClass>(Arg1, Arg2);
 */
template<typename T, typename... Args>
TUniquePtr<T> MakeUnique(Args&&... InArgs)
{
    return TUniquePtr<T>(std::make_unique<T>(std::forward<Args>(InArgs)...));
}

