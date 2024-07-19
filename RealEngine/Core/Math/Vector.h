#pragma once 
#include <cmath>
#include <format>
#include <DirectXMath.h>
struct Vector
{
public:
    float X;
    float Y;
    float Z;
    static const Vector ZeroVector;
    static const Vector OneVector;
    static const Vector XAxisVector;
    Vector();
    Vector(float In);
    Vector(float InX, float InY, float InZ);

    // A X B
    static Vector CrossProduct(const Vector& A, const Vector& B);
    static float DotProduct(const Vector& A, const Vector& B);

    static Vector ComponentWiseMin(const Vector& A, const Vector& B);
    static Vector ComponentWiseMax(const Vector& A, const Vector& B);

    inline explicit Vector(DirectX::FXMVECTOR vec); 
    inline operator DirectX::XMVECTOR() const { return DirectX::XMVectorSet(X, Y, Z, Z); }
    inline  DirectX::XMVECTOR GetWithWZero() const { return DirectX::XMVectorSet(X, Y, Z, 0); }; 
    inline  DirectX::XMVECTOR GetWithWOne() const { return DirectX::XMVectorSet(X, Y, Z, 1); }; 
    std::string ToString()const;
    // Component-wise addition 
    Vector operator+(const Vector& V) const;

    // Add Bias to each component 
    Vector operator+(float Bias) const;

    // Component-wise subtraction 
    Vector operator-(const Vector& V) const;
    
    // subtract Bias from each component
    Vector operator-(float Bias) const;
    Vector operator-() const;

    Vector operator*(const Vector& V ) const;

    Vector operator*(float Scale) const;

    Vector operator/(Vector V) const ;

    Vector operator/(float Scale) const ;

    bool operator==(const Vector& V) const ;

    bool operator!=(const Vector& V) const ;

    Vector operator+=(const Vector& V) ;

    Vector operator-=(const Vector& V)  ;

    float Length() const ;

    float LengthSquared() const ;

};
using Vector3D = Vector;

inline Vector::Vector(DirectX::FXMVECTOR vec)
{
    X = DirectX::XMVectorGetX(vec);
    Y = DirectX::XMVectorGetY(vec);
    Z = DirectX::XMVectorGetZ(vec);
}

inline Vector::Vector():X(0.f), Y(0.f), Z(0.f){}

inline Vector::Vector(float In):X(In), Y(In), Z(In){}

inline Vector::Vector(float InX, float InY, float InZ):X(InX), Y(InY), Z(InZ) {} 

// A X B
inline Vector Vector::CrossProduct(const Vector& A, const Vector& B)
{
    return Vector(A.Y * B.Z - A.Z * B.Y, 
                  A.Z * B.X - A.X * B.Z,
                  A.X * B.Y - A.Y * B.X);
}
inline float Vector::DotProduct(const Vector& A, const Vector& B)
{
    return A.X * B.X + A.Y * B.Y + A.Z * B.Z;
}

// Component-wise addition 
inline Vector Vector::operator+(const Vector& V) const
{
    return Vector(X + V.X, Y + V.Y, Z + V.Z);
}

// Add Bias to each component 
inline Vector Vector::operator+(float Bias) const
{
    return Vector(X + Bias, Y + Bias, Z + Bias);
}

// Component-wise subtraction 
inline Vector Vector::operator-(const Vector& V) const
{
    return Vector(X - V.X, Y - V.Y , Z - V.Z);
}
    
// subtract Bias from each component
inline Vector Vector::operator-(float Bias) const
{
    return Vector(X - Bias, Y - Bias, Z - Bias);
}
inline Vector Vector::operator-() const
{
    return Vector(0 - X, 0 - Y, 0 - Z); 
}

inline Vector Vector::operator*(const Vector& V ) const
{
    return Vector(X * V.X, Y * V.Y, Z * V.Z);
}
inline Vector Vector::operator*(float Scale) const
{
    return Vector(X * Scale, Y * Scale, Z * Scale) ;
}

inline Vector Vector::operator/(Vector V) const 
{
    return Vector(X / V.X, Y / V.Y, Z / V.Z);
}

inline Vector Vector::operator/(float Scale) const 
{
    const float S = 1.f / Scale ;
    return Vector(X / S, Y / S , Z / S);
}

inline bool Vector::operator==(const Vector& V) const 
{
    return X == V.X && Y == V.Y && Z == V.Z;
}

inline bool Vector::operator!=(const Vector& V) const 
{
    return X != V.X || Y != V.Y || Z != V.Z;
}

inline Vector Vector::operator+=(const Vector& V) 
{
    X += V.X; Y += V.Y; Z += V.Z;
    return *this;
}

inline Vector Vector::operator-=(const Vector& V) 
{
    X -= V.X; Y -= V.Y; Z -= V.Z;
    return *this;
}

inline float Vector::Length() const 
{
    return std::sqrt(X*X + Y*Y + Z*Z);
}

inline float Vector::LengthSquared() const 
{
    return X*X + Y*Y + Z*Z;
}

inline Vector operator*(float Scale, const Vector& V)
{
	return V.operator*(Scale);
}

inline Vector Vector::ComponentWiseMin(const Vector& A, const Vector& B)
{
    return Vector((std::min)(A.X, B.X), (std::min)(A.Y, B.Y), (std::min)(A.Z, B.Z));
}

inline Vector Vector::ComponentWiseMax(const Vector& A, const Vector& B)
{
    return Vector((std::max)(A.X, B.X), (std::max)(A.Y, B.Y), (std::max)(A.Z, B.Z));

}

inline std::string Vector::ToString() const
{
    return std::format("[{},{},{}]", X, Y, Z);
}