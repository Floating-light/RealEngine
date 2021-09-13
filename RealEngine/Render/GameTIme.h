#pragma once 

class GameTime
{
public:
    static GameTime Timer;
    GameTime();
    double TotalTime() const ;
    double DeltaTime() const ;
    double GetSeconds() const ;
    void Reset();
    void Start();
    void Stop();
    double Tick();
private:
    double m_secondPerCount;
    double m_lastSeconds;
    double m_currentSeconds;
    double m_deltaSecondTime;
    double m_startSecondTime; 
    bool m_stoped;
};