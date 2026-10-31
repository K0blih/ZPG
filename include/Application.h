#pragma once

#include "Scene.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <array>
#include <memory>

class Application {
public:
	Application() = default;
	~Application();

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	void resizeFramebuffer(int width, int height);
	void handleMouseMove(double x, double y);
	void setMouseLookActive(bool active);
	void resetMouseTracking();
	void handleKey(int key, int action);
	bool isKeyHeld(int key) const;
	void resetHeldKeys();

	void initialization();
	void createScenes();
	void switchScene(int index);
	void run();

private:
	GLFWwindow* window = nullptr;
	bool glfwinitialized = false;

	std::unique_ptr<Scene> signature;
	std::array<std::unique_ptr<Scene>, 6> scenes;
	int activeScene = 0;

	std::array<bool, GLFW_KEY_LAST + 1> heldKeys{ false };
	bool mouseLookActive = false;
	bool firstMouse = true;
	double lastMouseX = 0.0;
	double lastMouseY = 0.0;
};
