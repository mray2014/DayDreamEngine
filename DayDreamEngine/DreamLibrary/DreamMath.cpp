#include <iostream>
#include <pch.h>
#include <GLFW\glfw3.h>
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
	float decimalMove = pow(10, decimal);

	float store = (int)(num * decimalMove + .5f);

	return (float)store/decimalMove;
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