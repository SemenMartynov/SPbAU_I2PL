/*
 * StackMachine.h
 *
 *  Created on: 21 Feb 2013
 *      Author: Semen Martynov
 */

#ifndef STACKMACHINE_H_
#define STACKMACHINE_H_
#include "Memory.h"
#include "Stack.h"
#include "StackMachineCode.h"

class StackMachine
{
public:
	StackMachine(const char *codePath) :
			code(codePath)
	{
	}
	;
	int calc(const int val1, const int val2, const std::string &op);
	void start();
private:
	StackMachineCode code;
	Memory<int> memory;
	Stack<int> stack;
};

int StackMachine::calc(const int val1, const int val2, const std::string &op)
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
		throw Exception("Syntax error - unknown operation");
}

inline void StackMachine::start()
{
	bool isNotEnded = true;

	while (isNotEnded)
	{
		std::string command = code.getCommand();

		if (command.empty())
		{
			continue;
		}
		else if (!command.compare(0, 1, "$"))
		{
			continue;
		}
		else if (!command.compare("C"))
		{
			/**
			 * C n - load a constant n on stack peak
			 */
			int n;
			std::istringstream(code.getCommand()) >> n;
			stack.put(n);
		}
		else if (!command.compare("L"))
		{
			/**
			 * L x - load value from variable x in to the stack peak
			 */
			stack.put(memory.get(code.getCommand()));
		}
		else if (!command.compare("S"))
		{
			/**
			 * S x - load value from the stack peak in to the variable x
			 */
			memory.put(code.getCommand(), stack.get());
		}
		else if (!command.compare("R"))
		{
			/**
			 * R - load value from input stream in to the stack peak
			 */
			int n;
			std::cout << "> ";
			std::cin >> n;
			stack.put(n);
		}
		else if (!command.compare("W"))
		{
			/**
			 * W - load value from the stack peak in to the output stream
			 */
			std::cout << stack.get() << std::endl;
		}
		else if (!command.compare("B"))
		{
			/**
			 * B
			 * 1. Take two values from stack peak.
			 * 2. Execute operation {+ - * / % == != > < >= <= || &&}.
			 * 3. Save value in a stack.
			 */
			int val1 = stack.get();
			int val2 = stack.get();
			stack.put(calc(val1, val2, code.getCommand()));
		}
		else if (!command.compare("J"))
		{
			/**
			 * J a - goto label a
			 */
			code.goTo(code.getCommand());
		}
		else if (!command.compare("JT"))
		{
			/**
			 * JT a - goto label a, if TRUE is on the stack peak
			 */
			if (stack.get())
			{
				code.goTo(code.getCommand());
			}
		}
		else if (!command.compare("JF"))
		{
			/**
			 * JF a - goto label a, if FALSE is on the stack peak
			 */
			if (!stack.get())
			{
				code.goTo(code.getCommand());
			}
		}
		else if (!command.compare("E"))
		{
			/**
			 * E - end program
			 */
			std::cout << "The end." << std::endl;
			isNotEnded = false;
		}
	}
}

#endif /* STACKMACHINE_H_ */
