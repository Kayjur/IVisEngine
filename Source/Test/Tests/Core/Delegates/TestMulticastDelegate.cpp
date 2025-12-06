#include "TestFramework.h"
#include "Delegates/DelegateMacros.h"
#include "Delegates/MulticastDelegate.h"
#include <memory>

TEST_GROUP(TestMulticastDelegate)

namespace {
    int32 GlobalValue = 0;
    int32 CallCount = 0;

    // 多播委托声明
    DECLARE_MULTICAST_DELEGATE(FOnMulticastNoParams);
    DECLARE_MULTICAST_DELEGATE_OneParam(FOnMulticastOneParam, int32);

    class IMulticastProxy {
    public:
        FOnMulticastNoParams OnMulticastNoParams;
        FOnMulticastOneParam OnMulticastOneParam;
    };

    class IObserver {
    public:
        void OnMulticastNoParamsChanged() {
            GlobalValue = 1;
            CallCount++;
        }
        
        void OnMulticastOneParamChanged(int32 Val) {
            GlobalValue = Val;
            CallCount++;
        }
    };

    IMulticastProxy* Proxy = nullptr;
    IObserver* Observer = nullptr;
    std::shared_ptr<IObserver> SPObserver = nullptr;

    static void SetUp()
    {
        GlobalValue = 0;
        CallCount = 0;
        if (Proxy) delete Proxy;
        if (Observer) delete Observer;
        Proxy = new IMulticastProxy();
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
// 基础功能测试
// ============================================================================

TEST(TestMulticastDelegate_Basic)
{
    SetUp();
    
    // 添加多个 Lambda
    Proxy->OnMulticastNoParams.AddLambda([]() {
        GlobalValue += 1;
    });
    Proxy->OnMulticastNoParams.AddLambda([]() {
        GlobalValue += 2;
    });
    Proxy->OnMulticastNoParams.AddLambda([]() {
        GlobalValue += 3;
    });
    
    // 广播应该按顺序执行所有函数
    Proxy->OnMulticastNoParams.Broadcast();
    ASSERT(GlobalValue == 6);
}

TEST(TestMulticastDelegate_WithParams)
{
    SetUp();
    
    // 带参数的多播委托
    Proxy->OnMulticastOneParam.AddLambda([](int32 Val) {
        GlobalValue += Val;
    });
    Proxy->OnMulticastOneParam.AddLambda([](int32 Val) {
        GlobalValue += Val * 2;
    });
    
    Proxy->OnMulticastOneParam.Broadcast(5);
    ASSERT(GlobalValue == 15);  // 5 + 5*2 = 15
}

TEST(TestMulticastDelegate_MemberFunction)
{
    SetUp();
    
    // 添加成员函数
    Proxy->OnMulticastNoParams.AddObject(Observer, &IObserver::OnMulticastNoParamsChanged);
    Proxy->OnMulticastNoParams.AddObject(Observer, &IObserver::OnMulticastNoParamsChanged);
    
    Proxy->OnMulticastNoParams.Broadcast();
    ASSERT(CallCount == 2);
    ASSERT(GlobalValue == 1);  // 最后一次调用设置的值
}

TEST(TestMulticastDelegate_IsBound)
{
    SetUp();
    
    TMulticastDelegate<void()> Multicast;
    ASSERT(!Multicast.IsBound());
    
    Multicast.AddLambda([]() {});
    ASSERT(Multicast.IsBound());
    
    Multicast.RemoveAll();
    ASSERT(!Multicast.IsBound());
}

