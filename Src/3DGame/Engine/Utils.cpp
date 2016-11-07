#include <vector>
#include "Utils.h"

template <typename T>
bool Contains(std::vector<T> vec, T * element)
{
	for (size_t i = 0; i < vec.size(); i++)
	{
		if (vec.at(i) == *element) return true;
	}

	return false;
}

template<typename T>
void Sort(std::vector<T> *vec, std::function<bool(const T*, const T*)> pred)
{
	size_t n = vec->size();

	for (size_t i = 0; i < n; i++)
	{
		size_t min = i;
		for (size_t j = i + 1; j < n; j++)
		{
			if (pred(vec->at(j), vec->at(min))) min = j;
		}

		if (min != i)
		{
			T temp = vec->at(i);
			vec->insert(i, vec->at(min));
			vec->pop_back();
			vec->push_back(temp);
		}
	}
}