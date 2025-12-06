#pragma once

#include <memory>
#include "Memory/SharedPtr.h"

/**
 * TWeakPtr - 弱指针
 * 包装std::weak_ptr，提供UE风格的接口
 * 用于打破循环引用，不增加引用计数
 * 
 * 使用示例：
 *   TSharedPtr<MyClass> SharedObj = MakeShared<MyClass>();
 *   TWeakPtr<MyClass> WeakObj = SharedObj;
 *   
 *   if (TSharedPtr<MyClass> Pinned = WeakObj.Pin())
 *   {
 *       Pinned->DoSomething();
 *   }
 */
template<typename T>
class TWeakPtr {
public:
    using ElementType = T;
    using PointerType = T*;

    /**
     * 默认构造函数
     */
    TWeakPtr() = default;

    /**
     * 从std::weak_ptr构造
     */
    explicit TWeakPtr(std::weak_ptr<T> InPtr)
        : Ptr(std::move(InPtr))
    {
    }

    /**
     * 从TSharedPtr构造
     */
    TWeakPtr(const TSharedPtr<T>& SharedPtr)
        : Ptr(SharedPtr.GetSharedPtr())
    {
    }

    /**
     * 从std::shared_ptr构造
     */
    explicit TWeakPtr(const std::shared_ptr<T>& SharedPtr)
        : Ptr(SharedPtr)
    {
    }

    /**
     * 拷贝构造函数
     */
    TWeakPtr(const TWeakPtr& Other) = default;

    /**
     * 移动构造函数
     */
    TWeakPtr(TWeakPtr&& Other) noexcept = default;

    /**
     * 拷贝赋值
     */
    TWeakPtr& operator=(const TWeakPtr& Other) = default;

    /**
     * 移动赋值
     */
    TWeakPtr& operator=(TWeakPtr&& Other) noexcept = default;

    /**
     * 从TSharedPtr赋值
     */
    TWeakPtr& operator=(const TSharedPtr<T>& SharedPtr)
    {
        Ptr = SharedPtr.GetSharedPtr();
        return *this;
    }

    /**
     * 从std::weak_ptr赋值
     */
    TWeakPtr& operator=(std::weak_ptr<T> InPtr)
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
     * 尝试获取TSharedPtr（如果对象仍然存在）
     * @return 如果对象存在返回TSharedPtr，否则返回空的TSharedPtr
     */
    TSharedPtr<T> Pin() const
    {
        return TSharedPtr<T>(Ptr.lock());
    }

    /**
     * 检查关联的对象是否已被销毁
     * @return 如果对象已被销毁返回true
     */
    bool IsExpired() const
    {
        return Ptr.expired();
    }

    /**
     * 检查关联的对象是否仍然存在
     * @return 如果对象仍然存在返回true
     */
    bool IsValid() const
    {
        return !Ptr.expired();
    }

    /**
     * 获取引用计数（可能不准确，因为对象可能在任何时候被销毁）
     * @return 引用计数，如果已过期返回0
     */
    long UseCount() const
    {
        return Ptr.use_count();
    }

    /**
     * 交换两个指针
     */
    void Swap(TWeakPtr& Other)
    {
        Ptr.swap(Other.Ptr);
    }

    /**
     * 获取底层的std::weak_ptr
     * @return std::weak_ptr引用
     */
    std::weak_ptr<T>& GetWeakPtr()
    {
        return Ptr;
    }

    /**
     * 获取底层的std::weak_ptr（const版本）
     * @return const std::weak_ptr引用
     */
    const std::weak_ptr<T>& GetWeakPtr() const
    {
        return Ptr;
    }

    /**
     * 比较操作符
     */
    bool operator==(const TWeakPtr& Other) const
    {
        return !Ptr.owner_before(Other.Ptr) && !Other.Ptr.owner_before(Ptr);
    }

    bool operator!=(const TWeakPtr& Other) const
    {
        return !(*this == Other);
    }

private:
    std::weak_ptr<T> Ptr;
};

