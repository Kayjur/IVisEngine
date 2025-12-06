#include "Components/MappingComponent.h"

#include "Rendering/RenderCommandMacros.h"
#include "Rendering/Scene.h"

IMappingComponent::IMappingComponent() :
    bRenderStateDirty(false),
    bIsRegistered(false)
{
}

IMappingComponent::~IMappingComponent()
{
    if (bIsRegistered)
    {
        UnregisterComponent();
    }
}

void IMappingComponent::MarkRenderStateDirty()
{
    bRenderStateDirty = true;
}

void IMappingComponent::RegisterComponent()
{
    if (bIsRegistered)
    {
        return;
    }

    auto Proxy = CreateSceneProxy();
    if (Proxy.IsValid())
    {
        uint32 Id = GetComponentId();
        ENQUEUE_RENDER_COMMAND(AddPrimitiveCommand)(
            [Id, Proxy = std::move(Proxy)]() mutable {
                IScene::Get().AddPrimitive(Id, std::move(Proxy));
            }
        );
        bIsRegistered = true;
    }
}

void IMappingComponent::UnregisterComponent()
{
    if (!bIsRegistered)
    {
        return;
    }

    uint32 Id = GetComponentId();
    ENQUEUE_RENDER_COMMAND(RemovePrimitiveCommand)(
        [Id]() {
            IScene::Get().RemovePrimitive(Id);
        }
    );
    bIsRegistered = false;
}
