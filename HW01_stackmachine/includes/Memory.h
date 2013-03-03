/*
 * Memory
 *
 *  Created on: 21 Feb 2013
 *      Author: Semen Martynov
 */

#ifndef MEMORY_
#define MEMORY_
#include <map>
#include "Exception.h"

template<typename T>
class Memory
{
public:
	void put(std::string key, T val);
	T get(std::string);
private:
	std::map<std::string, T> memory;
};

template<typename T>
void Memory<T>::put(std::string key, T val)
{
	memory[key] = val;
}

template<typename T>
T Memory<T>::get(std::string key)
{
	typename std::map<std::string, T>::const_iterator kv = memory.find(key);
	if (kv != memory.end())
	{
		return kv->second;
	}
	else
		throw Exception("Can't find variable" + key);
}

#endif /* MEMORY_ */
