#pragma once

#include <map>
#include <algorithm>
#include <stdexcept>

/**
 * TMap - 键值对映射容器类
 * 使用 UE 风格的接口命名，底层使用 std::map 实现
 * 
 * @tparam KeyType 键类型
 * @tparam ValueType 值类型
 */
template<typename KeyType, typename ValueType>
class TMap
{
public:
    // ============================================================================
    // 类型定义
    // ============================================================================
    using KeyTypeAlias = KeyType;
    using ValueTypeAlias = ValueType;
    using MapType = std::map<KeyType, ValueType>;
    using SizeType = typename MapType::size_type;
    using Iterator = typename MapType::iterator;
    using ConstIterator = typename MapType::const_iterator;
    using PairType = std::pair<const KeyType, ValueType>;

    // ============================================================================
    // 构造函数和析构函数
    // ============================================================================
    
    /** 默认构造函数 */
    TMap() = default;
    
    /** 使用初始化列表构造 */
    TMap(std::initializer_list<PairType> InitList) : Data(InitList) {}
    
    /** 拷贝构造函数 */
    TMap(const TMap& Other) : Data(Other.Data) {}
    
    /** 移动构造函数 */
    TMap(TMap&& Other) noexcept : Data(std::move(Other.Data)) {}
    
    /** 析构函数 */
    ~TMap() = default;

    // ============================================================================
    // 赋值操作符
    // ============================================================================
    
    /** 拷贝赋值 */
    TMap& operator=(const TMap& Other)
    {
        if (this != &Other)
        {
            Data = Other.Data;
        }
        return *this;
    }
    
    /** 移动赋值 */
    TMap& operator=(TMap&& Other) noexcept
    {
        if (this != &Other)
        {
            Data = std::move(Other.Data);
        }
        return *this;
    }

    // ============================================================================
    // 元素访问
    // ============================================================================
    
    /** 获取指定键的值（如果不存在则创建） */
    ValueType& operator[](const KeyType& Key) { return Data[Key]; }
    
    /** 获取指定键的值（检查存在性） */
    ValueType& At(const KeyType& Key) { return Data.at(Key); }
    const ValueType& At(const KeyType& Key) const { return Data.at(Key); }
    
    /** 查找元素，返回值的指针，未找到返回 nullptr */
    ValueType* Find(const KeyType& Key)
    {
        auto It = Data.find(Key);
        if (It != Data.end())
        {
            return &It->second;
        }
        return nullptr;
    }
    
    /** 查找元素，返回值的常量指针，未找到返回 nullptr */
    const ValueType* Find(const KeyType& Key) const
    {
        auto It = Data.find(Key);
        if (It != Data.end())
        {
            return &It->second;
        }
        return nullptr;
    }
    
    /** 查找或添加元素，如果不存在则使用默认值添加 */
    ValueType& FindOrAdd(const KeyType& Key)
    {
        auto It = Data.find(Key);
        if (It != Data.end())
        {
            return It->second;
        }
        return Data[Key] = ValueType{};
    }
    
    /** 查找或添加元素，如果不存在则使用指定值添加 */
    ValueType& FindOrAdd(const KeyType& Key, const ValueType& DefaultValue)
    {
        auto It = Data.find(Key);
        if (It != Data.end())
        {
            return It->second;
        }
        return Data[Key] = DefaultValue;
    }
    
    /** 查找或添加元素，如果不存在则使用移动语义添加 */
    ValueType& FindOrAdd(const KeyType& Key, ValueType&& DefaultValue)
    {
        auto It = Data.find(Key);
        if (It != Data.end())
        {
            return It->second;
        }
        return Data[Key] = std::move(DefaultValue);
    }

    // ============================================================================
    // 容量相关
    // ============================================================================
    
    /** 获取元素数量 */
    SizeType Num() const { return Data.size(); }

    /** 获取元素数量 */
    SizeType Size() const { return Data.size(); }

    /** 检查是否为空 */
    [[nodiscard]] bool IsEmpty() const { return Data.empty(); }

    // ============================================================================
    // 添加元素
    // ============================================================================
    
    /** 添加键值对 */
    void Add(const KeyType& Key, const ValueType& Value)
    {
        Data[Key] = Value;
    }
    
    /** 添加键值对（移动语义） */
    void Add(const KeyType& Key, ValueType&& Value)
    {
        Data[Key] = std::move(Value);
    }
    
    /** 添加键值对（如果不存在） */
    bool AddUnique(const KeyType& Key, const ValueType& Value)
    {
        if (!Contains(Key))
        {
            Data[Key] = Value;
            return true;
        }
        return false;
    }
    
    /** 添加键值对（如果不存在，移动语义） */
    bool AddUnique(const KeyType& Key, ValueType&& Value)
    {
        if (!Contains(Key))
        {
            Data[Key] = std::move(Value);
            return true;
        }
        return false;
    }
    
    /** 原地构造添加元素 */
    template<typename... Args>
    std::pair<Iterator, bool> Emplace(const KeyType& Key, Args&&... InArgs)
    {
        return Data.emplace(Key, std::forward<Args>(InArgs)...);
    }
    
    /** 原地构造添加元素（如果不存在） */
    template<typename... Args>
    std::pair<Iterator, bool> EmplaceUnique(const KeyType& Key, Args&&... InArgs)
    {
        if (!Contains(Key))
        {
            return Data.emplace(Key, std::forward<Args>(InArgs)...);
        }
        return std::make_pair(Data.find(Key), false);
    }

    // ============================================================================
    // 删除元素
    // ============================================================================
    
    /** 删除指定键的元素 */
    bool Remove(const KeyType& Key)
    {
        auto It = Data.find(Key);
        if (It != Data.end())
        {
            Data.erase(It);
            return true;
        }
        return false;
    }
    
    /** 删除指定键的元素（如果存在） */
    bool RemoveAndCopyValue(const KeyType& Key, ValueType& OutValue)
    {
        auto It = Data.find(Key);
        if (It != Data.end())
        {
            OutValue = It->second;
            Data.erase(It);
            return true;
        }
        return false;
    }
    
    /** 清空所有元素 */
    void Reset() { Data.clear(); }
    void Empty() { Reset(); }
    void Clear() { Reset(); }

    // ============================================================================
    // 查找操作
    // ============================================================================
    
    /** 检查是否包含指定键 */
    bool Contains(const KeyType& Key) const
    {
        return Data.contains(Key);
    }
    
    /** 查找元素，返回迭代器 */
    Iterator FindIterator(const KeyType& Key)
    {
        return Data.find(Key);
    }
    
    /** 查找元素，返回常量迭代器 */
    ConstIterator FindIterator(const KeyType& Key) const
    {
        return Data.find(Key);
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

    // ============================================================================
    // 其他操作
    // ============================================================================
    
    /** 交换两个映射 */
    void Swap(TMap& Other) noexcept
    {
        Data.swap(Other.Data);
    }
    
    /** 获取底层 map 的引用（用于高级操作） */
    MapType& GetData() { return Data; }
    const MapType& GetData() const { return Data; }

private:
    // 底层使用 std::map 存储数据
    MapType Data;
};
