#pragma once 

struct Vector
{
public:
    float X;
    float Y;
    float Z;
    static const Vector ZeroVector;
    static const Vector OneVector;
    Vector();
    Vector(float In);
    Vector(float InX, float InY, float InZ);

    // A X B
    static Vector CrossProduct(const Vector& A, const Vector& B);
    static float DotProduct(const Vector& A, const Vector& B);

    // Component-wise addition 
    Vector operator+(const Vector& V) const;

    // Add Bias to each component 
    Vector operator+(float Bias) const;

    // Component-wise subtraction 
    Vector operator-(const Vector& V) const;
    
    // subtract Bias from each component
    Vector operator-(float Bias) const;

    Vector operator*(const Vector& V ) const;

    Vector operator*(float Scale) const;
};