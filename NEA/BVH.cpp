
#include "BVH.h"

static constexpr float MAX_DIST = 1000000.0;

void BVHNode::updateBoundRadius(Vec3 vertex) {
	const float tempRadius = (vertex - center).getLength();
	if (radius < tempRadius) {
		radius = tempRadius;
	}
}

void BVHNode::calcBounds(Model* model) {
	// Calculate the average center of all the triangles in the set
	for (int i = 0; i < triangles.size(); i++) {
		center = center + triangles[i].getCenter();
	}
	center = center / triangles.size();
	// Update the bounds, testing each vertex of every triangle in the set
	for (int i = 0; i < triangles.size(); i++) {
		updateBoundRadius(
			model->getVertex(
				triangles[i].getv0Index()));
		updateBoundRadius(
			model->getVertex(
				triangles[i].getv1Index()));
		updateBoundRadius(
			model->getVertex(
				triangles[i].getv2Index()));
	}
}

Axes::Axes getAxis(Vec3 sumOfSqrs) {
	// Get the axis with the greatest variance
	if (sumOfSqrs.x > sumOfSqrs.y && sumOfSqrs.x > sumOfSqrs.z) return Axes::x;
	else if (sumOfSqrs.y > sumOfSqrs.z)                         return Axes::y;
	else                                                        return Axes::z;
}

Axes::Axes BVHNode::calcAxisWithGreatestVariance() {
	Vec3 mean, sumOfSqrs;
	for (int i = 0; i < triangles.size(); i++) {
		Vec3 center = triangles[i].getCenter();
		Vec3 oldMean = mean;
		mean = mean + (center - mean) / (float)(i + 1);
		sumOfSqrs = sumOfSqrs + (center - mean) * (center - oldMean);
	}
	return getAxis(sumOfSqrs);
}

void BVHNode::partitionX(std::vector<Triangle>& leftPartitionTriangles, std::vector<Triangle>& rightPartitionTriangles) {
	for (int i = 0; i < triangles.size(); i++) {
		if (triangles[i].getCenter().x < center.x) {
			leftPartitionTriangles.push_back(triangles[i]);
		}
		else {
			rightPartitionTriangles.push_back(triangles[i]);
		}
	}
}

void BVHNode::partitionY(std::vector<Triangle>& leftPartitionTriangles, std::vector<Triangle>& rightPartitionTriangles) {
	for (int i = 0; i < triangles.size(); i++) {
		if (triangles[i].getCenter().y < center.y) {
			leftPartitionTriangles.push_back(triangles[i]);
		}
		else {
			rightPartitionTriangles.push_back(triangles[i]);
		}
	}
}

void BVHNode::partitionZ(std::vector<Triangle>& leftPartitionTriangles, std::vector<Triangle>& rightPartitionTriangles) {
	for (int i = 0; i < triangles.size(); i++) {
		if (triangles[i].getCenter().z < center.z) {
			leftPartitionTriangles.push_back(triangles[i]);
		}
		else {
			rightPartitionTriangles.push_back(triangles[i]);
		}
	}
}

void BVHNode::partition(Model* model) {
	std::vector<Triangle> leftPartitionTriangles;
	std::vector<Triangle> rightPartitionTriangles;
	Axes::Axes greatestVarianceAxis = calcAxisWithGreatestVariance();
	switch (greatestVarianceAxis) {
	case Axes::x:
		partitionX(leftPartitionTriangles, rightPartitionTriangles);
		break;
	case Axes::y:
		partitionY(leftPartitionTriangles, rightPartitionTriangles);
		break;
	case Axes::z:
		partitionZ(leftPartitionTriangles, rightPartitionTriangles);
		break;
	}

	child0 = std::make_unique<BVHNode>(leftPartitionTriangles, model);
	child1 = std::make_unique<BVHNode>(rightPartitionTriangles, model);
}

BVHNode::BVHNode(const std::vector<Triangle> _triangles, Model* model)
	: triangles(_triangles) {
	calcBounds(model);
	build(model);
	modelOffset = model->getPosition();
}

void BVHNode::build(Model* model) {
	isLeaf = true;
	if (triangles.size() > minTriangleNumPerLeaf) {
		partition(model);
		// Free memory
		std::vector<Triangle>().swap(triangles);
		triangles.clear();
		isLeaf = false;
	}
}

bool BVHNode::raySphereIntersection(const Ray& ray) {
	float t0, t1;
	const float radius2 = radius * radius;
	const Vec3 l = center + modelOffset - ray.getOrigin();
	const float tca = l.dot(ray.getDirection());
	const float d2 = l.dot(l) - tca * tca;
	if (d2 > radius2) return false; // Ray doesn't intersect with sphere
	const float thc = sqrt(radius2 - d2);
	t0 = tca - thc;
	t1 = tca + thc;

	if (t0 > t1) std::swap(t0, t1);
	if (t0 < 0) {
		t0 = t1; // If t0 is negative, use t1 instead
		if (t0 < 0) return false; // Both t0 and t1 are negative
	}
	return true;
}

bool BVHNode::rayTrianglesIntersection(const Ray& ray, float& t, int& triangleIndex) {
	bool isIntersection = false;
	for (int i = 0; i < triangles.size(); i++) {
		float dist = MAX_DIST;
		if (triangles[i].rayIntersection(ray, modelOffset, dist)) {
			if (dist < t) {
				t = dist;
				triangleIndex = triangles[i].getTriangleIndex();
				isIntersection = true;
			}
		}
	}
	return isIntersection;
}

bool BVHNode::rayIntersection(const Ray& ray, float& t, int& triangleIndex) {
	if (!raySphereIntersection(ray)) {
		return false;
	}
	else if (isLeaf) {
		return rayTrianglesIntersection(ray, t, triangleIndex);
	}
	else {
		return recurseRayIntersection(ray, t, triangleIndex);
	}
}

bool BVHNode::recurseRayIntersection(const Ray& ray, float& t, int& triangleIndex) {
	int triangleIndex0 = -1;
	int triangleIndex1 = -1;
	float dist0 = MAX_DIST * 2;
	float dist1 = MAX_DIST * 2;
	bool isIntersect0 = child0->rayIntersection(ray, dist0, triangleIndex0);
	bool isIntersect1 = child1->rayIntersection(ray, dist1, triangleIndex1);
	if (dist0 > dist1) {
		std::swap(dist0, dist1);
		std::swap(triangleIndex0, triangleIndex1);
	}
	if (dist0 < t) {
		t = dist0;
		triangleIndex = triangleIndex0;
	}
	return isIntersect0 || isIntersect1;
}