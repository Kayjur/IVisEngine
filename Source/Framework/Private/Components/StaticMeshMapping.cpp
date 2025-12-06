#include "Components/StaticMeshMapping.h"
#include "Memory/UniquePtr.h"
#include <iostream>

FMeshSceneProxy::FMeshSceneProxy(uint32 InComponentId, const std::string& InMeshName)
    : FPrimitiveSceneProxy(InComponentId)
    , MeshName(InMeshName)
{
}

void FMeshSceneProxy::UpdateData()
{
    // 更新网格数据
    std::cout << "[RendererThread] 更新网格代理: " << MeshName << " (ComponentId: " << GetPrimitiveComponentId() << ")" << std::endl;
}

void FMeshSceneProxy::GetDrawData() const
{
    // 获取绘制数据
    std::cout << "[RendererThread] 获取绘制数据: " << MeshName << std::endl;
}

IStaticMeshMapping::IStaticMeshMapping(const std::string& InMeshName)
    : IMappingComponent()
    , MeshName(InMeshName)
{
}

TUniquePtr<FPrimitiveSceneProxy> IStaticMeshMapping::CreateSceneProxy()
{
    return MakeUnique<FMeshSceneProxy>(GetComponentId(), MeshName);
}

void IStaticMeshMapping::SetMeshName(const std::string& InMeshName)
{
    MeshName = InMeshName;
    MarkRenderStateDirty();
}

