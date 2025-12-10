#include "Cell/CellTriangle.h"

ECellType ICellTriangle::GetCellType() const
{
    return ECellType::Triangle;
}

int32 ICellTriangle::GetVertexCount() const
{
    return 3;
}

int32 ICellTriangle::GetCellDimension() const
{
    return 2;
}

// ============================================================================
// 静态辅助函数
// ============================================================================

FVector ICellTriangle::ComputeNormal(const FVector& V0, const FVector& V1, const FVector& V2)
{
    FVector Edge1 = V1 - V0;
    FVector Edge2 = V2 - V0;
    FVector Normal = Edge1.Cross(Edge2);
    return Normal.GetSafeNormal();
}

FVector3d ICellTriangle::ComputeNormal(const FVector3d& V0, const FVector3d& V1, const FVector3d& V2)
{
    FVector3d Edge1 = V1 - V0;
    FVector3d Edge2 = V2 - V0;
    FVector3d Normal = Edge1.Cross(Edge2);
    return Normal.GetSafeNormal();
}

float ICellTriangle::ComputeArea(const FVector& V0, const FVector& V1, const FVector& V2)
{
    FVector Edge1 = V1 - V0;
    FVector Edge2 = V2 - V0;
    return Edge1.Cross(Edge2).Size() * 0.5f;
}

double ICellTriangle::ComputeArea(const FVector3d& V0, const FVector3d& V1, const FVector3d& V2)
{
    FVector3d Edge1 = V1 - V0;
    FVector3d Edge2 = V2 - V0;
    return Edge1.Cross(Edge2).Size() * 0.5;
}

FVector ICellTriangle::ComputeCenter(const FVector& V0, const FVector& V1, const FVector& V2)
{
    return (V0 + V1 + V2) / 3.0f;
}

FVector3d ICellTriangle::ComputeCenter(const FVector3d& V0, const FVector3d& V1, const FVector3d& V2)
{
    return (V0 + V1 + V2) / 3.0;
}

bool ICellTriangle::IsPointInTriangle(const FVector& Point, const FVector& V0, const FVector& V1, const FVector& V2)
{
    // 使用同向法 (Same Side Technique)
    FVector Edge1 = V1 - V0;
    FVector Edge2 = V2 - V0;
    if (Edge1.IsZero() || Edge2.IsZero()) return false;
    
    FVector VP0 = Point - V0;
    FVector VP1 = Point - V1;
    FVector VP2 = Point - V2;
    
    FVector C0 = (V1 - V0).Cross(VP0);
    FVector C1 = (V2 - V1).Cross(VP1);
    FVector C2 = (V0 - V2).Cross(VP2);
    
    FVector Normal = Edge1.Cross(Edge2);
    
    return Normal.Dot(C0) >= 0 && Normal.Dot(C1) >= 0 && Normal.Dot(C2) >= 0;
}

bool ICellTriangle::IsPointInTriangle(const FVector3d& Point, const FVector3d& V0, const FVector3d& V1, const FVector3d& V2)
{
    FVector3d Edge1 = V1 - V0;
    FVector3d Edge2 = V2 - V0;
    if (Edge1.IsZero() || Edge2.IsZero()) return false;
    
    FVector3d VP0 = Point - V0;
    FVector3d VP1 = Point - V1;
    FVector3d VP2 = Point - V2;
    
    FVector3d C0 = (V1 - V0).Cross(VP0);
    FVector3d C1 = (V2 - V1).Cross(VP1);
    FVector3d C2 = (V0 - V2).Cross(VP2);
    
    FVector3d Normal = Edge1.Cross(Edge2);
    
    return Normal.Dot(C0) >= 0 && Normal.Dot(C1) >= 0 && Normal.Dot(C2) >= 0;
}

void ICellTriangle::ComputeBarycentricCoordinates(const FVector& Point, const FVector& V0, const FVector& V1, const FVector& V2, float& OutU, float& OutV, float& OutW)
{
    // P = u*V0 + v*V1 + w*V2
    // 使用 Christer Ericson 的 Real-Time Collision Detection 中的算法
    FVector v0 = V1 - V0, v1 = V2 - V0, v2 = Point - V0;
    float d00 = v0.Dot(v0);
    float d01 = v0.Dot(v1);
    float d11 = v1.Dot(v1);
    float d20 = v2.Dot(v0);
    float d21 = v2.Dot(v1);
    float denom = d00 * d11 - d01 * d01;
    
    if (std::abs(denom) < 1e-6f)
    {
        // 退化三角形，简单返回中心
        OutU = OutV = OutW = 1.0f / 3.0f;
        return;
    }
    
    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    OutU = 1.0f - v - w;
    OutV = v;
    OutW = w;
}

void ICellTriangle::ComputeBarycentricCoordinates(const FVector3d& Point, const FVector3d& V0, const FVector3d& V1, const FVector3d& V2, double& OutU, double& OutV, double& OutW)
{
    FVector3d v0 = V1 - V0, v1 = V2 - V0, v2 = Point - V0;
    double d00 = v0.Dot(v0);
    double d01 = v0.Dot(v1);
    double d11 = v1.Dot(v1);
    double d20 = v2.Dot(v0);
    double d21 = v2.Dot(v1);
    double denom = d00 * d11 - d01 * d01;
    
    if (std::abs(denom) < 1e-12)
    {
        OutU = OutV = OutW = 1.0 / 3.0;
        return;
    }
    
    double v = (d11 * d20 - d01 * d21) / denom;
    double w = (d00 * d21 - d01 * d20) / denom;
    OutU = 1.0 - v - w;
    OutV = v;
    OutW = w;
}

FVector ICellTriangle::ProjectPointOnPlane(const FVector& Point, const FVector& V0, const FVector& V1, const FVector& V2)
{
    FVector Normal = ComputeNormal(V0, V1, V2);
    FVector V = Point - V0;
    float Dist = V.Dot(Normal);
    return Point - Normal * Dist;
}

FVector3d ICellTriangle::ProjectPointOnPlane(const FVector3d& Point, const FVector3d& V0, const FVector3d& V1, const FVector3d& V2)
{
    FVector3d Normal = ComputeNormal(V0, V1, V2);
    FVector3d V = Point - V0;
    double Dist = V.Dot(Normal);
    return Point - Normal * Dist;
}

FVector ICellTriangle::ClosestPointOnTriangle(const FVector& Point, const FVector& V0, const FVector& V1, const FVector& V2)
{
    // 检查点是否在三角形顶点Voronoi区域内
    FVector ab = V1 - V0;
    FVector ac = V2 - V0;
    FVector ap = Point - V0;
    float d1 = ab.Dot(ap);
    float d2 = ac.Dot(ap);
    if (d1 <= 0.0f && d2 <= 0.0f) return V0;
    
    FVector bp = Point - V1;
    float d3 = ab.Dot(bp);
    float d4 = ac.Dot(bp);
    if (d3 >= 0.0f && d4 <= d3) return V1;
    
    float vc = d1 * d4 - d3 * d2;
    if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
    {
        float v = d1 / (d1 - d3);
        return V0 + ab * v;
    }
    
    FVector cp = Point - V2;
    float d5 = ab.Dot(cp);
    float d6 = ac.Dot(cp);
    if (d6 >= 0.0f && d5 <= d6) return V2;
    
    float vb = d5 * d2 - d1 * d6;
    if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
    {
        float w = d2 / (d2 - d6);
        return V0 + ac * w;
    }
    
    float va = d3 * d6 - d5 * d4;
    if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
    {
        float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
        return V1 + (V2 - V1) * w;
    }
    
    float denom = 1.0f / (va + vb + vc);
    float v = vb * denom;
    float w = vc * denom;
    return V0 + ab * v + ac * w;
}

FVector3d ICellTriangle::ClosestPointOnTriangle(const FVector3d& Point, const FVector3d& V0, const FVector3d& V1, const FVector3d& V2)
{
    FVector3d ab = V1 - V0;
    FVector3d ac = V2 - V0;
    FVector3d ap = Point - V0;
    double d1 = ab.Dot(ap);
    double d2 = ac.Dot(ap);
    if (d1 <= 0.0 && d2 <= 0.0) return V0;
    
    FVector3d bp = Point - V1;
    double d3 = ab.Dot(bp);
    double d4 = ac.Dot(bp);
    if (d3 >= 0.0 && d4 <= d3) return V1;
    
    double vc = d1 * d4 - d3 * d2;
    if (vc <= 0.0 && d1 >= 0.0 && d3 <= 0.0)
    {
        double v = d1 / (d1 - d3);
        return V0 + ab * v;
    }
    
    FVector3d cp = Point - V2;
    double d5 = ab.Dot(cp);
    double d6 = ac.Dot(cp);
    if (d6 >= 0.0 && d5 <= d6) return V2;
    
    double vb = d5 * d2 - d1 * d6;
    if (vb <= 0.0 && d2 >= 0.0 && d6 <= 0.0)
    {
        double w = d2 / (d2 - d6);
        return V0 + ac * w;
    }
    
    double va = d3 * d6 - d5 * d4;
    if (va <= 0.0 && (d4 - d3) >= 0.0 && (d5 - d6) >= 0.0)
    {
        double w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
        return V1 + (V2 - V1) * w;
    }
    
    double denom = 1.0 / (va + vb + vc);
    double v = vb * denom;
    double w = vc * denom;
    return V0 + ab * v + ac * w;
}

bool ICellTriangle::IsDegenerate(const FVector& V0, const FVector& V1, const FVector& V2)
{
    float Area2 = (V1 - V0).Cross(V2 - V0).SizeSquared();
    return Area2 < 1e-12f;
}

bool ICellTriangle::IsDegenerate(const FVector3d& V0, const FVector3d& V1, const FVector3d& V2)
{
    double Area2 = (V1 - V0).Cross(V2 - V0).SizeSquared();
    return Area2 < 1e-24;
}
