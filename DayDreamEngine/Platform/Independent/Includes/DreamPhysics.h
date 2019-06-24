#pragma once
#include <DreamMath.h>

namespace DayDreamEngine {
	namespace Physics {
		using namespace Math;

		class DreamPhysics {
		public:
			static bool Sphere_SphereCollision(DreamVector3 position1, float radius1, DreamVector3 position2, float radius2);
		};
	}
}