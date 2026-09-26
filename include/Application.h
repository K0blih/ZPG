#pragma once

#include "Scene.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

class Application {
public:
	Application() = default;
	~Application();

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	void initialization();
	void createScene();
	void run();

private:
	GLFWwindow* window = nullptr;
	Scene scene;
	bool glfwinitialized = false;
};
