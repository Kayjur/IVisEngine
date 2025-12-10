#include "Cell/CellPolygon.h"

ECellType ICellPolygon::GetCellType() const
{
    return ECellType::Polygon;
}

int32 ICellPolygon::GetVertexCount() const
{
    return -1; // 动态顶点数
}

int32 ICellPolygon::GetCellDimension() const
{
    return 2;
}

// ============================================================================
// 静态辅助函数
// ============================================================================

FVector ICellPolygon::ComputeNormal(const TArray<FVector>& Vertices)
{
    int32 Count = Vertices.Num();
    if (Count < 3) return FVector::UpVector(); // 至少需要3个点

    FVector Center = ComputeCenter(Vertices);
    FVector Normal = FVector::ZeroVector();

    for (int32 i = 0; i < Count; ++i)
    {
        const FVector& V0 = Vertices[i];
        const FVector& V1 = Vertices[(i + 1) % Count];
        Normal += ICellTriangle::ComputeNormal(Center, V0, V1);
    }

    return Normal.GetSafeNormal();
}

FVector3d ICellPolygon::ComputeNormal(const TArray<FVector3d>& Vertices)
{
    int32 Count = Vertices.Num();
    if (Count < 3) return FVector3d::UpVector();

    FVector3d Center = ComputeCenter(Vertices);
    FVector3d Normal = FVector3d::ZeroVector();

    for (int32 i = 0; i < Count; ++i)
    {
        const FVector3d& V0 = Vertices[i];
        const FVector3d& V1 = Vertices[(i + 1) % Count];
        Normal += ICellTriangle::ComputeNormal(Center, V0, V1);
    }

    return Normal.GetSafeNormal();
}

float ICellPolygon::ComputeArea(const TArray<FVector>& Vertices)
{
    int32 Count = Vertices.Num();
    if (Count < 3) return 0.0f;

    // 选择第一个点作为基点，分割为三角形扇
    const FVector& Base = Vertices[0];
    float Area = 0.0f;

    for (int32 i = 1; i < Count - 1; ++i)
    {
        Area += ICellTriangle::ComputeArea(Base, Vertices[i], Vertices[i + 1]);
    }

    return Area;
}

double ICellPolygon::ComputeArea(const TArray<FVector3d>& Vertices)
{
    int32 Count = Vertices.Num();
    if (Count < 3) return 0.0;

    const FVector3d& Base = Vertices[0];
    double Area = 0.0;

    for (int32 i = 1; i < Count - 1; ++i)
    {
        Area += ICellTriangle::ComputeArea(Base, Vertices[i], Vertices[i + 1]);
    }

    return Area;
}

FVector ICellPolygon::ComputeCenter(const TArray<FVector>& Vertices)
{
    int32 Count = Vertices.Num();
    if (Count == 0) return FVector::ZeroVector();

    FVector Sum = FVector::ZeroVector();
    for (const auto& V : Vertices)
    {
        Sum += V;
    }

    return Sum / static_cast<float>(Count);
}

FVector3d ICellPolygon::ComputeCenter(const TArray<FVector3d>& Vertices)
{
    int32 Count = Vertices.Num();
    if (Count == 0) return FVector3d::ZeroVector();

    FVector3d Sum = FVector3d::ZeroVector();
    for (const auto& V : Vertices)
    {
        Sum += V;
    }

    return Sum / static_cast<double>(Count);
}

bool ICellPolygon::IsConvex(const TArray<FVector>& Vertices, const FVector& InNormal)
{
    int32 Count = Vertices.Num();
    if (Count < 3) return false;
    if (Count == 3) return true; // 三角形总是凸的

    FVector Normal = InNormal;
    if (Normal.IsZero())
    {
        Normal = ComputeNormal(Vertices);
    }

    for (int32 i = 0; i < Count; ++i)
    {
        const FVector& V0 = Vertices[i];
        const FVector& V1 = Vertices[(i + 1) % Count];
        const FVector& V2 = Vertices[(i + 2) % Count];

        FVector Edge1 = V1 - V0;
        FVector Edge2 = V2 - V1;
        FVector Cross = Edge1.Cross(Edge2);

        if (Normal.Dot(Cross) < 0.0f)
        {
            return false;
        }
    }

    return true;
}

bool ICellPolygon::IsConvex(const TArray<FVector3d>& Vertices, const FVector3d& InNormal)
{
    int32 Count = Vertices.Num();
    if (Count < 3) return false;
    if (Count == 3) return true;

    FVector3d Normal = InNormal;
    if (Normal.IsZero())
    {
        Normal = ComputeNormal(Vertices);
    }

    for (int32 i = 0; i < Count; ++i)
    {
        const FVector3d& V0 = Vertices[i];
        const FVector3d& V1 = Vertices[(i + 1) % Count];
        const FVector3d& V2 = Vertices[(i + 2) % Count];

        FVector3d Edge1 = V1 - V0;
        FVector3d Edge2 = V2 - V1;
        FVector3d Cross = Edge1.Cross(Edge2);

        if (Normal.Dot(Cross) < 0.0)
        {
            return false;
        }
    }

    return true;
}
