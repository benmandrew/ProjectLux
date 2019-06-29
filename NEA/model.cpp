
#include "model.h"

// --------------------------------- //
//               Model               //
// --------------------------------- //

Model::Model(
	std::string filePath,
	Vec3 _position,
	Transform transform)
	: position(_position) {
	std::vector<uint32_t> vertexIndices;
	std::vector<uint32_t> normalIndices;
	loadOBJ(filePath.c_str(), vertices, normals, vertexIndices, normalIndices, transform);
	int triNum = normalIndices.size() / 3;
	for (int i = 0; i < triNum; i++) {
		uint32_t index = i * 3;
		triangles.push_back(
			Triangle(
				vertexIndices[index] - 1,
				vertexIndices[index + 1] - 1,
				vertexIndices[index + 2] - 1,
				normalIndices[index] - 1,
				i,
				std::shared_ptr<Model>(this)
			));
	}
	rootNode = std::make_shared<BVHNode>(triangles, this);
}

Model::Model(const Model& _model)
	: position(_model.position),
	triangles(_model.triangles),
	vertices(_model.vertices),
	normals(_model.normals),
	rootNode(std::move(_model.rootNode)) {
}

Triangle Model::getTriangle(int index) const {
	return triangles[index];
}

Vec3 Model::getVertex(int index) const {
	return vertices[index];
}

int Model::getVertexNum() const {
	return vertices.size();
}

Vec3 Model::getNormal(int index) const {
	return normals[index];
}

bool Model::rayIntersection(const Ray& ray, float& t, int& triangleIndex) const {
	return rootNode->rayIntersection(ray, t, triangleIndex);
}

Vec3 Model::getPosition() const {
	return position;
}

// ------------------------------------ //
//               Triangle               //
// ------------------------------------ //

Triangle::Triangle(
	uint32_t _v0Index, uint32_t _v1Index, uint32_t _v2Index,
	uint32_t _normalIndex, uint32_t _triangleIndex,
	std::shared_ptr<Model> _parent)
	: v0Index(_v0Index), v1Index(_v1Index), v2Index(_v2Index), 
	normalIndex(_normalIndex), triangleIndex(_triangleIndex),
	parent(_parent) {
	
	planeOffset = -parent->getNormal(normalIndex).dot(parent->getVertex(v0Index));
	center = (parent->getVertex(v0Index) + parent->getVertex(v1Index) + parent->getVertex(v2Index)) / 3.0f;
}

Triangle::Triangle(const Triangle& other) 
	: v0Index(other.v0Index), v1Index(other.v1Index), v2Index(other.v2Index),
	normalIndex(other.normalIndex), triangleIndex(other.triangleIndex),
	parent(other.parent), planeOffset(other.planeOffset), center(other.center) {
}

bool Triangle::rayIntersection(const Ray& ray, const Vec3& offset, float& t) const {
	// Möller-Trumbore algorithm
	//https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
	//http://www.lighthouse3d.com/tutorials/maths/ray-triangle-intersection/
	Vec3 rayOrigin = ray.getOrigin();
	Vec3 rayDirection = ray.getDirection();
	Vec3 v0 = parent->getVertex(v0Index) + offset;
	Vec3 v1 = parent->getVertex(v1Index) + offset;
	Vec3 v2 = parent->getVertex(v2Index) + offset;

	Vec3 edge0 = v1 - v0;
	Vec3 edge1 = v2 - v0;
	Vec3 pvec = rayDirection.cross(edge1);
	float det = edge0.dot(pvec);
	if (det <= 0.00001f) return false;
	float invDet = 1 / det;

	Vec3 tvec = rayOrigin - v0;
	float u = tvec.dot(pvec) * invDet;
	if (u < 0.0f || u > 1.0f) return false;
	Vec3 qvec = tvec.cross(edge0);
	float v = rayDirection.dot(qvec) * invDet;
	if (v < 0.0f || u + v > 1.0f) return false;
	t = edge1.dot(qvec) * invDet;
	return true;
}

void Triangle::setParent(std::shared_ptr<Model> _parent) {
	parent = _parent;
}

int Triangle::getv0Index() const {
	return v0Index;
}

int Triangle::getv1Index() const {
	return v1Index;
}

int Triangle::getv2Index() const {
	return v2Index;
}

int Triangle::getNormalIndex() const {
	return normalIndex;
}

int Triangle::getTriangleIndex() const {
	return triangleIndex;
}

Vec3 Triangle::getCenter() const {
	return center;
}