#pragma once

#include <vector>
#include <SDL.h>
#include "model.h"

struct Camera {
private:
	Vec3 position;
	float aspectRatio;
	int pixelWidth, pixelHeight, halfPixelWidth, halfPixelHeight;
	float distToProjPlane;

	int lastModelIndex = 0;
	std::vector<std::shared_ptr<Model>> models;

	void renderPixel(SDL_Renderer* renderer, int pixelX, int pixelY);
	Ray emitScreenRay(int pixelX, int pixelY);
	Vec3 getRayIntersectionColour(Ray& ray);
	void getCollisionIndices(Ray& ray, int& modelIndex, int& triangleIndex);
	float getBrightnessAtPoint(int& modelIndex, int& triangleIndex);
	float getBrightnessAtNormal(Ray& normalRay);

	void setDrawColour(SDL_Renderer* renderer, Vec3& colour);
	float toneMap(float value);
public:
	Camera(Vec3 _position, int _pixelWidth, int _pixelHeight, float _horizontalFOV = 90.0f);

	void renderImage(SDL_Renderer* renderer, int screenWidth, int screenHeight);
	void insertModel(std::shared_ptr<Model> object);
};