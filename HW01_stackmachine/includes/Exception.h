/*
 * Exception.h
 *
 *  Created on: 21 Feb 2013
 *      Author: Semen Martynov
 */

#ifndef EXCEPTION_H_
#define EXCEPTION_H_
#include <exception>
#include <string>

class Exception: public std::exception
{
public:
	Exception(const std::string& message) :
			message(message)
	{
	}
	virtual ~Exception() throw ()
	{
	}
	virtual const char* what() const throw ()
	{
		return message.c_str();
	}
private:
	std::string message;
}
;

#endif /* EXCEPTION_H_ */
