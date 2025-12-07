#include "Field/Field.h"
#include "Exception/Exception.h"

// ============================================================================
// 构造函数
// ============================================================================

FField::FField()
    : DataCount(0)
    , FieldDimension(1)
    , FieldType(EFieldType::Custom)
    , Attachment(EFieldAttachment::Vertex)
    , bIsValid(false)
{
}

FField::FField(const std::string& InFieldName, EFieldType InFieldType, EFieldAttachment InAttachment, uint32 InFieldDimension)
{
    Initialize(InFieldName, InFieldType, InAttachment, InFieldDimension);
}

void FField::Initialize(const std::string &InFieldName, EFieldType InFieldType, EFieldAttachment InAttachment,
    uint32 InFieldDimension)
{
    FieldName = InFieldName;
    FieldType = InFieldType;
    Attachment = InAttachment;
    DataCount = 0;
    bIsValid = true;
    FieldDimension = 1;
    // 根据场类型设置默认维度
    switch (InFieldType)
    {
        case EFieldType::Custom:
            FieldDimension = InFieldDimension;
            break;
        case EFieldType::Scalar:
            FieldDimension = 1;
            break;
        case EFieldType::Vector:
            FieldDimension = 3;
            break;
        case EFieldType::Tensor:
            FieldDimension = 9;
            break;
    }
}

// ============================================================================
// Getter 方法
// ============================================================================

const std::string& FField::GetFieldName() const
{
    return FieldName;
}

EFieldType FField::GetFieldType() const
{
    return FieldType;
}

EFieldAttachment FField::GetAttachment() const
{
    return Attachment;
}

bool FField::IsValid() const
{
    return bIsValid;
}

uint32 FField::GetDataCount() const
{
    if (FieldDimension == 0)
    {
        return 0;
    }
    return DataCount;
}

bool FField::IsValidIndex(uint32 Index) const
{
    return Index < DataCount;
}

uint32 FField::GetFieldDimension() const
{
    return FieldDimension;
}

const double* FField::GetRawDataPtr() const
{
    return Data.GetData();
}

size_t FField::GetRawDataSize() const
{
    return Data.Num() * sizeof(double);
}

// ============================================================================
// 标量场操作
// ============================================================================

void FField::SetScalarData(const TArray<double>& InData)
{
    if (FieldType != EFieldType::Scalar)
    {
        THROW_EXCEPTION(FInvalidOperationException, "Not a Scalar Data");
    }

    Data = InData;
    DataCount = static_cast<uint32>(InData.Num());
}

void FField::SetScalarData(TArray<double> &&InData)
{
    if (FieldType != EFieldType::Scalar)
    {
        THROW_EXCEPTION(FInvalidOperationException, "Not a Scalar Data");
    }

    Data = std::move(InData);
    DataCount = static_cast<uint32>(Data.Num());
}

void FField::AddScalar(double Value)
{
    Data.Add(Value);
    DataCount = static_cast<uint32>(Data.Num());
}

double FField::GetScalar(uint32 Index) const
{
    return Data[Index];
}

void FField::SetScalar(uint32 Index, double Value)
{
    Data[Index] = Value;
}

// ============================================================================
// 向量场操作
// ============================================================================

void FField::SetVectorData(const TArray<double>& InData)
{
    if (FieldType != EFieldType::Vector)
    {
        THROW_EXCEPTION(FInvalidOperationException, "Not a Vector Data");
    }

    // 验证数据量必须是 3 的倍数
    if (InData.Num() % 3 != 0)
    {
        THROW_EXCEPTION(FInvalidOperationException, "DataCount must be a multiple of 3");
    }

    Data = InData;
    DataCount = static_cast<uint32>(InData.Num() / 3);
}

void FField::SetVectorData(TArray<double>&& InData)
{
    if (FieldType != EFieldType::Vector)
    {
        THROW_EXCEPTION(FInvalidOperationException, "Not a Vector Data");
    }

    // 验证数据量必须是 3 的倍数
    if (Data.Num() % 3 != 0)
    {
        THROW_EXCEPTION(FInvalidOperationException, "DataCount must be a multiple of 3");
    }

    Data = std::move(InData);
    DataCount = static_cast<uint32>(Data.Num() / 3);
}

void FField::AddVector(const FVector3d& Value)
{
    Data.Add(Value.X);
    Data.Add(Value.Y);
    Data.Add(Value.Z);
    DataCount = static_cast<uint32>(Data.Num() / 3);
}

FVector3d FField::GetVector(uint32 Index) const
{
    const uint32 Offset = Index * 3;
    return FVector3d{Data[Offset], Data[Offset + 1], Data[Offset + 2]};
}

void FField::SetVector(uint32 Index, const FVector3d& Value)
{
    uint32 Offset = Index * 3;

    Data[Offset] = Value.X;
    Data[Offset + 1] = Value.Y;
    Data[Offset + 2] = Value.Z;
}

// ============================================================================
// 张量场操作
// ============================================================================

void FField::SetTensorData(const TArray<double>& InData)
{
    if (FieldType != EFieldType::Tensor)
    {
        THROW_EXCEPTION(FInvalidOperationException, "Not a Tensor Data");
    }

    // 验证数据量必须是 9 的倍数
    if (InData.Num() % 9 != 0)
    {
        THROW_EXCEPTION(FInvalidOperationException, "DataCount must be a multiple of 9");
    }

    Data = InData;
    DataCount = static_cast<uint32>(InData.Num() / 9);
}

void FField::SetTensorData(TArray<double>&& InData)
{
    if (FieldType != EFieldType::Tensor)
    {
        THROW_EXCEPTION(FInvalidOperationException, "Not a Tensor Data");
    }

    // 验证数据量必须是 9 的倍数
    if (InData.Num() % 9 != 0)
    {
        THROW_EXCEPTION(FInvalidOperationException, "DataCount must be a multiple of 9");
    }

    Data = std::move(InData);
    DataCount = static_cast<uint32>(Data.Num() / 9);
}

void FField::AddTensor(const TArray<double>& Value)
{
    for (uint32 i = 0; i < 9; ++i)
    {
        Data.Add(Value[i]);
    }
    DataCount = static_cast<uint32>(Data.Num() / 9);
}

void FField::GetTensor(uint32 Index, TArray<double>& OutValue) const
{
    const uint32 Offset = Index * 9;

    OutValue.Reset();
    for (uint32 i = 0; i < 9; ++i)
    {
        OutValue.Add(Data[Offset + i]);
    }
}

void FField::SetTensor(uint32 Index, const TArray<double>& Value)
{
    const uint32 Offset = Index * 9;

    for (uint32 i = 0; i < 9; ++i)
    {
        Data[Offset + i] = Value[i];
    }
}

// ============================================================================
// 通用操作
// ============================================================================

void FField::SetFieldData(const TArray<double>& InData)
{
    // 验证数据量必须是 FieldDimension 的倍数
    if (InData.Num() % FieldDimension != 0)
    {
        THROW_EXCEPTION(FInvalidOperationException, "DataCount must be a multiple of FieldDimension");
    }

    Data = InData;
    DataCount = static_cast<uint32>(InData.Num() / FieldDimension);
}

void FField::SetFieldData(TArray<double>&& InData)
{
    // 验证数据量必须是 FieldDimension 的倍数
    if (InData.Num() % FieldDimension != 0)
    {
        THROW_EXCEPTION(FInvalidOperationException, "DataCount must be a multiple of FieldDimension");
    }

    Data = std::move(InData);
    DataCount = static_cast<uint32>(Data.Num() / FieldDimension);
}

void FField::AddData(const TArray<double>& Value)
{
    for (uint32 i = 0; i < FieldDimension; ++i)
    {
        Data.Add(Value[i]);
    }
    
    DataCount = static_cast<uint32>(Data.Num() / FieldDimension);
}

void FField::GetData(uint32 Index, TArray<double>& OutValue) const
{
    const uint32 Offset = Index * FieldDimension;

    OutValue.Reset();
    for (uint32 i = 0; i < FieldDimension; ++i)
    {
        OutValue.Add(Data[Offset + i]);
    }
}

void FField::SetData(uint32 Index, const TArray<double>& Value)
{
    const uint32 Offset = Index * FieldDimension;

    for (uint32 i = 0; i < FieldDimension; ++i)
    {
        Data[Offset + i] = Value[i];
    }
}

TArray<double>& FField::GetFieldData()
{
    return Data;
}

const TArray<double>& FField::GetFieldData() const
{
    return Data;
}

void FField::Clear()
{
    Data.Clear();
    DataCount = 0;
}

void FField::Reset()
{
    Clear();
    FieldName.clear();
    FieldType = EFieldType::Custom;
    Attachment = EFieldAttachment::Vertex;
    FieldDimension = 1;
    bIsValid = false;
}

void FField::Reserve(uint32 Capacity)
{
    if (FieldDimension == 0)
    {
        return;
    }
    
    Data.Reserve(Capacity * FieldDimension);
    DataCount = static_cast<uint32>(Data.Num() / FieldDimension);
}

void FField::Resize(uint32 Size)
{
    if (FieldDimension == 0)
    {
        return;
    }

    Data.Resize(Size * FieldDimension);
    DataCount = static_cast<uint32>(Data.Num() / FieldDimension);
}
