#pragma once

#include "Components/MappingComponent.h"
#include "Rendering/SceneProxy.h"
#include <string>

/**
 * FMeshSceneProxy - 网格场景代理
 */
class FMeshSceneProxy : public FPrimitiveSceneProxy {
public:
    FMeshSceneProxy(uint32 InComponentId, const std::string& InMeshName);
    
    void UpdateData() override;
    void GetDrawData() const override;

    const std::string& GetMeshName() const { return MeshName; }

private:
    std::string MeshName;
};

/**
 * IStaticMeshMapping - 静态网格映射组件
 */
class IStaticMeshMapping : public IMappingComponent {
public:
    explicit IStaticMeshMapping(const std::string& InMeshName);

    TUniquePtr<FPrimitiveSceneProxy> CreateSceneProxy() override;

    void SetMeshName(const std::string& InMeshName);

    const std::string& GetMeshName() const { return MeshName; }

private:
    std::string MeshName;
};

