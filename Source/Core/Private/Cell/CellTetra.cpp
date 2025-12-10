#include "Cell/CellTetra.h"

ECellType ICellTetra::GetCellType() const
{
    return ECellType::Tetra;
}

int32 ICellTetra::GetVertexCount() const
{
    return 4;
}

int32 ICellTetra::GetCellDimension() const
{
    return 3;
}

// ============================================================================
// 静态辅助函数
// ============================================================================

float ICellTetra::ComputeVolume(const FVector& V0, const FVector& V1, const FVector& V2, const FVector& V3)
{
    // 标量三重积: (V1 - V0) . ((V2 - V0) x (V3 - V0))
    // Volume = |det(A, B, C)| / 6
    FVector A = V1 - V0;
    FVector B = V2 - V0;
    FVector C = V3 - V0;
    
    return FMath::Abs(A.Dot(B.Cross(C))) / 6.0f;
}

double ICellTetra::ComputeVolume(const FVector3d& V0, const FVector3d& V1, const FVector3d& V2, const FVector3d& V3)
{
    FVector3d A = V1 - V0;
    FVector3d B = V2 - V0;
    FVector3d C = V3 - V0;
    
    return FMath::Abs(A.Dot(B.Cross(C))) / 6.0;
}

FVector ICellTetra::ComputeCenter(const FVector& V0, const FVector& V1, const FVector& V2, const FVector& V3)
{
    return (V0 + V1 + V2 + V3) * 0.25f;
}

FVector3d ICellTetra::ComputeCenter(const FVector3d& V0, const FVector3d& V1, const FVector3d& V2, const FVector3d& V3)
{
    return (V0 + V1 + V2 + V3) * 0.25;
}