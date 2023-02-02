#include <iostream>
#include <pch.h>
#include "DreamMath.h"




#ifdef WINDOWS

#include<cmath>

float DreamMath::sin(float degrees)
{	
	return std::sinf(degrees * DEG2RAD);
}

float DreamMath::cos(float degrees)
{
	return std::cosf(degrees * DEG2RAD);
}

float DreamMath::tan(float degrees)
{
	return std::tanf(degrees * DEG2RAD);
}

float DreamMath::asin(float num)
{
	if (num < -1 || num > 1) {
		printf("Not a valid number...\n");
		return -9999;
	}
	return std::asinf(num);
}

float DreamMath::acos(float num)
{
	if (num < -1 && num > 1) {
		printf("Not a valid number...\n");
		return -1;
	}
	return std::acosf(num);
}

float DreamMath::atan(float num)
{
	if (num < -1 && num > 1) {
		printf("Not a valid number...\n");
		return -1;
	}
	return std::atanf(num);
}

float DreamMath::abs(float num)
{
	return std::abs(num);
}

float DreamMath::pow(float num, float exp)
{
	return std::powf(num, exp);
}

float DreamMath::floor(float a)
{
	return std::floorf(a);
}

float DreamMath::ceiling(float a)
{
	return std::ceilf(a);
}

float DreamMath::rad2deg(float radians)
{
	return radians * RAD2DEG;
}

float DreamMath::deg2rad(float degrees)
{
	return degrees * DEG2RAD;
}

float DreamMath::sqrtf(float num)
{
	return std::sqrtf(num);
}

float DreamMath::truncf(float num)
{
	return std::truncf(num);
}

float DreamMath::round(float num, int decimal)
{
	float decimalMove = pow(10.0f, (float)decimal);

	int store = (int)(num * decimalMove + .5f);

	return ((float)store)/decimalMove;
}

float DreamMath::lerp(float A, float B, float time)
{
	return ((1 - time) * A) + (time * B);
}

float DreamMath::Dot(float * vec1, float * vec2, int size )
{
	float dot = 0;

	for(int i = 0; i < size; i++)
	{
		dot += vec1[i] * vec2[i];
	}

	return dot;
}

float DreamMath::Dot(float vec1[], float vec2[])
{
	float dot = 0;
	int size = (int) sizeof(vec1) / sizeof(float);

	for (int i = 0; i < size; i++)
	{
		dot += vec1[i] * vec2[i];
	}

	return dot;
}

float DreamMath::FixFloatingPointError(float num)
{
	float returnNum = num;
	float truncatedNum = truncf(returnNum);
	if (DreamMath::abs(returnNum - truncatedNum) < EPSILON) {
		returnNum = truncatedNum;
	}
	return returnNum;
}

#endif

DreamMath::DreamVector3 DreamMath::DreamVector3::operator* (DreamMath::DreamMatrix3X3 m) {
	DreamVector3 newVec = DreamMath::DreamVector3();

	newVec.x = (this->x * m.matrix.data[0][0]) + (this->y * m.matrix.data[1][0]) + (this->z * m.matrix.data[2][0]);
	newVec.y = (this->x * m.matrix.data[0][1]) + (this->y * m.matrix.data[1][1]) + (this->z * m.matrix.data[2][1]);
	newVec.z = (this->x * m.matrix.data[0][2]) + (this->y * m.matrix.data[1][2]) + (this->z * m.matrix.data[2][2]);

	return newVec;
}
void DreamMath::DreamVector3::operator*= (DreamMath::DreamMatrix3X3 m) {
	DreamMath::DreamVector3 newVec = DreamMath::DreamVector3();

	newVec.x = (this->x * m.matrix.data[0][0]) + (this->y * m.matrix.data[1][0]) + (this->z * m.matrix.data[2][0]);
	newVec.y = (this->x * m.matrix.data[0][1]) + (this->y * m.matrix.data[1][1]) + (this->z * m.matrix.data[2][1]);
	newVec.z = (this->x * m.matrix.data[0][2]) + (this->y * m.matrix.data[1][2]) + (this->z * m.matrix.data[2][2]);

	*this = newVec;
}


DreamMath::DreamVector4 DreamMath::DreamVector4::operator* (DreamMath::DreamMatrix4X4 m) {
	DreamMath::DreamVector4 newVec = DreamMath::DreamVector4();

	newVec.x = (this->x * m.matrix.data[0][0]) + (this->y * m.matrix.data[1][0]) + (this->z * m.matrix.data[2][0]) + (this->w * m.matrix.data[3][0]);
	newVec.y = (this->x * m.matrix.data[0][1]) + (this->y * m.matrix.data[1][1]) + (this->z * m.matrix.data[2][1]) + (this->w * m.matrix.data[3][1]);
	newVec.z = (this->x * m.matrix.data[0][2]) + (this->y * m.matrix.data[1][2]) + (this->z * m.matrix.data[2][2]) + (this->w * m.matrix.data[3][2]);
	newVec.w = (this->x * m.matrix.data[0][3]) + (this->y * m.matrix.data[1][3]) + (this->z * m.matrix.data[2][3]) + (this->w * m.matrix.data[3][3]);

	return newVec;
}

void DreamMath::DreamVector4::operator*= (DreamMath::DreamMatrix4X4 m) {
	DreamMath::DreamVector4 newVec = DreamMath::DreamVector4();

	newVec.x = (this->x * m.matrix.data[0][0]) + (this->y * m.matrix.data[1][0]) + (this->z * m.matrix.data[2][0]) + (this->w * m.matrix.data[3][0]);
	newVec.y = (this->x * m.matrix.data[0][1]) + (this->y * m.matrix.data[1][1]) + (this->z * m.matrix.data[2][1]) + (this->w * m.matrix.data[3][1]);
	newVec.z = (this->x * m.matrix.data[0][2]) + (this->y * m.matrix.data[1][2]) + (this->z * m.matrix.data[2][2]) + (this->w * m.matrix.data[3][2]);
	newVec.w = (this->x * m.matrix.data[0][3]) + (this->y * m.matrix.data[1][3]) + (this->z * m.matrix.data[2][3]) + (this->w * m.matrix.data[3][3]);

	*this = newVec;
}