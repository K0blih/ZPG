/**
 * @file main.cpp
 *
 * @brief Main function
 *
 * @author Richard Chovanec
  **/

#include "Application.h"

#include <exception>
#include <iostream>

int main(void)
{
	Application app;
	try
	{
		app.initialization();
		app.createScene();
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

