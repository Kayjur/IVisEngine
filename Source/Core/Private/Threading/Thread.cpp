#include "Threading/Thread.h"
#include <iostream>
#include <thread>
#include <chrono>

IThread::IThread(const std::string& InThreadName)
    : ThreadName(InThreadName)
    , bShouldStop(false)
    , bIsRunning(false)
{
}

IThread::~IThread()
{
    if (bIsRunning.load())
    {
        Stop();
        WaitForCompletion(5000); // 等待最多5秒
    }
}

bool IThread::Start()
{
    if (bIsRunning.load())
    {
        std::cerr << "线程 " << ThreadName << " 已经在运行中" << std::endl;
        return false;
    }

    bShouldStop.store(false);
    bIsRunning.store(true);
    
    try
    {
        ThreadHandle = std::thread(&IThread::ThreadEntry, this);
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "启动线程 " << ThreadName << " 失败: " << e.what() << std::endl;
        bIsRunning.store(false);
        return false;
    }
}

void IThread::Stop()
{
    bShouldStop.store(true);
}

bool IThread::WaitForCompletion(uint32 TimeoutMs)
{
    if (!bIsRunning.load())
    {
        return true;
    }

    if (ThreadHandle.joinable())
    {
        if (TimeoutMs == 0)
        {
            ThreadHandle.join();
            return true;
        }
        else
        {
            // 简单的超时等待实现
            auto StartTime = std::chrono::steady_clock::now();
            while (bIsRunning.load())
            {
                auto Elapsed = std::chrono::steady_clock::now() - StartTime;
                if (std::chrono::duration_cast<std::chrono::milliseconds>(Elapsed).count() >= TimeoutMs)
                {
                    return false; // 超时
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            if (ThreadHandle.joinable())
            {
                ThreadHandle.join();
            }
            return true;
        }
    }
    
    return true;
}

bool IThread::IsRunning() const
{
    return bIsRunning.load();
}

const std::string& IThread::GetThreadName() const
{
    return ThreadName;
}

std::thread::id IThread::GetThreadId() const
{
    if (ThreadHandle.joinable())
    {
        return ThreadHandle.get_id();
    }
    return std::thread::id();
}

bool IThread::ShouldStop() const
{
    return bShouldStop.load();
}

void IThread::ThreadEntry()
{
    try
    {
        OnThreadStart();
        Run();
        OnThreadEnd();
    }
    catch (const std::exception& e)
    {
        std::cerr << "线程 " << ThreadName << " 运行时发生异常: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "线程 " << ThreadName << " 运行时发生未知异常" << std::endl;
    }
    
    bIsRunning.store(false);
}

