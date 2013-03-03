//============================================================================
// Name        : hHW01-stackmachine
// Author      : Semen Martynov
// Version     : 2
// Copyright   : The MIT license
// Description : Implementation a stack machine for programming languages course
//============================================================================

#include <cstdlib>
#include <iostream>

#include "includes/Exception.h"
#include "includes/StackMachine.h"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "usage: " << argv[0] << " prog.sm" << std::endl;
		return EXIT_FAILURE;
	}
	// Let's try to start stack machine
	try
	{
		StackMachine stackMachine(argv[1]);
		stackMachine.start();
	} catch (Exception& caught)
	{
		std::cerr << caught.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

