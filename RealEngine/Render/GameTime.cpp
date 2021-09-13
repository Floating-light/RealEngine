#include "GameTime.h"
#include <chrono>
#include <windows.h>
GameTime GameTime::Timer;

GameTime::GameTime()
{
    m_deltaSecondTime = 1.0f/30.0f;
    LARGE_INTEGER CountPerSecond;
    QueryPerformanceFrequency(&CountPerSecond);
    m_secondPerCount = 1.0f / static_cast<double>(CountPerSecond.QuadPart);
    m_lastSeconds = GetSeconds();
}
double GameTime::GetSeconds() const 
{
    LARGE_INTEGER Count;
    QueryPerformanceCounter(&Count);
    return m_secondPerCount * static_cast<double>(Count.QuadPart);
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
    m_deltaSecondTime = NewCurrentSeconds - m_currentSeconds;
    m_lastSeconds = m_currentSeconds;
    m_currentSeconds = NewCurrentSeconds;
    return m_deltaSecondTime;
}