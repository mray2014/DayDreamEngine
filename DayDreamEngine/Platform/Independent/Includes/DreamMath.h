#pragma once
#include <cmath>

#if defined DREAM_OPENGL
#define COL_MAJOR
#elif defined DREAM_DX
#define ROW_MAJOR
#elif defined DREAM_VULKAN
#define COL_MAJOR
#endif

struct DreamVector2 {
public:
	float x = 0;
	float y = 0;

	DreamVector2(float newX = 0, float newY = 0) {
		x = newX;
		y = newY;
	}

	float GetSqrMagnitude() {
		return (x * x) + (y * y);
	}
	float GetMagnitude() {
		return sqrtf((x * x) + (y * y));
	}
	DreamVector2 GetNormalizedVector() {
		DreamVector2 newVec = DreamVector2();

		float mag = this->GetMagnitude();

		if (mag == 0) {
			return newVec;
		}

		newVec.x = x / mag;
		newVec.y = y / mag;

		return newVec;
	}
	void Normalize() {
		float mag = this->GetMagnitude();

		if (mag == 0) {
			return;
		}

		x /= mag;
		y /= mag;
	}

	static float Dot(DreamVector2 A, DreamVector2 B) {
		return (A.x * B.x) + (A.y * B.y);
	}

#pragma region Operator OverLoads

	DreamVector2& operator+(DreamVector2& v) {
		DreamVector2 newVec = DreamVector2();
		newVec.x = this->x + v.x;
		newVec.y = this->y + v.y;

		return newVec;
	}
	DreamVector2& operator-(DreamVector2& v) {
		DreamVector2 newVec = DreamVector2();
		newVec.x = this->x - v.x;
		newVec.y = this->y - v.y;

		return newVec;
	}
	DreamVector2& operator*(DreamVector2& v) {
		DreamVector2 newVec = DreamVector2();
		newVec.x = this->x * v.x;
		newVec.y = this->y * v.y;

		return newVec;
	}
	DreamVector2& operator/(DreamVector2& v) {
		DreamVector2 newVec = DreamVector2();
		newVec.x = this->x / v.x;
		newVec.y = this->y / v.y;

		return newVec;
	}
	void operator+=(DreamVector2& v) {
		this->x += v.x;
		this->y += v.y;
	}
	void operator-=(DreamVector2& v) {
		this->x -= v.x;
		this->y -= v.y;
	}
	void operator*=(DreamVector2& v) {
		this->x *= v.x;
		this->y *= v.y;
	}
    void operator/=(DreamVector2& v) {
		this->x /= v.x;
		this->y /= v.y;

	}


	DreamVector2& operator*(int num) {
		DreamVector2 newVec = DreamVector2();
		newVec.x = this->x * num;
		newVec.y = this->y * num;
		return newVec;
	}
	DreamVector2&  operator*(float num) {
		DreamVector2 newVec = DreamVector2();
		newVec.x = this->x * num;
		newVec.y = this->y * num;
		return newVec;
	}
	void operator*=(int num) {
		this->x *= num;
		this->y *= num;
	}
	void operator*=(float num) {
		this->x *= num;
		this->y *= num;
	}

	DreamVector2& operator/(int num) {
		DreamVector2 newVec = DreamVector2();
		newVec.x = this->x / num;
		newVec.y = this->y / num;
		return newVec;
	}
	DreamVector2&  operator/(float num) {
		DreamVector2 newVec = DreamVector2();
		newVec.x = this->x / num;
		newVec.y = this->y / num;
		return newVec;
	}
	void operator/=(int num) {
		this->x /= num;
		this->y /= num;
	}
	void operator/=(float num) {
		this->x /= num;
		this->y /= num;

	}

#pragma endregion

};
struct DreamVector3 {
public:
	float x = 0;
	float y = 0;
	float z = 0;

	DreamVector3(float newX = 0, float newY = 0, float newZ = 0) {
		x = newX;
		y = newY;
		z = newZ;
	}

	float GetSqrMagnitude() {
		return (x * x) + (y * y) + (z * z);
	}
	float GetMagnitude() {
		return sqrtf((x * x) + (y * y) + (z * z));
	}
	DreamVector3 GetNormalizedVector() {
		DreamVector3 newVec = DreamVector3();

		float mag = this->GetMagnitude();

		if (mag == 0) {
			return newVec;
		}
		
		newVec.x = x / mag;
		newVec.y = y / mag;
		newVec.z = z / mag;

		return newVec;
	}
	void Normalize() {
		float mag = this->GetMagnitude();

		if (mag == 0) {
			return;
		}

		x /= mag;
		y /= mag;
		z /= mag;
	}

	static float Dot(DreamVector3 A, DreamVector3 B) {
		return (A.x * B.x) + (A.y * B.y) + (A.z * B.z);
	}
	static DreamVector3 Cross(DreamVector3 A, DreamVector3 B) {
		DreamVector3 newVec = DreamVector3();

		newVec.x = (A.y * B.z) - (A.z * B.y);
		newVec.x = (A.z * B.x) - (A.x * B.z);
		newVec.x = (A.x * B.y) - (A.y * B.x);

		return newVec;
	}

#pragma region Operator OverLoads

	DreamVector3& operator+(DreamVector3& v) {
		DreamVector3 newVec = DreamVector3();
		newVec.x = this->x + v.x;
		newVec.y = this->y + v.y;
		newVec.z = this->z + v.z;

		return newVec;
	}
	DreamVector3& operator-(DreamVector3& v) {
		DreamVector3 newVec = DreamVector3();
		newVec.x = this->x - v.x;
		newVec.y = this->y - v.y;
		newVec.z = this->z - v.z;

		return newVec;
	}
	DreamVector3& operator*(DreamVector3& v) {
		DreamVector3 newVec = DreamVector3();
		newVec.x = this->x * v.x;
		newVec.y = this->y * v.y;
		newVec.z = this->z * v.z;

		return newVec;
	}
	DreamVector3& operator/(DreamVector3& v) {
		DreamVector3 newVec = DreamVector3();
		newVec.x = this->x / v.x;
		newVec.y = this->y / v.y;
		newVec.z = this->z / v.z;

		return newVec;
	}
	void operator+=(DreamVector3& v) {
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
	}
	void operator-=(DreamVector3& v) {
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;
	}
	void operator*=(DreamVector3& v) {
		this->x *= v.x;
		this->y *= v.y;
		this->z *= v.z;
	}
	void operator/=(DreamVector3& v) {
		this->x /= v.x;
		this->y /= v.y;
		this->z /= v.z;

	}


	DreamVector3& operator*(int num) {
		DreamVector3 newVec = DreamVector3();
		newVec.x = this->x * num;
		newVec.y = this->y * num;
		newVec.z = this->z * num;
		return newVec;
	}
	DreamVector3&  operator*(float num) {
		DreamVector3 newVec = DreamVector3();
		newVec.x = this->x * num;
		newVec.y = this->y * num;
		newVec.z = this->z * num;
		return newVec;
	}
	void operator*=(int num) {
		this->x *= num;
		this->y *= num;
		this->z *= num;
	}
	void operator*=(float num) {
		this->x *= num;
		this->y *= num;
		this->z *= num;
	}

	DreamVector3& operator/(int num) {
		DreamVector3 newVec = DreamVector3();
		newVec.x = this->x / num;
		newVec.y = this->y / num;
		newVec.z = this->z / num;
		return newVec;
	}
	DreamVector3&  operator/(float num) {
		DreamVector3 newVec = DreamVector3();
		newVec.x = this->x / num;
		newVec.y = this->y / num;
		newVec.z = this->z / num;
		return newVec;
	}
	void operator/=(int num) {
		this->x /= num;
		this->y /= num;
		this->z /= num;
	}
	void operator/=(float num) {
		this->x /= num;
		this->y /= num;
		this->z /= num;
	}

#pragma endregion

};
struct DreamVector4 {
public:
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 0;

	DreamVector4(float newX = 0, float newY = 0, float newZ = 0, float newW = 0) {
		x = newX;
		y = newY;
		z = newZ;
		w = newW;
	}

	float GetSqrMagnitude() {
		return (x * x) + (y * y) + (z * z) + (w * w);
	}
	float GetMagnitude() {
		return sqrtf((x * x) + (y * y) + (z * z) + (w * w));
	}
	DreamVector4 GetNormalizedVector() {
		DreamVector4 newVec = DreamVector4();

		float mag = this->GetMagnitude();

		if (mag == 0) {
			return newVec;
		}

		newVec.x = x / mag;
		newVec.y = y / mag;
		newVec.z = z / mag;
		newVec.w = w / mag;

		return newVec;
	}
	void Normalize() {
		float mag = this->GetMagnitude();

		if (mag == 0) {
			return;
		}

		x /= mag;
		y /= mag;
		z /= mag;
		w /= mag;
	}

	static float Dot(DreamVector4 A, DreamVector4 B) {
		return (A.x * B.x) + (A.y * B.y) + (A.z * B.z) + (A.w * B.w);
	}

#pragma region Operator OverLoads

	DreamVector4& operator+(DreamVector4& v) {
		DreamVector4 newVec = DreamVector4();
		newVec.x = this->x + v.x;
		newVec.y = this->y + v.y;
		newVec.z = this->z + v.z;
		newVec.w = this->w + v.w;

		return newVec;
	}
	DreamVector4& operator-(DreamVector4& v) {
		DreamVector4 newVec = DreamVector4();
		newVec.x = this->x - v.x;
		newVec.y = this->y - v.y;
		newVec.z = this->z - v.z;
		newVec.w = this->w - v.w;

		return newVec;
	}
	DreamVector4& operator*(DreamVector4& v) {
		DreamVector4 newVec = DreamVector4();
		newVec.x = this->x * v.x;
		newVec.y = this->y * v.y;
		newVec.z = this->z * v.z;
		newVec.w = this->w * v.w;

		return newVec;
	}
	DreamVector4& operator/(DreamVector4& v) {
		DreamVector4 newVec = DreamVector4();
		newVec.x = this->x / v.x;
		newVec.y = this->y / v.y;
		newVec.z = this->z / v.z;
		newVec.w = this->w / v.w;

		return newVec;
	}
	void operator+=(DreamVector4& v) {
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
		this->w += v.w;
	}
	void operator-=(DreamVector4& v) {
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;
		this->w -= v.w;
	}
	void operator*=(DreamVector4& v) {
		this->x *= v.x;
		this->y *= v.y;
		this->z *= v.z;
		this->w *= v.w;
	}
	void operator/=(DreamVector4& v) {
		this->x /= v.x;
		this->y /= v.y;
		this->z /= v.z;
		this->w /= v.w;

	}


	DreamVector4& operator*(int num) {
		DreamVector4 newVec = DreamVector4();
		newVec.x = this->x * num;
		newVec.y = this->y * num;
		newVec.z = this->z * num;
		newVec.w = this->w * num;
		return newVec;
	}
	DreamVector4&  operator*(float num) {
		DreamVector4 newVec = DreamVector4();
		newVec.x = this->x * num;
		newVec.y = this->y * num;
		newVec.z = this->z * num;
		newVec.w = this->w * num;
		return newVec;
	}
	void operator*=(int num) {
		this->x *= num;
		this->y *= num;
		this->z *= num;
		this->w *= num;
	}
	void operator*=(float num) {
		this->x *= num;
		this->y *= num;
		this->z *= num;
		this->w *= num;
	}

	DreamVector4& operator/(int num) {
		DreamVector4 newVec = DreamVector4();
		newVec.x = this->x / num;
		newVec.y = this->y / num;
		newVec.z = this->z / num;
		newVec.w = this->w / num;
		return newVec;
	}
	DreamVector4&  operator/(float num) {
		DreamVector4 newVec = DreamVector4();
		newVec.x = this->x / num;
		newVec.y = this->y / num;
		newVec.z = this->z / num;
		newVec.w = this->w / num;
		return newVec;
	}
	void operator/=(int num) {
		this->x /= num;
		this->y /= num;
		this->z /= num;
		this->w /= num;
	}
	void operator/=(float num) {
		this->x /= num;
		this->y /= num;
		this->z /= num;
		this->w /= num;
	}

#pragma endregion

};

struct DreamMatrix3X3 {
public:
#ifdef ROW_MAJOR

	float x1, y1, z1 = 0;
	float x2, y2, z2 = 0;
	float x3, y3, z3 = 0;

#elif defined COL_MAJOR
	float x1, x2, x3 = 0;
	float y1, y2, y3 = 0;
	float z1, z2, z3 = 0;
#endif


};

struct DreamMatrix4X4 {
public:
#ifdef ROW_MAJOR

	float x1, y1, z1, w1 = 0;
	float x2, y2, z2, w2 = 0;
	float x3, y3, z3, w3 = 0;
	float x4, y4, z4, w4 = 0;

#elif defined COL_MAJOR
	float x1, x2, x3, x4 = 0;
	float y1, y2, y3, y4 = 0;
	float z1, z2, z3, z4 = 0;
	float w1, w2, w3, w4 = 0;
#endif


};

class DreamMath {
public:
	static int AddNum(int a, int b);
};
