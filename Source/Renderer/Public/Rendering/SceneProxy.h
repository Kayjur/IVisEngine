#pragma once

#include "HAL/Platform.h"
#include <memory>

/**
 * FPrimitiveSceneProxy - 场景代理基类
 * 在渲染线程中使用的渲染数据代理
 * 游戏线程中的Component会创建对应的SceneProxy，传递给渲染线程使用
 */
class FPrimitiveSceneProxy {
public:
    /**
     * 构造函数
     * @param InPrimitiveComponentId 关联的Component ID
     */
    explicit FPrimitiveSceneProxy(uint32 InPrimitiveComponentId);
    
    /**
     * 析构函数
     */
    virtual ~FPrimitiveSceneProxy();

    /**
     * 获取关联的Component ID
     * @return Component ID
     */
    uint32 GetPrimitiveComponentId() const { return PrimitiveComponentId; }

    /**
     * 更新代理数据（在渲染线程中调用）
     * 从Component同步最新的渲染数据
     */
    virtual void UpdateData() {}

    /**
     * 获取绘制数据（在渲染线程中调用）
     * 返回渲染所需的所有数据
     */
    virtual void GetDrawData() const {}

    /**
     * 检查代理是否有效
     * @return 是否有效
     */
    bool IsValid() const { return bIsValid; }

    /**
     * 标记代理为无效
     */
    void MarkInvalid() { bIsValid = false; }

protected:
    uint32 PrimitiveComponentId;
    bool bIsValid;
};
