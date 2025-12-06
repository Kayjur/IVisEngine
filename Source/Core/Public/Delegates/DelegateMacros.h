#pragma once

#include "Delegate.h"
#include "MulticastDelegate.h"
#include "Event.h"

/**
 * 委托声明宏
 * 类似于 UE 的委托宏，用于简化委托类型声明
 * 
 * 使用示例：
 *   // 在头文件中声明委托类型
 *   DECLARE_DELEGATE(FMyDelegate);
 *   DECLARE_DELEGATE_OneParam(FMyDelegateWithParam, int32);
 *   DECLARE_DELEGATE_TwoParams(FMyDelegateWithTwoParams, int32, float);
 *   DECLARE_DELEGATE_RetVal(int32, FMyDelegateWithReturn);
 *   DECLARE_DELEGATE_RetVal_OneParam(int32, FMyDelegateWithReturnAndParam, int32);
 * 
 *   // 在类中使用
 *   class MyClass {
 *   public:
 *       FMyDelegate OnSomething;
 *       FMyDelegateWithParam OnValueChanged;
 *   };
 */

// ============================================================================
// 单播委托宏（无返回值）
// ============================================================================

/** 声明无参数无返回值委托 */
#define DECLARE_DELEGATE(DelegateName) \
    using DelegateName = TDelegate<void()>;

/** 声明一个参数无返回值委托 */
#define DECLARE_DELEGATE_OneParam(DelegateName, Param1Type) \
    using DelegateName = TDelegate<void(Param1Type)>;

/** 声明两个参数无返回值委托 */
#define DECLARE_DELEGATE_TwoParams(DelegateName, Param1Type, Param2Type) \
    using DelegateName = TDelegate<void(Param1Type, Param2Type)>;

/** 声明三个参数无返回值委托 */
#define DECLARE_DELEGATE_ThreeParams(DelegateName, Param1Type, Param2Type, Param3Type) \
    using DelegateName = TDelegate<void(Param1Type, Param2Type, Param3Type)>;

/** 声明四个参数无返回值委托 */
#define DECLARE_DELEGATE_FourParams(DelegateName, Param1Type, Param2Type, Param3Type, Param4Type) \
    using DelegateName = TDelegate<void(Param1Type, Param2Type, Param3Type, Param4Type)>;

/** 声明五个参数无返回值委托 */
#define DECLARE_DELEGATE_FiveParams(DelegateName, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type) \
    using DelegateName = TDelegate<void(Param1Type, Param2Type, Param3Type, Param4Type, Param5Type)>;

// ============================================================================
// 单播委托宏（有返回值）
// ============================================================================

/** 声明无参数有返回值委托 */
#define DECLARE_DELEGATE_RetVal(RetValType, DelegateName) \
    using DelegateName = TDelegate<RetValType()>;

/** 声明一个参数有返回值委托 */
#define DECLARE_DELEGATE_RetVal_OneParam(RetValType, DelegateName, Param1Type) \
    using DelegateName = TDelegate<RetValType(Param1Type)>;

/** 声明两个参数有返回值委托 */
#define DECLARE_DELEGATE_RetVal_TwoParams(RetValType, DelegateName, Param1Type, Param2Type) \
    using DelegateName = TDelegate<RetValType(Param1Type, Param2Type)>;

/** 声明三个参数有返回值委托 */
#define DECLARE_DELEGATE_RetVal_ThreeParams(RetValType, DelegateName, Param1Type, Param2Type, Param3Type) \
    using DelegateName = TDelegate<RetValType(Param1Type, Param2Type, Param3Type)>;

/** 声明四个参数有返回值委托 */
#define DECLARE_DELEGATE_RetVal_FourParams(RetValType, DelegateName, Param1Type, Param2Type, Param3Type, Param4Type) \
    using DelegateName = TDelegate<RetValType(Param1Type, Param2Type, Param3Type, Param4Type)>;

/** 声明五个参数有返回值委托 */
#define DECLARE_DELEGATE_RetVal_FiveParams(RetValType, DelegateName, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type) \
    using DelegateName = TDelegate<RetValType(Param1Type, Param2Type, Param3Type, Param4Type, Param5Type)>;

// ============================================================================
// 多播委托宏
// ============================================================================

/** 声明无参数多播委托 */
#define DECLARE_MULTICAST_DELEGATE(DelegateName) \
    using DelegateName = TMulticastDelegate<void()>;

/** 声明一个参数多播委托 */
#define DECLARE_MULTICAST_DELEGATE_OneParam(DelegateName, Param1Type) \
    using DelegateName = TMulticastDelegate<void(Param1Type)>;

/** 声明两个参数多播委托 */
#define DECLARE_MULTICAST_DELEGATE_TwoParams(DelegateName, Param1Type, Param2Type) \
    using DelegateName = TMulticastDelegate<void(Param1Type, Param2Type)>;

/** 声明三个参数多播委托 */
#define DECLARE_MULTICAST_DELEGATE_ThreeParams(DelegateName, Param1Type, Param2Type, Param3Type) \
    using DelegateName = TMulticastDelegate<void(Param1Type, Param2Type, Param3Type)>;

/** 声明四个参数多播委托 */
#define DECLARE_MULTICAST_DELEGATE_FourParams(DelegateName, Param1Type, Param2Type, Param3Type, Param4Type) \
    using DelegateName = TMulticastDelegate<void(Param1Type, Param2Type, Param3Type, Param4Type)>;

/** 声明五个参数多播委托 */
#define DECLARE_MULTICAST_DELEGATE_FiveParams(DelegateName, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type) \
    using DelegateName = TMulticastDelegate<void(Param1Type, Param2Type, Param3Type, Param4Type, Param5Type)>;

// ============================================================================
// 事件宏
// ============================================================================

/** 声明无参数事件 */
#define DECLARE_EVENT(OwnerClass, EventName) \
    using EventName = TEvent<void()>;

/** 声明一个参数事件 */
#define DECLARE_EVENT_OneParam(OwnerClass, EventName, Param1Type) \
    using EventName = TEvent<void(Param1Type)>;

/** 声明两个参数事件 */
#define DECLARE_EVENT_TwoParams(OwnerClass, EventName, Param1Type, Param2Type) \
    using EventName = TEvent<void(Param1Type, Param2Type)>;

/** 声明三个参数事件 */
#define DECLARE_EVENT_ThreeParams(OwnerClass, EventName, Param1Type, Param2Type, Param3Type) \
    using EventName = TEvent<void(Param1Type, Param2Type, Param3Type)>;

/** 声明四个参数事件 */
#define DECLARE_EVENT_FourParams(OwnerClass, EventName, Param1Type, Param2Type, Param3Type, Param4Type) \
    using EventName = TEvent<void(Param1Type, Param2Type, Param3Type, Param4Type)>;

/** 声明五个参数事件 */
#define DECLARE_EVENT_FiveParams(OwnerClass, EventName, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type) \
    using EventName = TEvent<void(Param1Type, Param2Type, Param3Type, Param4Type, Param5Type)>;

// ============================================================================
// 动态多播委托宏（与静态多播委托相同，提供兼容性）
// ============================================================================

/** 声明动态多播委托（与 DECLARE_MULTICAST_DELEGATE 相同） */
#define DECLARE_DYNAMIC_MULTICAST_DELEGATE(DelegateName) \
    DECLARE_MULTICAST_DELEGATE(DelegateName)

/** 声明动态多播委托（一个参数） */
#define DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(DelegateName, Param1Type) \
    DECLARE_MULTICAST_DELEGATE_OneParam(DelegateName, Param1Type)

/** 声明动态多播委托（两个参数） */
#define DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(DelegateName, Param1Type, Param2Type) \
    DECLARE_MULTICAST_DELEGATE_TwoParams(DelegateName, Param1Type, Param2Type)

/** 声明动态多播委托（三个参数） */
#define DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(DelegateName, Param1Type, Param2Type, Param3Type) \
    DECLARE_MULTICAST_DELEGATE_ThreeParams(DelegateName, Param1Type, Param2Type, Param3Type)

/** 声明动态多播委托（四个参数） */
#define DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(DelegateName, Param1Type, Param2Type, Param3Type, Param4Type) \
    DECLARE_MULTICAST_DELEGATE_FourParams(DelegateName, Param1Type, Param2Type, Param3Type, Param4Type)

/** 声明动态多播委托（五个参数） */
#define DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(DelegateName, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type) \
    DECLARE_MULTICAST_DELEGATE_FiveParams(DelegateName, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type)
