#include "../../Public/Threading/FrameworkThread.h"
#include <iostream>
#include <thread>
#include <chrono>

IFrameworkThread::IFrameworkThread()
    : IThread("FrameworkThread")
    , FrameCount(0)
    , AverageFrameTime(0.0)
{
}

void IFrameworkThread::OnThreadStart()
{
    std::cout << "[FrameworkThread] 线程启动，ID: " << std::this_thread::get_id() << std::endl;
}

void IFrameworkThread::OnThreadEnd()
{
    std::cout << "[FrameworkThread] 线程结束，总帧数: " << FrameCount.load() << std::endl;
}

void IFrameworkThread::Run()
{
    const float TargetFrameTime = 1.0f / 60.0f; // 60 FPS
    auto LastTime = std::chrono::steady_clock::now();
    double TotalFrameTime = 0.0;

    while (!ShouldStop())
    {
        auto CurrentTime = std::chrono::steady_clock::now();
        auto Elapsed = std::chrono::duration_cast<std::chrono::microseconds>(CurrentTime - LastTime);
        float DeltaTime = Elapsed.count() / 1000000.0f;

        // 执行Framework逻辑
        Tick(DeltaTime);

        // 更新统计信息
        uint64 CurrentFrame = FrameCount.fetch_add(1) + 1;
        TotalFrameTime += DeltaTime;
        AverageFrameTime.store(TotalFrameTime / CurrentFrame * 1000.0); // 转换为毫秒

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

void IFrameworkThread::Tick(float DeltaTime)
{
    // TODO: 在这里实现Framework的具体逻辑
    // 例如：游戏逻辑更新、物理模拟、AI计算等
    
    // 示例：每100帧输出一次信息
    if (FrameCount.load() % 100 == 0)
    {
        std::cout << "[FrameworkThread] 帧数: " << FrameCount.load() 
                  << ", DeltaTime: " << DeltaTime * 1000.0f << "ms" << std::endl;
    }
}

uint64 IFrameworkThread::GetFrameCount() const
{
    return FrameCount.load();
}

double IFrameworkThread::GetAverageFrameTime() const
{
    return AverageFrameTime.load();
}

