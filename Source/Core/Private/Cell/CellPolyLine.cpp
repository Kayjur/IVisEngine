#include "Cell/CellPolyLine.h"

ECellType ICellPolyLine::GetCellType() const
{
    return ECellType::PolyLine;
}

int32 ICellPolyLine::GetVertexCount() const
{
    return -1;
}

int32 ICellPolyLine::GetCellDimension() const
{
    return 1;
}

// ============================================================================
// 静态辅助函数
// ============================================================================

float ICellPolyLine::ComputeLength(const TArray<FVector>& Vertices)
{
    int32 Count = Vertices.Num();
    if (Count < 2)
    {
        return 0.0f;
    }

    float Length = 0.0f;
    for (int32 i = 0; i < Count - 1; ++i)
    {
        Length += (Vertices[i + 1] - Vertices[i]).Size();
    }
    return Length;
}

double ICellPolyLine::ComputeLength(const TArray<FVector3d>& Vertices)
{
    int32 Count = Vertices.Num();
    if (Count < 2)
    {
        return 0.0;
    }

    double Length = 0.0;
    for (int32 i = 0; i < Count - 1; ++i)
    {
        Length += (Vertices[i + 1] - Vertices[i]).Size();
    }
    return Length;
}

FVector ICellPolyLine::ComputeCenter(const TArray<FVector>& Vertices)
{
    int32 Count = Vertices.Num();
    if (Count == 0)
    {
        return FVector::ZeroVector();
    }

    FVector Sum = FVector::ZeroVector();
    for (const auto& Vertex : Vertices)
    {
        Sum += Vertex;
    }
    return Sum / static_cast<float>(Count);
}

FVector3d ICellPolyLine::ComputeCenter(const TArray<FVector3d>& Vertices)
{
    int32 Count = Vertices.Num();
    if (Count == 0)
    {
        return FVector3d::ZeroVector();
    }

    FVector3d Sum = FVector3d::ZeroVector();
    for (const auto& Vertex : Vertices)
    {
        Sum += Vertex;
    }
    return Sum / static_cast<double>(Count);
}