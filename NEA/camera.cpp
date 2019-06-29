
#include <iostream>
#include <chrono>
#include <algorithm>
#include "camera.h"

static constexpr float PI = 3.14159265;
static constexpr float DEG2RAD = PI / 180;
static constexpr float MAX_DIST = 1000000.0;

Camera::Camera(Vec3 _position, int _pixelWidth, int _pixelHeight, float _horizontalFOV)
	: position(_position), pixelWidth(_pixelWidth), pixelHeight(_pixelHeight) {
	aspectRatio = pixelWidth / (float)pixelHeight;
	float verticalFOV = _horizontalFOV / aspectRatio;
	float halfFOV = _horizontalFOV / 2.0f;
	// The distance to the projection plane is the cotangent of half the horizontal field of view
	distToProjPlane = 1 / tan(halfFOV * DEG2RAD); // Converting degrees to radians
	halfPixelWidth = pixelWidth / 2;
	halfPixelHeight = pixelHeight / 2;
}

void Camera::renderImage(SDL_Renderer* renderer, int screenWidth, int screenHeight) {
	time_t start = time(0);
	// Iterate over each pixel in the screen, emitting a ray for each
	for (int y = 0; y < screenHeight; y++) {
		for (int x = 0; x < screenWidth; x++) {
			renderPixel(renderer, x, y);
		}
		// Calculate the time taken to render the row, and print it along with the number of the row
		float timePerRow = difftime(time(0), start) / (y + 1);
		float timeLeft = timePerRow * (screenHeight - y + 1);
		std::cout << "Row " << y + 1 << "/" << screenHeight << " complete : " << (int)round(timeLeft) << " Seconds left\n";
		// Update the screen with the new row
		SDL_RenderPresent(renderer);
	}
}

void Camera::renderPixel(SDL_Renderer* renderer, int pixelX, int pixelY) {
	// Emit a ray into the scene, and get the colour of whatever it collides with
	Ray ray = emitScreenRay(pixelX, pixelY);
	Vec3 colour = getRayIntersectionColour(ray);
	// Set the draw colour, and draw it to the required pixel
	setDrawColour(renderer, colour);
	SDL_RenderDrawPoint(renderer, pixelX, pixelY);
}

Ray Camera::emitScreenRay(int pixelX, int pixelY) {
	// Get the coordinates of the ray in view-space coordinates
	float screenX = (pixelX - halfPixelWidth) / (float)halfPixelWidth;
	float screenY = (pixelY - halfPixelHeight) / (float)halfPixelHeight;
	// The coordinates are distributed in a square, so scale the x-coordinate
	// by the aspect ratio to get the correct proportions in world-space
	screenX *= aspectRatio;
	// Find the normalised ray direction in world space
	Vec3 rayDirection = Vec3(screenX, screenY, distToProjPlane).normalise();
	return Ray(position, rayDirection);
}

Vec3 Camera::getRayIntersectionColour(Ray& ray) {
	Vec3 intersection = Vec3();
	// Indices are initialised to '-1' to detect if no collision occurs
	int modelIndex = -1;
	int triangleIndex = -1;
	// Get the index of the triangle (and it's parent model)
	// that the ray has it's closest intersection with
	getCollisionIndices(ray, modelIndex, triangleIndex);
	// Initially set the pixel colour to the background colour
	Vec3 colour = Vec3(0.02, 0.02, 0.04);

	if (modelIndex != -1 && triangleIndex != -1) {
		// Find the brightness of the point on the triangle
		float brightness = getBrightnessAtPoint(modelIndex, triangleIndex);
		colour = models[modelIndex]->colour * brightness;
	}
	return colour;
}

void Camera::getCollisionIndices(Ray& ray, int& modelIndex, int& triangleIndex) {
	float t = (float)MAX_DIST;
	float closest = t * 2;
	int tempTriangleIndex = -1;
	// Iterate over models, checking for collisions
	// If there is a successful intersection and it's the closest one yet, set all the out variables
	for (int i = 0; i < lastModelIndex; i++) {
		bool isIntersection = models[i]->rayIntersection(ray, t, tempTriangleIndex);
		if (isIntersection && t < closest) {
			modelIndex = i;
			triangleIndex = tempTriangleIndex;
			closest = t;
		}
	}
}

float Camera::getBrightnessAtPoint(int& modelIndex, int& triangleIndex) {
	// Get the normal vector to the triangle, and use it to calculate the brightness at that point
	Triangle triangle = models[modelIndex]->getTriangle(triangleIndex);
	int normalIndex = triangle.getNormalIndex();
	Ray normalRay = Ray(
		models[modelIndex]->getPosition(),
		models[modelIndex]->getNormal(normalIndex));
	return getBrightnessAtNormal(normalRay);
}

float Camera::getBrightnessAtNormal(Ray& normalRay) {
	// Get brightness by angle towards positive x-axis
	// Brightness is in the range [0, 1] so raising
	// to a power of 3 creates sharper highlights
	float brightness = (normalRay.getDirection().dot(Vec3(1.0, 0.0, 0.0)) / 2.0) + 0.5;
	return pow(brightness, 3.0);
}

void Camera::setDrawColour(SDL_Renderer* renderer, Vec3& colour) {
	// Tone-map the colour, and convert each component to an 8 bit integer
	SDL_SetRenderDrawColor(
		renderer,
		(Uint8)(toneMap(colour.x) * 256),
		(Uint8)(toneMap(colour.y) * 256),
		(Uint8)(toneMap(colour.z) * 256),
		(Uint8)255);
}

float Camera::toneMap(float value) {
	// Tone-map HDR (high dynamic range) colours to LDR (low dynamic range)
	// colours that can be represented on a screen
	float mapped = value / (value + 1);
	return pow(mapped, 1 / 2.2); // Gamma value
}

void Camera::insertModel(std::shared_ptr<Model> model) {
	models.push_back(model);
	lastModelIndex += 1;
}