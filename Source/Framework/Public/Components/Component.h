#pragma once

#include "HAL/Platform.h"
#include "Container/Array.h"
#include <memory>

class FPrimitiveSceneProxy;

/**
 * Component基类
 * 主线程中的组件
 */
class IComponent {
public:
    IComponent();
    virtual ~IComponent();

    /**
     * 获取Component的唯一ID
     * @return Component ID
     */
    uint32 GetComponentId() const { return ComponentId; }

protected:
    uint32 ComponentId;

private:
    static uint32 NextComponentId;
};

