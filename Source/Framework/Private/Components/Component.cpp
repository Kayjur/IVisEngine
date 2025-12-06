#include "Components/Component.h"
#include "Rendering/SceneProxy.h"
#include "Rendering/Scene.h"
#include "Rendering/RenderCommandQueue.h"
#include "Rendering/RenderCommandMacros.h"

uint32 IComponent::NextComponentId = 1;

IComponent::IComponent()
    : ComponentId(NextComponentId++)
{
}

IComponent::~IComponent()
{

}


