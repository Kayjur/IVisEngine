#pragma once

#include "SceneComponent.h"
#include "Memory/UniquePtr.h"

class IMappingComponent : public ISceneComponent {
public:
    IMappingComponent();
    ~IMappingComponent() override;

    /**
     * 创建场景代理（在主线程中调用）
     * 子类需要重写此方法，创建对应的SceneProxy
     * @return 创建的SceneProxy，如果不需要渲染则返回nullptr
     */
    virtual TUniquePtr<FPrimitiveSceneProxy> CreateSceneProxy() = 0;

    /**
     * 标记需要更新渲染数据
     * 调用此方法后，渲染线程会在下一帧更新SceneProxy的数据
     */
    void MarkRenderStateDirty();

    /**
     * 检查渲染状态是否需要更新
     * @return 是否需要更新
     */
    bool IsRenderStateDirty() const { return bRenderStateDirty; }

    /**
     * 清除渲染状态脏标记
     */
    void ClearRenderStateDirty() { bRenderStateDirty = false; }

    /**
     * 注册到场景（将Component添加到渲染系统）
     */
    void RegisterComponent();

    /**
     * 从场景注销（从渲染系统移除Component）
     */
    void UnregisterComponent();

    /**
     * 检查Component是否已注册
     * @return 是否已注册
     */
    bool IsRegistered() const { return bIsRegistered; }

protected:
    bool bRenderStateDirty;
    bool bIsRegistered;

private:

};