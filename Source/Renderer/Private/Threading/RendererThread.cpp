#include "../../Public/Threading/RendererThread.h"
#include "Rendering/RenderCommandQueue.h"
#include "Rendering/Scene.h"
#include <iostream>
#include <thread>
#include <chrono>

IRendererThread::IRendererThread()
    : IThread("RendererThread")
    , RenderFrameCount(0)
    , AverageRenderTime(0.0)
{
}

void IRendererThread::OnThreadStart()
{
    std::cout << "[RendererThread] 线程启动，ID: " << std::this_thread::get_id() << std::endl;
}

void IRendererThread::OnThreadEnd()
{
    std::cout << "[RendererThread] 线程结束，总渲染帧数: " << RenderFrameCount.load() << std::endl;
}

void IRendererThread::Run()
{
    const float TargetFrameTime = 1.0f / 60.0f; // 60 FPS
    auto LastTime = std::chrono::steady_clock::now();
    double TotalRenderTime = 0.0;

    while (!ShouldStop())
    {
        auto CurrentTime = std::chrono::steady_clock::now();
        auto Elapsed = std::chrono::duration_cast<std::chrono::microseconds>(CurrentTime - LastTime);
        float DeltaTime = Elapsed.count() / 1000000.0f;

        // 执行渲染逻辑
        RenderFrame(DeltaTime);

        // 更新统计信息
        uint64 CurrentFrame = RenderFrameCount.fetch_add(1) + 1;
        TotalRenderTime += DeltaTime;
        AverageRenderTime.store(TotalRenderTime / CurrentFrame * 1000.0); // 转换为毫秒

        // 控制帧率
        auto FrameEndTime = std::chrono::steady_clock::now();
        auto FrameElapsed = std::chrono::duration_cast<std::chrono::microseconds>(FrameEndTime - CurrentTime);
        int64 SleepTime = static_cast<int64>(TargetFrameTime * 1000000.0f) - FrameElapsed.count();
        
        if (SleepTime > 0)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(SleepTime));
        }

        LastTime = std::chrono::steady_clock::now();
    }
}

void IRendererThread::RenderFrame(float DeltaTime)
{
    // 处理渲染命令队列中的所有命令
    FRenderCommandQueue::Get().ProcessCommands();
    
    // 渲染场景中的所有代理
    IScene& Scene = IScene::Get();
    uint32 PrimitiveCount = Scene.GetPrimitiveCount();
    
    // 示例：每100帧输出一次信息
    if (RenderFrameCount.load() % 100 == 0)
    {
        std::cout << "[RendererThread] 渲染帧数: " << RenderFrameCount.load() 
                  << ", DeltaTime: " << DeltaTime * 1000.0f << "ms"
                  << ", 场景代理数: " << PrimitiveCount << std::endl;
    }
}

uint64 IRendererThread::GetRenderFrameCount() const
{
    return RenderFrameCount.load();
}

double IRendererThread::GetAverageRenderTime() const
{
    return AverageRenderTime.load();
}

