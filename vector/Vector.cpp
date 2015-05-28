#include "Vector.h"
#include <iostream>
#include <cmath>
template <typename T>
Vector<T> :: Vector () : m_size(0), m_capacity(0), arr(nullptr)
{

}
template<typename T>
Vector <T> :: Vector(int number, const T& value) : m_size(number), m_capacity(number),arr(allocate_memory(number))
{
	construct_function (arr, arr+m_size, value);
}
template<typename T>
Vector <T> :: Vector(const Vector& v) : m_capacity(v.m_capacity), m_size(v.m_size), arr(allocate_memory(v.m_capacity))
{
	copy_function(v.arr, v.arr+v.m_size, arr);
}
template<typename T>
Vector <T> :: Vector (const Vector&& v) : m_size(0), m_capacity(0), arr(nullptr)
{
	m_size = v.m_size;
	m_capacity = v.m_capacity;
	arr = v.arr;
	v.m_size = 0;
	v.m_capacity = 0;
	v.arr = nullptr;
}

template <typename T>
Vector<T>& Vector<T> :: operator=(const Vector& v)
{
      if (this == &v) 
		return *this;
delete_function (arr, arr+m_size);
operator delete (arr);
m_capacity = v.m_capacity;
m_size = v.m_size;
arr = allocate_memory(v.m_capacity);
copy_function(v.arr, v.arr + v.m_size, arr);
return *this;
}
template<typename T>
void Vector<T>::push_back(const T& value)
{
	if (m_size != m_capacity) {
		new((void*)(arr + m_size)) T(value);
		++m_size;
		return;
	}
	int new_capacity = m_capacity ? (int)round(m_capacity * 1.5) : 1;
	T* new_arr = allocate_memory(new_capacity);
	copy_function(arr, arr + m_size, new_arr);
	new((void*)(new_arr + m_size)) T(value);
	delete_function(arr, arr + m_size);
	operator delete(arr);
	arr = new_arr;
	m_capacity = new_capacity;
	++m_size;
}
template<typename T>
void Vector<T>::pop_back()
{
	(arr + m_size - 1)->~T();
	--m_size;
}
template<typename T>
void Vector <T> :: resize(int newSize)
{
	if (newSize <= m_size) {
		delete_function(arr + newSize, arr + m_size);
		m_size = newSize;
		return;
	}
    if (newSize <= m_capacity) {
		construct_function(arr + m_size, arr + newSize);
		m_size = newSize;
		return;
	}

	int new_capacity = newSize;
	int default_growth_capacity = (int)round(m_size * 1.5);
	if (new_capacity < default_growth_capacity) {
		new_capacity = default_growth_capacity;
	}
	T* new_arr = allocate_memory(new_capacity);
	copy_function(arr, arr + m_size, new_arr);
	delete_function(arr, arr + m_size);
	operator delete(arr);
	arr = new_arr;
	m_capacity = new_capacity;
	construct_function(arr + m_size, arr + newSize);
	m_size = newSize;
}
template <typename T>
void Vector <T> :: insert(int index, const T& value)
{
	m_size ++;
	for (int i = index; i < m_size+1;i++)
	{
		arr[i+1]=arr[i];
	}
	arr[index] = value;
}	
template <typename T>
void Vector <T> :: erase (int index)
{
	for (int i = index; i < m_size-1; i++)
	{
		arr[i]=arr[i+1];
	}
	m_size--;
}
template <typename T>
void Vector <T> :: clear()
{
	m_size = 0;
	m_capacity = 0;
	arr = nullptr;
}
template <typename T>
void Vector <T> :: shrink_to_fit ()
{
	int new_capacity = m_size;
	T* new_arr = allocate_memory(new_capacity);
	copy_function(arr, arr + m_size, new_arr);
	delete_function(arr, arr + m_size);
	operator delete(arr);
	arr = new_arr;
	m_capacity = new_capacity;
	construct_function(arr, arr + m_capacity);	
}
template <typename T>
void Vector <T> :: swap(Vector& v)
{
  
}

template <typename T>
void Vector <T> :: reserve(int newCapacity)
{
	if (newCapacity > m_capacity) {
		T* new_arr = allocate_memory(newCapacity);
		copy_function(arr, arr + m_size, new_arr);
		delete_function(arr, arr + m_size);
		operator delete(arr);
		arr = new_arr;
		m_capacity = newCapacity;
	}
}
template<typename T>
Vector<T>::~Vector()
{
	delete_function(arr, arr + m_size);
	operator delete(arr);
}

