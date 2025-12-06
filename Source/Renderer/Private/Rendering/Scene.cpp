#include "Rendering/Scene.h"

IScene& IScene::Get()
{
    static IScene Instance;
    return Instance;
}

void IScene::AddPrimitive(uint32 ComponentId, TUniquePtr<FPrimitiveSceneProxy> Proxy)
{
    std::lock_guard<std::mutex> Lock(SceneMutex);
    Primitives[ComponentId] = std::move(Proxy);
}

void IScene::RemovePrimitive(uint32 ComponentId)
{
    std::lock_guard<std::mutex> Lock(SceneMutex);
    Primitives.Remove(ComponentId);
}

void IScene::UpdatePrimitive(uint32 ComponentId)
{
    std::lock_guard<std::mutex> Lock(SceneMutex);
    const auto It = Primitives.Find(ComponentId);
    if (It && It->IsValid())
    {
        (*It)->UpdateData();
    }
}

FPrimitiveSceneProxy* IScene::GetPrimitive(uint32 ComponentId) const
{
    std::lock_guard<std::mutex> Lock(SceneMutex);
    const auto It = Primitives.Find(ComponentId);
    if (It && It->IsValid())
    {
        return It->Get();
    }
    return nullptr;
}

uint32 IScene::GetPrimitiveCount() const
{
    std::lock_guard<std::mutex> Lock(SceneMutex);
    return static_cast<uint32>(Primitives.Num());
}

void IScene::Clear()
{
    std::lock_guard<std::mutex> Lock(SceneMutex);
    Primitives.Clear();
}

