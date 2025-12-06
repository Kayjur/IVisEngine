#pragma once

#include "Component.h"

class ISceneComponent : public IComponent {
public:
    ISceneComponent();
    ~ISceneComponent() override;

    //FTransform ComponentToWorld;  // 在世界坐标的变换（位置、旋转、缩放）

    //FTransform RelativeTransform; // 表示相对于父组件的变换（位置、旋转、缩放）

    //FBoxSphereBounds Bounds;

private:
    ISceneComponent* AttachParent;    //表示该组件当前的父组件
    TArray<ISceneComponent*> AttachChildren;   //指向所有附加到当前组件的子组件数组
    bool bVisible;

};