#pragma once 

#define KINDA_SMALL_NUMBER	(1.e-4f)
#define SMALL_NUMBER		(1.e-8f)
#define PI 					(3.1415926535897932f)	/* Extra digits if needed: 3.1415926535897932384626433832795f */

#define INV_PI			(0.31830988618f)
#define HALF_PI			(1.57079632679f)

#define THRESH_QUAT_NORMALIZED			(0.01f)		/** Allowed error for a normalized quaternion (against squared magnitude) */

enum EForceInit 
{
	ForceInit,
	ForceInitToZero
};

enum EIdentityTag{ kIdentity };