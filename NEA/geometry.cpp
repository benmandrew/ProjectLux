#include <math.h>
#include <algorithm>
#include "geometry.h"

// ------------------------------------ //
//               Vector 3               //
// ------------------------------------ //

// Default constructor
Vec3::Vec3(float _x, float _y, float _z)
	: x(_x), y(_y), z(_z) {
}
// Copy constructor
Vec3::Vec3(const Vec3& other)
	: x(other.x), y(other.y), z(other.z) {
}
// Dot product
float Vec3::dot(const Vec3& other) const {
	return x * other.x + y * other.y + z * other.z;
}
// Cross product
Vec3 Vec3::cross(const Vec3& other) const {
	return Vec3(
		y * other.z - z * other.y,
		z * other.x - x * other.z,
		x * other.y - y * other.x);
}
// Addition operator
Vec3 Vec3::operator+(const Vec3& other) const {
	return Vec3(x + other.x, y + other.y, z + other.z);
}
// Subtraction operator
Vec3 Vec3::operator-(const Vec3& other) const {
	return Vec3(x - other.x, y - other.y, z - other.z);
}
// Vector-scalar multiplication operator
Vec3 Vec3::operator*(const float other) const {
	return Vec3(x * other, y * other, z * other);
}
// Component-wise vector-vector multiplication operator
Vec3 Vec3::operator*(const Vec3& other) const {
	return Vec3(x * other.x, y * other.y, z * other.z);
}
// Vector-scalar division operator
Vec3 Vec3::operator/(const float other) const {
	return Vec3(x / other, y / other, z / other);
}
// Get the magnitude of the vector
float Vec3::getLength() const {
	return sqrt(x * x + y * y + z * z);
}
// Return a normalised version of the vector (i.e of magnitude 1)
Vec3 Vec3::normalise() const {
	float length = getLength();
	return Vec3(x / length, y / length, z / length);
}
// Get the vector in the opposite direction with the same magnitude
Vec3 Vec3::getReverse() const {
	return Vec3(-x, -y, -z);
}

// -------------------------------------- //
//               Matrix 3x3               //
// -------------------------------------- //

// Default constructor
Mat3::Mat3(float _x0, float _y0, float _z0, float _x1, float _y1, float _z1, float _x2, float _y2, float _z2)
	: x0(_x0), y0(_y0), z0(_z0), x1(_x1), y1(_y1), z1(_z1), x2(_x2), y2(_y2), z2(_z2) {
}
// Copy constructor
Mat3::Mat3(const Mat3& other)
	: x0(other.x0), y0(other.y0), z0(other.z0),
	x1(other.x1), y1(other.y1), z1(other.z1),
	x2(other.x2), y2(other.y2), z2(other.z2) {
}
// Get the matrix multiplied by itself
Mat3 Mat3::getSquare() const {
	return Mat3(
		x0 * x0 + y0 * x1 + z0 * x2,
		x0 * y0 + y0 * y1 + z0 * y2,
		x0 * z0 + y0 * z1 + z0 * z2,
		x1 * x0 + y1 * x1 + z1 * x2,
		x1 * y0 + y1 * y1 + z1 * y2,
		x1 * z0 + y1 * z1 + z1 * z2,
		x2 * x0 + y2 * x1 + z2 * x2,
		x2 * y0 + y2 * y1 + z2 * y2,
		x2 * z0 + y2 * z1 + z2 * z2);
}
// Addition operator
Mat3 Mat3::operator+(const Mat3& other) const {
	return Mat3(
		x0 + other.x0, y0 + other.y0, z0 + other.z0,
		x1 + other.x1, y1 + other.y1, z1 + other.z1,
		x2 + other.x2, y2 + other.y2, z2 + other.z2);
}
// Matrix-scalar multiplication operator
Mat3 Mat3::operator*(const float other) const {
	return Mat3(
		x0 * other, y0 * other, z0 * other,
		x1 * other, y1 * other, z1 * other,
		x2 * other, y2 * other, z2 * other);
}
// Matrix-vector multiplication operator
Vec3 Mat3::operator*(const Vec3& other) const {
	return Vec3(
		other.x * x0 + other.y * y0 + other.z * z0,
		other.x * x1 + other.y * y1 + other.z * z1,
		other.x * x2 + other.y * y2 + other.z * z2);
}
// Matrix-matrix multiplication operator
Mat3 Mat3::operator*(const Mat3& other) const {
	return Mat3(
		x0 * other.x0 + y0 * other.x1 + z0 * other.x2,
		x0 * other.y0 + y0 * other.y1 + z0 * other.y2,
		x0 * other.z0 + y0 * other.z1 + z0 * other.z2,
		x1 * other.x0 + y1 * other.x1 + z1 * other.x2,
		x1 * other.y0 + y1 * other.y1 + z1 * other.y2,
		x1 * other.z0 + y1 * other.z1 + z1 * other.z2,
		x2 * other.x0 + y2 * other.x1 + z2 * other.x2,
		x2 * other.y0 + y2 * other.y1 + z2 * other.y2,
		x2 * other.z0 + y2 * other.z1 + z2 * other.z2);
}

// ------------------------------- //
//               Ray               //
// ------------------------------- //

// Default constructor
Ray::Ray(Vec3 _origin, Vec3 _direction)
	: origin(_origin), direction(_direction.normalise()) {
}
// Copy constructor
Ray::Ray(const Ray& other)
	: origin(other.origin), direction(other.direction) {
}
// Get the origin of the ray
Vec3 Ray::getOrigin() const {
	return origin;
}
// Get the direction of the ray
Vec3 Ray::getDirection() const {
	return direction;
}
// Set the origin of the ray
void Ray::setOrigin(const Vec3& _origin) {
	origin = _origin;
}
// Set the direction of the ray
void Ray::setDirection(const Vec3& _direction) {
	direction = _direction;
}
// Normalise the direction of the ray
void Ray::normalise() {
	direction = direction.normalise();
}
// Get the point at distance 't' from the origin along the direction vector
Vec3 Ray::project(const float t) const {
	return origin + direction * t;
}
