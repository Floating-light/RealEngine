#pragma once 

#include <string>
#include <cassert>
#include <DirectXMath.h>
#include "MathUtilityDefine.h"
#include "Vector.h"
#include "Rotator.h"

class Error;

__declspec(align(16)) class RQuat 
{
public:
    inline RQuat() { m_vec = DirectX::XMQuaternionIdentity(); } 
    //inline RQuat( const Vector& axis, const Scalar& angle ) { m_vec = DirectX::XMQuaternionRotationAxis( axis, angle ); }
    inline RQuat( float pitch, float yaw, float roll) { m_vec = DirectX::XMQuaternionRotationRollPitchYaw(pitch, yaw, roll); }
    inline explicit RQuat( const DirectX::XMMATRIX& matrix ) { m_vec = DirectX::XMQuaternionRotationMatrix( matrix ); }	
    inline explicit RQuat( const DirectX::XMVECTOR& vec ) { m_vec = vec; }
    inline explicit RQuat( EIdentityTag ) { m_vec = DirectX::XMQuaternionIdentity(); }
    inline explicit RQuat(Rotator Rot) { m_vec = DirectX::XMQuaternionRotationRollPitchYaw(Rot.Pitch, Rot.Yaw, Rot.Roll); }

    inline operator DirectX::XMVECTOR() const { return m_vec; }

    inline RQuat operator~ ( void ) const { return RQuat(DirectX::XMQuaternionConjugate(m_vec)); }
    inline RQuat operator- ( void ) const { return RQuat(DirectX::XMVectorNegate(m_vec)); }

    inline RQuat operator* ( RQuat rhs ) const { return RQuat(DirectX::XMQuaternionMultiply(rhs, m_vec)); }
    inline Vector operator* ( Vector rhs ) const { return Vector(DirectX::XMVector3Rotate(rhs, m_vec)); }

    inline RQuat& operator= ( RQuat rhs ) { m_vec = rhs; return *this; }
    inline RQuat& operator*= ( RQuat rhs ) { *this = *this * rhs; return *this; }
    inline Vector RotateVector(const Vector& InVec) const { return Vector(DirectX::XMVector3Rotate(InVec, m_vec)); }; 
protected:
    DirectX::XMVECTOR m_vec;
};