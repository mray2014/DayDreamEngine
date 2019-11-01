#pragma once
#if defined DREAM_OPENGL
#define COL_MAJOR
#elif defined DREAM_DX
#define ROW_MAJOR
#elif defined DREAM_VULKAN
#define COL_MAJOR
#endif


#define PI 3.14159265358979323846

#define RAD2DEG (float)(180 / PI)

#define DEG2RAD (float)(PI / 180)

struct DreamVector2;
struct DreamVector3;
struct DreamVector4;

struct DreamMatrix2x2;
struct DreamMatrix3x3;
struct DreamMatrix4x4;

class DreamMath {
public:
	static float sin(float degrees);
	static float cos(float degrees);
	static float tan(float degrees);
	static float asin(float num);
	static float acos(float num);
	static float atan(float num);
	static float pow(float num, float exp);
	static float floor(float a);
	static float ceiling(float a);
	static float rad2deg(float radians);
	static float deg2rad(float degrees);
	static float sqrtf(float num);
	static float lerp(float A, float B, float time);
	static float lerp(DreamVector2 A, DreamVector2 B, float time);
	static float lerp(DreamVector3 A, DreamVector3 B, float time);
	static float lerp(DreamVector4 A, DreamVector4 B, float time);
	static float Dot(float* vec1, float* vec2, int size);
	static float Dot(float vec1[], float vec2[]);
};


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
		return DreamMath::sqrtf((x * x) + (y * y) + (z * z));
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
		return DreamMath::sqrtf((x * x) + (y * y) + (z * z) + (w * w));
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
	float matrix[3][3];


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

struct DreamMatrix4X4 {
public:
	float matrix[4][4];

	void Transpose() {

		float store = 0;

		store = this->matrix[1][0];
		this->matrix[1][0] = this->matrix[0][1];
		this->matrix[0][1] = store;

		store = this->matrix[2][0];
		this->matrix[2][0] = this->matrix[0][2];
		this->matrix[0][2] = store;

		store = this->matrix[3][0];
		this->matrix[3][0] = this->matrix[0][3];
		this->matrix[0][3] = store;

		store = this->matrix[2][1];
		this->matrix[2][1] = this->matrix[1][2];
		this->matrix[1][2] = store;

		store = this->matrix[3][1];
		this->matrix[3][1] = this->matrix[1][3];
		this->matrix[1][3] = store;

		store = this->matrix[3][2];
		this->matrix[3][2] = this->matrix[2][3];
		this->matrix[2][3] = store;

	}
	void Inverse() {

	}

	float Determinate() {

		return (this->matrix[0][0] * this->matrix[0][1] * this->matrix[0][2] * this->matrix[0][3]) 
			- (this->matrix[1][0] * this->matrix[1][1] * this->matrix[1][2] * this->matrix[1][3])
			- (this->matrix[2][0] * this->matrix[2][1] * this->matrix[2][2] * this->matrix[2][3])
			- (this->matrix[3][0] * this->matrix[3][1] * this->matrix[3][2] * this->matrix[3][3]);
	}

	static DreamMatrix4X4 Identity() {
		DreamMatrix4X4 newMatrix = DreamMatrix4X4();

		newMatrix.matrix[0][0] = 1;
		newMatrix.matrix[1][1] = 1;
		newMatrix.matrix[2][2] = 1;
		newMatrix.matrix[3][3] = 1;

		return newMatrix;
	}
	static DreamMatrix4X4 CreateTranslationMatrix(DreamVector3 translation) {

	}
	static DreamMatrix4X4 CreateRotationMatrix(DreamVector3 rotation) {

	}
	static DreamMatrix4X4 CreateScaleMatrix(DreamVector3 scale) {

	}

	DreamMatrix4X4()
	{
	}

	DreamMatrix4X4(const DreamMatrix4X4& m) {

		this->matrix[0][0] = m.matrix[0][0];
		this->matrix[0][1] = m.matrix[0][1];
		this->matrix[0][2] = m.matrix[0][2];
		this->matrix[0][3] = m.matrix[0][3];


		this->matrix[1][0] = m.matrix[1][0];
		this->matrix[1][1] = m.matrix[1][1];
		this->matrix[1][2] = m.matrix[1][2];
		this->matrix[1][3] = m.matrix[1][3];


		this->matrix[2][0] = m.matrix[2][0];
		this->matrix[2][1] = m.matrix[2][1];
		this->matrix[2][2] = m.matrix[2][2];
		this->matrix[2][3] = m.matrix[2][3];


		this->matrix[3][0] = m.matrix[3][0];
		this->matrix[3][1] = m.matrix[3][1];
		this->matrix[3][2] = m.matrix[3][2];
		this->matrix[3][3] = m.matrix[3][3];
	}

	DreamMatrix4X4 operator*(DreamMatrix4X4 m) {
		DreamMatrix4X4 newMatrix = DreamMatrix4X4();

		newMatrix.matrix[0][0] = (this->matrix[0][0] * m.matrix[0][0]) 
								+ (this->matrix[0][1] * m.matrix[1][0])
								+ (this->matrix[0][2] * m.matrix[2][0]) 
								+ (this->matrix[0][3] * m.matrix[3][0]);
		newMatrix.matrix[0][1] = (this->matrix[0][0] * m.matrix[0][1])
								+ (this->matrix[0][1] * m.matrix[1][1])
								+ (this->matrix[0][2] * m.matrix[2][1])
								+ (this->matrix[0][3] * m.matrix[3][1]);
		newMatrix.matrix[0][2] = (this->matrix[0][0] * m.matrix[0][2])
								+ (this->matrix[0][1] * m.matrix[1][2])
								+ (this->matrix[0][2] * m.matrix[2][2])
								+ (this->matrix[0][3] * m.matrix[3][2]);
		newMatrix.matrix[0][3] = (this->matrix[0][0] * m.matrix[0][3])
								+ (this->matrix[0][1] * m.matrix[1][3])
								+ (this->matrix[0][2] * m.matrix[2][3])
								+ (this->matrix[0][3] * m.matrix[3][3]);


		newMatrix.matrix[1][0] = (this->matrix[1][0] * m.matrix[0][0])
								+ (this->matrix[1][1] * m.matrix[1][0])
								+ (this->matrix[1][2] * m.matrix[2][0])
								+ (this->matrix[1][3] * m.matrix[3][0]);
		newMatrix.matrix[1][1] = (this->matrix[1][0] * m.matrix[0][1])
								+ (this->matrix[1][1] * m.matrix[1][1])
								+ (this->matrix[1][2] * m.matrix[2][1])
								+ (this->matrix[1][3] * m.matrix[3][1]);
		newMatrix.matrix[1][2] = (this->matrix[1][0] * m.matrix[0][2])
								+ (this->matrix[1][1] * m.matrix[1][2])
								+ (this->matrix[1][2] * m.matrix[2][2])
								+ (this->matrix[1][3] * m.matrix[3][2]);
		newMatrix.matrix[1][3] = (this->matrix[1][0] * m.matrix[0][3])
								+ (this->matrix[1][1] * m.matrix[1][3])
								+ (this->matrix[1][2] * m.matrix[2][3])
								+ (this->matrix[1][3] * m.matrix[3][3]);


		newMatrix.matrix[2][0] = (this->matrix[2][0] * m.matrix[0][0])
								+ (this->matrix[2][1] * m.matrix[1][0])
								+ (this->matrix[2][2] * m.matrix[2][0])
								+ (this->matrix[2][3] * m.matrix[3][0]);
		newMatrix.matrix[2][1] = (this->matrix[2][0] * m.matrix[0][1])
								+ (this->matrix[2][1] * m.matrix[1][1])
								+ (this->matrix[2][2] * m.matrix[2][1])
								+ (this->matrix[2][3] * m.matrix[3][1]);
		newMatrix.matrix[2][2] = (this->matrix[2][0] * m.matrix[0][2])
								+ (this->matrix[2][1] * m.matrix[1][2])
								+ (this->matrix[2][2] * m.matrix[2][2])
								+ (this->matrix[2][3] * m.matrix[3][2]);
		newMatrix.matrix[2][3] = (this->matrix[2][0] * m.matrix[0][3])
								+ (this->matrix[2][1] * m.matrix[1][3])
								+ (this->matrix[2][2] * m.matrix[2][3])
								+ (this->matrix[2][3] * m.matrix[3][3]);



		newMatrix.matrix[3][0] = (this->matrix[3][0] * m.matrix[0][0])
								+ (this->matrix[3][1] * m.matrix[1][0])
								+ (this->matrix[3][2] * m.matrix[2][0])
								+ (this->matrix[3][3] * m.matrix[3][0]);
		newMatrix.matrix[3][1] = (this->matrix[3][0] * m.matrix[0][1])
								+ (this->matrix[3][1] * m.matrix[1][1])
								+ (this->matrix[3][2] * m.matrix[2][1])
								+ (this->matrix[3][3] * m.matrix[3][1]);
		newMatrix.matrix[3][2] = (this->matrix[3][0] * m.matrix[0][2])
								+ (this->matrix[3][1] * m.matrix[1][2])
								+ (this->matrix[3][2] * m.matrix[2][2])
								+ (this->matrix[3][3] * m.matrix[3][2]);
		newMatrix.matrix[3][3] = (this->matrix[3][0] * m.matrix[0][3])
								+ (this->matrix[3][1] * m.matrix[1][3])
								+ (this->matrix[3][2] * m.matrix[2][3])
								+ (this->matrix[3][3] * m.matrix[3][3]);

		return newMatrix;
	}
	void operator*=(DreamMatrix4X4 m) {

		DreamMatrix4X4 newMatrix = DreamMatrix4X4();

		newMatrix.matrix[0][0] = (this->matrix[0][0] * m.matrix[0][0])
			+ (this->matrix[0][1] * m.matrix[1][0])
			+ (this->matrix[0][2] * m.matrix[2][0])
			+ (this->matrix[0][3] * m.matrix[3][0]);
		newMatrix.matrix[0][1] = (this->matrix[0][0] * m.matrix[0][1])
			+ (this->matrix[0][1] * m.matrix[1][1])
			+ (this->matrix[0][2] * m.matrix[2][1])
			+ (this->matrix[0][3] * m.matrix[3][1]);
		newMatrix.matrix[0][2] = (this->matrix[0][0] * m.matrix[0][2])
			+ (this->matrix[0][1] * m.matrix[1][2])
			+ (this->matrix[0][2] * m.matrix[2][2])
			+ (this->matrix[0][3] * m.matrix[3][2]);
		newMatrix.matrix[0][3] = (this->matrix[0][0] * m.matrix[0][3])
			+ (this->matrix[0][1] * m.matrix[1][3])
			+ (this->matrix[0][2] * m.matrix[2][3])
			+ (this->matrix[0][3] * m.matrix[3][3]);


		newMatrix.matrix[1][0] = (this->matrix[1][0] * m.matrix[0][0])
			+ (this->matrix[1][1] * m.matrix[1][0])
			+ (this->matrix[1][2] * m.matrix[2][0])
			+ (this->matrix[1][3] * m.matrix[3][0]);
		newMatrix.matrix[1][1] = (this->matrix[1][0] * m.matrix[0][1])
			+ (this->matrix[1][1] * m.matrix[1][1])
			+ (this->matrix[1][2] * m.matrix[2][1])
			+ (this->matrix[1][3] * m.matrix[3][1]);
		newMatrix.matrix[1][2] = (this->matrix[1][0] * m.matrix[0][2])
			+ (this->matrix[1][1] * m.matrix[1][2])
			+ (this->matrix[1][2] * m.matrix[2][2])
			+ (this->matrix[1][3] * m.matrix[3][2]);
		newMatrix.matrix[1][3] = (this->matrix[1][0] * m.matrix[0][3])
			+ (this->matrix[1][1] * m.matrix[1][3])
			+ (this->matrix[1][2] * m.matrix[2][3])
			+ (this->matrix[1][3] * m.matrix[3][3]);


		newMatrix.matrix[2][0] = (this->matrix[2][0] * m.matrix[0][0])
			+ (this->matrix[2][1] * m.matrix[1][0])
			+ (this->matrix[2][2] * m.matrix[2][0])
			+ (this->matrix[2][3] * m.matrix[3][0]);
		newMatrix.matrix[2][1] = (this->matrix[2][0] * m.matrix[0][1])
			+ (this->matrix[2][1] * m.matrix[1][1])
			+ (this->matrix[2][2] * m.matrix[2][1])
			+ (this->matrix[2][3] * m.matrix[3][1]);
		newMatrix.matrix[2][2] = (this->matrix[2][0] * m.matrix[0][2])
			+ (this->matrix[2][1] * m.matrix[1][2])
			+ (this->matrix[2][2] * m.matrix[2][2])
			+ (this->matrix[2][3] * m.matrix[3][2]);
		newMatrix.matrix[2][3] = (this->matrix[2][0] * m.matrix[0][3])
			+ (this->matrix[2][1] * m.matrix[1][3])
			+ (this->matrix[2][2] * m.matrix[2][3])
			+ (this->matrix[2][3] * m.matrix[3][3]);



		newMatrix.matrix[3][0] = (this->matrix[3][0] * m.matrix[0][0])
			+ (this->matrix[3][1] * m.matrix[1][0])
			+ (this->matrix[3][2] * m.matrix[2][0])
			+ (this->matrix[3][3] * m.matrix[3][0]);
		newMatrix.matrix[3][1] = (this->matrix[3][0] * m.matrix[0][1])
			+ (this->matrix[3][1] * m.matrix[1][1])
			+ (this->matrix[3][2] * m.matrix[2][1])
			+ (this->matrix[3][3] * m.matrix[3][1]);
		newMatrix.matrix[3][2] = (this->matrix[3][0] * m.matrix[0][2])
			+ (this->matrix[3][1] * m.matrix[1][2])
			+ (this->matrix[3][2] * m.matrix[2][2])
			+ (this->matrix[3][3] * m.matrix[3][2]);
		newMatrix.matrix[3][3] = (this->matrix[3][0] * m.matrix[0][3])
			+ (this->matrix[3][1] * m.matrix[1][3])
			+ (this->matrix[3][2] * m.matrix[2][3])
			+ (this->matrix[3][3] * m.matrix[3][3]);

		*this = newMatrix;
	}
};

struct DreamTransform {
public:
	DreamVector3 position;
	DreamVector3 rotation;
	DreamVector3 scale;

	DreamMatrix4X4 GetWorldMatrix() {
		return DreamMatrix4X4::CreateScaleMatrix(scale)
			* DreamMatrix4X4::CreateRotationMatrix(rotation)
			* DreamMatrix4X4::CreateTranslationMatrix(position);
	}
};
