/*
 * LInterpritator.h
 *
 *  Created on: 22 Feb 2013
 *      Author: Semen Martynov
 */

#ifndef LINTERPRITATOR_H_
#define LINTERPRITATOR_H_
#include <sstream>
#include "Exception.h"
#include "LCode.h"
#include "Memory.h"

class LInterpritator
{
public:
	LInterpritator(const char *codePath) :
			code(codePath)
	{
	}
	int calc(const int val1, const int val2, const std::string &op);
	int execute(bool fakeExecution);
private:
	LCode code;
	Memory<int> memory;
};

inline int LInterpritator::calc(const int val1, const int val2,
		const std::string &op)
{
	// Execute operation {+ - * / % == != > < >= <= || &&}
	if (!op.compare("+"))
	{
		return (val1 + val2);
	}
	else if (!op.compare("-"))
	{
		return (val1 - val2);
	}
	else if (!op.compare("*"))
	{
		return (val1 * val2);
	}
	else if (!op.compare("/"))
	{
		return (val1 / val2);
	}
	else if (!op.compare("%"))
	{
		return (val1 % val2);
	}
	else if (!op.compare("=="))
	{
		return (val1 == val2);
	}
	else if (!op.compare("!="))
	{
		return (val1 != val2);
	}
	else if (!op.compare(">"))
	{
		return (val1 > val2);
	}
	else if (!op.compare("<"))
	{
		return (val1 < val2);
	}
	else if (!op.compare(">="))
	{
		return (val1 >= val2);
	}
	else if (!op.compare("<="))
	{
		return (val1 <= val2);
	}
	else if (!op.compare("||"))
	{
		return (val1 || val2);
	}
	else if (!op.compare("&&"))
	{
		return (val1 && val2);
	}
	else
		throw Exception("Syntax error - unknown operation " + op);
}

inline int LInterpritator::execute(bool fakeExecution = false)
{
	std::string command = code.getCommand();

	if (command.empty())
	{
		//do nothing
	}
	else if (!command.compare("s"))
	{
		/*
		 * skip
		 */
		//do nothing
	}
	else if (!command.compare("="))
	{
		/*
		 * x:=e
		 */
		std::string variable = code.getCommand();
		int value = execute(fakeExecution);
		if (!fakeExecution)
		{
			memory.put(variable, value);
		}
	}
	else if (!command.compare("r"))
	{
		/*
		 * read
		 */
		std::string variable = code.getCommand();
		if (!fakeExecution)
		{
			int value = 0;
			std::cout << "> ";
			std::cin >> value;

			memory.put(variable, value);
		}
	}
	else if (!command.compare("w"))
	{
		/*
		 * write
		 */
		int value = execute(fakeExecution);
		if (!fakeExecution)
		{
			std::cout << value << std::endl;
		}
	}
	else if (!command.compare(";"))
	{
		/*
		 * S_1;S_2
		 */
		execute(fakeExecution);
		execute(fakeExecution);
	}
	else if (!command.compare("i"))
	{
		/*
		 * if E then S_1 else S_2
		 */
		if (execute(fakeExecution))
		{
			execute(fakeExecution);
			execute(true);
		}
		else
		{
			execute(true);
			execute(fakeExecution);
		}
	}
	else if (!command.compare("l"))
	{
		/*
		 * while E do S
		 */
		code.saveLabel();
		while (execute(fakeExecution))
		{
			execute(fakeExecution);
			code.loadLabel();
		}
		execute(true);

	}
	else if (!command.compare("!"))
	{
		/*
		 * = n (natural number)
		 */
		int n = 0;
		std::istringstream(code.getCommand()) >> n;
		return n;
	}
	else if (!command.compare("x"))
	{
		/*
		 * = x (variable from memory)
		 */
		int value = 0;
		std::string variable = code.getCommand();
		if (!fakeExecution)
		{
			value = memory.get(variable);
		}
		return value;
	}
	else if (!command.compare("@"))
	{
		/*
		 * (X) (some operation with 2 operators)
		 */
		std::string op = code.getCommand();
		int var1 = execute(fakeExecution);
		int var2 = execute(fakeExecution);

		int result = 0;
		if (!fakeExecution)
		{
			result = calc(var1, var2, op);
		}
		return result;
	}
	else
		throw Exception("Syntax error - unknown operation " + command);
	return 0;
}

#endif /* LINTERPRITATOR_H_ */
