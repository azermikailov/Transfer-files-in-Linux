// dell.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<iostream>
#include<thread>
#include<functional>
#include<algorithm>
#include<vector>
#include<iterator>
#include<stack>
#include<functional>

//
class SomeClass
{
public:

	void DisplayMembers(void)
	{

	} // функция представления должна							   // быть константной
};
//Когда вы создаете такую функцию, как эта, происходит отказ :
void DisplayAllData(const SomeClass& mData)
{
	SomeClass & temp = const_cast<SomeClass&>(mData); // Отказ компиляции
							// причина отказа: вызов не константного члена
							// класса с использованием константной ссылки
	temp.DisplayMembers();
}


template <typename T>
class MyFirstTemplateClass
{
	T Value;
public:
	MyFirstTemplateClass(void) {}
	void SetValue(const T & newValue) { Value = newValue; }
	const T& GetValue(void) const { return Value; }
};



int fmain()
{
	const int N{ 10 };
	std::vector<int> v = { 2,3,4,5,6,7,2 };

	v.erase(std::remove_if(v.begin(), v.end(),[](int& i) { return i == 2; }), v.end());

	std::copy(v.cbegin(), v.cend(), std::ostream_iterator<int, char>(std::cout, " "));

	return 0;
}

void sort(int* arr, int size)
{
	bool flag = false;
	while (!flag)
	{
		flag = true;
		for (int i{}; i < size - 1; ++i)
		{
			if (arr[i] > arr[i + 1])
			{
				int temp = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = temp;
				flag = false;
			}
		}
	}
}

std::function<int(int a, int b)> fun() { return [](int i, int b)->int {return i *b; }; }

void Today()
{
	int ptr[]{ 1, 2, 3, 4, 5 },
	    size = sizeof(ptr) / sizeof(int),
		fac{},
		*p = &ptr[0];

	while (p < &ptr[size])
		fac += *p++;

	std::cout << fac << std::endl;
	std::cin.get();

}

#if 0

size_t strLen(const char *ptr)
{
	size_t size{};
	while (*ptr++)
		size++;

	return size;
}

#endif

int main(void)
{
	
	int ptr = __LINE__;


	std::cout << ptr << " " << __TIME__ << std::endl;
	std::cin.get();

	Today();

	int v[8] = { 3,4,6, v[0] = 666,v[5] = 10 };

	auto raz = sizeof(v);

	auto size = sizeof(v) / sizeof(int);
    sort(v,size);
	for (const auto& decl : v)
		std::cout << decl << " ";
	std::cout << std::endl;
	

	//std::copy(&v[0], &v[size], std::ostream_iterator<int, char>(std::cout, " "));
	
	
	return 0;
}
