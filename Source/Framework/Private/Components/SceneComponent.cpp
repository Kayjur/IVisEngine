#include "Components/SceneComponent.h"

ISceneComponent::ISceneComponent():
    AttachParent(nullptr),
    bVisible(true)
{

}

ISceneComponent::~ISceneComponent()
{

}
