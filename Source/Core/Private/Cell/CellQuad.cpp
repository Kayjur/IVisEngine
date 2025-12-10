#include "Cell/CellQuad.h"

#include "Cell/CellTriangle.h"

ECellType ICellQuad::GetCellType() const
{
    return ECellType::Quad;
}

int32 ICellQuad::GetVertexCount() const
{
    return 4;
}

int32 ICellQuad::GetCellDimension() const
{
    return 2;
}

// ============================================================================
// 静态辅助函数
// ============================================================================

FVector ICellQuad::ComputeNormal(const FVector& V0, const FVector& V1, const FVector& V2, const FVector& V3)
{
    // 计算平均法线：计算四个三角形的法线并平均
    // 也可以直接使用对角线叉积，但这对于非平面四边形可能不准确
    // iGameCore 使用的是 Center 与每条边构成的四个三角形的法线之和
    
    FVector Center = (V0 + V1 + V2 + V3) * 0.25f;
    FVector N1 = ICellTriangle::ComputeNormal(Center, V0, V1);
    FVector N2 = ICellTriangle::ComputeNormal(Center, V1, V2);
    FVector N3 = ICellTriangle::ComputeNormal(Center, V2, V3);
    FVector N4 = ICellTriangle::ComputeNormal(Center, V3, V0);
    
    return (N1 + N2 + N3 + N4).GetSafeNormal();
}

FVector3d ICellQuad::ComputeNormal(const FVector3d& V0, const FVector3d& V1, const FVector3d& V2, const FVector3d& V3)
{
    FVector3d Center = (V0 + V1 + V2 + V3) * 0.25;
    FVector3d N1 = ICellTriangle::ComputeNormal(Center, V0, V1);
    FVector3d N2 = ICellTriangle::ComputeNormal(Center, V1, V2);
    FVector3d N3 = ICellTriangle::ComputeNormal(Center, V2, V3);
    FVector3d N4 = ICellTriangle::ComputeNormal(Center, V3, V0);
    
    return (N1 + N2 + N3 + N4).GetSafeNormal();
}

float ICellQuad::ComputeArea(const FVector& V0, const FVector& V1, const FVector& V2, const FVector& V3)
{
    // 分割为两个三角形计算面积
    // (0,1,2) 和 (0,2,3)
    return ICellTriangle::ComputeArea(V0, V1, V2) + ICellTriangle::ComputeArea(V0, V2, V3);
}

double ICellQuad::ComputeArea(const FVector3d& V0, const FVector3d& V1, const FVector3d& V2, const FVector3d& V3)
{
    return ICellTriangle::ComputeArea(V0, V1, V2) + ICellTriangle::ComputeArea(V0, V2, V3);
}

FVector ICellQuad::ComputeCenter(const FVector& V0, const FVector& V1, const FVector& V2, const FVector& V3)
{
    return (V0 + V1 + V2 + V3) * 0.25f;
}

FVector3d ICellQuad::ComputeCenter(const FVector3d& V0, const FVector3d& V1, const FVector3d& V2, const FVector3d& V3)
{
    return (V0 + V1 + V2 + V3) * 0.25;
}

bool ICellQuad::IsConvex(const FVector& V0, const FVector& V1, const FVector& V2, const FVector& V3)
{
    // 检查对角线是否相交，或者检查所有内角是否小于180度
    // 使用叉积方向一致性检查
    FVector E0 = V1 - V0;
    FVector E1 = V2 - V1;
    FVector E2 = V3 - V2;
    FVector E3 = V0 - V3;
    
    // 假设四边形近似共面
    FVector Normal = ComputeNormal(V0, V1, V2, V3);
    
    FVector C0 = E0.Cross(E1);
    FVector C1 = E1.Cross(E2);
    FVector C2 = E2.Cross(E3);
    FVector C3 = E3.Cross(E0);
    
    return Normal.Dot(C0) > 0 && Normal.Dot(C1) > 0 && Normal.Dot(C2) > 0 && Normal.Dot(C3) > 0;
}

bool ICellQuad::IsConvex(const FVector3d& V0, const FVector3d& V1, const FVector3d& V2, const FVector3d& V3)
{
    FVector3d E0 = V1 - V0;
    FVector3d E1 = V2 - V1;
    FVector3d E2 = V3 - V2;
    FVector3d E3 = V0 - V3;
    
    FVector3d Normal = ComputeNormal(V0, V1, V2, V3);
    
    FVector3d C0 = E0.Cross(E1);
    FVector3d C1 = E1.Cross(E2);
    FVector3d C2 = E2.Cross(E3);
    FVector3d C3 = E3.Cross(E0);
    
    return Normal.Dot(C0) > 0 && Normal.Dot(C1) > 0 && Normal.Dot(C2) > 0 && Normal.Dot(C3) > 0;
}
