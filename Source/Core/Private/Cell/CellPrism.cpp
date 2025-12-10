#include "Cell/CellPrism.h"
#include "Cell/CellTetra.h"

ECellType ICellPrism::GetCellType() const
{
    return ECellType::Prism;
}

int32 ICellPrism::GetVertexCount() const
{
    return 6;
}

int32 ICellPrism::GetCellDimension() const
{
    return 3;
}

// ============================================================================
// 静态辅助函数
// ============================================================================

float ICellPrism::ComputeVolume(const TArray<FVector>& Vertices)
{
    if (Vertices.Num() < 6) return 0.0f;

    // 分割为三个四面体: (0,1,2,3), (1,2,3,4), (2,3,4,5)
    return ICellTetra::ComputeVolume(Vertices[0], Vertices[1], Vertices[2], Vertices[3]) +
           ICellTetra::ComputeVolume(Vertices[1], Vertices[2], Vertices[3], Vertices[4]) +
           ICellTetra::ComputeVolume(Vertices[2], Vertices[3], Vertices[4], Vertices[5]);
}

double ICellPrism::ComputeVolume(const TArray<FVector3d>& Vertices)
{
    if (Vertices.Num() < 6) return 0.0;

    return ICellTetra::ComputeVolume(Vertices[0], Vertices[1], Vertices[2], Vertices[3]) +
           ICellTetra::ComputeVolume(Vertices[1], Vertices[2], Vertices[3], Vertices[4]) +
           ICellTetra::ComputeVolume(Vertices[2], Vertices[3], Vertices[4], Vertices[5]);
}

FVector ICellPrism::ComputeCenter(const TArray<FVector>& Vertices)
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

FVector3d ICellPrism::ComputeCenter(const TArray<FVector3d>& Vertices)
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