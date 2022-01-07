#pragma once 

#include "Vector.h"
#include "Rotator.h"

struct Transform
{
public:
    Rotator Rotation;
    Vector Translation;
    Vector Scale;
 
    Transform()
        : Rotation(Rotator::ZeroRotator)
        , Translation(0.f)
        , Scale(Vector::OneVector)
        {}
    
    explicit Transform(const Vector& InTranslation)
        : Rotation(Rotator::ZeroRotator)
        , Translation(InTranslation)
        , Scale(Vector::OneVector)
        {}
    explicit Transform(const Rotator& InRotation)
        : Rotation(InRotation)
        , Translation(Vector::ZeroVector)
        , Scale(Vector::OneVector)
        {}
    explicit Transform(const Rotator& InRotation, const Vector& InTranslation, const Vector& InScale = Vector::OneVector)
        : Rotation(InRotation)
        , Translation(InTranslation)
        , Scale(InScale)
        {}
};
