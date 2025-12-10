#include "Cell/CellPolyhedron.h"

ECellType ICellPolyhedron::GetCellType() const
{
    return ECellType::Polyhedron;
}

int32 ICellPolyhedron::GetVertexCount() const
{
    return -1;
}

int32 ICellPolyhedron::GetCellDimension() const
{
    return 3;
}

// ============================================================================
// 静态辅助函数
// ============================================================================

FVector ICellPolyhedron::ComputeCenter(const TArray<FVector>& Vertices)
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

FVector3d ICellPolyhedron::ComputeCenter(const TArray<FVector3d>& Vertices)
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