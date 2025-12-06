#pragma once

#include <thread>
#include <atomic>
#include <string>
#include <chrono>
#include "HAL/Platform.h"

/**
 * Thread - 基础线程类
 * 提供线程的创建、启动、停止等基础功能
 */
class IThread {
public:
    /**
     * 构造函数
     * @param InThreadName 线程名称
     */
    explicit IThread(const std::string& InThreadName = "UnnamedThread");

    /**
     * 析构函数 - 自动停止线程
     */
    virtual ~IThread();

    /**
     * 启动线程
     * @return 是否成功启动
     */
    bool Start();

    /**
     * 停止线程（异步）
     * 设置停止标志，线程会在下次检查时退出
     */
    void Stop();

    /**
     * 等待线程结束
     * @param TimeoutMs 超时时间（毫秒），0表示无限等待
     * @return 是否成功等待线程结束
     */
    bool WaitForCompletion(uint32 TimeoutMs = 0);

    /**
     * 检查线程是否正在运行
     * @return 线程是否运行中
     */
    bool IsRunning() const;

    /**
     * 获取线程名称
     * @return 线程名称
     */
    const std::string& GetThreadName() const;

    /**
     * 获取线程ID
     * @return 线程ID
     */
    std::thread::id GetThreadId() const;

protected:
    /**
     * 线程主循环函数（子类需要重写）
     * 此函数会在独立线程中运行
     */
    virtual void Run() = 0;

    /**
     * 检查是否应该停止
     * @return 是否应该停止
     */
    bool ShouldStop() const;

    /**
     * 线程初始化（子类可以重写）
     * 在Run()之前调用
     */
    virtual void OnThreadStart() {}

    /**
     * 线程清理（子类可以重写）
     * 在Run()之后调用
     */
    virtual void OnThreadEnd() {}

private:
    std::string ThreadName;
    std::thread ThreadHandle;
    std::atomic<bool> bShouldStop;
    std::atomic<bool> bIsRunning;

    /**
     * 线程入口函数
     */
    void ThreadEntry();
};

