#pragma once

struct BVHNode;

#include <vector>
#include <memory>
#include "BVH.h"
#include "modelloader.h"

struct Model {
private:
	Vec3 position;
	std::vector<Triangle> triangles;
	std::vector<Vec3> vertices;
	std::vector<Vec3> normals;
	std::shared_ptr<BVHNode> rootNode;
public:
	Vec3 colour = Vec3(1.0f, 0.0f, 0.0f);

	Model(
		std::string filePath,
		Vec3 _position = Vec3(),
		Transform transform = Transform());
	Model(const Model& _object);
	Triangle getTriangle(int index) const;
	Vec3 getPosition() const;
	Vec3 getVertex(int index) const;
	int getVertexNum() const;
	Vec3 getNormal(int index) const;
	bool rayIntersection(const Ray& ray, float& t, int& triangleIndex) const;
};

struct Triangle {
private:
	uint32_t v0Index, v1Index, v2Index;
	uint32_t normalIndex;
	uint32_t triangleIndex;
	std::shared_ptr<Model> parent;
	float planeOffset;
	Vec3 center;
public:
	Triangle(
		uint32_t _v0Index = -1,
		uint32_t _v1Index = -1,
		uint32_t _v2Index = -1,
		uint32_t _normalIndex = -1,
		uint32_t _triangleIndex = -1,
		std::shared_ptr<Model> _parent = nullptr);
	Triangle(const Triangle& other);
	bool rayIntersection(const Ray& ray, const Vec3& offset, float& t) const;
	void setParent(std::shared_ptr<Model> parent);
	int getv0Index() const;
	int getv1Index() const;
	int getv2Index() const;
	int getNormalIndex() const;
	int getTriangleIndex() const;
	Vec3 getCenter() const;
};