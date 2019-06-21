#pragma once
namespace DayDreamMath {
	struct DreamVector2{
	public:
		float x = 0;
		float y = 0;

		DreamVector2(float newX = 0, float newY = 0) {
			x = newX;
			y = newY;
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
	};

	class DreamMath {
	public:
		static int AddNum(int a, int b);
	};
}

