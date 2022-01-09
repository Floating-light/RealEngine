#pragma once 

struct Matrix4
{
    float Mat[4][4];

    static const Matrix4 Identity ;

    Matrix4() ;

    Matrix4(const Vector& InX, const Vector& InY, const Vector& InZ, const Vector& InW);
    
    void SetIdentity();

    Matrix4 operator*(const Matrix4& M);
    Matrix4 operator*=(const Matrix4& M);

    bool operator==(const Matrix4& M);
    bool operator!=(const Matrix4& M);
};

inline Matrix4::Matrix4() : Mat{0.f}
{

};

inline Matrix4::Matrix4(const Vector& InX, const Vector& InY, const Vector& InZ, const Vector& InW)
{
    Mat[0][0] = InX.X; Mat[0][1] = InX.Y; Mat[0][2] = InX.Z; Mat[0][3] = 0.f;
    Mat[1][0] = InY.X; Mat[1][1] = InY.Y; Mat[1][2] = InY.Z; Mat[1][3] = 0.f;
    Mat[2][0] = InZ.X; Mat[2][1] = InZ.Y; Mat[2][2] = InZ.Z; Mat[2][3] = 0.f;
    Mat[3][0] = InW.X; Mat[3][1] = InW.Y; Mat[3][2] = InW.Z; Mat[3][3] = 1.f;
}

void Matrix4::SetIdentity()
{
    *this = Matrix4::Identity;
};

Matrix4 Matrix4::operator*(const Matrix4& M)
{
    
};

Matrix4 Matrix4::operator*=(const Matrix4& M)
{

};

bool Matrix4::operator==(const Matrix4& M)
{

};

bool Matrix4::operator!=(const Matrix4& M)
{

};
