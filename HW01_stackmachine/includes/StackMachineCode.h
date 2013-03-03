/*
 * StackMachineCode.h
 *
 *  Created on: 21 Feb 2013
 *      Author: Semen Martynov
 */

#ifndef STACKMACHINECODE_H_
#define STACKMACHINECODE_H_
#include <memory>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>

class StackMachineCode
{
public:
	StackMachineCode(const char *codePath);
	std::string getCommand();
	void goTo(std::string label);
	bool isEOF();
private:
	std::vector<std::string> code;
	std::vector<std::string>::const_iterator codePointer;
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
};

StackMachineCode::StackMachineCode(const char* codePath)
{
	// I know that auto_ptr is deprecated, but I have old compiler on my desktop
	std::string codeFile(
			(std::istreambuf_iterator<char>(
					*(std::auto_ptr<std::ifstream>(new std::ifstream(codePath))).get())),
			std::istreambuf_iterator<char>());

	// Parse code
	std::stringstream ss(codeFile);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;

	code.assign(begin, end);
	codePointer = code.begin();
}

bool StackMachineCode::isEOF()
{
	return codePointer == code.end();
}

std::string StackMachineCode::getCommand()
{
	if (!isEOF())
	{
		std::vector<std::string>::const_iterator tmp = codePointer;
		codePointer++;
		return (*tmp);
	}
	else
		throw Exception("Unexpected EOF");
}

void StackMachineCode::goTo(std::string label)
{
	codePointer = std::find_if(code.begin(), code.end(), comparator(label));
	if (!isEOF())
	{
		codePointer++;
	}
	else
		throw Exception("Cant't find label " + label);
}

#endif /* STACKMACHINECODE_H_ */
