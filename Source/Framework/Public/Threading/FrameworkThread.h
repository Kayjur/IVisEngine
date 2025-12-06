#pragma once

#include "Threading/Thread.h"
#include <atomic>
#include <chrono>
#include "HAL/Platform.h"

/**
 * IFrameworkThread - Framework线程类
 * 负责Framework的主循环和帧处理
 */
class IFrameworkThread : public IThread {
public:
    /**
     * 构造函数
     */
    IFrameworkThread();

    /**
     * 析构函数
     */
    virtual ~IFrameworkThread() = default;

    /**
     * 获取帧数统计
     * @return 当前帧数
     */
    uint64 GetFrameCount() const;

    /**
     * 获取平均帧时间（毫秒）
     * @return 平均帧时间
     */
    double GetAverageFrameTime() const;

protected:
    /**
     * 线程主循环
     */
    virtual void Run() override;

    /**
     * 线程初始化
     */
    virtual void OnThreadStart() override;

    /**
     * 线程清理
     */
    virtual void OnThreadEnd() override;

private:
    std::atomic<uint64> FrameCount;
    std::atomic<double> AverageFrameTime;
    
    /**
     * 执行一帧的Framework逻辑
     */
    void Tick(float DeltaTime);
};

