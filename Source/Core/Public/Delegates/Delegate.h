#pragma once

#include <functional>
#include <memory>
#include <cstdint>
#include <tuple>

/**
 * TDelegate - 单播委托类
 * 类似于 UE 的 Delegate，只能绑定一个函数
 * 支持静态函数、成员函数和 Lambda 表达式
 *
 * 使用示例：
 *   // 无参数委托 - 使用统一的 Bind 接口
 *   TDelegate<void()> MyDelegate;
 *   MyDelegate.Bind([]() { printf("Hello\n"); });
 *   MyDelegate.Execute();
 * 
 *   // 带参数委托
 *   TDelegate<void(int32, float)> MyDelegate2;
 *   MyDelegate2.Bind([](int32 X, float Y) { printf("%d %f\n", X, Y); });
 *   MyDelegate2.Execute(10, 3.14f);
 * 
 *   // 有返回值委托
 *   TDelegate<int32(int32)> ReturnDelegate;
 *   ReturnDelegate.Bind([](int32 X) -> int32 { return X * 2; });
 *   int32 Result = ReturnDelegate.Execute(21);  // Result = 42
 *   // 如果未绑定，Execute 返回 RetType{}（对于 int32 是 0）
 * 
 *   // 静态函数绑定
 *   void StaticFunc(int32 Value) { printf("Static: %d\n", Value); }
 *   TDelegate<void(int32)> StaticDelegate;
 *   StaticDelegate.Bind(&StaticFunc);
 *   StaticDelegate.Execute(100);
 * 
 *   // 成员函数绑定（原始指针）
 *   class MyClass {
 *   public:
 *       void MyFunction(int32 Value) { printf("Value: %d\n", Value); }
 *   };
 *   MyClass Obj;
 *   TDelegate<void(int32)> Delegate;
 *   Delegate.Bind(&Obj, &MyClass::MyFunction);  // 统一接口
 *   Delegate.Broadcast(42);
 * 
 *   // 成员函数绑定（共享指针）
 *   auto SharedObj = std::make_shared<MyClass>();
 *   TDelegate<void(int32)> SafeDelegate;
 *   SafeDelegate.Bind(SharedObj, &MyClass::MyFunction);  // 统一接口
 *   SafeDelegate.Broadcast(42);
 * 
 * 注意：为了兼容性，原有的 BindLambda、BindStatic、BindObject、BindSharedPtrObject 函数仍然可用
 * 
 * @tparam Signature 函数签名（如 void(), void(int32), int32(int32) 等）
 */
template<typename Signature>
class TDelegate;

// ============================================================================
// 特化：支持函数签名语法 void(ParamTypes...)
// ============================================================================

template<typename... ParamTypes>
class TDelegate<void(ParamTypes...)>
{
public:
    using FuncType = std::function<void(ParamTypes...)>;

    // ============================================================================
    // 构造函数和析构函数
    // ============================================================================
    
    /** 默认构造函数 */
    TDelegate() = default;
    
    /** 拷贝构造函数 */
    TDelegate(const TDelegate& Other) = default;
    
    /** 移动构造函数 */
    TDelegate(TDelegate&& Other) noexcept = default;
    
    /** 析构函数 */
    ~TDelegate() = default;

    // ============================================================================
    // 赋值操作符
    // ============================================================================
    
    /** 拷贝赋值 */
    TDelegate& operator=(const TDelegate& Other) = default;
    
    /** 移动赋值 */
    TDelegate& operator=(TDelegate&& Other) noexcept = default;

    // ============================================================================
    // 绑定操作（统一接口）
    // ============================================================================
    
    /**
     * 统一绑定接口 - 自动识别绑定类型
     * 
     * 支持以下绑定方式：
     * 1. Lambda/函数对象: Bind([]() { ... })
     * 2. 静态函数: Bind(&StaticFunction)
     * 3. 成员函数+原始指针: Bind(&Obj, &Class::Method)
     * 4. 成员函数+共享指针: Bind(SharedPtr, &Class::Method)
     */
    
    // 重载1: 绑定Lambda
    template<typename Functor>
    void Bind(Functor&& InFunctor)
    {
        BindLambda(std::forward<Functor>(InFunctor));
    }

    // 重载2: 绑定静态函数指针
    void Bind(void(*InFunc)(ParamTypes...))
    {
        BindStatic(InFunc);
    }

    // 重载3: 绑定成员函数（原始指针）
    template<typename UserClass>
    void Bind(UserClass* InUserObject, void(UserClass::*InFunc)(ParamTypes...))
    {
        BindObject(InUserObject, InFunc);
    }
    
    // 重载4: 绑定成员函数（原始指针，const 版本）
    template<typename UserClass>
    void Bind(UserClass* InUserObject, void(UserClass::*InFunc)(ParamTypes...) const)
    {
        BindObject(InUserObject, InFunc);
    }
    
    // 重载5: 绑定成员函数（共享指针）
    template<typename UserClass>
    void Bind(const std::shared_ptr<UserClass>& InUserObject, void(UserClass::*InFunc)(ParamTypes...))
    {
        BindSharedPtrObject(InUserObject, InFunc);
    }
    
    // 重载6: 绑定成员函数（共享指针，const 版本）
    template<typename UserClass>
    void Bind(const std::shared_ptr<UserClass>& InUserObject, void(UserClass::*InFunc)(ParamTypes...) const)
    {
        BindSharedPtrObject(InUserObject, InFunc);
    }

    // ============================================================================
    // 绑定操作（添加委托）
    // ============================================================================
    
    /** 绑定 Lambda 表达式或函数对象 */
    template<typename Functor>
    void BindLambda(Functor&& InFunctor)
    {
        Func = std::forward<Functor>(InFunctor);
    }
    
    /** 绑定静态函数 */
    void BindStatic(void(*InFunc)(ParamTypes...))
    {
        Func = InFunc;
    }
    
    /** 绑定成员函数（使用原始指针，调用者需确保对象生命周期） */
    template<typename UserClass>
    void BindObject(UserClass* InUserObject, void(UserClass::*InFunc)(ParamTypes...))
    {
        Func = [InUserObject, InFunc](ParamTypes... Params) {
            (InUserObject->*InFunc)(Params...);
        };
    }
    
    /** 绑定成员函数（使用原始指针，const 版本） */
    template<typename UserClass>
    void BindObject(UserClass* InUserObject, void(UserClass::*InFunc)(ParamTypes...) const)
    {
        Func = [InUserObject, InFunc](ParamTypes... Params) {
            (InUserObject->*InFunc)(Params...);
        };
    }
    
    /** 绑定成员函数（使用共享指针，自动管理生命周期） */
    template<typename UserClass>
    void BindSharedPtrObject(const std::shared_ptr<UserClass>& InUserObject, void(UserClass::*InFunc)(ParamTypes...))
    {
        std::weak_ptr<UserClass> WeakPtr = InUserObject;
        Func = [WeakPtr, InFunc](ParamTypes... Params) {
            if (auto SharedPtr = WeakPtr.lock())
            {
                (SharedPtr.get()->*InFunc)(Params...);
            }
        };
    }
    
    /** 绑定成员函数（使用共享指针，const 版本）（兼容性接口） */
    template<typename UserClass>
    void BindSharedPtrObject(const std::shared_ptr<UserClass>& InUserObject, void(UserClass::*InFunc)(ParamTypes...) const)
    {
        std::weak_ptr<UserClass> WeakPtr = InUserObject;
        Func = [WeakPtr, InFunc](ParamTypes... Params) {
            if (auto SharedPtr = WeakPtr.lock())
            {
                (SharedPtr.get()->*InFunc)(Params...);
            }
        };
    }

    // ============================================================================
    // 执行操作
    // ============================================================================
    
    /** 执行委托（如果已绑定） */
    void Execute(ParamTypes... Params) const
    {
        if (IsBound())
        {
            Func(Params...);
        }
    }

    // ============================================================================
    // 状态查询
    // ============================================================================
    
    /** 检查委托是否已绑定 */
    bool IsBound() const
    {
        return Func != nullptr;
    }
    
    /** 解绑委托 */
    void Unbind()
    {
        Func = nullptr;
    }
    
    /** 清空委托（同 Unbind） */
    void Clear()
    {
        Unbind();
    }

private:
    FuncType Func;
};

// ============================================================================
// 特化：支持函数签名语法 RetType(ParamTypes...)
// ============================================================================

template<typename RetType, typename... ParamTypes>
class TDelegate<RetType(ParamTypes...)>
{
public:
    using FuncType = std::function<RetType(ParamTypes...)>;

    // ============================================================================
    // 构造函数和析构函数
    // ============================================================================
    
    /** 默认构造函数 */
    TDelegate() = default;
    
    /** 拷贝构造函数 */
    TDelegate(const TDelegate& Other) = default;
    
    /** 移动构造函数 */
    TDelegate(TDelegate&& Other) noexcept = default;
    
    /** 析构函数 */
    ~TDelegate() = default;

    // ============================================================================
    // 赋值操作符
    // ============================================================================
    
    /** 拷贝赋值 */
    TDelegate& operator=(const TDelegate& Other) = default;
    
    /** 移动赋值 */
    TDelegate& operator=(TDelegate&& Other) noexcept = default;

    // ============================================================================
    // 绑定操作（统一接口）
    // ============================================================================
    
    /**
     * 统一绑定接口 - 自动识别绑定类型
     * 
     * 支持以下绑定方式：
     * 1. Lambda/函数对象: Bind([]() { ... })
     * 2. 静态函数: Bind(&StaticFunction)
     * 3. 成员函数+原始指针: Bind(&Obj, &Class::Method)
     * 4. 成员函数+共享指针: Bind(SharedPtr, &Class::Method)
     */
    
    // 重载1: 绑定Lambda
    template<typename Functor>
    void Bind(Functor&& InFunctor)
    {
        BindLambda(std::forward<Functor>(InFunctor));
    }

    // 重载2: 绑定静态函数指针
    void Bind(RetType(*InFunc)(ParamTypes...))
    {
        BindStatic(InFunc);
    }

    // 重载3: 绑定成员函数（原始指针）
    template<typename UserClass>
    void Bind(UserClass* InUserObject, RetType(UserClass::*InFunc)(ParamTypes...))
    {
        BindObject(InUserObject, InFunc);
    }
    
    // 重载4: 绑定成员函数（原始指针，const 版本）
    template<typename UserClass>
    void Bind(UserClass* InUserObject, RetType(UserClass::*InFunc)(ParamTypes...) const)
    {
        BindObject(InUserObject, InFunc);
    }
    
    // 重载5: 绑定成员函数（共享指针）
    template<typename UserClass>
    void Bind(const std::shared_ptr<UserClass>& InUserObject, RetType(UserClass::*InFunc)(ParamTypes...))
    {
        BindSharedPtrObject(InUserObject, InFunc);
    }
    
    // 重载6: 绑定成员函数（共享指针，const 版本）
    template<typename UserClass>
    void Bind(const std::shared_ptr<UserClass>& InUserObject, RetType(UserClass::*InFunc)(ParamTypes...) const)
    {
        BindSharedPtrObject(InUserObject, InFunc);
    }

    // ============================================================================
    // 绑定操作（添加委托）
    // ============================================================================
    
    /** 绑定 Lambda 表达式或函数对象 */
    template<typename Functor>
    void BindLambda(Functor&& InFunctor)
    {
        Func = std::forward<Functor>(InFunctor);
    }
    
    /** 绑定静态函数 */
    void BindStatic(RetType(*InFunc)(ParamTypes...))
    {
        Func = InFunc;
    }
    
    /** 绑定成员函数（使用原始指针，调用者需确保对象生命周期） */
    template<typename UserClass>
    void BindObject(UserClass* InUserObject, RetType(UserClass::*InFunc)(ParamTypes...))
    {
        Func = [InUserObject, InFunc](ParamTypes... Params) -> RetType {
            return (InUserObject->*InFunc)(Params...);
        };
    }
    
    /** 绑定成员函数（使用原始指针，const 版本） */
    template<typename UserClass>
    void BindObject(UserClass* InUserObject, RetType(UserClass::*InFunc)(ParamTypes...) const)
    {
        Func = [InUserObject, InFunc](ParamTypes... Params) -> RetType {
            return (InUserObject->*InFunc)(Params...);
        };
    }
    
    /** 绑定成员函数（使用共享指针，自动管理生命周期） */
    template<typename UserClass>
    void BindSharedPtrObject(const std::shared_ptr<UserClass>& InUserObject, RetType(UserClass::*InFunc)(ParamTypes...))
    {
        std::weak_ptr<UserClass> WeakPtr = InUserObject;
        Func = [WeakPtr, InFunc](ParamTypes... Params) -> RetType {
            if (auto SharedPtr = WeakPtr.lock())
            {
                return (SharedPtr.get()->*InFunc)(Params...);
            }
            return RetType{};
        };
    }
    
    /** 绑定成员函数（使用共享指针，const 版本） */
    template<typename UserClass>
    void BindSharedPtrObject(const std::shared_ptr<UserClass>& InUserObject, RetType(UserClass::*InFunc)(ParamTypes...) const)
    {
        std::weak_ptr<UserClass> WeakPtr = InUserObject;
        Func = [WeakPtr, InFunc](ParamTypes... Params) -> RetType {
            if (auto SharedPtr = WeakPtr.lock())
            {
                return (SharedPtr.get()->*InFunc)(Params...);
            }
            return RetType{};
        };
    }

    // ============================================================================
    // 执行操作
    // ============================================================================
    
    /** 执行委托（如果已绑定），否则返回默认值 */
    RetType Execute(ParamTypes... Params) const
    {
        if (IsBound())
        {
            return Func(Params...);
        }
        return RetType{};
    }

    // ============================================================================
    // 状态查询
    // ============================================================================
    
    /** 检查委托是否已绑定 */
    bool IsBound() const
    {
        return Func != nullptr;
    }
    
    /** 解绑委托 */
    void Unbind()
    {
        Func = nullptr;
    }
    
    /** 清空委托（同 Unbind） */
    void Clear()
    {
        Unbind();
    }

private:
    FuncType Func;
};
