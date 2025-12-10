#include "Cell/CellHex.h"

ECellType ICellHex::GetCellType() const
{
    return ECellType::Hex;
}

int32 ICellHex::GetVertexCount() const
{
    return 8;
}

int32 ICellHex::GetCellDimension() const
{
    return 3;
}

// ============================================================================
// 静态辅助函数
// ============================================================================

FVector ICellHex::ComputeCenter(const TArray<FVector>& Vertices)
{
    int32 Count = Vertices.Num();
    if (Count == 0)
    {
        return FVector::ZeroVector();
    }

    FVector Sum = FVector::ZeroVector();
    for (const auto& V : Vertices)
    {
        Sum += V;
    }
    return Sum / static_cast<float>(Count);
}

FVector3d ICellHex::ComputeCenter(const TArray<FVector3d>& Vertices)
{
    int32 Count = Vertices.Num();
    if (Count == 0)
    {
        return FVector3d::ZeroVector();
    }

    FVector3d Sum = FVector3d::ZeroVector();
    for (const auto& V : Vertices)
    {
        Sum += V;
    }
    return Sum / static_cast<double>(Count);
}