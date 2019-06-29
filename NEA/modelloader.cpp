#define _CRT_SECURE_NO_WARNINGS

#include "modelloader.h"

bool loadOBJ(
	const char* path,
	std::vector<Vec3>& outVertices,
	std::vector<Vec3>& outNormals,
	std::vector<uint32_t>& outVertexIndices,
	std::vector<uint32_t>& outNormalIndices,
	const Transform& transform
) {
	FILE* file = fopen(path, "r");
	if (file == nullptr) {
		printf("Impossible to open OBJ file!\n");
		printf(path);
		printf("\n");
		return false;
	}
	parseOBJ(file,
		outVertexIndices, outNormalIndices,
		outVertices, outNormals, transform);
	return true;
}

static void parseVertex(
	FILE* file,
	std::vector<Vec3>& tempVertices,
	const Transform& transform
) {
	Vec3 vertex;
	fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
	tempVertices.push_back(transform.transform(vertex));
}

static void parseNormal(
	FILE* file,
	std::vector<Vec3>& tempNormals,
	const Transform& transform
) {
	Vec3 normal;
	fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
	tempNormals.push_back(transform.transform(normal));
}

static void parseFace(
	FILE* file,
	std::vector<unsigned int>& vertexIndices,
	std::vector<unsigned int>& normalIndices,
	const Transform& transform
) {
	std::string vertex1, vertex2, vertex3;
	unsigned int vertexIndex[3], normalIndex[3];
	int matches = fscanf_s(
		file, "%d//%d %d//%d %d//%d\n",
		&vertexIndex[0], &normalIndex[0],
		&vertexIndex[1], &normalIndex[1],
		&vertexIndex[2], &normalIndex[2]);
	if (matches != 6) {
		printf("File can't be read, try exporting with other options\n");
		return;
	}
	int v0IndexIndex = 0;
	int v1IndexIndex = 1;
	int v2IndexIndex = 2;
		transform.flipVertexIndices(
		v1IndexIndex,
		v2IndexIndex);

	vertexIndices.push_back(vertexIndex[v0IndexIndex]);
	vertexIndices.push_back(vertexIndex[v1IndexIndex]);
	vertexIndices.push_back(vertexIndex[v2IndexIndex]);
	normalIndices.push_back(normalIndex[0]);
	normalIndices.push_back(normalIndex[1]);
	normalIndices.push_back(normalIndex[2]);
}

static void parseOBJ(
	FILE* file,
	std::vector<unsigned int>& vertexIndices,
	std::vector<unsigned int>& normalIndices,
	std::vector<Vec3>& tempVertices,
	std::vector<Vec3>& tempNormals,
	const Transform& transform
) {
	while (true) {
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF) break;

		if (strcmp(lineHeader, "v") == 0) {
			parseVertex(file, tempVertices, transform);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			parseNormal(file, tempNormals, transform);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			parseFace(file, vertexIndices, normalIndices, transform);
		}
	}
}
