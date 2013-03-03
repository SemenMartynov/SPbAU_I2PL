//============================================================================
// Name        : HW02_L-interpretation
// Author      : Semen Martynov
// Version     : 1
// Copyright   : The MIT license
// Description : L-language interpretation
//============================================================================

#include <cstdlib>
#include <iostream>
#include "includes/Exception.h"
#include "includes/LInterpritator.h"


Memory<int> memory;

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "usage: " << argv[0] << " prog.sm" << std::endl;
		return EXIT_FAILURE;
	}

	try
	{
		LInterpritator linterp(argv[1]);
		linterp.execute();
	} catch (Exception& caught)
	{
		std::cerr << caught.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
