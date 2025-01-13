#include "mathematics.h"

#include "../../engine/utils/utils.h"



#include "../../sdk/globals.h"



//Vector3D CMath::CalcAngle(const Vector3D& vecSource, const Vector3D& vecDestination)
//{
//
//	Vector3D qAngles;
//	Vector3D delta = Vector3D((vecSource[0] - vecDestination[0]), (vecSource[1] - vecDestination[1]), (vecSource[2] - vecDestination[2]));
//	double hyp = sqrtf(delta[0] * delta[0] + delta[1] * delta[1]);
//	qAngles[0] = (float)(atan(delta[2] / hyp) * (180.0 / 3.14159265358979323846));
//	qAngles[1] = (float)(atan(delta[1] / delta[0]) * (180.0 / 3.14159265358979323846));
//	qAngles[2] = 0.f;
//	if (delta[0] >= 0.f)
//		qAngles[1] += 180.f;
//
//	return qAngles;
//}
Vector3D CMath::CalcAngle(const Vector3D& vecSource, const Vector3D& vecDestination)
{

	Vector3D qAngles;
	Vector3D delta = Vector3D((vecSource[0] - vecDestination[0]), (vecSource[1] - vecDestination[1]), (vecSource[2] - vecDestination[2]));
	double hyp = sqrtf(delta[0] * delta[0] + delta[1] * delta[1]);
	qAngles[0] = (float)(atan(delta[2] / hyp) * (180.0 / 3.14159265358979323846));
	qAngles[1] = (float)(atan(delta[1] / delta[0]) * (180.0 / 3.14159265358979323846));
	qAngles[2] = 0.f;
	if (delta[0] >= 0.f)
		qAngles[1] += 180.f;

	return qAngles;
}
