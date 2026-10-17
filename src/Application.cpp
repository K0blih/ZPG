// Only define this in one file
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#undef GLAD_GL_IMPLEMENTATION

// Include modules
#include "Application.h"
#include "Scene.h"
#include "Callbacks.h"

// Include the standard C++ headers  
#include <stdio.h>
#include <stdexcept>

void Application::resizeFramebuffer(int width, int height)
{
	glViewport(0, 0, width, height);

	for (Scene& scene : scenes)
	{
		scene.resizeCamera(width, height);
	}
}

void Application::handleMouseMove(double x, double y)
{
	if (!mouseLookActive)
	{
		return;
	}

	if (firstMouse)
	{
		lastMouseX = x;
		lastMouseY = y;
		firstMouse = false;
		return;
	}

	const float deltaX = static_cast<float>(x - lastMouseX);
	const float deltaY = static_cast<float>(y - lastMouseY);

	lastMouseX = x;
	lastMouseY = y;

	scenes[activeScene].rotateCamera(deltaX, deltaY);
}

void Application::setMouseLookActive(bool active)
{
	if (mouseLookActive == active)
	{
		return;
	}

	mouseLookActive = active;
	resetMouseTracking();

	glfwSetInputMode(window, GLFW_CURSOR, active ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void Application::resetMouseTracking()
{
	firstMouse = true;
}

void Application::handleKey(int key, int action)
{
	if (key < 0 || key > GLFW_KEY_LAST)
	{
		return;
	}

	if (action == GLFW_PRESS)
	{
		heldKeys[key] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		heldKeys[key] = false;
	}
}

bool Application::isKeyHeld(int key) const
{
	if (key < 0 || key > GLFW_KEY_LAST)
	{
		return false;
	}

	return heldKeys[key];
}

void Application::resetHeldKeys()
{
	heldKeys.fill(false);
}

void Application::initialization()
{
	registerErrorCallback();

	// Initialize GLFW
	if (!glfwInit())
	{
		throw std::runtime_error("GLFW initialization failed");
	}
	glfwinitialized = true;

	// Initialization of a specific version
	/*
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE,
	GLFW_OPENGL_CORE_PROFILE);  //*/


	window = glfwCreateWindow(800, 600, "ZPG", nullptr, nullptr);
	if (!window)
	{
		throw std::runtime_error("Window creation failed");
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// Initialize GLAD and load OpenGL function pointers
	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
	{
		throw std::runtime_error("GLAD initialization failed");
	}
	glEnable(GL_DEPTH_TEST);

	// Get version info
	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
	printf("Vendor %s\n", glGetString(GL_VENDOR));
	printf("Renderer %s\n", glGetString(GL_RENDERER));
	printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	int major, minor, revision;
	glfwGetVersion(&major, &minor, &revision);
	printf("Using GLFW %i.%i.%i\n", major, minor, revision);

	// Associate this window with the application so callbacks can access it.
	glfwSetWindowUserPointer(window, this);

	// Register callbacks for the window
	registerWindowCallbacks(window);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	resizeFramebuffer(width, height);
}

void Application::createScene()
{
	// Create the scenes
	scenes[0].createTriangle();
	scenes[1].createClock();
	scenes[2].createForest();
	scenes[3].createLogin();
	scenes[4].createSolarSystem();
	signature.createSignature();
}

void Application::switchScene(int index)
{
	if (index >= 0 && index < 5)
	{
		activeScene = index;
		resetMouseTracking();
	}
}

void Application::run()
{
	const double startTime = glfwGetTime();
	double previousTime = startTime;

	while (!glfwWindowShouldClose(window))
	{
		const double currentTime = glfwGetTime();
		float deltaTime = static_cast<float>(currentTime - previousTime);
		previousTime = currentTime;

		if (deltaTime > 0.1f)
        {
            deltaTime = 0.1f;
        }

		float forwardInput = 0.0f;
		float rightInput = 0.0f;

		if (isKeyHeld(GLFW_KEY_W))
		{
			forwardInput += 1.0f;
		}
		if (isKeyHeld(GLFW_KEY_S))
		{
			forwardInput -= 1.0f;
		}
		if (isKeyHeld(GLFW_KEY_D))
		{
			rightInput += 1.0f;
		}
		if (isKeyHeld(GLFW_KEY_A))
		{
			rightInput -= 1.0f;
		}

		scenes[activeScene].moveCamera(forwardInput, rightInput, deltaTime);

		const double timeSeconds = currentTime - startTime;

		// Clear color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Draw a triangles
		scenes[activeScene].draw(timeSeconds);
		signature.draw(timeSeconds);

		// Display the rendered frame
		glfwSwapBuffers(window);
		// Process events
		glfwPollEvents();
	}
}

Application::~Application()
{
	if (window)
	{
		for (Scene& scene : scenes)
		{
			scene.clear();
		}

		signature.clear();

		glfwDestroyWindow(window);
	}

	if (glfwinitialized)
	{
		glfwTerminate();
	}
}
