#include "Rendering/SceneProxy.h"

FPrimitiveSceneProxy::FPrimitiveSceneProxy(uint32 InPrimitiveComponentId)
    : PrimitiveComponentId(InPrimitiveComponentId)
    , bIsValid(true)
{
}

FPrimitiveSceneProxy::~FPrimitiveSceneProxy()
{
}

