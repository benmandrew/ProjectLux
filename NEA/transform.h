#pragma once
#include "geometry.h"
#include <math.h>
#include <algorithm>

struct Transform {

	Transform(
		float rotX = 0.0f,
		float rotY = 0.0f,
		float rotZ = 0.0f,
		bool _flipX = false,
		bool _flipY = false,
		bool _flipZ = false
	);
	Transform(const Transform& other);

	Vec3 transform(const Vec3& vector) const;
	void flipVertexIndices(int& v0Index, int& v1Index) const;
private:
	Mat3 mat;
	bool flipX, flipY, flipZ;

	Mat3 calcRotMat(const float rotX, const float rotY, const float rotZ) const;
	Mat3 calcRefMat(const bool flipX, const bool flipY, const bool flipZ) const;
};