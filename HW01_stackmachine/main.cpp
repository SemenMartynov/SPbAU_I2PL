//============================================================================
// Name        : hHW01-stackmachine
// Author      : Semen Martynov
// Version     : 1
// Copyright   : The MIT license
// Description : Implementation a stack machine for programming languages course
//============================================================================

#include <cstdlib>
#include <iostream>

#include <memory>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <map>
#include <stack>

int calc(const int val1, const int val2, const std::string &op);
class comparator
{
public:
	comparator(std::string label)
	{
		this->label = label + ":";
	}
	bool operator()(std::string &strForCmp) const
	{
		return !label.compare(strForCmp);
	}
private:
	std::string label;
};

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "usage: " << argv[0] << " prog.sm" << std::endl;
		return EXIT_FAILURE;
	}

	// I know that auto_ptr is deprecated, but I have old compiler on my desktop
	std::string stackMachineProg(
			(std::istreambuf_iterator<char>(
					*(std::auto_ptr<std::ifstream>(new std::ifstream(argv[1]))).get())),
			std::istreambuf_iterator<char>());

	// Parse prog
	std::stringstream ss(stackMachineProg);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> stackMachine(begin, end);

	std::map<std::string, int> variables;
	std::stack<int> constants;

	for (std::vector<std::string>::const_iterator it = stackMachine.begin();
			it != stackMachine.end(); ++it)
	{

		if ((*it).empty())
		{
			continue;
		}
		else if (!(*it).compare(0, 1, "$"))
		{
			continue;
		}
		else if (!(*it).compare("C"))
		{
			/**
			 * C n - load a constant n on stack peak
			 */
			int n;
			std::istringstream(*++it) >> n;
			constants.push(n);
		}
		else if (!(*it).compare("L"))
		{
			/**
			 * L x - load value from variable x in to the stack peak
			 */
			std::map<std::string, int>::const_iterator x = variables.find(
					*++it);
			if (x != variables.end())
			{
				constants.push(x->second);
			}
			else
			{
				std::cerr << "Cant't find variable " << *it << std::endl;
				return EXIT_FAILURE;
			}
		}
		else if (!(*it).compare("S"))
		{
			/**
			 * S x - load value from the stack peak in to the variable x
			 */
			if (constants.size() > 0)
			{
				variables[*++it] = constants.top();
				constants.pop();
			}
			else
			{
				std::cerr << "Stack is empty" << std::endl;
				return EXIT_FAILURE;
			}
		}
		else if (!(*it).compare("R"))
		{
			/**
			 * R - load value from input stream in to the stack peak
			 */
			int n;
			std::cout << "> ";
			std::cin >> n;
			constants.push(n);
		}
		else if (!(*it).compare("W"))
		{
			/**
			 * W - load value from the stack peak in to the output stream
			 */
			if (constants.size() > 0)
			{
				std::cout << constants.top() << std::endl;
				constants.pop();
			}
			else
			{
				std::cerr << "Stack is empty" << std::endl;
				return EXIT_FAILURE;
			}
		}
		else if (!(*it).compare("B"))
		{
			/**
			 * B
			 * 1. Take two values from stack peak.
			 * 2. Execute operation {+ - * / % == != > < >= <= || &&}.
			 * 3. Save value in a stack.
			 */
			int val1;
			int val2;
			if (constants.size() > 1)
			{
				val1 = constants.top();
				constants.pop();
				val2 = constants.top();
				constants.pop();
				constants.push(calc(val1, val2, *++it));
			}
			else
			{
				std::cerr << "Stack is empty" << std::endl;
				return EXIT_FAILURE;
			}
		}
		else if (!(*it).compare("J"))
		{
			/**
			 * J a - goto label a
			 */
			it = std::find_if(stackMachine.begin(), stackMachine.end(),
					comparator(*++it));
			if (it == stackMachine.end())
			{
				std::cerr << "Syntax error - can't find label for J"
						<< std::endl;
				return EXIT_FAILURE;
			}
		}
		else if (!(*it).compare("JT"))
		{
			/**
			 * JT a - goto label a, if TRUE is on the stack peak
			 */
			int b;
			if (constants.size() > 0)
			{
				b = constants.top();
				constants.pop();
			}
			else
			{
				std::cerr << "Stack is empty" << std::endl;
				return EXIT_FAILURE;
			}
			if (b)
			{
				it = std::find_if(stackMachine.begin(), stackMachine.end(),
						comparator(*++it));
				if (it == stackMachine.end())
				{
					std::cerr << "Syntax error - can't find label for JT"
							<< std::endl;
					return EXIT_FAILURE;
				}
			}
		}
		else if (!(*it).compare("JF"))
		{
			/**
			 * JF a - goto label a, if FALSE is on the stack peak
			 */
			int b;
			if (constants.size() > 0)
			{
				b = constants.top();
				constants.pop();
			}
			else
			{
				std::cerr << "Stack is empty" << std::endl;
				return EXIT_FAILURE;
			}
			if (!b)
			{
				it = std::find_if(stackMachine.begin(), stackMachine.end(),
						comparator(*++it));
				if (it == stackMachine.end())
				{
					std::cerr << "Syntax error - can't find label for JF"
							<< std::endl;
					return EXIT_FAILURE;
				}
			}
		}
		else if (!(*it).compare("E"))
		{
			/**
			 * E - end program
			 */
			std::cout << "The end." << std::endl;
			return EXIT_SUCCESS;
		}
		else
		{
			std::cerr << "Syntax error - can't understand command '" << *it
					<< "'" << std::endl;
			return EXIT_FAILURE;
		}
	}
	std::cerr << "Syntax error - unexpected END" << std::endl;
	return EXIT_FAILURE;
}

int calc(const int val1, const int val2, const std::string &op)
{
	// Execute operation {+ - * / % == != > < >= <= || &&}
	if (!op.compare("+"))
	{
		return (val2 + val1);
	}
	else if (!op.compare("-"))
	{
		return (val2 - val1);
	}
	else if (!op.compare("*"))
	{
		return (val2 * val1);
	}
	else if (!op.compare("/"))
	{
		return (val2 / val1);
	}
	else if (!op.compare("%"))
	{
		return (val2 % val1);
	}
	else if (!op.compare("=="))
	{
		return (val2 == val1);
	}
	else if (!op.compare("!="))
	{
		return (val2 != val1);
	}
	else if (!op.compare(">"))
	{
		return (val2 > val1);
	}
	else if (!op.compare("<"))
	{
		return (val2 < val1);
	}
	else if (!op.compare(">="))
	{
		return (val2 >= val1);
	}
	else if (!op.compare("<="))
	{
		return (val2 <= val1);
	}
	else if (!op.compare("||"))
	{
		return (val2 || val1);
	}
	else if (!op.compare("&&"))
	{
		return (val2 && val1);
	}
	else
	{
		std::cerr << "Syntax error - unknown operation" << std::endl;
		return EXIT_FAILURE;
	}
}
