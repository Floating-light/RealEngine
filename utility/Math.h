#pragma once 

#include "CoreType.h"

class RMath
{
public:
	/**
	* �õ� ��EyeΪ���, targetΪע�ӵ�Ŀ��, Up Ϊ�任�����ϵ���, �任��ԭ��, ע��Z�Ḻ����� View�任����.
	* @param Up ����ָ�������������, ���������right, ͨ����y��������(0,1,0)
	*/
	static Matrix4 ViewTransform(const Vector3D& Eye, const Vector3D& Target, const Vector3D& Up);
	static Matrix4 PerspectiveTransform(float fov, float aspect, float znear, float zfar);
	static Vector3D Cross(const Vector3D& v1, const Vector3D& v2);
	static float Dot(const Vector3D& v1, const Vector3D& v2);
	static Vector3D Normalize(const Vector3D& v);
	static float Radians(float Degrees);
	static float Degrees(float Radians);
	static float Clamp(float a, float min, float max);
};