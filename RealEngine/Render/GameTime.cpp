#include "GameTime.h"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <Windows.h>

GameTime GameTime::Timer;

GameTime::GameTime()
{
    m_deltaSecondTime = 1.0f/60.0f;
    m_lastSeconds = GetSeconds();
    m_fps = 60.0f;
}
double GameTime::GetSeconds() const 
{
    return std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1,1>>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

double GameTime::TotalTime() const 
{
    return GetSeconds() - m_startSecondTime;
}

double GameTime::DeltaTime() const 
{
    return m_deltaSecondTime;
}

void GameTime::Reset()
{
    m_startSecondTime = GetSeconds();
}
void GameTime::Start()
{

}
void GameTime::Stop()
{

}
double GameTime::Tick()
{
    const double NewCurrentSeconds = GetSeconds();
    m_deltaSecondTime = NewCurrentSeconds - m_lastSeconds;
    m_lastSeconds = NewCurrentSeconds;

    CalculateFrameStats();
    return m_deltaSecondTime;
}

void GameTime::CalculateFrameStats()
{
    // static int64_t lastPoint = 0;
    // static int frameCount = 0;
    // frameCount++;
    // // double CurrentTime = GetSeconds();
    // __int64 CurrentCount ;
    // QueryPerformanceCounter((LARGE_INTEGER*)&CurrentCount);

    // // std::cout<< std::dec <<std::setprecision(10) << "frameCount : " << frameCount << "----------->> " << CurrentTime << std::endl;
    // if(CurrentCount - lastPoint >=10000000)
    // {
    //     m_fps = double(frameCount) * ((double)(CurrentCount - lastPoint) / 10000000);

    //     lastPoint = CurrentCount;
    //     frameCount = 0;
    // }

    static double lastPoint = 0;
    static int frameCount = 0;
    frameCount++;
    double CurrentTime = GetSeconds();

    // std::cout<< std::dec <<std::setprecision(10) << "frameCount : " << frameCount << "----------->> " << CurrentTime << std::endl;
    if(CurrentTime - lastPoint >=1.0f)
    {
        m_fps = double(frameCount) / (CurrentTime - lastPoint);

        lastPoint = CurrentTime;
        frameCount = 0;
    }
}