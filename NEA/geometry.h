#pragma once

struct Vec3 {
	float x, y, z;

	Vec3(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f);
	Vec3(const Vec3& other);
	float dot(const Vec3& other) const;
	Vec3 cross(const Vec3& other) const;
	Vec3 operator+(const Vec3& other) const;
	Vec3 operator-(const Vec3& other) const;
	Vec3 operator*(const float other) const;
	Vec3 operator*(const Vec3& other) const;
	Vec3 operator/(const float other) const;
	float getLength() const;
	Vec3 normalise() const;
	Vec3 getReverse() const;
};

struct Mat3 {
	float x0, y0, z0, x1, y1, z1, x2, y2, z2;

	Mat3(
		float _x0 = 1.0f, float _y0 = 0.0f, float _z0 = 0.0f,
		float _x1 = 0.0f, float _y1 = 1.0f, float _z1 = 0.0f,
		float _x2 = 0.0f, float _y2 = 0.0f, float _z2 = 1.0f);
	Mat3(const Mat3& other);

	Mat3 getSquare() const;
	Mat3 operator+(const Mat3& other) const;
	Mat3 operator*(const float other) const;
	Vec3 operator*(const Vec3& other) const;
	Mat3 operator*(const Mat3& other) const;
};

struct Ray {
private:
	Vec3 origin;
	Vec3 direction;
public:
	Ray(Vec3 _origin = Vec3(), Vec3 _direction = Vec3(1.0f, 0.0f, 0.0f));
	Ray(const Ray& other);

	Vec3 getOrigin() const;
	Vec3 getDirection() const;
	void setOrigin(const Vec3& _origin);
	void setDirection(const Vec3& _direction);
	void normalise();
	Vec3 project(const float t) const;
};
