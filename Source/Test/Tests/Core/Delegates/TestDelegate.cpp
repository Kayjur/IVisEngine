#include "TestFramework.h"
#include "Delegates/DelegateMacros.h"
#include <memory>

TEST_GROUP(TestDelegate)

namespace {
    int32 GlobalValue = 0;
    int32 CallCount = 0;

    // 无返回值委托声明
    DECLARE_DELEGATE(FOnDelegateNoParams);
    DECLARE_DELEGATE_OneParam(FOnDelegateOneParams, int32);
    DECLARE_DELEGATE_TwoParams(FOnDelegateTwoParams, int32, int32);
    DECLARE_DELEGATE_ThreeParams(FOnDelegateThreeParams, int32, int32, int32);
    DECLARE_DELEGATE_FourParams(FOnDelegateFourParams, int32, int32, int32, int32);

    // 有返回值委托声明
    DECLARE_DELEGATE_RetVal(int32, FOnDelegateRetVal);
    DECLARE_DELEGATE_RetVal_OneParam(int32, FOnDelegateRetValOneParam, int32);
    DECLARE_DELEGATE_RetVal_TwoParams(int32, FOnDelegateRetValTwoParams, int32, int32);

    class IDelegateProxy {
    public:
        FOnDelegateNoParams OnDelegateNoParams;
        FOnDelegateOneParams OnDelegateOneParams;
        FOnDelegateTwoParams OnDelegateTwoParams;
        FOnDelegateThreeParams OnDelegateThreeParams;
        FOnDelegateFourParams OnDelegateFourParams;
        
        FOnDelegateRetVal OnDelegateRetVal;
        FOnDelegateRetValOneParam OnDelegateRetValOneParam;
        FOnDelegateRetValTwoParams OnDelegateRetValTwoParams;
    };

    class IObserver {
    public:
        void OnDelegateNoParamsChanged() {
            GlobalValue = 1;
            CallCount++;
        }
        
        void OnDelegateOneParamsChanged(int32 Val) {
            GlobalValue = Val;
            CallCount++;
        }
        
        void OnDelegateTwoParamsChanged(int32 Val1, int32 Val2) {
            GlobalValue = Val1 + Val2;
            CallCount++;
        }
        
        void OnDelegateThreeParamsChanged(int32 Val1, int32 Val2, int32 Val3) {
            GlobalValue = Val1 + Val2 + Val3;
            CallCount++;
        }
        
        void OnDelegateFourParamsChanged(int32 Val1, int32 Val2, int32 Val3, int32 Val4) {
            GlobalValue = Val1 + Val2 + Val3 + Val4;
            CallCount++;
        }
        
        int32 OnDelegateRetValChanged() {
            CallCount++;
            return 42;
        }
        
        int32 OnDelegateRetValOneParamChanged(int32 Val) {
            CallCount++;
            return Val * 2;
        }
        
        int32 OnDelegateRetValTwoParamsChanged(int32 Val1, int32 Val2) {
            CallCount++;
            return Val1 + Val2;
        }
    };

    static void StaticFunction() {
        GlobalValue = 100;
    }
    
    static void StaticFunctionWithParam(int32 Val) {
        GlobalValue = Val * 10;
    }
    
    static int32 StaticFunctionRetVal() {
        return 200;
    }
    
    static int32 StaticFunctionRetValWithParam(int32 Val) {
        return Val * 3;
    }

    IDelegateProxy* Proxy = nullptr;
    IObserver* Observer = nullptr;
    std::shared_ptr<IObserver> SPObserver = nullptr;

    static void SetUp()
    {
        GlobalValue = 0;
        CallCount = 0;
        if (Proxy) delete Proxy;
        if (Observer) delete Observer;
        Proxy = new IDelegateProxy();
        Observer = new IObserver();
        SPObserver = std::make_shared<IObserver>();
    }
    
    static void TearDown()
    {
        if (Proxy) {
            delete Proxy;
            Proxy = nullptr;
        }
        if (Observer) {
            delete Observer;
            Observer = nullptr;
        }
        SPObserver.reset();
    }
}

// ============================================================================
// 基础绑定测试
// ============================================================================

TEST(TestDelegate_BindLambda)
{
    SetUp();
    
    // 无参数 Lambda
    Proxy->OnDelegateNoParams.Bind([]() {
        GlobalValue = 10;
    });
    Proxy->OnDelegateNoParams.Execute();
    ASSERT(GlobalValue == 10);
    
    // 单参数 Lambda
    Proxy->OnDelegateOneParams.Bind([](int32 Val) {
        GlobalValue = Val;
    });
    Proxy->OnDelegateOneParams.Execute(20);
    ASSERT(GlobalValue == 20);
    
    // 多参数 Lambda
    Proxy->OnDelegateTwoParams.Bind([](int32 Val1, int32 Val2) {
        GlobalValue = Val1 + Val2;
    });
    Proxy->OnDelegateTwoParams.Execute(5, 15);
    ASSERT(GlobalValue == 20);

    // 无参数静态函数
    Proxy->OnDelegateNoParams.Bind(&StaticFunction);
    Proxy->OnDelegateNoParams.Execute();
    ASSERT(GlobalValue == 100);

    // 单参数静态函数
    Proxy->OnDelegateOneParams.Bind(&StaticFunctionWithParam);
    Proxy->OnDelegateOneParams.Execute(5);
    ASSERT(GlobalValue == 50);

    // 原始指针绑定
    Proxy->OnDelegateNoParams.Bind(Observer, &IObserver::OnDelegateNoParamsChanged);
    Proxy->OnDelegateNoParams.Execute();
    ASSERT(GlobalValue == 1);
    ASSERT(CallCount == 1);

    // 共享指针绑定
    Proxy->OnDelegateOneParams.Bind(SPObserver, &IObserver::OnDelegateOneParamsChanged);
    Proxy->OnDelegateOneParams.Execute(30);
    ASSERT(GlobalValue == 30);
    ASSERT(CallCount == 2);
}

// ============================================================================
// 有返回值委托测试
// ============================================================================

TEST(TestDelegate_ReturnValue)
{
    SetUp();
    
    // Lambda 返回值
    Proxy->OnDelegateRetVal.Bind([]() -> int32 {
        return 100;
    });
    int32 Result = Proxy->OnDelegateRetVal.Execute();
    ASSERT(Result == 100);
    
    // 静态函数返回值
    Proxy->OnDelegateRetVal.Bind(&StaticFunctionRetVal);
    Result = Proxy->OnDelegateRetVal.Execute();
    ASSERT(Result == 200);
    
    // 成员函数返回值
    Proxy->OnDelegateRetVal.Bind(Observer, &IObserver::OnDelegateRetValChanged);
    Result = Proxy->OnDelegateRetVal.Execute();
    ASSERT(Result == 42);
    
    // 带参数返回值
    Proxy->OnDelegateRetValOneParam.Bind([](int32 Val) -> int32 {
        return Val * 2;
    });
    Result = Proxy->OnDelegateRetValOneParam.Execute(21);
    ASSERT(Result == 42);
    
    Proxy->OnDelegateRetValOneParam.Bind(&StaticFunctionRetValWithParam);
    Result = Proxy->OnDelegateRetValOneParam.Execute(10);
    ASSERT(Result == 30);
    
    Proxy->OnDelegateRetValOneParam.Bind(Observer, &IObserver::OnDelegateRetValOneParamChanged);
    Result = Proxy->OnDelegateRetValOneParam.Execute(15);
    ASSERT(Result == 30);
    
    // 多参数返回值
    Proxy->OnDelegateRetValTwoParams.Bind(Observer, &IObserver::OnDelegateRetValTwoParamsChanged);
    Result = Proxy->OnDelegateRetValTwoParams.Execute(10, 20);
    ASSERT(Result == 30);
}

// ============================================================================
// 状态管理测试
// ============================================================================

TEST(TestDelegate_IsBound)
{
    SetUp();
    
    TDelegate<void()> Delegate;
    ASSERT(!Delegate.IsBound());
    
    Delegate.Bind([]() {});
    ASSERT(Delegate.IsBound());
    
    Delegate.Unbind();
    ASSERT(!Delegate.IsBound());
}

// ============================================================================
// 多参数测试
// ============================================================================

TEST(TestDelegate_MultipleParams)
{
    SetUp();
    
    // 两个参数
    Proxy->OnDelegateTwoParams.Bind(Observer, &IObserver::OnDelegateTwoParamsChanged);
    Proxy->OnDelegateTwoParams.Execute(10, 20);
    ASSERT(GlobalValue == 30);
    
    // 三个参数
    Proxy->OnDelegateThreeParams.Bind(Observer, &IObserver::OnDelegateThreeParamsChanged);
    Proxy->OnDelegateThreeParams.Execute(1, 2, 3);
    ASSERT(GlobalValue == 6);
    
    // 四个参数
    Proxy->OnDelegateFourParams.Bind(Observer, &IObserver::OnDelegateFourParamsChanged);
    Proxy->OnDelegateFourParams.Execute(1, 2, 3, 4);
    ASSERT(GlobalValue == 10);
}

// ============================================================================
// 共享指针生命周期测试
// ============================================================================

TEST(TestDelegate_SharedPtrLifetime)
{
    SetUp();
    
    {
        auto LocalObserver = std::make_shared<IObserver>();
        Proxy->OnDelegateNoParams.Bind(LocalObserver, &IObserver::OnDelegateNoParamsChanged);
        Proxy->OnDelegateNoParams.Execute();
        ASSERT(GlobalValue == 1);
    }
    
    // 对象已销毁，但委托仍持有 weak_ptr，调用应该安全（不执行）
    GlobalValue = 0;
    Proxy->OnDelegateNoParams.Execute();
    ASSERT(GlobalValue == 0);  // 应该不执行，因为对象已销毁
}

// ============================================================================
// 重新绑定测试
// ============================================================================

TEST(TestDelegate_Rebind)
{
    SetUp();
    
    // 第一次绑定
    Proxy->OnDelegateNoParams.Bind([]() {
        GlobalValue = 1;
    });
    Proxy->OnDelegateNoParams.Execute();
    ASSERT(GlobalValue == 1);
    
    // 重新绑定
    Proxy->OnDelegateNoParams.Bind([]() {
        GlobalValue = 2;
    });
    Proxy->OnDelegateNoParams.Execute();
    ASSERT(GlobalValue == 2);
    
    // 再次重新绑定为成员函数
    Proxy->OnDelegateNoParams.Bind(Observer, &IObserver::OnDelegateNoParamsChanged);
    Proxy->OnDelegateNoParams.Execute();
    ASSERT(GlobalValue == 1);
}