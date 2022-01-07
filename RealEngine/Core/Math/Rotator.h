#pragma once 

struct Rotator
{
public:
    float Pitch;
    float Yaw;
    float Roll ;

    static const Rotator ZeroRotator;
    
    inline Rotator() : Pitch(0.f), Yaw(0.f), Roll(0.f){};
    
    Rotator(float In): Pitch(In), Yaw(In), Roll(In){};
    
    Rotator(float InPitch, float InYaw, float InRoll)
        : Pitch(InPitch), Yaw(InYaw), Roll(InRoll){};

    Rotator operator+(const Rotator& R) const ;
    Rotator operator+=(const Rotator& R) ;
    Rotator operator-(const Rotator& R) const ;
    Rotator operator-=(const Rotator& R) ;
    Rotator operator*(float Scall) const ;
    Rotator operator*=(float Scall) ;
    Rotator operator==(const Rotator& R)const ;
    Rotator operator!=(const Rotator& R) const ;
};

inline Rotator Rotator::operator+(const Rotator& R) const 
{
    return Rotator(Pitch + R.Pitch, Yaw + R.Yaw, Roll + R.Roll);
}
inline Rotator Rotator::operator+=(const Rotator& R) 
{
    Pitch += R.Pitch; Yaw += R.Yaw; Roll += R.Roll;
    return *this;
}
inline Rotator Rotator::operator-(const Rotator& R) const 
{
    return Rotator(Pitch - R.Pitch, Yaw - R.Yaw, Roll - R.Roll);
}
inline Rotator Rotator::operator-=(const Rotator& R) 
{
    Pitch -= R.Pitch; Yaw -= R.Yaw; Roll -= R.Roll;
    return *this;
}
inline Rotator Rotator::operator*(float Scale) const 
{
    return Rotator(Pitch * Scale, Yaw * Scale, Roll * Scale);
}
inline Rotator Rotator::operator*=(float Scale) 
{
    Pitch *= Scale; Yaw *= Scale; Roll *= Scale;
    return *this;
}
inline Rotator Rotator::operator==(const Rotator& R)const 
{
    return Pitch == R.Pitch && Yaw == R.Yaw && Roll == R.Roll;
}
inline Rotator Rotator::operator!=(const Rotator& R) const 
{
    return Pitch != R.Pitch || Yaw != R.Yaw || Roll != R.Roll;
}