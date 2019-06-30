#pragma once
#include <ostream>

struct DreamVector2 {
public:
	float x = 0;
	float y = 0;

	DreamVector2(float newX = 0, float newY = 0) {
		x = newX;
		y = newY;
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

class DreamMath {
public:
	static int AddNum(int a, int b);
};
