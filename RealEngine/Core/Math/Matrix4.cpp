#include "Matrix4.h"

#include "MathUtility.h"

LookFromMatrix::LookFromMatrix(const Vector& EyeLocation, const Vector& LookDirection, const Vector& UpDirection)
{
    const Vector s(RMath::Normalize(RMath::Cross(LookDirection, UpDirection)));
	const Vector u(RMath::Cross(s, LookDirection));

    // Matrix4 M(1.0f);
	Mat[0][0] = s.X;
	Mat[1][0] = s.Y;
	Mat[2][0] = s.Z;
    Mat[3][0] = 0.0;
	Mat[0][1] = u.X;
	Mat[1][1] = u.Y;
	Mat[2][1] = u.Z;
    Mat[3][1] = 0.0;
	Mat[0][2] = -LookDirection.X;
	Mat[1][2] = -LookDirection.Y;
	Mat[2][2] = -LookDirection.Z;
    Mat[3][2] = 0.0;
	Mat[3][0] = -RMath::Dot(s, EyeLocation);
	Mat[3][1] = -RMath::Dot(u, EyeLocation);
	Mat[3][2] =  RMath::Dot(LookDirection, EyeLocation);
}
