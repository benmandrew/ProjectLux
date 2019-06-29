
#include "transform.h"

constexpr float PI = 3.14159265;
constexpr float DEG2RAD = PI / 180.0;

Transform::Transform(float rotX, float rotY, float rotZ, bool _flipX, bool _flipY, bool _flipZ)
	: flipX(_flipX), flipY(_flipY), flipZ(_flipZ) {
	Mat3 rotMat = calcRotMat(rotX, rotY, rotZ);
	Mat3 refMat = calcRefMat(_flipX, _flipY, _flipZ);
	mat = rotMat * refMat;
}

Transform::Transform(const Transform& other) 
	: mat(other.mat) {
}

Vec3 Transform::transform(const Vec3& vec) const {
	return mat * vec;
}

void Transform::flipVertexIndices(int& v0Index, int& v1Index) const {
	bool flipOneAxis = flipX != flipY != flipZ;
	bool flipAllAxes = flipX && flipY && flipZ;
	if (flipOneAxis || flipAllAxes) { // XOR or all equal
		std::swap(v0Index, v1Index);
	}
}

Mat3 Transform::calcRotMat(const float rotX, const float rotY, const float rotZ) const {
	float cx = cos(rotX * DEG2RAD);
	float sx = sin(rotX * DEG2RAD);
	float cy = cos(rotY * DEG2RAD);
	float sy = sin(rotY * DEG2RAD);
	float cz = cos(rotZ * DEG2RAD);
	float sz = sin(rotZ * DEG2RAD);
	Mat3 rotXMat = Mat3(
		1.0f, 0.0f, 0.0f,
		0.0f, cx, -sx,
		0.0f, sx, cx);
	Mat3 rotYMat = Mat3(
		cy, 0.0f, sy,
		0.0f, 1.0f, 0.0f,
		-sy, 0.0f, cy);
	Mat3 rotZMat = Mat3(
		cz, -sz, 0.0f,
		sz, cz, 0.0f,
		0.0f, 0.0f, 1.0f);
	return rotXMat * rotYMat * rotZMat;
}

Mat3 Transform::calcRefMat(const bool flipX, const bool flipY, const bool flipZ) const {
	return Mat3(
		1 - 2 * flipX, 0, 0,
		0, 1 - 2 * flipY, 0,
		0, 0, 1 - 2 * flipZ
	);
}