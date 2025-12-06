#pragma once

#include "Rendering/SceneProxy.h"
#include "HAL/Platform.h"
#include "Memory/UniquePtr.h"
#include "Container/Map.h"

#include <mutex>

/**
 * IScene - 场景类
 * 在渲染线程中管理所有SceneProxy
 */
class IScene {
public:

    IScene(const IScene&) = delete;
    IScene& operator=(const IScene&) = delete;

    /**
     * 获取全局场景单例
     * @return 场景引用
     */
    static IScene& Get();

    /**
     * 添加场景代理到场景中（从游戏线程调用，通过渲染命令）
     * @param ComponentId Component ID
     * @param Proxy 要添加的SceneProxy
     */
    void AddPrimitive(uint32 ComponentId, TUniquePtr<FPrimitiveSceneProxy> Proxy);

    /**
     * 从场景中移除代理（从游戏线程调用，通过渲染命令）
     * @param ComponentId Component ID
     */
    void RemovePrimitive(uint32 ComponentId);

    /**
     * 更新场景代理数据（在渲染线程中调用）
     * @param ComponentId Component ID
     */
    void UpdatePrimitive(uint32 ComponentId);

    /**
     * 获取场景代理
     * @param ComponentId Component ID
     * @return SceneProxy指针，如果不存在则返回nullptr
     */
    FPrimitiveSceneProxy* GetPrimitive(uint32 ComponentId) const;

    /**
     * 获取所有场景代理的数量
     * @return 代理数量
     */
    uint32 GetPrimitiveCount() const;

    /**
     * 清空场景中的所有代理
     */
    void Clear();

private:
    IScene() = default;
    ~IScene() = default;

    mutable std::mutex SceneMutex;
    TMap<uint32, TUniquePtr<FPrimitiveSceneProxy>> Primitives;
};
