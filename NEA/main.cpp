// Disable warnings for unsafe file I/O
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <memory>
#include <chrono>
#include <string>
#include <SDL.h>
#include "camera.h"

static int NUMCOMMANDLINEARGS = 5;

// Screen dimensions
static int WIDTH;
static int HEIGHT;
// Camera field of view in degrees
static float camFOV;

static Vec3 camPos = Vec3(0.0, 0.0, -10);
// Rotations and reflections in x, y, and z axes. To be applied to every model
static Transform transform = Transform(25.0, 45.0, 5.0, false, true, false);
static std::string root = "C:\\Users\\Mirrorworld\\Desktop\\NEA\\OBJ files\\";

// Class containing SDL functionality
struct Context {
	SDL_Window* window;
	SDL_Renderer* renderer;
	bool initFailure;
};

Context initialise() {
	// Seed random number generator
	srand((unsigned int)time(NULL));
	Context context;
	// Initialise SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "SDL could not initialise. SDL error: " << SDL_GetError() << "\n";
		context.initFailure = EXIT_FAILURE;
		return context;
	}
	// Initialise the SDL window and renderer
	SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI, &context.window, &context.renderer);
	if (context.window == NULL) {
		std::cout << "Could not create window. SDL error: " << SDL_GetError() << "\n";
		context.initFailure = EXIT_FAILURE;
		return context;
	}
	context.initFailure = EXIT_SUCCESS;
	return context;
}

// Set the resolution of the window
void setWindowDimensions(int _width, int _height) {
	WIDTH = _width;
	HEIGHT = _height;
}

// Check if a char array represents an integer (used to check command line arguments)
bool isInteger(char* string) {
	if (atoi(string) == 0 && string[0] != '0') return false;
	return EXIT_SUCCESS;
}

// Check if a char array represents a floating point number (used to check command line arguments)
bool isFloat(char* string) {
	bool pointFound = false;
	// Iterate through the characters
	for (int i = 0; i < strlen(string); i++) {
		// Must have either zero or one decimal points
		if (string[i] ==  '.' && !pointFound) {
			pointFound = true;
		}
		// If the character is not a digit, fail
		else if (!isdigit(string[i])) {
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

bool checkCommandLineArgs(char *argv[]) {
	// Check if number of arguments different than required
	if (sizeof(*argv) / sizeof(char) != 5) {
		std::cout << "Wrong number of command line arguments\n";
		std::cout << "Argument syntax: width height fieldOfView OBJfilename\n";
		return EXIT_FAILURE;
	}
	// Check if the supplied width and height are integers
	if (!isInteger(argv[1]) || !isInteger(argv[2])) {
		std::cout << "Window dimensions are not integers\n";
		std::cout << argv[1] << " " << argv[2] << "\n";
		return EXIT_FAILURE;
	}
	// Check if the supplied field of view is a decimal number
	if (!isFloat(argv[3])) {
		std::cout << "Camera field of view must be a decimal number\n";
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

bool parseCommandLineArgs(int argc, char *argv[], std::vector<std::string>& modelFileNames) {
	// Check the command line arguments for syntax errors
	if (checkCommandLineArgs(argv) == EXIT_FAILURE)	return EXIT_FAILURE;

	setWindowDimensions(
		strtol(argv[1], nullptr, 0),
		strtol(argv[2], nullptr, 0));
	camFOV = atof(argv[3]);
	for (int i = 4; i < argc; i++) {
		modelFileNames.push_back(std::string(argv[i]));
	}
	return EXIT_SUCCESS;
}

std::shared_ptr<Camera> initCam(std::vector<std::string>& filenames) {
	std::shared_ptr<Camera> cam(new Camera(camPos, WIDTH, HEIGHT, camFOV));
	for (int i = 0; i < filenames.size(); i++) {
		std::string path = root + filenames[i];
		std::shared_ptr<Model> model = std::make_shared<Model>(path, Vec3(), transform);
		cam->insertModel(model);
	}
	return cam;
}

float getTimeElapsed(std::chrono::steady_clock::time_point start) {
	auto end = std::chrono::high_resolution_clock::now();
	auto dur = end - start;
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
	return ms / 1000.0;
}

void outputRenderInfo(std::chrono::steady_clock::time_point start) {
	std::cout << "\n";
	std::cout << "Time taken (s): " << getTimeElapsed(start) << "\n";
	std::cout << "Field of View (Degrees): " << camFOV << "\n";
	std::cout << "Display Resolution: " << WIDTH << " x " << HEIGHT << "\n";
}

void mainLoop(Context context) {
	SDL_Event windowEvent;
	while (true) {
		if (SDL_PollEvent(&windowEvent)) {
			if (SDL_QUIT == windowEvent.type) {
				return;
			}
		}
		SDL_RenderPresent(context.renderer);
	}
}

bool quit(Context context) {
	SDL_DestroyWindow(context.window);
	SDL_Quit();
	return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
	std::vector<std::string> filenames;
	if (parseCommandLineArgs(argc, argv, filenames) == EXIT_FAILURE) return EXIT_FAILURE;

	Context context = initialise();
	if (context.initFailure == EXIT_FAILURE) return EXIT_FAILURE;

	std::shared_ptr<Camera> cam = initCam(filenames);
	auto start = std::chrono::high_resolution_clock::now();
	cam->renderImage(context.renderer, WIDTH, HEIGHT);
	outputRenderInfo(start);

	mainLoop(context);

	return quit(context);
}