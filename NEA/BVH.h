#pragma once

struct Model;
struct Triangle;

#include "geometry.h"
#include "model.h"

namespace Axes {
	enum Axes {
		x, y, z
	};
}

struct BVHNode {
private:
	static constexpr int minTriangleNumPerLeaf = 3;

	std::unique_ptr<BVHNode> child0;
	std::unique_ptr<BVHNode> child1;
	std::vector<Triangle> triangles;
	Vec3 center = Vec3();
	Vec3 modelOffset;
	float radius;
	bool isLeaf;

	void updateBoundRadius(Vec3 vertex);
	void calcBounds(Model* Model);

	Axes::Axes calcAxisWithGreatestVariance();
	void partitionX(
		std::vector<Triangle>& leftPartitionTriangles, 
		std::vector<Triangle>& rightPartitionTriangles);
	void partitionY(
		std::vector<Triangle>& leftPartitionTriangles, 
		std::vector<Triangle>& rightPartitionTriangles);
	void partitionZ(
		std::vector<Triangle>& leftPartitionTriangles, 
		std::vector<Triangle>& rightPartitionTriangles);
	void partition(Model* Model);

	bool raySphereIntersection(const Ray& ray);
	bool rayTrianglesIntersection(const Ray& ray, float& t, int& triangleIndex);
	bool recurseRayIntersection(const Ray& ray, float& t, int& triangleIndex);
public:
	BVHNode(std::vector<Triangle> triangles, Model* Model);
	void build(Model* Model);

	bool rayIntersection(const Ray& ray, float& t, int& triangleIndex);
};