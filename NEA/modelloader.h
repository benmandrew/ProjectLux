#pragma once

#include <vector>
#include "transform.h"

bool loadOBJ(
	const char* path,
	std::vector<Vec3>& outVertices,
	std::vector<Vec3>& outNormals,
	std::vector<uint32_t>& outVertexIndices,
	std::vector<uint32_t>& outNormalIndices,
	const Transform& transform);

static void parseVertex(
	FILE* file,
	std::vector<Vec3>& tempVertices,
	const Transform& transform);

static void parseNormal(
	FILE* file,
	std::vector<Vec3>& tempNormals,
	const Transform& transform);

static void parseFace(FILE* file,
	std::vector<uint32_t>& vertexIndices,
	std::vector<uint32_t>& normalIndices,
	const Transform& transform);

static void parseOBJ(FILE* file,
	std::vector<uint32_t>& vertexIndices,
	std::vector<uint32_t>& normalIndices,
	std::vector<Vec3>& tempVertices,
	std::vector<Vec3>& tempNormals,
	const Transform& transform);