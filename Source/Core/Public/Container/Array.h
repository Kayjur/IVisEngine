#pragma once

#include <vector>
#include <algorithm>
#include <cstdint>
#include "HAL/Platform.h"

/**
 * TArray - 动态数组容器类
 * 使用 UE 风格的接口命名，底层使用 std::vector 实现
 * 
 * @tparam T 元素类型
 */
template<typename T>
class TArray
{
public:
    // ============================================================================
    // 类型定义
    // ============================================================================
    using ElementType = T;
    using SizeType = typename std::vector<T>::size_type;
    using Iterator = typename std::vector<T>::iterator;
    using ConstIterator = typename std::vector<T>::const_iterator;

    // ============================================================================
    // 构造函数和析构函数
    // ============================================================================
    
    /** 默认构造函数 */
    TArray() = default;
    
    /** 使用初始化列表构造 */
    TArray(std::initializer_list<T> InitList) : Data(InitList) {}
    
    /** 拷贝构造函数 */
    TArray(const TArray& Other) : Data(Other.Data) {}
    
    /** 移动构造函数 */
    TArray(TArray&& Other) noexcept : Data(std::move(Other.Data)) {}
    
    /** 析构函数 */
    ~TArray() = default;

    // ============================================================================
    // 赋值操作符
    // ============================================================================
    
    /** 拷贝赋值 */
    TArray& operator=(const TArray& Other)
    {
        if (this != &Other)
        {
            Data = Other.Data;
        }
        return *this;
    }
    
    /** 移动赋值 */
    TArray& operator=(TArray&& Other) noexcept
    {
        if (this != &Other)
        {
            Data = std::move(Other.Data);
        }
        return *this;
    }

    // ============================================================================
    // 算术运算
    // ============================================================================

    /** 数组加法 */
    TArray operator+(const TArray& Other) const
    {
        if (Num() == Other.Num())
        {
            TArray Result(*this);
            Result += Other;
            return Result;
        }
        return TArray{};
    }

    /** 数组减法 */
    TArray operator-(const TArray& Other) const
    {
        if (Num() == Other.Num())
        {
            TArray Result(*this);
            Result -= Other;
            return Result;
        }
        return TArray{};
    }

    /** 数组数乘 */
    TArray operator*(T Scalar) const
    {
        TArray Result(*this);
        Result *= Scalar;
        return Result;
    }

    /** 数组数除 */
    TArray operator/(T Scalar) const
    {
        TArray Result(*this);
        Result /= Scalar;
        return Result;
    }

    /** 数组取反 */
    TArray operator-() const
    {
        TArray Result(*this);
        for (int32 i = 0; i < Num(); ++i)
        {
            Result[i] = -Result[i];
        }
        return Result;
    }

    /** 数组加法赋值 */
    TArray& operator+=(const TArray& Other)
    {
        if (Num() == Other.Num())
        {
            for (int32 i = 0; i < Num(); ++i)
            {
                At(i) += Other[i];
            }
        }
        return *this;
    }

    /** 数组减法赋值 */
    TArray& operator-=(const TArray& Other)
    {
        if (Num() == Other.Num())
        {
            for (int32 i = 0; i < Num(); ++i)
            {
                At(i) -= Other[i];
            }
        }
        return *this;
    }

    /** 数组数乘赋值 */
    TArray& operator*=(T Scalar)
    {
        for (int32 i = 0; i < Num(); ++i)
        {
            At(i) *= Scalar;
        }
        return *this;
    }

    /** 数组数除赋值 */
    TArray& operator/=(T Scalar)
    {
        for (int32 i = 0; i < Num(); ++i)
        {
            At(i) /= Scalar;
        }
        return *this;
    }

    /** 数组相等比较 */
    bool operator==(const TArray& Other) const
    {
        for (int32 i = 0; i < Num(); ++i)
        {
            if (At(i) != Other.At(i))
            {
                return false;
            }
        }
        return true;
    }

    /** 数组不等比较 */
    bool operator!=(const TArray& Other) const
    {
        return !(*this == Other);
    }

    // ============================================================================
    // 元素访问
    // ============================================================================
    
    /** 获取指定索引的元素（不检查边界） */
    T& operator[](SizeType Index) { return Data[Index]; }
    const T& operator[](SizeType Index) const { return Data[Index]; }
    
    /** 获取指定索引的元素（检查边界） */
    T& At(SizeType Index) { return Data.at(Index); }
    const T& At(SizeType Index) const { return Data.at(Index); }
    
    /** 获取第一个元素 */
    T& First() { return Data.front(); }
    const T& First() const { return Data.front(); }
    
    /** 获取最后一个元素 */
    T& Last() { return Data.back(); }
    const T& Last() const { return Data.back(); }
    
    /** 获取底层数据指针 */
    T* GetData() { return Data.data(); }
    const T* GetData() const { return Data.data(); }

    // ============================================================================
    // 容量相关
    // ============================================================================
    
    /** 获取元素数量 */
    SizeType Num() const { return Data.size(); }

    /** 获取元素数量 */
    SizeType Size() const { return Data.size(); }
    
    /** 检查是否为空 */
    [[nodiscard]] bool IsEmpty() const { return Data.empty(); }

    /** 设置容量 */
    void Resize(SizeType Size) { Data.resize(Size); }

    /** 预留容量 */
    void Reserve(SizeType Capacity) { Data.reserve(Capacity); }
    
    /** 获取当前容量 */
    SizeType Capacity() const { return Data.capacity(); }
    
    /** 收缩容量以匹配大小 */
    void Shrink() { Data.shrink_to_fit(); }

    // ============================================================================
    // 索引检查
    // ============================================================================
    
    /** 检查索引是否有效 */
    bool IsValidIndex(SizeType Index) const { return Index < Num(); }

    // ============================================================================
    // 添加元素
    // ============================================================================
    
    /** 在末尾添加元素 */
    void Add(const T& Item) { Data.push_back(Item); }
    void Add(T&& Item) { Data.push_back(std::move(Item)); }

    template<typename... Args>
    void Emplace(Args&&... args) { Data.emplace_back(std::forward<Args>(args)...); }

    /** 在末尾添加元素（如果不存在） */
    void AddUnique(const T& Item)
    {
        if (!Contains(Item))
        {
            Add(Item);
        }
    }
    
    /** 在指定位置插入元素 */
    void Insert(SizeType Index, const T& Item)
    {
        if (Index > Num()) Index = Num();
        Data.insert(Data.begin() + Index, Item);
    }
    
    /** 在指定位置插入元素（移动语义） */
    void Insert(SizeType Index, T&& Item)
    {
        if (Index > Num()) Index = Num();
        Data.insert(Data.begin() + Index, std::move(Item));
    }
    
    /** 追加另一个数组的所有元素 */
    void Append(const TArray& Other)
    {
        Data.insert(Data.end(), Other.Data.begin(), Other.Data.end());
    }
    
    /** 追加另一个数组的所有元素（移动语义） */
    void Append(TArray&& Other)
    {
        Data.insert(Data.end(), 
                   std::make_move_iterator(Other.Data.begin()),
                   std::make_move_iterator(Other.Data.end()));
    }

    // ============================================================================
    // 删除元素
    // ============================================================================
    
    /** 删除指定索引的元素 */
    void RemoveAt(SizeType Index)
    {
        if (IsValidIndex(Index))
        {
            Data.erase(Data.begin() + Index);
        }
    }
    
    /** 删除指定范围的元素 */
    void RemoveAt(SizeType Index, SizeType Count)
    {
        if (IsValidIndex(Index))
        {
            SizeType EndIndex = std::min(Index + Count, Num());
            Data.erase(Data.begin() + Index, Data.begin() + EndIndex);
        }
    }
    
    /** 删除第一个匹配的元素 */
    bool Remove(const T& Item)
    {
        auto It = std::find(Data.begin(), Data.end(), Item);
        if (It != Data.end())
        {
            Data.erase(It);
            return true;
        }
        return false;
    }
    
    /** 删除所有匹配的元素 */
    SizeType RemoveAll(const T& Item)
    {
        SizeType RemovedCount = 0;
        auto It = std::remove(Data.begin(), Data.end(), Item);
        if (It != Data.end())
        {
            RemovedCount = Data.end() - It;
            Data.erase(It, Data.end());
        }
        return RemovedCount;
    }
    
    /** 删除最后一个元素 */
    void Pop() { if (!IsEmpty()) Data.pop_back(); }
    
    /** 清空所有元素 */
    void Reset() { Data.clear(); }
    void Empty() { Reset(); }
    void Clear() { Reset(); }

    // ============================================================================
    // 查找操作
    // ============================================================================
    
    /** 查找元素，返回索引，未找到返回 -1 */
    int32 Find(const T& Item) const
    {
        auto It = std::find(Data.begin(), Data.end(), Item);
        if (It != Data.end())
        {
            return static_cast<int32_t>(It - Data.begin());
        }
        return -1;
    }
    
    /** 从末尾查找元素，返回索引，未找到返回 -1 */
    int32 FindLast(const T& Item) const
    {
        auto It = std::find(Data.rbegin(), Data.rend(), Item);
        if (It != Data.rend())
        {
            return static_cast<int32_t>(Data.rend() - It - 1);
        }
        return -1;
    }
    
    /** 检查是否包含元素 */
    bool Contains(const T& Item) const
    {
        return std::find(Data.begin(), Data.end(), Item) != Data.end();
    }

    // ============================================================================
    // 迭代器
    // ============================================================================
    
    // UE 风格的大写方法
    Iterator Begin() { return Data.begin(); }
    ConstIterator Begin() const { return Data.begin(); }
    ConstIterator cBegin() const { return Data.cbegin(); }
    
    Iterator End() { return Data.end(); }
    ConstIterator End() const { return Data.end(); }
    ConstIterator cEnd() const { return Data.cend(); }

    // STL 风格的小写方法（用于范围 for 循环和 STL 算法）
    Iterator begin() { return Data.begin(); }
    ConstIterator begin() const { return Data.begin(); }
    ConstIterator cbegin() const { return Data.cbegin(); }

    Iterator end() { return Data.end(); }
    ConstIterator end() const { return Data.end(); }
    ConstIterator cend() const { return Data.cend(); }

private:
    // 底层使用 std::vector 存储数据
    std::vector<T> Data;
};
