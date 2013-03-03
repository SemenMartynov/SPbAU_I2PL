/*
 * LCode.h
 *
 *  Created on: 22 Feb 2013
 *      Author: Semen Martynov
 */

#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <vector>
#include <string>
#include <fstream>
#include <iterator>
#include "Exception.h"

class LCode {
public:
	LCode(const char* codePath);
	bool isEOF();
	std::string getCommand();
	void saveLabel();
	void loadLabel();
private:
	std::vector<std::string> code;
	std::vector<std::string>::const_iterator codePointer;
	std::vector<std::string>::const_iterator codeLabel;
};

inline LCode::LCode(const char* codePath)
{
	std::ifstream input_buffer(codePath, std::ios::in);

	std::istream_iterator<std::string> beginFile(input_buffer);
	std::istream_iterator<std::string> endFile;

	code.assign(beginFile, endFile);
	codePointer = code.begin();
}

inline bool LCode::isEOF()
{
	return codePointer == code.end();
}

inline std::string LCode::getCommand()
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

inline void LCode::saveLabel()
{
	codeLabel = codePointer;
}

inline void LCode::loadLabel()
{
	codePointer = codeLabel;
}

#endif /* PROGRAM_H_ */
