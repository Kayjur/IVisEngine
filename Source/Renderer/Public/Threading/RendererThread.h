#pragma once

#include "Threading/Thread.h"
#include <atomic>
#include "HAL/Platform.h"

/**
 * IRendererThread - 渲染线程类
 * 负责处理渲染相关的任务，如绘制、GPU命令提交等
 */
class IRendererThread : public IThread {
public:
    /**
     * 构造函数
     */
    IRendererThread();

    /**
     * 析构函数
     */
    virtual ~IRendererThread() = default;

    /**
     * 获取渲染帧数统计
     * @return 当前渲染帧数
     */
    uint64 GetRenderFrameCount() const;

    /**
     * 获取平均渲染时间（毫秒）
     * @return 平均渲染时间
     */
    double GetAverageRenderTime() const;

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
    std::atomic<uint64> RenderFrameCount;
    std::atomic<double> AverageRenderTime;
    
    /**
     * 执行一帧的渲染逻辑
     */
    void RenderFrame(float DeltaTime);
};

