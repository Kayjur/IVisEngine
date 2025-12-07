#pragma once

#include "Container/Array.h"
#include "Math/Vector.h"
#include "HAL/Platform.h"
#include <string>

#include "Math/MathType.h"

/**
 * 场数据绑定位置
 */
enum class EFieldAttachment : uint8
{
    Vertex,     // 绑定到顶点（节点场）
    Cell        // 绑定到单元（单元场）
};

/**
 * 场数据类型
 */
enum class EFieldType : uint8
{
    Custom,     // 自定义类型
    Scalar,     // 标量场（如温度、压力）- 每个元素 1 个 double
    Vector,     // 向量场（如位移、速度）- 每个元素 3 个 double
    Tensor,     // 张量场（如应力、应变）- 每个元素 9 个 double (3x3 矩阵)
};

/**
 * FField - 场数据类
 * 
 * 设计特点：
 * 1. 统一使用连续存储：所有数据存储在 TArray<double> 中
 * 2. 支持标量、向量、张量三种数据类型
 * 3. 明确区分节点场和单元场
 * 4. 提供统一的数据访问接口
 * 5. 支持获取底层指针（用于 GPU 上传等）
 * 
 * 存储格式：
 * - Scalar: [v0, v1, v2, ...] (每个元素 1 个 double)
 * - Vector: [x0, y0, z0, x1, y1, z1, ...] (每个元素 3 个 double)
 * - Tensor: [t0_00, t0_01, ..., t0_22, t1_00, ...] (每个元素 9 个 double，按行优先)
 */
class FField
{
private:
    /** 场名称 */
    std::string FieldName;

    /** 数据数组（连续存储） */
    TArray<double> Data;
    
    /** 数据数量（节点数或单元数） */
    uint32 DataCount;

    /** 属性维度 */
    uint32 FieldDimension;

    /** 场类型（标量/向量/张量） */
    EFieldType FieldType;

    /** 绑定位置（顶点/单元） */
    EFieldAttachment Attachment;

    /** 是否有效 */
    bool bIsValid;

public:
    /** 构造函数 */
    FField();
    FField(const std::string& InFieldName, EFieldType InFieldType, EFieldAttachment InAttachment, uint32 FieldDimension = 1);
    
    /** 析构函数 */
    ~FField() = default;

    void Initialize(const std::string& InFieldName, EFieldType InFieldType, EFieldAttachment InAttachment, uint32 FieldDimension = 1);

    // ============================================================================
    // Getter操作
    // ============================================================================

    /** 获取场名称 */
    const std::string& GetFieldName() const;
    
    /** 获取场类型 */
    EFieldType GetFieldType() const;
    
    /** 获取绑定位置 */
    EFieldAttachment GetAttachment() const;
    
    /** 检查是否有效 */
    bool IsValid() const;
    
    /** 获取数据数量（节点数或单元数） */
    uint32 GetDataCount() const;
    
    /** 检查索引是否有效 */
    bool IsValidIndex(uint32 Index) const;
    
    /** 获取每个元素包含的 double 数量 */
    uint32 GetFieldDimension() const;
    
    /** 获取底层数据指针（用于 GPU 上传等） */
    const double* GetRawDataPtr() const;
    
    /** 获取底层数据大小（字节） */
    size_t GetRawDataSize() const;

    // ============================================================================
    // 标量场操作
    // ============================================================================
    
    /** 设置标量数据 */
    void SetScalarData(const TArray<double>& InData);
    void SetScalarData(TArray<double>&& InData) noexcept;

    /** 添加标量值 */
    void AddScalar(double Value);
    
    /** 获取指定索引的标量值 */
    double GetScalar(uint32 Index) const;

    /** 设置指定索引的标量值 */
    void SetScalar(uint32 Index, double Value);
    
    // ============================================================================
    // 向量场操作
    // ============================================================================
    
    /** 设置向量数据 */
    void SetVectorData(const TArray<double>& InData);
    void SetVectorData(TArray<double>&& InData) noexcept;

    /** 添加向量值 */
    void AddVector(const FVector3d& Value);
    
    /** 获取指定索引的向量值 */
    FVector3d GetVector(uint32 Index) const;

    /** 设置指定索引的向量值 */
    void SetVector(uint32 Index, const FVector3d& Value);
    
    // ============================================================================
    // 张量场操作
    // ============================================================================
    
    /** 设置张量数据（每个张量是 9 个 double，3x3 矩阵按行优先） */
    void SetTensorData(const TArray<double>& InData);
    void SetTensorData(TArray<double>&& InData);

    /** 添加张量数据（9 个 double，3x3 矩阵按行优先） */
    void AddTensor(const TArray<double>& Value);
    
    /** 获取指定索引的张量数据指针 */
    void GetTensor(uint32 Index, TArray<double>& OutValue) const;

    /** 设置指定索引的张量数据 */
    void SetTensor(uint32 Index, const TArray<double>& Value);
    
    // ============================================================================
    // 通用操作
    // ============================================================================

    /** 设置场数据 */
    void SetFieldData(const TArray<double>& InData, uint32 InFieldDimension);
    void SetFieldData(TArray<double>&& InData, uint32 InFieldDimension);

    /** 添加场数据 */
    void AddData(const TArray<double>& Value);

    /** 获取指定索引的场数据 */
    void GetData(uint32 Index, TArray<double>& OutValue) const;

    /** 设置指定索引的张量数据 */
    void SetData(uint32 Index, const TArray<double>& Value);

    /** 获取所有数据 */
    TArray<double>& GetFieldData();
    const TArray<double>& GetFieldData() const;

    /** 清空所有数据 */
    void Clear();

    /** 预留容量，实际大小与 FieldDimension 相关 */
    void Reserve(uint32 Capacity);

    /** 设置大小，实际大小与 FieldDimension 相关 */
    void Resize(uint32 Size);
};

