#pragma once
#if defined DREAM_OPENGL
#define COL_MAJOR
#elif defined DREAM_DX
#define ROW_MAJOR
#elif defined DREAM_VULKAN
#define COL_MAJOR
#endif

#include <string>

//TODO: Pass refence into functions and operator over loads
//TODO: Somehow std math is being referenced in the unit test class somehow

namespace DreamMath {

#define EPSILON 0.00001

#define PI 3.14159265358979323846

#define RAD2DEG (float)(180 / PI)

#define DEG2RAD (float)(PI / 180)


	float sin(float degrees);
	float cos(float degrees);
	float tan(float degrees);
	float asin(float num);
	float acos(float num);
	float atan(float num);
	float abs(float num);
	float pow(float num, float exp);
	float floor(float a);
	float ceiling(float a);
	float rad2deg(float radians);
	float deg2rad(float degrees);
	float sqrtf(float num);
	float truncf(float num);
	float round(float num, int decimal = 2);
	float lerp(float A, float B, float time);
	float Dot(float* vec1, float* vec2, int size);
	float Dot(float vec1[], float vec2[]);
	float FixFloatingPointError(float num);





struct DreamMatrix3X3;
struct DreamMatrix4X4;


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
		return DreamMath::sqrtf((x * x) + (y * y));
	}

	void FixFloatingPointError() {
		if (DreamMath::abs(this->x - truncf(this->x)) < EPSILON) {
			this->x = truncf(this->x);
		}
		if (DreamMath::abs(this->y - truncf(this->y)) < EPSILON) {
			this->y = truncf(this->y);
		}
	}

	DreamVector2 GetNormalizedVector() {
		DreamVector2 newVec = DreamVector2();

		float mag = this->GetMagnitude();

		if (mag == 0) {
			return newVec;
		}

		newVec.x = x / mag;
		newVec.y = y / mag;
		
		newVec.FixFloatingPointError();

		return newVec;
	}
	void Normalize() {
		float mag = this->GetMagnitude();

		if (mag == 0) {
			return;
		}

		this->x /= mag;
		this->y /= mag;

		this->FixFloatingPointError();
	}

	static float Dot(DreamVector2 A, DreamVector2 B) {
		return (A.x * B.x) + (A.y * B.y);
	}

	static DreamVector2 lerp(DreamVector2 A, DreamVector2 B, float time)
	{
		return DreamVector2(((1 - time)*A.x) + (time * B.x),
							((1 - time)*A.y) + (time * B.y));
	}

#pragma region Operator OverLoads

	DreamVector2 operator+(DreamVector2 v) {
		DreamVector2 newVec = DreamVector2();
		newVec.x = this->x + v.x;
		newVec.y = this->y + v.y;

		return newVec;
	}
	DreamVector2 operator-(DreamVector2 v) {
		DreamVector2 newVec = DreamVector2();
		newVec.x = this->x - v.x;
		newVec.y = this->y - v.y;

		return newVec;
	}
	DreamVector2 operator*(DreamVector2 v) {
		DreamVector2 newVec = DreamVector2();
		newVec.x = this->x * v.x;
		newVec.y = this->y * v.y;

		return newVec;
	}
	DreamVector2 operator/(DreamVector2 v) {
		DreamVector2 newVec = DreamVector2();
		newVec.x = this->x / v.x;
		newVec.y = this->y / v.y;

		return newVec;
	}
	void operator+=(DreamVector2 v) {
		this->x += v.x;
		this->y += v.y;
	}
	void operator-=(DreamVector2 v) {
		this->x -= v.x;
		this->y -= v.y;
	}
	void operator*=(DreamVector2 v) {
		this->x *= v.x;
		this->y *= v.y;
	}
	void operator/=(DreamVector2 v) {
		this->x /= v.x;
		this->y /= v.y;

	}
	DreamVector2 operator*(int num) {
		DreamVector2 newVec = DreamVector2();
		newVec.x = this->x * num;
		newVec.y = this->y * num;
		return newVec;
	}
	DreamVector2  operator*(float num) {
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

	DreamVector2 operator/(int num) {
		DreamVector2 newVec = DreamVector2();
		newVec.x = this->x / num;
		newVec.y = this->y / num;
		return newVec;
	}
	DreamVector2  operator/(float num) {
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
	bool operator==(DreamVector2 v) {
		return (abs(this->x - v.x) <= EPSILON) && (abs(this->y - v.y) <= EPSILON);
	}

#pragma endregion

	std::string ToString() {
		return std::string("[x: " + std::to_string(this->x) + ", y: " + std::to_string(this->y) + "]");
	}
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
		return DreamMath::sqrtf((x * x) + (y * y) + (z * z));
	}
	void FixFloatingPointError() {
		if (DreamMath::abs(this->x - truncf(this->x)) < EPSILON) {
			this->x = truncf(this->x);
		}
		if (DreamMath::abs(this->y - truncf(this->y)) < EPSILON) {
			this->y = truncf(this->y);
		}
		if (DreamMath::abs(this->z - truncf(this->z)) < EPSILON) {
			this->z = truncf(this->z);
		}
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

		newVec.FixFloatingPointError();

		return newVec;
	}
	void Normalize() {
		float mag = this->GetMagnitude();

		if (mag == 0) {
			return;
		}

		this->x /= mag;
		this->y /= mag;
		this->z /= mag;

		this->FixFloatingPointError();
	}


	static float Dot(DreamVector3 A, DreamVector3 B) {
		return (A.x * B.x) + (A.y * B.y) + (A.z * B.z);
	}
	static DreamVector3 Cross(DreamVector3 A, DreamVector3 B) {
		DreamVector3 newVec = DreamVector3();

		newVec.x = (A.y * B.z) - (A.z * B.y);
		newVec.y = (A.z * B.x) - (A.x * B.z);
		newVec.z = (A.x * B.y) - (A.y * B.x);

		newVec.FixFloatingPointError();

		return newVec;
	}

	static DreamVector3 lerp(DreamVector3 A, DreamVector3 B, float time)
	{
		return DreamVector3(((1 - time)*A.x) + (time * B.x),
							((1 - time)*A.y) + (time * B.y),
							((1 - time)*A.z) + (time * B.z));
	}

#pragma region Operator OverLoads

	DreamVector3 operator+(DreamVector3 v) {
		DreamVector3 newVec = DreamVector3();
		newVec.x = this->x + v.x;
		newVec.y = this->y + v.y;
		newVec.z = this->z + v.z;

		return newVec;
	}
	DreamVector3 operator-(DreamVector3 v) {
		DreamVector3 newVec = DreamVector3();
		newVec.x = this->x - v.x;
		newVec.y = this->y - v.y;
		newVec.z = this->z - v.z;

		return newVec;
	}
	DreamVector3 operator*(DreamVector3 v) {
		DreamVector3 newVec = DreamVector3();
		newVec.x = this->x * v.x;
		newVec.y = this->y * v.y;
		newVec.z = this->z * v.z;

		return newVec;
	}
	DreamVector3 operator/(DreamVector3 v) {
		DreamVector3 newVec = DreamVector3();
		newVec.x = this->x / v.x;
		newVec.y = this->y / v.y;
		newVec.z = this->z / v.z;

		return newVec;
	}
	void operator+=(DreamVector3 v) {
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
	}
	void operator-=(DreamVector3 v) {
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;
	}
	void operator*=(DreamVector3 v) {
		this->x *= v.x;
		this->y *= v.y;
		this->z *= v.z;
	}
	void operator/=(DreamVector3 v) {
		this->x /= v.x;
		this->y /= v.y;
		this->z /= v.z;

	}
	DreamVector3 operator*(int num) {
		DreamVector3 newVec = DreamVector3();
		newVec.x = this->x * num;
		newVec.y = this->y * num;
		newVec.z = this->z * num;
		return newVec;
	}
	DreamVector3  operator*(float num) {
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

	DreamVector3 operator/(int num) {
		DreamVector3 newVec = DreamVector3();
		newVec.x = this->x / num;
		newVec.y = this->y / num;
		newVec.z = this->z / num;
		return newVec;
	}
	DreamVector3 operator/(float num) {
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
	bool operator==(DreamVector3 v) {
		return (abs(this->x - v.x) <= EPSILON) && (abs(this->y - v.y) <= EPSILON) && (abs(this->y - v.y) <= EPSILON);
	}

	DreamVector3 operator* (DreamMatrix3X3 m);
	void operator*= (DreamMatrix3X3 m);

#pragma endregion

	std::string ToString() {
		return std::string("[x: " + std::to_string(this->x) + ", y: " + std::to_string(this->y) + ", z: " + std::to_string(this->z) + "]");
	}

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

	DreamVector4(DreamVector3& vec, float newW = 0) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
		w = newW;
	}

	float GetSqrMagnitude() {
		return (x * x) + (y * y) + (z * z) + (w * w);
	}
	float GetMagnitude() {
		return DreamMath::sqrtf((x * x) + (y * y) + (z * z) + (w * w));
	}
	void FixFloatingPointError() {
		if (DreamMath::abs(this->x - truncf(this->x)) < EPSILON) {
			this->x = truncf(this->x);
		}
		if (DreamMath::abs(this->y - truncf(this->y)) < EPSILON) {
			this->y = truncf(this->y);
		}
		if (DreamMath::abs(this->z - truncf(this->z)) < EPSILON) {
			this->z = truncf(this->z);
		}
		if (DreamMath::abs(this->w - truncf(this->w)) < EPSILON) {
			this->w = truncf(this->w);
		}
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

		newVec.FixFloatingPointError();

		return newVec;
	}
	void Normalize() {
		float mag = this->GetMagnitude();

		if (mag == 0) {
			return;
		}

		this->x /= mag;
		this->y /= mag;
		this->z /= mag;
		this->w /= mag;

		this->FixFloatingPointError();
	}

	static float Dot(DreamVector4 A, DreamVector4 B) {
		return (A.x * B.x) + (A.y * B.y) + (A.z * B.z) + (A.w * B.w);
	}

	static DreamVector4 lerp(DreamVector4 A, DreamVector4 B, float time)
	{
		return DreamVector4(((1 - time)*A.x) + (time * B.x) ,
							((1 - time)*A.y) + (time * B.y), 
							((1 - time)*A.z) + (time * B.z),
							((1 - time)*A.w) + (time * B.w));
	}

#pragma region Operator OverLoads

	DreamVector4 operator+(DreamVector4 v) {
		DreamVector4 newVec = DreamVector4();
		newVec.x = this->x + v.x;
		newVec.y = this->y + v.y;
		newVec.z = this->z + v.z;
		newVec.w = this->w + v.w;

		return newVec;
	}
	DreamVector4 operator-(DreamVector4 v) {
		DreamVector4 newVec = DreamVector4();
		newVec.x = this->x - v.x;
		newVec.y = this->y - v.y;
		newVec.z = this->z - v.z;
		newVec.w = this->w - v.w;

		return newVec;
	}
	DreamVector4 operator*(DreamVector4 v) {
		DreamVector4 newVec = DreamVector4();
		newVec.x = this->x * v.x;
		newVec.y = this->y * v.y;
		newVec.z = this->z * v.z;
		newVec.w = this->w * v.w;

		return newVec;
	}
	DreamVector4 operator/(DreamVector4 v) {
		DreamVector4 newVec = DreamVector4();
		newVec.x = this->x / v.x;
		newVec.y = this->y / v.y;
		newVec.z = this->z / v.z;
		newVec.w = this->w / v.w;

		return newVec;
	}
	void operator+=(DreamVector4 v) {
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
		this->w += v.w;
	}
	void operator-=(DreamVector4 v) {
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;
		this->w -= v.w;
	}
	void operator*=(DreamVector4 v) {
		this->x *= v.x;
		this->y *= v.y;
		this->z *= v.z;
		this->w *= v.w;
	}
	void operator/=(DreamVector4 v) {
		this->x /= v.x;
		this->y /= v.y;
		this->z /= v.z;
		this->w /= v.w;

	}

	DreamVector4 operator*(int num) {
		DreamVector4 newVec = DreamVector4();
		newVec.x = this->x * num;
		newVec.y = this->y * num;
		newVec.z = this->z * num;
		newVec.w = this->w * num;
		return newVec;
	}
	DreamVector4  operator*(float num) {
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

	DreamVector4 operator/(int num) {
		DreamVector4 newVec = DreamVector4();
		newVec.x = this->x / num;
		newVec.y = this->y / num;
		newVec.z = this->z / num;
		newVec.w = this->w / num;
		return newVec;
	}
	DreamVector4  operator/(float num) {
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

	bool operator==(DreamVector4 v) {
		return (abs(this->x - v.x) <= EPSILON) && (abs(this->y - v.y) <= EPSILON) && (abs(this->z - v.z) <= EPSILON) && (abs(this->w - v.w) <= EPSILON);
	}

	DreamVector4 operator* (DreamMatrix4X4 m);
	void operator*= (DreamMatrix4X4 m);


#pragma endregion

	std::string ToString() {
		return std::string("[x: " + std::to_string(this->x) + ", y: " + std::to_string(this->y) + ", z: " + std::to_string(this->z) + ", w: " + std::to_string(this->w) + "]");
	}

};



union Matrix3 {
	float data[3][3];
	struct {
		DreamVector3 xRow;
		DreamVector3 yRow;
		DreamVector3 zRow;
	};
};

union Matrix4 {
	float data[4][4];
	struct {
		DreamVector4 xRow;
		DreamVector4 yRow;
		DreamVector4 zRow;
		DreamVector4 wRow;
	};
};

struct DreamMatrix3X3 {
public:


	Matrix3 matrix = { 0 };

	DreamMatrix3X3()
	{

	}

	DreamMatrix3X3(const DreamMatrix3X3& m) {

		for (int i = 0; i < (3 * 3); i++) {
			int curRow = i / 3;
			int curCol = i % 3;
			this->matrix.data[curRow][curCol] = m.matrix.data[curRow][curCol];
		}
	}

	void FixFloatingPointError() {

		for (int i = 0; i < (3 * 3); i++) {
			int curRow = i / 3;
			int curCol = i % 3;
			float truncatedNum = truncf(this->matrix.data[curRow][curCol]);
			if (DreamMath::abs(this->matrix.data[curRow][curCol] - truncatedNum) < EPSILON) {
				this->matrix.data[curRow][curCol] = truncatedNum;
			}
		}
	}

	void Transpose() {

		float store = 0;

		int curRow = 0;
		for (int i = 1; i < (3 * 3); i++) {
			int curCol = i % 3;

			store = this->matrix.data[curRow][curCol];
			this->matrix.data[curRow][curCol] = this->matrix.data[curCol][curRow];
			this->matrix.data[curCol][curRow] = store;

			if (curCol == 2) {
				curRow++;
				i = (curRow * 3) + curRow;
			}
		}
	}

	float Determinate() {

		float det = 0;

		for (int i = 0; i < 3; i++) {
			int curRow = 0;
			int curCol = i;

			int top = 1;
			int bot = 2;
			int left = (curCol == 0) ? 1 : 0;
			int right = (curCol == 2) ? 1 : 2;


			float smallDet = (this->matrix.data[top][left] * this->matrix.data[bot][right]) - (this->matrix.data[bot][left] * this->matrix.data[top][right]);

			if (i == 1) {
				det -= this->matrix.data[curRow][curCol] * smallDet;
			}
			else {
				det += this->matrix.data[curRow][curCol] * smallDet;
			}
		}

		return det;
	}

	void Inverse() {
		DreamMatrix3X3 newMatrix = DreamMatrix3X3();

		float det = this->Determinate();
		int sign = 1;

		for (int i = 0; i < (3 * 3); i++) {
			int curRow = i / 3;
			int curCol = i % 3;

			int top = (curRow == 0) ? 1 : 0;
			int bot = (curRow == 2) ? 1 : 2;
			int left = (curCol == 0) ? 1 : 0;
			int right = (curCol == 2) ? 1 : 2;


			newMatrix.matrix.data[curRow][curCol] = (this->matrix.data[top][left] * this->matrix.data[bot][right]) - (this->matrix.data[bot][left] * this->matrix.data[top][right]);
			newMatrix.matrix.data[curRow][curCol] *= sign;

			sign *= -1;
		}


		newMatrix.Transpose();

		newMatrix *= (1 / det);

		newMatrix.FixFloatingPointError();

		*this = newMatrix;
	}

	static DreamMatrix3X3 Identity() {
		DreamMatrix3X3 newMatrix = DreamMatrix3X3();

		newMatrix.matrix.data[0][0] = 1;
		newMatrix.matrix.data[1][1] = 1;
		newMatrix.matrix.data[2][2] = 1;

		return newMatrix;
	}

	std::string ToString() {

		std::string finalString = "";
		int matrixSize = 3;

		for (int i = 0; i < (matrixSize * matrixSize); i++) {
			int curRow = i / matrixSize;
			int curCol = i % matrixSize;

			if (finalString != "" && curCol == 0) {
				finalString += "\n";
			}
			if (curCol == 0) {
				finalString += "[ ";
			}

			finalString += std::to_string(this->matrix.data[curRow][curCol]);

			if (curCol == (matrixSize - 1)) {
				finalString += "]";
			}
			else {
				finalString += ", ";
			}
		}

		return finalString;
	}

#pragma region Operator Overload


	DreamMatrix3X3 operator*(DreamMatrix3X3 m) {
		DreamMatrix3X3 newMatrix = DreamMatrix3X3();

		for (int i = 0; i < (3 * 3); i++) {
			int curRow = i / 3;
			int curCol = i % 3;
			newMatrix.matrix.data[curRow][curCol] = (this->matrix.data[curRow][0] * m.matrix.data[0][curCol])
				+ (this->matrix.data[curRow][1] * m.matrix.data[1][curCol])
				+ (this->matrix.data[curRow][2] * m.matrix.data[2][curCol]);

		}

		return newMatrix;
	}
	void operator*=(DreamMatrix3X3 m) {

		DreamMatrix3X3 newMatrix = DreamMatrix3X3();

		for (int i = 0; i < (3 * 3); i++) {
			int curRow = i / 3;
			int curCol = i % 3;
			newMatrix.matrix.data[curRow][curCol] = (this->matrix.data[curRow][0] * m.matrix.data[0][curCol])
				+ (this->matrix.data[curRow][1] * m.matrix.data[1][curCol])
				+ (this->matrix.data[curRow][2] * m.matrix.data[2][curCol]);
		}

		*this = newMatrix;
	}
	DreamMatrix3X3 operator*(int num) {
		DreamMatrix3X3 newMatrix = DreamMatrix3X3();
		for (int i = 0; i < (3 * 3); i++) {
			int curRow = i / 3;
			int curCol = i % 3;
			newMatrix.matrix.data[curRow][curCol] = this->matrix.data[curRow][curCol] * num;
		}
		return newMatrix;
	}
	DreamMatrix3X3 operator*(float num) {
		DreamMatrix3X3 newMatrix = DreamMatrix3X3();
		for (int i = 0; i < (3 * 3); i++) {
			int curRow = i / 3;
			int curCol = i % 3;
			newMatrix.matrix.data[curRow][curCol] = this->matrix.data[curRow][curCol] * num;
		}
		return newMatrix;
	}
	void operator*=(int num) {
		for (int i = 0; i < (3 * 3); i++) {
			int curRow = i / 3;
			int curCol = i % 3;
			this->matrix.data[curRow][curCol] = this->matrix.data[curRow][curCol] * num;
		}
	}
	void operator*=(float num) {
		for (int i = 0; i < (3 * 3); i++) {
			int curRow = i / 3;
			int curCol = i % 3;
			this->matrix.data[curRow][curCol] = this->matrix.data[curRow][curCol] * num;
		}
	}
	DreamMatrix3X3 operator/(int num) {
		DreamMatrix3X3 newMatrix = DreamMatrix3X3();
		for (int i = 0; i < (3 * 3); i++) {
			int curRow = i / 3;
			int curCol = i % 3;
			newMatrix.matrix.data[curRow][curCol] = this->matrix.data[curRow][curCol] / num;
		}
		return newMatrix;
	}
	DreamMatrix3X3 operator/(float num) {
		DreamMatrix3X3 newMatrix = DreamMatrix3X3();
		for (int i = 0; i < (3 * 3); i++) {
			int curRow = i / 3;
			int curCol = i % 3;
			newMatrix.matrix.data[curRow][curCol] = this->matrix.data[curRow][curCol] / num;
		}
		return newMatrix;
	}
	void operator/=(int num) {
		for (int i = 0; i < (3 * 3); i++) {
			int curRow = i / 3;
			int curCol = i % 3;
			this->matrix.data[curRow][curCol] = this->matrix.data[curRow][curCol] / num;
		}
	}
	void operator/=(float num) {
		for (int i = 0; i < (3 * 3); i++) {
			int curRow = i / 3;
			int curCol = i % 3;
			this->matrix.data[curRow][curCol] = this->matrix.data[curRow][curCol] / num;
		}
	}

	bool operator==(DreamMatrix3X3 m) {
		for (int i = 0; i < (3 * 3); i++) {
			int curRow = i / 3;
			int curCol = i % 3;
			float num = this->matrix.data[curRow][curCol] - m.matrix.data[curRow][curCol];

			if (abs(num) > EPSILON) {
				return false;
			}
		}
		return true;
	}

#pragma endregion

};

struct DreamMatrix4X4 {
public:

	Matrix4 matrix = { 0 };


	DreamMatrix4X4()
	{
	}

	DreamMatrix4X4(const DreamMatrix4X4& m) {

		for (int i = 0; i < (4 * 4); i++) {
			int curRow = i / 4;
			int curCol = i % 4;
			this->matrix.data[curRow][curCol] = m.matrix.data[curRow][curCol];
		}
	}
	DreamMatrix4X4(const DreamMatrix3X3& m) {
		for (int i = 0; i < (3 * 3); i++) {
			int curRow = i / 3;
			int curCol = i % 3;
			this->matrix.data[curRow][curCol] = m.matrix.data[curRow][curCol];
		}
	}
	void FixFloatingPointError() {

		for (int i = 0; i < (4 * 4); i++) {
			int curRow = i / 4;
			int curCol = i % 4;
			float truncatedNum = truncf(this->matrix.data[curRow][curCol]);
			if (DreamMath::abs(this->matrix.data[curRow][curCol] - truncatedNum) < EPSILON) {
				this->matrix.data[curRow][curCol] = truncatedNum;
			}
		}
	}

	void Transpose() {

		float store = 0;

		int curRow = 0;
		for (int i = 1; i < (4 * 4); i++) {
			int curCol = i % 4;

			store = this->matrix.data[curRow][curCol];
			this->matrix.data[curRow][curCol] = this->matrix.data[curCol][curRow];
			this->matrix.data[curCol][curRow] = store;

			if (curCol == 3) {
				curRow++;
				i = (curRow * 4) + curRow;
			}
		}

	}

	float Determinate() {

		float det = 0;

		return det;
	}

	void Inverse() {

	}

	DreamMatrix3X3 Get3X3() {

		DreamMatrix3X3 newMatrix = DreamMatrix3X3();

		for (int i = 0; i < (3 * 3); i++) {
			int curRow = i / 3;
			int curCol = i % 3;
			newMatrix.matrix.data[curRow][curCol] = this->matrix.data[curRow][curCol];
		}
		return newMatrix;
	}

	static DreamMatrix4X4 Identity() {
		DreamMatrix4X4 newMatrix = DreamMatrix4X4();

		newMatrix.matrix.data[0][0] = 1;
		newMatrix.matrix.data[1][1] = 1;
		newMatrix.matrix.data[2][2] = 1;
		newMatrix.matrix.data[3][3] = 1;

		return newMatrix;
	}

#pragma region Operator Overloads


	DreamMatrix4X4 operator*(DreamMatrix4X4 m) {
		DreamMatrix4X4 newMatrix = DreamMatrix4X4();

		for (int i = 0; i < (4 * 4); i++) {
			int curRow = i / 4;
			int curCol = i % 4;
			newMatrix.matrix.data[curRow][curCol] = (this->matrix.data[curRow][0] * m.matrix.data[0][curCol])
				+ (this->matrix.data[curRow][1] * m.matrix.data[1][curCol])
				+ (this->matrix.data[curRow][2] * m.matrix.data[2][curCol])
				+ (this->matrix.data[curRow][3] * m.matrix.data[3][curCol]);

		}
		return newMatrix;
	}

	void operator*=(DreamMatrix4X4 m) {

		DreamMatrix4X4 newMatrix = DreamMatrix4X4();

		for (int i = 0; i < (4 * 4); i++) {
			int curRow = i / 4;
			int curCol = i % 4;
			newMatrix.matrix.data[curRow][curCol] = (this->matrix.data[curRow][0] * m.matrix.data[0][curCol])
				+ (this->matrix.data[curRow][1] * m.matrix.data[1][curCol])
				+ (this->matrix.data[curRow][2] * m.matrix.data[2][curCol])
				+ (this->matrix.data[curRow][3] * m.matrix.data[3][curCol]);

		}

		*this = newMatrix;
	}
	DreamMatrix4X4 operator*(int num) {
		DreamMatrix4X4 newMatrix = DreamMatrix4X4();
		for (int i = 0; i < (4 * 4); i++) {
			int curRow = i / 4;
			int curCol = i % 4;
			newMatrix.matrix.data[curRow][curCol] = this->matrix.data[curRow][curCol] * num;
		}
		return newMatrix;
	}
	DreamMatrix4X4  operator*(float num) {
		DreamMatrix4X4 newMatrix = DreamMatrix4X4();
		for (int i = 0; i < (4 * 4); i++) {
			int curRow = i / 4;
			int curCol = i % 4;
			newMatrix.matrix.data[curRow][curCol] = this->matrix.data[curRow][curCol] * num;
		}
		return newMatrix;
	}
	void operator*=(int num) {
		for (int i = 0; i < (4 * 4); i++) {
			int curRow = i / 4;
			int curCol = i % 4;
			this->matrix.data[curRow][curCol] = this->matrix.data[curRow][curCol] * num;
		}
	}
	void operator*=(float num) {
		for (int i = 0; i < (4 * 4); i++) {
			int curRow = i / 4;
			int curCol = i % 4;
			this->matrix.data[curRow][curCol] = this->matrix.data[curRow][curCol] * num;
		}
	}
	DreamMatrix4X4 operator/(int num) {
		DreamMatrix4X4 newMatrix = DreamMatrix4X4();
		for (int i = 0; i < (4 * 4); i++) {
			int curRow = i / 4;
			int curCol = i % 4;
			newMatrix.matrix.data[curRow][curCol] = this->matrix.data[curRow][curCol] / num;
		}
		return newMatrix;
	}
	DreamMatrix4X4  operator/(float num) {
		DreamMatrix4X4 newMatrix = DreamMatrix4X4();
		for (int i = 0; i < (4 * 4); i++) {
			int curRow = i / 4;
			int curCol = i % 4;
			newMatrix.matrix.data[curRow][curCol] = this->matrix.data[curRow][curCol] / num;
		}
		return newMatrix;
	}
	void operator/=(int num) {
		for (int i = 0; i < (4 * 4); i++) {
			int curRow = i / 4;
			int curCol = i % 4;
			this->matrix.data[curRow][curCol] = this->matrix.data[curRow][curCol] / num;
		}
	}
	void operator/=(float num) {
		for (int i = 0; i < (4 * 4); i++) {
			int curRow = i / 4;
			int curCol = i % 4;
			this->matrix.data[curRow][curCol] = this->matrix.data[curRow][curCol] / num;
		}
	}

	bool operator==(DreamMatrix4X4 m) {
		for (int i = 0; i < (4 * 4); i++) {
			int curRow = i / 4;
			int curCol = i % 4;
			float num = this->matrix.data[curRow][curCol] - m.matrix.data[curRow][curCol];

			if (abs(num) > EPSILON) {
				return false;
			}
		}
		return true;
	}

#pragma endregion

	std::string ToString() {
		std::string finalString = "";
		int matrixSize = 4;

		for (int i = 0; i < (matrixSize * matrixSize); i++) {
			int curRow = i / matrixSize;
			int curCol = i % matrixSize;

			if (finalString != "" && curCol == 0) {
				finalString += "\n";
			}
			if (curCol == 0) {
				finalString += "[ ";
			}

			finalString += std::to_string(this->matrix.data[curRow][curCol]);

			if (curCol == (matrixSize - 1)) {
				finalString += "]";
			}
			else {
				finalString += ", ";
			}
		}

		return finalString;
	}

};


struct DreamQuaternion {
public:
	DreamVector3 qVector;
	float wScalar;

	DreamQuaternion() {
		this->qVector = DreamVector3();
		this->wScalar = 0;
	}
	DreamQuaternion(DreamVector3 axisOfRot, float degrees) {

		float vecDegree = DreamMath::sin(degrees / 2);
		float scalDegree = DreamMath::cos(degrees / 2);

		this->qVector = axisOfRot * vecDegree;
		this->wScalar = scalDegree;

		//this->Normalize();
	}
	DreamQuaternion(DreamMatrix3X3 rotMatrix) {

	
	}
	DreamQuaternion(const DreamQuaternion& quat) {
		this->qVector = quat.qVector;
		this->wScalar = quat.wScalar;
	}
	void FixFloatingPointError() {
		this->qVector.FixFloatingPointError();
		this->wScalar = DreamMath::FixFloatingPointError(this->wScalar);
	}
	float GetMagnitude() {
		return DreamMath::sqrtf(this->qVector.GetSqrMagnitude() + (this->wScalar * this->wScalar));
	}
	void Normalize() {
		float mag = this->GetMagnitude();

		if (mag == 0) {
			return;
		}

		this->qVector /= mag;
		this->wScalar /= mag;

		this->FixFloatingPointError();
	}
#pragma region Operator Overloads


	DreamQuaternion operator* (DreamQuaternion quat) {
		DreamQuaternion returnQuat = DreamQuaternion();

		returnQuat.qVector = (this->qVector * quat.wScalar) + (quat.qVector * this->wScalar) + DreamVector3::Cross(this->qVector, quat.qVector);
		returnQuat.wScalar = (this->wScalar * quat.wScalar) - DreamVector3::Dot(this->qVector, quat.qVector);

		//returnQuat.Normalize();

		returnQuat.FixFloatingPointError();

		return returnQuat;
	}

	void operator*=(DreamQuaternion quat) {
		DreamQuaternion returnQuat = DreamQuaternion();

		returnQuat.qVector = (this->qVector * quat.wScalar) + (quat.qVector * this->wScalar) + DreamVector3::Cross(this->qVector, quat.qVector);
		returnQuat.wScalar = (this->wScalar * quat.wScalar) - DreamVector3::Dot(this->qVector, quat.qVector);

		//returnQuat.Normalize();

		returnQuat.FixFloatingPointError();

		*this = returnQuat;
	}

	DreamQuaternion operator* (float num) {
		DreamQuaternion returnQuat = DreamQuaternion();

		returnQuat.qVector = this->qVector * num;
		returnQuat.wScalar = this->wScalar * num;

		returnQuat.FixFloatingPointError();

		return returnQuat;
	}

	void operator*=(float num) {
		DreamQuaternion returnQuat = DreamQuaternion();

		returnQuat.qVector = this->qVector * num;
		returnQuat.wScalar = this->wScalar * num;

		returnQuat.FixFloatingPointError();

		*this = returnQuat;
	}

	DreamQuaternion operator/ (float num) {
		DreamQuaternion returnQuat = DreamQuaternion();

		returnQuat.qVector = this->qVector / num;
		returnQuat.wScalar = this->wScalar / num;

		returnQuat.FixFloatingPointError();

		return returnQuat;
	}

	void operator/=(float num) {
		DreamQuaternion returnQuat = DreamQuaternion();

		returnQuat.qVector = this->qVector / num;
		returnQuat.wScalar = this->wScalar / num;

		returnQuat.FixFloatingPointError();

		*this = returnQuat;
	}


	DreamQuaternion operator+ (DreamQuaternion quat) {
		DreamQuaternion returnQuat = DreamQuaternion();

		returnQuat.qVector = this->qVector + quat.qVector;
		returnQuat.wScalar = this->wScalar + quat.wScalar;

		return returnQuat;
	}

	void operator+=(DreamQuaternion quat) {
		DreamQuaternion returnQuat = DreamQuaternion();

		returnQuat.qVector = this->qVector + quat.qVector;
		returnQuat.wScalar = this->wScalar + quat.wScalar;

		*this = returnQuat;
	}

	bool operator==(DreamQuaternion quat) {
		return (this->qVector == quat.qVector) && (this->wScalar == quat.wScalar);
	}

#pragma endregion

	void Inverse() {
		this->qVector *= -1;
	}
	DreamQuaternion GetInverse() {
		DreamQuaternion returnQuat = *this;
		returnQuat.qVector *= -1;

		return returnQuat;
	}
	DreamMatrix3X3 GetMatrix() {
		DreamMatrix3X3 rotMatrix = DreamMatrix3X3();

		rotMatrix.matrix.data[0][0] = 1 - (2 * (this->qVector.y *this->qVector.y)) - (2 * (this->qVector.z *this->qVector.z));
		rotMatrix.matrix.data[1][1] = 1 - (2 * (this->qVector.x *this->qVector.x)) - (2 * (this->qVector.z *this->qVector.z));
		rotMatrix.matrix.data[2][2] = 1 - (2 * (this->qVector.x *this->qVector.x)) - (2 * (this->qVector.y *this->qVector.y));

		rotMatrix.matrix.data[0][1] = (2 * this->qVector.x * this->qVector.y) + (2 * this->qVector.z * this->wScalar);
		rotMatrix.matrix.data[1][0] = (2 * this->qVector.x * this->qVector.y) - (2 * this->qVector.z * this->wScalar);

		rotMatrix.matrix.data[0][2] = (2 * this->qVector.x * this->qVector.z) - (2 * this->qVector.y * this->wScalar);
		rotMatrix.matrix.data[2][0] = (2 * this->qVector.x * this->qVector.z) + (2 * this->qVector.y * this->wScalar);

		rotMatrix.matrix.data[1][2] = (2 * this->qVector.y * this->qVector.z) + (2 * this->qVector.x * this->wScalar);
		rotMatrix.matrix.data[2][1] = (2 * this->qVector.y * this->qVector.z) - (2 * this->qVector.x * this->wScalar);

		rotMatrix.FixFloatingPointError();

		return rotMatrix;
	}
	DreamVector3 RotateVector(float x, float y, float z) {

		DreamQuaternion vecToQuat = DreamQuaternion();
		vecToQuat.qVector = DreamVector3(x, y, z).GetNormalizedVector();
		vecToQuat.wScalar = 0;

		DreamQuaternion finalRot = ((*this * vecToQuat)* this->GetInverse());

		finalRot.FixFloatingPointError();

		return finalRot.qVector;
	}
	DreamVector3 RotateVector(DreamVector3 vec) {

		DreamQuaternion vecToQuat = DreamQuaternion();
		vecToQuat.qVector = vec.GetNormalizedVector();
		vecToQuat.wScalar = 0;

		DreamQuaternion finalRot = ((*this * vecToQuat)* this->GetInverse());

		finalRot.FixFloatingPointError();

		return finalRot.qVector;
	}
	static float Dot(DreamQuaternion A, DreamQuaternion B) {
		return (A.qVector.x * B.qVector.x) + (A.qVector.y * B.qVector.y) + (A.qVector.z * B.qVector.z) + (A.wScalar * B.wScalar);
	}
	static DreamQuaternion MakeQuaternionEuler(float x, float y, float z) {

		DreamQuaternion xQuat = DreamQuaternion(DreamVector3(1, 0, 0), x);
		DreamQuaternion yQuat = DreamQuaternion(DreamVector3(0, 1, 0), y);
		DreamQuaternion zQuat = DreamQuaternion(DreamVector3(0, 0, 1), z);

		DreamQuaternion finalQuat = ((zQuat * yQuat) * xQuat);

		finalQuat.FixFloatingPointError();

		return finalQuat;
	}
	static DreamQuaternion MakeQuaternionEuler(DreamVector3 rotation) {

		DreamQuaternion xQuat = DreamQuaternion(DreamVector3(1, 0, 0), rotation.x);
		DreamQuaternion yQuat = DreamQuaternion(DreamVector3(0, 1, 0), rotation.y);
		DreamQuaternion zQuat = DreamQuaternion(DreamVector3(0, 0, 1), rotation.z);

		DreamQuaternion finalQuat = ((zQuat * yQuat) * xQuat);

		finalQuat.FixFloatingPointError();

		return finalQuat;
	}
	static DreamQuaternion lerp(DreamQuaternion A, DreamQuaternion B, float time) {
		DreamQuaternion lerpQuat = (A * (1-time)) + (B * time);

		lerpQuat.Normalize();

		return lerpQuat / lerpQuat.GetMagnitude();
	}
	static DreamQuaternion slerp(DreamQuaternion A, DreamQuaternion B, float time) {

		float cosAngle = DreamQuaternion::Dot(A, B);
		float angle = DreamMath::acos(cosAngle);

		float wA = DreamMath::sin((1 - time) * angle) / DreamMath::sin(angle);
		float wB = DreamMath::sin(time * angle) / DreamMath::sin(angle);

		DreamQuaternion slerpQuat = (A * wA) + (B * wB);

		slerpQuat.Normalize();

		return slerpQuat;
	}

	std::string ToString() {
		return std::string("[x: " + std::to_string(this->qVector.x) + ", y: " + std::to_string(this->qVector.y) + ", z: " + std::to_string(this->qVector.z) + ", w: " + std::to_string(this->wScalar) + "]");
	}

};




// More Static functions

static DreamMatrix4X4 CreateTranslationMatrix(float x, float y, float z) {
	DreamMatrix4X4 translationMatrix = DreamMatrix4X4::Identity();
	translationMatrix.matrix.data[3][0] = x;
	translationMatrix.matrix.data[3][1] = y;
	translationMatrix.matrix.data[3][2] = z;
	return translationMatrix;
}
static DreamMatrix4X4 CreateRotationMatrix(float x, float y, float z) {
	DreamMatrix3X3 rotMatrixX = DreamMatrix3X3();
	DreamMatrix3X3 rotMatrixY = DreamMatrix3X3();
	DreamMatrix3X3 rotMatrixZ = DreamMatrix3X3();

	rotMatrixX.matrix.data[0][0] = 1;
	rotMatrixX.matrix.data[1][1] = DreamMath::cos(x);
	rotMatrixX.matrix.data[1][2] = DreamMath::sin(x);
	rotMatrixX.matrix.data[2][1] = -DreamMath::sin(x);
	rotMatrixX.matrix.data[2][2] = DreamMath::cos(x);

	rotMatrixY.matrix.data[1][1] = 1;
	rotMatrixY.matrix.data[0][0] = DreamMath::cos(y);
	rotMatrixY.matrix.data[0][2] = -DreamMath::sin(y);
	rotMatrixY.matrix.data[2][0] = DreamMath::sin(y);
	rotMatrixY.matrix.data[2][2] = DreamMath::cos(y);

	rotMatrixZ.matrix.data[2][2] = 1;
	rotMatrixZ.matrix.data[0][0] = DreamMath::cos(z);
	rotMatrixZ.matrix.data[0][1] = DreamMath::sin(z);
	rotMatrixZ.matrix.data[1][0] = -DreamMath::sin(z);
	rotMatrixZ.matrix.data[1][1] = DreamMath::cos(z);

	DreamMatrix3X3 finalRotMatrix = ((rotMatrixX * rotMatrixY) * rotMatrixZ);

	finalRotMatrix.FixFloatingPointError();

	DreamMatrix4X4 final4X4Mat = finalRotMatrix;
	final4X4Mat.matrix.data[3][3] = 1;

	return final4X4Mat;
}
static DreamMatrix4X4 CreateScaleMatrix(float x, float y, float z) {
	DreamMatrix4X4 scaleMatrix = DreamMatrix4X4();
	scaleMatrix.matrix.data[0][0] = x;
	scaleMatrix.matrix.data[1][1] = y;
	scaleMatrix.matrix.data[2][2] = z;
	scaleMatrix.matrix.data[3][3] = 1;
	return scaleMatrix;
}

static DreamMatrix4X4 CreateTranslationMatrix(DreamVector3 translation) {
	DreamMatrix4X4 translationMatrix = DreamMatrix4X4::Identity();
	translationMatrix.matrix.data[3][0] = translation.x;
	translationMatrix.matrix.data[3][1] = translation.y;
	translationMatrix.matrix.data[3][2] = translation.z;
	return translationMatrix;
}
static DreamMatrix4X4 CreateRotationMatrix(DreamVector3 rotation) {
	DreamMatrix3X3 rotMatrixX = DreamMatrix3X3();
	DreamMatrix3X3 rotMatrixY = DreamMatrix3X3();
	DreamMatrix3X3 rotMatrixZ = DreamMatrix3X3();

	rotMatrixX.matrix.data[0][0] = 1;
	rotMatrixX.matrix.data[1][1] = DreamMath::cos(rotation.x);
	rotMatrixX.matrix.data[1][2] = DreamMath::sin(rotation.x);
	rotMatrixX.matrix.data[2][1] = -DreamMath::sin(rotation.x);
	rotMatrixX.matrix.data[2][2] = DreamMath::cos(rotation.x);

	rotMatrixY.matrix.data[1][1] = 1;
	rotMatrixY.matrix.data[0][0] = DreamMath::cos(rotation.y);
	rotMatrixY.matrix.data[0][2] = -DreamMath::sin(rotation.y);
	rotMatrixY.matrix.data[2][0] = DreamMath::sin(rotation.y);
	rotMatrixY.matrix.data[2][2] = DreamMath::cos(rotation.y);

	rotMatrixZ.matrix.data[2][2] = 1;
	rotMatrixZ.matrix.data[0][0] = DreamMath::cos(rotation.z);
	rotMatrixZ.matrix.data[0][1] = DreamMath::sin(rotation.z);
	rotMatrixZ.matrix.data[1][0] = -DreamMath::sin(rotation.z);
	rotMatrixZ.matrix.data[1][1] = DreamMath::cos(rotation.z);

	DreamMatrix3X3 finalRotMatrix = ((rotMatrixX * rotMatrixY) * rotMatrixZ);

	finalRotMatrix.FixFloatingPointError();

	DreamMatrix4X4 final4X4Mat = finalRotMatrix;
	final4X4Mat.matrix.data[3][3] = 1;

	return final4X4Mat;
}
static DreamMatrix4X4 CreateScaleMatrix(DreamVector3 scale) {
	DreamMatrix4X4 scaleMatrix = DreamMatrix4X4();
	scaleMatrix.matrix.data[0][0] = scale.x;
	scaleMatrix.matrix.data[1][1] = scale.y;
	scaleMatrix.matrix.data[2][2] = scale.z;
	scaleMatrix.matrix.data[3][3] = 1;
	return scaleMatrix;
}


static DreamVector3 GetEulerFrom3X3Rot(DreamMatrix3X3 mat) {

}


struct DreamTransform {
public:
	DreamTransform() {
		forward = DreamVector3(0.0f, 0.0f, 1.0f);
		up = DreamVector3(0.0f, 1.0f, 0.0f);
		right = DreamVector3(1.0f, 0.0f, 0.0f);

		position = DreamVector3();
		rotation = DreamVector3();
		scale = DreamVector3(1.0f, 1.0f, 1.0f);
	}
	DreamVector3 position;
	DreamVector3 scale;

	DreamMatrix4X4 GetWorldMatrix() {
		return ((DreamMath::CreateScaleMatrix(scale)
			* DreamMath::CreateRotationMatrix(rotation))
			* DreamMath::CreateTranslationMatrix(position));
	}

	DreamVector3 GetForward() {
		return forward;
	}
	DreamVector3 GetUp() {
		return up;
	}
	DreamVector3 GetRight() {
		return right;
	}

	void UpdateAxis() {
		this->forward.Normalize();
		this->right = DreamVector3::Cross(this->forward, DreamVector3(0.0f, 1.0f, 0.0f));
		this->up = DreamVector3::Cross(this->right, this->forward);
	}

	void SetRotation(DreamVector3 newRot) {
		this->rotation = newRot;
		DreamQuaternion quat = DreamQuaternion::MakeQuaternionEuler(this->rotation);

		this->forward = quat.RotateVector(0,0,1);
		UpdateAxis();
	}

	void Rotate(DreamVector3 rot) {
		this->rotation += rot;
		DreamQuaternion quat = DreamQuaternion::MakeQuaternionEuler(this->rotation);

		this->forward = quat.RotateVector(0, 0, 1);
		UpdateAxis();
	}

	void SetForward(DreamVector3 newForward) {

		this->forward = newForward;
		this->right = DreamVector3::Cross(this->forward, DreamVector3(0.0f, 1.0f, 0.0f));
		this->up = DreamVector3::Cross(this->right, this->forward);

		SetRotationFromDirections();
	}
	void SetUp(DreamVector3 newUp) {

		this->up = newUp;
		this->forward = DreamVector3::Cross(this->up, DreamVector3(1.0f, 0.0f, 0.0f));
		this->right = DreamVector3::Cross(this->forward, this->up);

		SetRotationFromDirections();
	}
	void SetRight(DreamVector3 newRight) {

		this->right = newRight;
		this->up = DreamVector3::Cross(this->right, DreamVector3(0.0f, 0.0f, 1.0f));
		this->forward = DreamVector3::Cross(this->up, this->right);

		SetRotationFromDirections();
	}

	void LookAt(DreamVector3 target, DreamVector3 staticUp) {

		this->forward = target - position;
		this->right = DreamVector3::Cross(this->forward, staticUp);
		this->up = DreamVector3::Cross(this->right, this->forward);

		SetRotationFromDirections();

	}

private:
	DreamVector3 rotation;

	DreamVector3 forward;
	DreamVector3 up;
	DreamVector3 right;

	void SetRotationFromDirections() {

		this->forward.Normalize();
		this->right.Normalize();
		this->up.Normalize();

		DreamMatrix3X3 newRot = DreamMatrix3X3();

		newRot.matrix.data[0][0] = this->right.x;
		newRot.matrix.data[0][1] = this->right.y;
		newRot.matrix.data[0][2] = this->right.z;

		newRot.matrix.data[1][0] = this->up.x;
		newRot.matrix.data[1][1] = this->up.y;
		newRot.matrix.data[1][2] = this->up.z;

		newRot.matrix.data[2][0] = this->forward.x;
		newRot.matrix.data[2][1] = this->forward.y;
		newRot.matrix.data[2][2] = this->forward.z;

		rotation = DreamMath::GetEulerFrom3X3Rot(newRot);
	}
};

static DreamMatrix4X4 CreateProjectionMatix(const float& angleOfView, const float& near, const float& far, const float& aspectRatio, const float& zoom) {

	DreamMatrix4X4 projMat;
	// set the basic projection matrix
	float scale = 1.0f / (DreamMath::tan(angleOfView * 0.5f) / zoom);
	float zNormalScale = -far / (far - near);
	float wNormalScale = zNormalScale * near;

	projMat.matrix.xRow = DreamVector4(aspectRatio * scale, 0.0f, 0.0f, 0.0f);
	projMat.matrix.yRow = DreamVector4(0.0f, scale, 0.0f, 0.0f);
	projMat.matrix.zRow = DreamVector4(0.0f, 0.0f, zNormalScale, -1.0f);
	projMat.matrix.wRow = DreamVector4(0.0f, 0.0f, wNormalScale, 0.0f);

	return projMat;
}

static DreamMatrix4X4 LookAtViewMatix(DreamVector3 eye, DreamVector3 target, DreamVector3 up) {
	DreamVector3 zaxis = (eye - target).GetNormalizedVector();   // The "forward" vector.
	DreamVector3 xaxis = (DreamVector3::Cross(up, zaxis)).GetNormalizedVector();// The "right" vector.
	DreamVector3 yaxis = DreamVector3::Cross(zaxis, xaxis);     // The "up" vector.

	DreamVector3 waxis = DreamVector3(-DreamVector3::Dot(xaxis, eye), -DreamVector3::Dot(yaxis, eye), -DreamVector3::Dot(zaxis, eye));

	DreamMatrix4X4 lookAtMat;
	lookAtMat.matrix.xRow = DreamVector4(xaxis, 0.0f);
	lookAtMat.matrix.yRow = DreamVector4(yaxis, 0.0f);
	lookAtMat.matrix.zRow = DreamVector4(zaxis, 0.0f);
	lookAtMat.Transpose();
	lookAtMat.matrix.wRow = DreamVector4(waxis, 1.0f);

	return lookAtMat;
}

};



//#ifdef ROW_MAJOR
//
//float x1, y1, z1, w1 = 0;
//float x2, y2, z2, w2 = 0;
//float x3, y3, z3, w3 = 0;
//float x4, y4, z4, w4 = 0;
//
//#elif defined COL_MAJOR
//float x1, x2, x3, x4 = 0;
//float y1, y2, y3, y4 = 0;
//float z1, z2, z3, z4 = 0;
//float w1, w2, w3, w4 = 0;
//#endif