#include "Mesh/Mesh.h"
#include "Container/CellArray.h"
#include "Field/Field.h"

// ============================================================================
// 构造函数和析构函数
// ============================================================================

IMesh::IMesh()
    : MeshName("UnnamedMesh")
    , bIsValid(false)
{
    Cells = MakeUnique<FCellArray>();
}

IMesh::IMesh(const std::string& InMeshName)
    : MeshName(InMeshName)
    , bIsValid(false)
{
    Cells = MakeUnique<FCellArray>();
}

IMesh::IMesh(const IMesh& Other)
    : VerticesPositions(Other.VerticesPositions)
    , MeshName(Other.MeshName)
    , bIsValid(Other.bIsValid)
{
    // 拷贝单元数组
    if (Other.Cells)
    {
        Cells = MakeUnique<FCellArray>(*Other.Cells);
    }
    else
    {
        Cells = MakeUnique<FCellArray>();
    }
    
    // 拷贝顶点场
    for (const auto& Pair : Other.VertexFields)
    {
        if (Pair.second)
        {
            // 创建新的 FField 对象并使用拷贝赋值运算符复制数据
            VertexFields[Pair.first] = MakeUnique<FField>();
            *VertexFields[Pair.first] = *Pair.second;
        }
    }
    
    // 拷贝单元场
    for (const auto& Pair : Other.CellFields)
    {
        if (Pair.second)
        {
            // 创建新的 FField 对象并使用拷贝赋值运算符复制数据
            CellFields[Pair.first] = MakeUnique<FField>();
            *CellFields[Pair.first] = *Pair.second;
        }
    }
}

IMesh::IMesh(IMesh&& Other) noexcept
    : VerticesPositions(std::move(Other.VerticesPositions))
    , Cells(std::move(Other.Cells))
    , VertexFields(std::move(Other.VertexFields))
    , CellFields(std::move(Other.CellFields))
    , MeshName(std::move(Other.MeshName))
    , bIsValid(Other.bIsValid)
{
    Other.bIsValid = false;
}

IMesh& IMesh::operator=(const IMesh& Other)
{
    if (this != &Other)
    {
        VerticesPositions = Other.VerticesPositions;
        MeshName = Other.MeshName;
        bIsValid = Other.bIsValid;
        
        // 拷贝单元数组
        if (Other.Cells)
        {
            Cells = MakeUnique<FCellArray>(*Other.Cells);
        }
        else
        {
            Cells = MakeUnique<FCellArray>();
        }
        
        // 清空现有场数据
        VertexFields.Clear();
        CellFields.Clear();
        
        // 拷贝顶点场
        for (const auto& Pair : Other.VertexFields)
        {
            if (Pair.second)
            {
                VertexFields[Pair.first] = MakeUnique<FField>();
                *VertexFields[Pair.first] = *Pair.second;
            }
        }
        
        // 拷贝单元场
        for (const auto& Pair : Other.CellFields)
        {
            if (Pair.second)
            {
                CellFields[Pair.first] = MakeUnique<FField>();
                *CellFields[Pair.first] = *Pair.second;
            }
        }
    }
    return *this;
}

IMesh& IMesh::operator=(IMesh&& Other) noexcept
{
    if (this != &Other)
    {
        VerticesPositions = std::move(Other.VerticesPositions);
        Cells = std::move(Other.Cells);
        VertexFields = std::move(Other.VertexFields);
        CellFields = std::move(Other.CellFields);
        MeshName = std::move(Other.MeshName);
        bIsValid = Other.bIsValid;
        
        Other.bIsValid = false;
    }
    return *this;
}

// ============================================================================
// 几何数据操作（实现IMeshBase接口）
// ============================================================================

uint32 IMesh::GetVertexCount() const
{
    return VerticesPositions.Num();
}

FVector IMesh::GetVertexPosition(uint32 Index) const
{
    if (IsValidVertexIndex(Index))
    {
        return VerticesPositions[Index];
    }
    return FVector::ZeroVector();
}

const FVector* IMesh::GetVerticesPositionsPtr() const
{
    return VerticesPositions.GetData();
}

bool IMesh::IsValidVertexIndex(uint32 Index) const
{
    return Index < static_cast<uint32>(VerticesPositions.Num());
}

// ============================================================================
// 几何数据操作（扩展接口）
// ============================================================================

void IMesh::AddVertexPosition(const FVector& Vertex)
{
    VerticesPositions.Add(Vertex);
}

void IMesh::AddVertexPosition(float X, float Y, float Z)
{
    VerticesPositions.Emplace(X, Y, Z);
}

void IMesh::AddVerticesPositions(const TArray<FVector>& InVerticesPositions)
{
    VerticesPositions.Append(InVerticesPositions);
}

void IMesh::AddVerticesPositions(TArray<FVector>&& InVerticesPositions)
{
    VerticesPositions.Append(std::move(InVerticesPositions));
}

void IMesh::SetVertexPosition(uint32 Index, const FVector& Vertex)
{
    if (IsValidVertexIndex(Index))
    {
        VerticesPositions[Index] = Vertex;
    }
}

void IMesh::SetVertexPosition(uint32 Index, float X, float Y, float Z)
{
    if (IsValidVertexIndex(Index))
    {
        VerticesPositions[Index] = {X, Y, Z};
    }
}

const TArray<FVector>& IMesh::GetVerticesPositions() const
{
    return VerticesPositions;
}

// ============================================================================
// 拓扑数据操作（实现IMeshBase接口）
// ============================================================================

uint32 IMesh::GetCellCount() const
{
    if (Cells)
    {
        return Cells->GetCellCount();
    }
    return 0;
}

FCellArray& IMesh::GetCells()
{
    if (!Cells)
    {
        Cells = MakeUnique<FCellArray>();
    }
    return *Cells;
}

const FCellArray& IMesh::GetCells() const
{
    if (!Cells)
    {
        // 返回一个静态的空对象，但这不太安全
        // 更好的做法是确保 Cells 总是被初始化
        static FCellArray EmptyCells;
        return EmptyCells;
    }
    return *Cells;
}

bool IMesh::IsValidCellIndex(uint32 Index) const
{
    if (Cells)
    {
        return Cells->IsValidCellIndex(static_cast<FCellArray::CellIndexType>(Index));
    }
    return false;
}

// ============================================================================
// 场数据操作（实现IMeshBase接口）
// ============================================================================

FField* IMesh::GetField(const std::string& FieldName)
{
    // 先查找顶点场
    TUniquePtr<FField>* VertexFieldPtr = VertexFields.Find(FieldName);
    if (VertexFieldPtr != nullptr && VertexFieldPtr->IsValid())
    {
        return VertexFieldPtr->Get();
    }
    
    // 再查找单元场
    TUniquePtr<FField>* CellFieldPtr = CellFields.Find(FieldName);
    if (CellFieldPtr != nullptr && CellFieldPtr->IsValid())
    {
        return CellFieldPtr->Get();
    }
    
    return nullptr;
}

const FField* IMesh::GetField(const std::string& FieldName) const
{
    // 先查找顶点场
    const TUniquePtr<FField>* VertexFieldPtr = VertexFields.Find(FieldName);
    if (VertexFieldPtr != nullptr && VertexFieldPtr->IsValid())
    {
        return VertexFieldPtr->Get();
    }
    
    // 再查找单元场
    const TUniquePtr<FField>* CellFieldPtr = CellFields.Find(FieldName);
    if (CellFieldPtr != nullptr && CellFieldPtr->IsValid())
    {
        return CellFieldPtr->Get();
    }
    
    return nullptr;
}

bool IMesh::HasField(const std::string& FieldName) const
{
    return HasVertexField(FieldName) || HasCellField(FieldName);
}

FField* IMesh::GetVertexField(const std::string& FieldName)
{
    TUniquePtr<FField>* FieldPtr = VertexFields.Find(FieldName);
    if (FieldPtr != nullptr && FieldPtr->IsValid())
    {
        return FieldPtr->Get();
    }
    return nullptr;
}

const FField* IMesh::GetVertexField(const std::string& FieldName) const
{
    const TUniquePtr<FField>* FieldPtr = VertexFields.Find(FieldName);
    if (FieldPtr != nullptr && FieldPtr->IsValid())
    {
        return FieldPtr->Get();
    }
    return nullptr;
}

FField* IMesh::GetCellField(const std::string& FieldName)
{
    TUniquePtr<FField>* FieldPtr = CellFields.Find(FieldName);
    if (FieldPtr != nullptr && FieldPtr->IsValid())
    {
        return FieldPtr->Get();
    }
    return nullptr;
}

const FField* IMesh::GetCellField(const std::string& FieldName) const
{
    const TUniquePtr<FField>* FieldPtr = CellFields.Find(FieldName);
    if (FieldPtr != nullptr && FieldPtr->IsValid())
    {
        return FieldPtr->Get();
    }
    return nullptr;
}

bool IMesh::HasVertexField(const std::string& FieldName) const
{
    const TUniquePtr<FField>* FieldPtr = VertexFields.Find(FieldName);
    return FieldPtr != nullptr && FieldPtr->IsValid();
}

bool IMesh::HasCellField(const std::string& FieldName) const
{
    const TUniquePtr<FField>* FieldPtr = CellFields.Find(FieldName);
    return FieldPtr != nullptr && FieldPtr->IsValid();
}

// ============================================================================
// 场数据操作（扩展接口）
// ============================================================================

bool IMesh::AddField(TUniquePtr<FField> Field)
{
    if (!Field || !Field->IsValid())
    {
        return false;
    }
    
    const std::string& FieldName = Field->GetFieldName();
    EFieldAttachment Attachment = Field->GetAttachment();
    
    // 检查是否已存在同名场
    if (Attachment == EFieldAttachment::Vertex)
    {
        if (HasVertexField(FieldName))
        {
            return false;
        }
        VertexFields[FieldName] = std::move(Field);
    }
    else if (Attachment == EFieldAttachment::Cell)
    {
        if (HasCellField(FieldName))
        {
            return false;
        }
        CellFields[FieldName] = std::move(Field);
    }
    else
    {
        return false;
    }
    
    return true;
}

void IMesh::SetField(TUniquePtr<FField> Field)
{
    if (!Field || !Field->IsValid())
    {
        return;
    }
    
    const std::string& FieldName = Field->GetFieldName();
    EFieldAttachment Attachment = Field->GetAttachment();
    
    if (Attachment == EFieldAttachment::Vertex)
    {
        VertexFields[FieldName] = std::move(Field);
    }
    else if (Attachment == EFieldAttachment::Cell)
    {
        CellFields[FieldName] = std::move(Field);
    }
}

bool IMesh::RemoveField(const std::string& FieldName)
{
    bool Removed = false;
    
    // 尝试从顶点场删除
    if (VertexFields.Contains(FieldName))
    {
        VertexFields.Remove(FieldName);
        Removed = true;
    }
    
    // 尝试从单元场删除
    if (CellFields.Contains(FieldName))
    {
        CellFields.Remove(FieldName);
        Removed = true;
    }
    
    return Removed;
}

bool IMesh::RemoveVertexField(const std::string& FieldName)
{
    if (VertexFields.Contains(FieldName))
    {
        VertexFields.Remove(FieldName);
        return true;
    }
    return false;
}

bool IMesh::RemoveCellField(const std::string& FieldName)
{
    if (CellFields.Contains(FieldName))
    {
        CellFields.Remove(FieldName);
        return true;
    }
    return false;
}

void IMesh::GetVertexFieldNames(TArray<std::string>& OutFieldNames) const
{
    OutFieldNames.Empty();
    for (const auto& Pair : VertexFields)
    {
        OutFieldNames.Add(Pair.first);
    }
}

void IMesh::GetCellFieldNames(TArray<std::string>& OutFieldNames) const
{
    OutFieldNames.Empty();
    for (const auto& Pair : CellFields)
    {
        OutFieldNames.Add(Pair.first);
    }
}

void IMesh::GetAllFieldNames(TArray<std::string>& OutFieldNames) const
{
    OutFieldNames.Empty();
    
    // 添加顶点场名称
    for (const auto& Pair : VertexFields)
    {
        OutFieldNames.Add(Pair.first);
    }
    
    // 添加单元场名称
    for (const auto& Pair : CellFields)
    {
        OutFieldNames.Add(Pair.first);
    }
}

// ============================================================================
// 元数据操作（实现IMeshBase接口）
// ============================================================================

const std::string& IMesh::GetMeshName() const
{
    return MeshName;
}

bool IMesh::IsValid() const
{
    return bIsValid;
}

// ============================================================================
// 元数据操作（扩展接口）
// ============================================================================

void IMesh::SetMeshName(const std::string& InName)
{
    MeshName = InName;
}

// ============================================================================
// 数据验证（实现IMeshBase接口）
// ============================================================================

bool IMesh::Validate() const
{
    // 检查单元数组是否有效
    if (!Cells)
    {
        return false;
    }
    
    // 检查单元索引是否有效
    uint32 CellCount = Cells->GetCellCount();
    for (uint32 i = 0; i < CellCount; ++i)
    {
        if (!Cells->IsValidCellIndex(static_cast<FCellArray::CellIndexType>(i)))
        {
            return false;
        }
        
        // 检查单元中的顶点索引是否有效
        FCellInfo CellInfo;
        if (Cells->GetCell(static_cast<FCellArray::CellIndexType>(i), CellInfo))
        {
            for (int32 VertexIndex : CellInfo.VertexIndices)
            {
                if (VertexIndex < 0 || !IsValidVertexIndex(static_cast<uint32>(VertexIndex)))
                {
                    return false;
                }
            }
        }
    }
    
    // 检查顶点场数据数量是否匹配顶点数
    uint32 VertexCount = GetVertexCount();
    for (const auto& Pair : VertexFields)
    {
        if (Pair.second && Pair.second->IsValid())
        {
            if (Pair.second->GetDataCount() != VertexCount)
            {
                return false;
            }
        }
    }
    
    // 检查单元场数据数量是否匹配单元数
    for (const auto& Pair : CellFields)
    {
        if (Pair.second && Pair.second->IsValid())
        {
            if (Pair.second->GetDataCount() != CellCount)
            {
                return false;
            }
        }
    }
    
    return true;
}

// ============================================================================
// 清空和重置（实现IMeshBase接口）
// ============================================================================

void IMesh::Clear()
{
    VerticesPositions.Empty();
    if (Cells)
    {
        Cells->Clear();
    }
    VertexFields.Empty();
    CellFields.Empty();
    bIsValid = false;
}

void IMesh::Reset()
{
    Clear();
    MeshName = "UnnamedMesh";
    if (!Cells)
    {
        Cells = MakeUnique<FCellArray>();
    }
}

// ============================================================================
// 内存管理
// ============================================================================

void IMesh::ReserveVerticesPositions(uint32 Capacity)
{
    VerticesPositions.Reserve(Capacity);
}

void IMesh::ReserveCells(uint32 Capacity)
{
    if (!Cells)
    {
        Cells = MakeUnique<FCellArray>();
    }
    Cells->Reserve(Capacity);
}

void IMesh::Shrink()
{
    VerticesPositions.Shrink();
    if (Cells)
    {
        Cells->Shrink();
    }
    
    // 场数据的内存收缩需要遍历所有场
    for (auto& Pair : VertexFields)
    {
        if (Pair.second)
        {
            // FField 可能没有 Shrink 方法，这里先不处理
        }
    }
    
    for (auto& Pair : CellFields)
    {
        if (Pair.second)
        {
            // FField 可能没有 Shrink 方法，这里先不处理
        }
    }
}
