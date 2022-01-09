#include "Vector.h"
#include "Rotator.h"
#include "Transform.h"
#include "Matrix4.h"

const Vector Vector::ZeroVector(0.f, 0.f,0.f);
const Vector Vector::OneVector(1.f, 1.f, 1.f);

const Rotator Rotator::ZeroRotator(0.f, 0.f,0.f);

const Matrix4 Matrix4::Identity(Vector(1.f, 0.f,0.f), Vector(0.f, 1.f,0.f), Vector(0.f, 0.f,1.f), Vector(0.f));