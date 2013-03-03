/*
 * Stack.h
 *
 *  Created on: 21 Feb 2013
 *      Author: Semen Martynov
 */

#ifndef STACK_H_
#define STACK_H_
#include <stack>
#include "Exception.h"

template<typename T>
class Stack
{
public:
	void put(T val);
	T get();
private:
	std::stack<T> stack;
};

template<typename T>
void Stack<T>::put(T val)
{
	stack.push(val);
}

template<typename T>
T Stack<T>::get()
{
	if (stack.size() > 0)
	{
		T val = stack.top();
		stack.pop();
		return val;
	}
	else
		throw Exception("Stack is empty");
}

#endif /* STACK_H_ */
