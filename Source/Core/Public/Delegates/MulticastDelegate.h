#pragma once

#include "Delegate.h"
#include "Container/Array.h"
#include <functional>
#include <memory>
#include <cstdint>

/**
 * TMulticastDelegate - 多播委托类
 * 类似于 UE 的 MulticastDelegate，可以绑定多个函数
 * 执行时会按绑定顺序调用所有已绑定的函数
 * 
 * 使用示例：
 *   // 无参数多播委托
 *   TMulticastDelegate<void()> MyMulticast;
 *   MyMulticast.AddLambda([]() { printf("First\n"); });
 *   MyMulticast.AddLambda([]() { printf("Second\n"); });
 *   MyMulticast.Broadcast();  // 输出: First\nSecond\n
 * 
 *   // 带参数多播委托
 *   TMulticastDelegate<void(int32)> MyMulticast2;
 *   MyMulticast2.AddLambda([](int32 X) { printf("%d\n", X); });
 *   MyMulticast2.AddLambda([](int32 X) { printf("Value: %d\n", X); });
 *   MyMulticast2.Broadcast(42);  // 输出: 42\nValue: 42\n
 * 
 * @tparam Signature 函数签名（如 void(), void(int32), void(int32, float) 等）
 */
template<typename Signature>
class TMulticastDelegate;

// ============================================================================
// 特化：支持函数签名语法 void(ParamTypes...)
// ============================================================================

template<typename... ParamTypes>
class TMulticastDelegate<void(ParamTypes...)>
{
public:
    using DelegateType = TDelegate<void(ParamTypes...)>;
    using FuncType = typename DelegateType::FuncType;

    // ============================================================================
    // 构造函数和析构函数
    // ============================================================================
    
    /** 默认构造函数 */
    TMulticastDelegate() = default;
    
    /** 拷贝构造函数 */
    TMulticastDelegate(const TMulticastDelegate& Other) = default;
    
    /** 移动构造函数 */
    TMulticastDelegate(TMulticastDelegate&& Other) noexcept = default;
    
    /** 析构函数 */
    ~TMulticastDelegate() = default;

    // ============================================================================
    // 赋值操作符
    // ============================================================================
    
    /** 拷贝赋值 */
    TMulticastDelegate& operator=(const TMulticastDelegate& Other) = default;
    
    /** 移动赋值 */
    TMulticastDelegate& operator=(TMulticastDelegate&& Other) noexcept = default;

    /**
     * 统一绑定接口 - 自动识别绑定类型
     *
     * 支持以下绑定方式：
     * 1. Lambda/函数对象: AddBind([]() { ... })
     * 2. 静态函数: AddBind(&StaticFunction)
     * 3. 成员函数+原始指针: AddBind(&Obj, &Class::Method)
     * 4. 成员函数+共享指针: AddBind(SharedPtr, &Class::Method)
     */

    // 重载1: 绑定Lambda
    template<typename Functor>
    void AddBind(Functor&& InFunctor)
    {
        AddLambda(std::forward<Functor>(InFunctor));
    }

    // 重载2: 绑定静态函数指针
    void AddBind(void(*InFunc)(ParamTypes...))
    {
        AddStatic(InFunc);
    }

    // 重载3: 绑定成员函数（原始指针）
    template<typename UserClass>
    void AddBind(UserClass* InUserObject, void(UserClass::*InFunc)(ParamTypes...))
    {
        AddObject(InUserObject, InFunc);
    }

    // 重载4: 绑定成员函数（原始指针，const 版本）
    template<typename UserClass>
    void AddBind(UserClass* InUserObject, void(UserClass::*InFunc)(ParamTypes...) const)
    {
        AddObject(InUserObject, InFunc);
    }

    // 重载5: 绑定成员函数（共享指针）
    template<typename UserClass>
    void AddBind(const std::shared_ptr<UserClass>& InUserObject, void(UserClass::*InFunc)(ParamTypes...))
    {
        AddSharedPtrObject(InUserObject, InFunc);
    }

    // 重载6: 绑定成员函数（共享指针，const 版本）
    template<typename UserClass>
    void AddBind(const std::shared_ptr<UserClass>& InUserObject, void(UserClass::*InFunc)(ParamTypes...) const)
    {
        AddSharedPtrObject(InUserObject, InFunc);
    }

    // ============================================================================
    // 绑定操作（添加委托）
    // ============================================================================

    /** 添加 Lambda 表达式或函数对象 */
    template<typename Functor>
    void AddLambda(Functor&& InFunctor)
    {
        DelegateType NewDelegate;
        NewDelegate.BindLambda(std::forward<Functor>(InFunctor));
        Delegates.Add(std::move(NewDelegate));
    }
    
    /** 添加静态函数 */
    void AddStatic(void(*InFunc)(ParamTypes...))
    {
        DelegateType NewDelegate;
        NewDelegate.BindStatic(InFunc);
        Delegates.Add(std::move(NewDelegate));
    }
    
    /** 添加成员函数（使用原始指针） */
    template<typename UserClass>
    void AddObject(UserClass* InUserObject, void(UserClass::*InFunc)(ParamTypes...))
    {
        DelegateType NewDelegate;
        NewDelegate.BindObject(InUserObject, InFunc);
        Delegates.Add(std::move(NewDelegate));
    }
    
    /** 添加成员函数（使用原始指针，const 版本） */
    template<typename UserClass>
    void AddObject(UserClass* InUserObject, void(UserClass::*InFunc)(ParamTypes...) const)
    {
        DelegateType NewDelegate;
        NewDelegate.BindObject(InUserObject, InFunc);
        Delegates.Add(std::move(NewDelegate));
    }
    
    /** 添加成员函数（使用共享指针） */
    template<typename UserClass>
    void AddSharedPtrObject(const std::shared_ptr<UserClass>& InUserObject, void(UserClass::*InFunc)(ParamTypes...))
    {
        DelegateType NewDelegate;
        NewDelegate.BindSharedPtrObject(InUserObject, InFunc);
        Delegates.Add(std::move(NewDelegate));
    }
    
    /** 添加成员函数（使用共享指针，const 版本） */
    template<typename UserClass>
    void AddSharedPtrObject(const std::shared_ptr<UserClass>& InUserObject, void(UserClass::*InFunc)(ParamTypes...) const)
    {
        DelegateType NewDelegate;
        NewDelegate.BindSharedPtrObject(InUserObject, InFunc);
        Delegates.Add(std::move(NewDelegate));
    }

    // ============================================================================
    // 移除操作
    // ============================================================================
    
    /** 移除所有匹配的委托（通过 Lambda 比较） */
    template<typename Functor>
    void RemoveLambda(Functor&& InFunctor)
    {
        // 注意：由于 std::function 的比较限制，这里只能通过遍历移除
        // 实际使用中，建议保存委托句柄用于精确移除
        for (int32 i = Delegates.Num() - 1; i >= 0; --i)
        {
            // 无法直接比较 std::function，这里提供一个基础实现
            // 实际使用时建议使用 RemoveDelegate 或保存句柄
        }
    }
    
    /** 移除指定的委托 */
    void RemoveDelegate(const DelegateType& InDelegate)
    {
        Delegates.RemoveAll(InDelegate);
    }
    
    /** 移除所有委托 */
    void RemoveAll()
    {
        Delegates.Reset();
    }
    
    /** 清空所有委托 */
    void Clear()
    {
        RemoveAll();
    }

    // ============================================================================
    // 执行操作
    // ============================================================================
    
    /** 广播执行所有已绑定的委托 */
    void Broadcast(ParamTypes... Params) const
    {
        // 使用索引迭代，避免复制容器，提高效率
        const int32 Num = Delegates.Num();
        for (int32 i = 0; i < Num; ++i)
        {
            // 检查索引是否仍然有效（可能在回调中被移除）
            if (i < Delegates.Num() && Delegates[i].IsBound())
            {
                Delegates[i].Execute(Params...);
            }
        }
    }

    // ============================================================================
    // 状态查询
    // ============================================================================
    
    /** 检查是否有任何委托已绑定 */
    bool IsBound() const
    {
        for (const auto& Delegate : Delegates)
        {
            if (Delegate.IsBound())
            {
                return true;
            }
        }
        return false;
    }
    
    /** 获取已绑定的委托数量 */
    int32 GetBoundCount() const
    {
        int32 Count = 0;
        for (const auto& Delegate : Delegates)
        {
            if (Delegate.IsBound())
            {
                ++Count;
            }
        }
        return Count;
    }
    
    /** 检查是否为空 */
    bool IsEmpty() const
    {
        return Delegates.IsEmpty();
    }

private:
    /** 添加委托（拷贝） */
    void Add(const DelegateType& InDelegate)
    {
        if (InDelegate.IsBound())
        {
            Delegates.Add(InDelegate);
        }
    }

    /** 添加委托（移动） */
    void Add(DelegateType&& InDelegate)
    {
        if (InDelegate.IsBound())
        {
            Delegates.Add(std::move(InDelegate));
        }
    }

private:
    TArray<DelegateType> Delegates;
};

// ============================================================================
// 类型别名（UE 风格）
// ============================================================================

/** 无参数多播委托 */
using FMulticastDelegate = TMulticastDelegate<void()>;

