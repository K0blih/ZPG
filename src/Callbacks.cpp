#include "Callbacks.h"
#include "Application.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <cstdio>

namespace {
	void error_callback(int error, const char* description) { fputs(description, stderr); }

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
		if (!app)
		{
			return;
		}
		app->handleKey(key, action);

		if (action != GLFW_PRESS)
		{
			return;
		}

		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		case GLFW_KEY_1:
			app->switchScene(0);
			break;
		case GLFW_KEY_2:
			app->switchScene(1);
			break;
		case GLFW_KEY_3:
			app->switchScene(2);
			break;
		case GLFW_KEY_4:
			app->switchScene(3);
			break;
		case GLFW_KEY_5:
			app->switchScene(4);
			break;
		}

		//printf("key_callback [%d,%d,%d,%d] \n", key, scancode, action, mods);
	}

	void window_focus_callback(GLFWwindow* window, int focused)
	{
		Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
		if (!app)
		{
			return;
		}
		if (!focused)
		{
			app->resetHeldKeys();
			app->setMouseLookActive(false);
		}
		app->resetMouseTracking();
			
		printf("window_focus_callback \n");
	}

	void window_iconify_callback(GLFWwindow* window, int iconified) { printf("window_iconify_callback \n"); }

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
		if (!app)
		{
			return;
		}
		app->resizeFramebuffer(width, height);

		printf("resize %d, %d \n", width, height);
	}

	void cursor_callback(GLFWwindow* window, double x, double y)
	{
		Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
		if (!app)
		{
			return;
		}
		app->handleMouseMove(x, y);

		//printf("cursor_callback \n");
	}

	void button_callback(GLFWwindow* window, int button, int action, int mode)
	{
		if (button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
			if (!app)
			{
				return;
			}
			app->setMouseLookActive(action == GLFW_PRESS);
		}

		//if (action == GLFW_PRESS)
		//{
		//	printf("button_callback [%d,%d,%d]\n", button, action, mode);
		//}
	}
} //namespace

void registerErrorCallback()
{
	glfwSetErrorCallback(error_callback);
}

void registerWindowCallbacks(GLFWwindow* window)
{
	// Sets the key callback
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_callback);
	glfwSetMouseButtonCallback(window, button_callback);
	glfwSetWindowFocusCallback(window, window_focus_callback);
	glfwSetWindowIconifyCallback(window, window_iconify_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}
