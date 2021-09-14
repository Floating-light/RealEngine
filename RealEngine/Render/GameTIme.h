#pragma once 

class GameTime
{
public:
    static GameTime Timer;
    GameTime();
    double TotalTime() const ;
    double DeltaTime() const ;
    double GetSeconds() const ;
    float GetFps() const { return m_fps;};
    void Reset();
    void Start();
    void Stop();
    double Tick();
private:

    void CalculateFrameStats();
    double m_lastSeconds;
    double m_deltaSecondTime;
    double m_startSecondTime; 
    float m_fps;
    bool m_stoped;

};