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

void Application::initialization()
{
	registerErrorCallback();

	// Initialize GLFW
	if (!glfwInit()) {
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
	glViewport(0, 0, width, height);
}

void Application::createScene()
{
	// Create the scenes
	scenes[0].createTriangle();
	scenes[1].createSphere();
	scenes[2].createForest();
	scenes[3].createLogin();
}

void Application::switchScene(int index)
{
	if (index >= 0 && index < 4)
	{
		activeScene = index;
	}
}

void Application::run()
{
	while (!glfwWindowShouldClose(window))
	{
		// Clear color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Draw a triangles
		scenes[activeScene].draw();

		// Display the rendered frame and process events
		glfwSwapBuffers(window);
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
		glfwDestroyWindow(window);
	}

	if (glfwinitialized)
	{
		glfwTerminate();
	}
}
