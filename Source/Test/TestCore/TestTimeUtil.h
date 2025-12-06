#pragma once

#include <chrono>
#include <string>
#include <iostream>
#include "HAL/Platform.h"

/**
 * FTestTimeUtil - 测试计时工具类
 * 
 * 提供简单的计时功能，用于测试性能
 * 
 * 使用示例：
 *   // 方式1：手动控制
 *   FTestTimeUtil Timer;
 *   Timer.Start();
 *   // ... 执行代码 ...
 *   Timer.Stop();
 *   std::cout << "耗时: " << Timer.GetElapsedMilliseconds() << " ms" << std::endl;
 * 
 *   // 方式2：自动作用域计时（RAII）
 *   {
 *       FTestTimeUtil::ScopedTimer Scoped("测试操作");
 *       // ... 执行代码 ...
 *       // 析构时自动输出耗时
 *   }
 */
class FTimer
{
public:
    using Clock = std::chrono::steady_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    using Duration = std::chrono::steady_clock::duration;

    /**
     * 构造函数
     */
    FTimer()
        : StartTime()
        , EndTime()
        , bIsRunning(false)
    {}

    /**
     * 开始计时
     */
    void Start()
    {
        StartTime = Clock::now();
        bIsRunning = true;
    }

    /**
     * 停止计时
     */
    void Stop()
    {
        if (bIsRunning)
        {
            EndTime = Clock::now();
            bIsRunning = false;
        }
    }

    /**
     * 重置计时器
     */
    void Reset()
    {
        StartTime = TimePoint();
        EndTime = TimePoint();
        bIsRunning = false;
    }

    /**
     * 重新开始计时（等同于 Reset + Start）
     */
    void Restart()
    {
        Reset();
        Start();
    }

    /**
     * 获取经过的时间（毫秒）
     * @return 如果正在运行，返回从开始到现在的耗时；否则返回从开始到停止的耗时
     */
    double GetElapsedMilliseconds() const
    {
        TimePoint End = bIsRunning ? Clock::now() : EndTime;
        auto Duration = std::chrono::duration_cast<std::chrono::microseconds>(End - StartTime);
        return Duration.count() / 1000.0;
    }

    /**
     * 获取经过的时间（微秒）
     */
    int64 GetElapsedMicroseconds() const
    {
        TimePoint End = bIsRunning ? Clock::now() : EndTime;
        auto Duration = std::chrono::duration_cast<std::chrono::microseconds>(End - StartTime);
        return static_cast<int64>(Duration.count());
    }

    /**
     * 获取经过的时间（秒）
     */
    double GetElapsedSeconds() const
    {
        return GetElapsedMilliseconds() / 1000.0;
    }

    /**
     * 检查是否正在运行
     */
    bool IsRunning() const
    {
        return bIsRunning;
    }

    void Print() const
    {
        std::cout << "耗时: " << GetElapsedMilliseconds() << " ms" << std::endl;
    }

private:
    TimePoint StartTime;
    TimePoint EndTime;
    bool bIsRunning;
};

/**
 * 作用域计时器（RAII）
 * 在构造时开始计时，析构时自动停止并输出耗时
 */
class FScopedTimer
{
public:
    /**
     * 构造函数，开始计时
     * @param InLabel 计时标签（用于输出）
     */
    explicit FScopedTimer(const std::string& InLabel = "操作")
        : Timer()
        , Label(InLabel)
    {
        Timer.Start();
    }

    /**
     * 析构函数，自动停止并输出耗时
     */
    ~FScopedTimer()
    {
        Timer.Stop();
        double ElapsedMs = Timer.GetElapsedMilliseconds();
        std::cout << "[" << Label << "] 耗时: " << ElapsedMs << " ms" << std::endl;
    }

    /**
     * 获取当前耗时（不停止计时）
     */
    double GetElapsedMilliseconds() const
    {
        return Timer.GetElapsedMilliseconds();
    }

    /**
     * 获取当前耗时（秒）
     */
    double GetElapsedSeconds() const
    {
        return Timer.GetElapsedSeconds();
    }

private:
    FTimer Timer;
    std::string Label;
};
