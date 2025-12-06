#pragma once

#include "MulticastDelegate.h"

/**
 * TEvent - 事件类
 * 类似于 UE 的 Event，基于多播委托，但只有声明类可以广播
 * 其他类只能订阅（Add）和取消订阅（Remove）
 * 
 * 使用示例：
 *   // 在类中声明事件
 *   class MyClass {
 *   public:
 *       // 声明事件类型
 *       using FOnValueChanged = TEvent<void(int32)>;
 *       
 *       // 声明事件实例
 *       FOnValueChanged OnValueChanged;
 *       
 *       void SetValue(int32 NewValue) {
 *           Value = NewValue;
 *           // 只有 MyClass 可以广播
 *           OnValueChanged.Broadcast(NewValue);
 *       }
 *   
 *   private:
 *       int32 Value = 0;
 *   };
 * 
 *   // 在其他类中订阅事件
 *   class Observer {
 *   public:
 *       void OnValueChangedHandler(int32 NewValue) {
 *           printf("Value changed to: %d\n", NewValue);
 *       }
 *   };
 * 
 *   MyClass MyObj;
 *   Observer Obs;
 *   MyObj.OnValueChanged.AddRaw(&Obs, &Observer::OnValueChangedHandler);
 *   MyObj.SetValue(42);  // 触发事件，输出: Value changed to: 42
 * 
 * @tparam Signature 函数签名（如 void(), void(int32), void(int32, float) 等）
 */
template<typename Signature>
class TEvent;

// ============================================================================
// 特化：支持函数签名语法 void(ParamTypes...)
// ============================================================================

template<typename... ParamTypes>
class TEvent<void(ParamTypes...)>
{
public:
    using MulticastDelegateType = TMulticastDelegate<void(ParamTypes...)>;
    using DelegateType = typename MulticastDelegateType::DelegateType;

    // ============================================================================
    // 构造函数和析构函数
    // ============================================================================
    
    /** 默认构造函数 */
    TEvent() = default;
    
    /** 拷贝构造函数 */
    TEvent(const TEvent& Other) = default;
    
    /** 移动构造函数 */
    TEvent(TEvent&& Other) noexcept = default;
    
    /** 析构函数 */
    ~TEvent() = default;

    // ============================================================================
    // 赋值操作符
    // ============================================================================
    
    /** 拷贝赋值 */
    TEvent& operator=(const TEvent& Other) = default;
    
    /** 移动赋值 */
    TEvent& operator=(TEvent&& Other) noexcept = default;

    // ============================================================================
    // 公共接口：订阅操作（所有类都可以使用）
    // ============================================================================
    
    /** 添加 Lambda 表达式或函数对象 */
    template<typename Functor>
    void AddLambda(Functor&& InFunctor)
    {
        MulticastDelegate.AddLambda(std::forward<Functor>(InFunctor));
    }
    
    /** 添加静态函数 */
    void AddStatic(void(*InFunc)(ParamTypes...))
    {
        MulticastDelegate.AddStatic(InFunc);
    }
    
    /** 添加成员函数（使用原始指针） */
    template<typename UserClass>
    void AddRaw(UserClass* InUserObject, void(UserClass::*InFunc)(ParamTypes...))
    {
        MulticastDelegate.AddRaw(InUserObject, InFunc);
    }
    
    /** 添加成员函数（使用原始指针，const 版本） */
    template<typename UserClass>
    void AddRaw(UserClass* InUserObject, void(UserClass::*InFunc)(ParamTypes...) const)
    {
        MulticastDelegate.AddRaw(InUserObject, InFunc);
    }
    
    /** 添加成员函数（使用共享指针） */
    template<typename UserClass>
    void AddSP(const std::shared_ptr<UserClass>& InUserObject, void(UserClass::*InFunc)(ParamTypes...))
    {
        MulticastDelegate.AddSP(InUserObject, InFunc);
    }
    
    /** 添加成员函数（使用共享指针，const 版本） */
    template<typename UserClass>
    void AddSP(const std::shared_ptr<UserClass>& InUserObject, void(UserClass::*InFunc)(ParamTypes...) const)
    {
        MulticastDelegate.AddSP(InUserObject, InFunc);
    }
    
    /** 添加委托 */
    void Add(const DelegateType& InDelegate)
    {
        MulticastDelegate.Add(InDelegate);
    }
    
    /** 添加委托（移动） */
    void Add(DelegateType&& InDelegate)
    {
        MulticastDelegate.Add(std::move(InDelegate));
    }

    // ============================================================================
    // 公共接口：取消订阅操作（所有类都可以使用）
    // ============================================================================
    
    /** 移除指定的委托 */
    void Remove(const DelegateType& InDelegate)
    {
        MulticastDelegate.RemoveDelegate(InDelegate);
    }
    
    /** 移除所有委托 */
    void RemoveAll()
    {
        MulticastDelegate.RemoveAll();
    }
    
    /** 清空所有委托 */
    void Clear()
    {
        MulticastDelegate.Clear();
    }

    // ============================================================================
    // 公共接口：状态查询（所有类都可以使用）
    // ============================================================================
    
    /** 检查是否有任何委托已绑定 */
    bool IsBound() const
    {
        return MulticastDelegate.IsBound();
    }
    
    /** 获取已绑定的委托数量 */
    int32 GetBoundCount() const
    {
        return MulticastDelegate.GetBoundCount();
    }
    
    /** 检查是否为空 */
    bool IsEmpty() const
    {
        return MulticastDelegate.IsEmpty();
    }

    // ============================================================================
    // 受保护接口：广播操作（只有友元类或声明类可以使用）
    // ============================================================================
    
    /** 广播执行所有已绑定的委托 */
    void Broadcast(ParamTypes... Params) const
    {
        MulticastDelegate.Broadcast(Params...);
    }
    
    /** 广播执行所有已绑定的委托（如果至少有一个已绑定） */
    void BroadcastIfBound(ParamTypes... Params) const
    {
        MulticastDelegate.BroadcastIfBound(Params...);
    }

    // ============================================================================
    // 友元类模板（允许特定类广播事件）
    // ============================================================================
    
    /** 声明友元类，允许该类广播此事件 */
    template<typename FriendClass>
    friend class TEventFriend;

private:
    MulticastDelegateType MulticastDelegate;
};

// ============================================================================
// 事件友元类（用于允许特定类广播事件）
// ============================================================================

/**
 * TEventFriend - 事件友元类
 * 用于在类中声明对特定事件的广播权限
 * 
 * 使用示例：
 *   class MyClass {
 *   public:
 *       using FOnValueChanged = TEvent<void(int32)>;
 *       FOnValueChanged OnValueChanged;
 *       
 *       void SetValue(int32 NewValue) {
 *           Value = NewValue;
 *           // 通过友元类广播
 *           TEventFriend<MyClass>::Broadcast(OnValueChanged, NewValue);
 *       }
 *   
 *   private:
 *       int32 Value = 0;
 *   };
 */
template<typename OwnerClass>
class TEventFriend
{
public:
    /** 广播事件（只有 OwnerClass 可以调用） */
    template<typename... ParamTypes>
    static void Broadcast(const TEvent<void(ParamTypes...)>& InEvent, ParamTypes... Params)
    {
        InEvent.Broadcast(Params...);
    }
    
    /** 广播事件（如果已绑定） */
    template<typename... ParamTypes>
    static void BroadcastIfBound(const TEvent<void(ParamTypes...)>& InEvent, ParamTypes... Params)
    {
        InEvent.BroadcastIfBound(Params...);
    }
};

// ============================================================================
// 更简单的使用方式：在类中直接使用 Broadcast 方法
// ============================================================================

/**
 * 注意：由于 C++ 的限制，无法完全阻止外部类调用 Broadcast
 * 但可以通过以下方式使用：
 * 
 * 方式1：直接在类中调用 Broadcast（推荐）
 *   class MyClass {
 *   public:
 *       TEvent<void(int32)> OnValueChanged;
 *       
 *       void SetValue(int32 NewValue) {
 *           Value = NewValue;
 *           OnValueChanged.Broadcast(NewValue);  // 直接调用
 *       }
 *   };
 * 
 * 方式2：使用私有成员 + 公共访问器
 *   class MyClass {
 *   private:
 *       TEvent<void(int32)> OnValueChanged;
 *   
 *   public:
 *       // 只暴露订阅接口
 *       void SubscribeToValueChanged(...) {
 *           OnValueChanged.AddLambda(...);
 *       }
 *       
 *       void SetValue(int32 NewValue) {
 *           Value = NewValue;
 *           OnValueChanged.Broadcast(NewValue);  // 类内部可以调用
 *       }
 *   };
 */

// ============================================================================
// 类型别名（UE 风格）
// ============================================================================

/** 无参数事件 */
using FEvent = TEvent<void()>;

