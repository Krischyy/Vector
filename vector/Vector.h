#ifndef VECTOR_H
#define VECTOR_H
#include <math.h>
template<typename T>
class Vector
{
public: 
	Vector(); //����������� �� ������������ 
	Vector(size_t number, const T& value); //�������� ����������� 
	Vector(const Vector& v); //����������� �� �������� 
	Vector (const Vector&& v); //����������� �� �����������
	Vector& operator=(const Vector& v); //�������� �� �������� �����������
	void push_back(const T& value); //������ ������� � ���� 
	void pop_back(); //�������� ��������� ������� 
	void resize(size_t newSize); //������� ������� �� ���������� �����, ���� �� �� ����� newSize
	T& back()
		{
			T& y = arr[m_size-1];
			return y;  //����� ���������� �� �������� � ���� �� Vector
	    }
	T& front() 
		{
			T& x = arr[0];
			return x;  //����� ���������� �� �������� � �������� �� Vector
	    }
	void insert(size_t index, const T& value); //������ ������� ��� �������� value �� ������� index
	void erase(size_t index);  //������� ������� �� ������� index
	T& operator[](size_t index)
		{
			return arr[index];  //����� ��������� ��� ������� �� ������� ������
	    }
	int size() const
		{
			return m_size; //����� ���� �� ����������
	    }
	int capacity() const
		{
			return m_capacity;  //����� ���������� 
	    }
	bool empty() const
		{
			if (m_size && m_capacity == 0)   //��������� ���� ��� ��������
				return true;
			else return false;
	    }
	void clear(); //������� ������ ���������� �� Vector
	void shrink_to_fit(); //������� �������� ���������
	void swap (Vector& v); //������� ��� ������ �� ���� � ���� ���
	void reserve(size_t newCapacity); //������ ����� � ��������� ��������� newCapacity.
	~Vector(); 
private:
	size_t m_size;
	size_t m_capacity;
	T* arr;
	
	//������ ����� �� ��� T � ��������� m_capacity
	//@param m_capcity - ������ �� ���������� �����
	//@return �������� ��� ���������� �����
	static T* allocate_memory(size_t m_capacity)
	{
		return static_cast<T*> (operator new(sizeof(T) * m_capacity));
	}
	
	//������ ���������� �� begin �� end � new_place.
	//@param begin - �������� ��� �������� �� ���������
	//@param end - �������� ��� ���� 
	//@param new_place - �������� ��� �������, ������ �� �� ������� ����������
	//@return
	static void copy_function(T* begin, T* end, T* new_place)
	{
		while (begin != end) {
			//�������������� �� ����������� �� �������� � placement new
			new((void*)new_place) T(*begin);
			++begin;
			++new_place;
		}
	}
	//���������� � ����������� �� ������������ �������� ����� ������� �� begin �� end
	//@param begin - �������� ��� ��������
	//@param end - �������� ��� ����
	//@return
	static void construct_function(T* begin, T* end)
	{
		while (begin != end) {
			//�������������� �� ����������� �� ����������� � placement new
			new((void*)begin) T();
			++begin;
		}
	}
	//���������� � �����������, ������� �������� fill_value �������� ����� ������� �� begin �� end 
	//@param begin - �������� ��� ��������
	//@param end - �������� ��� ����
	//@return
	static void construct_function(T* begin, T* end, const T& fill_value)
	{
		while (begin != end) {
			//�������������� �� ����������� �� ��������� � placement new
			new((void*)begin) T(fill_value);
			++begin;
		}
	}
	//������� ������������� �� �������� ����� �������, ��������� �� begin �� end
	//@param begin - �������� ��� ��������
	//@param end - �������� ��� ����
	static void delete_function(T* begin, T* end)
	{
		while (begin != end) {
			begin->~T();
			++begin;
		}
	}
};

//������� ������ ������ � ��������� 0 � ���� �������� 0.
template <typename T>
Vector<T> :: Vector () : m_size(0), m_capacity(0), arr(nullptr)
{

}

//@param number - ������ �� ��������
//@param value - �������� � ����� �� �� ������� ��������
template<typename T>
Vector <T> :: Vector(size_t number, const T& value) : m_size(number), m_capacity(number),arr(allocate_memory(number))
{
	construct_function (arr, arr+m_size, value);
}

//������� ������, ����� ������� ����� �� ����� �� ���������� �� v.
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

//������ ������� �� v.
template <typename T>
Vector<T>& Vector<T> :: operator=(const Vector& v)
{
      if (this == &v) 
	  {
		return *this;
	  }
//������� ������������� �� �������� �������� � ����������� ������ ���������� �����
delete_function (arr, arr+m_size);
operator delete (arr);
m_capacity = v.m_capacity;
m_size = v.m_size;
arr = allocate_memory(v.m_capacity);
//������ ����������
copy_function(v.arr, v.arr + v.m_size, arr);
return *this;
}


template<typename T>
void Vector<T>::push_back(const T& value)
{
	if (m_size != m_capacity) {
		//�������������� �� ����������� �� �������� � placement new
		new((void*)(arr + m_size)) T(value);
		++m_size;
		return;
	}
	//�������� ��� ��������� � 1 ��-����� �� ��������� � �������� ����� �� ����
	size_t new_capacity = m_capacity + 1;
	T* new_arr = allocate_memory(new_capacity);
	//�������� �� ������� ���������
	copy_function(arr, arr + m_size, new_arr);
	//�������������� �� ����������� �� �������� � placement new
	new((void*)(new_arr + m_size)) T(value);
	//������� ������������� �� ������� �������� � ����������� ���������� �����
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
void Vector <T> :: resize(size_t newSize)
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

	size_t new_capacity = newSize;
	size_t default_growth_capacity = (size_t)round(m_size * 1.5);
	if (new_capacity < default_growth_capacity) {
		new_capacity = default_growth_capacity;
	}
	//�������� ����� �� ����� ���������
	T* new_arr = allocate_memory(new_capacity);
	//�������� ������� ���������
	copy_function(arr, arr + m_size, new_arr);
	//������� ������������� �� ������� �������� � ����������� ���������� �����
	delete_function(arr, arr + m_size);
	operator delete(arr);
	arr = new_arr;
	m_capacity = new_capacity;
	construct_function(arr + m_size, arr + newSize);
	m_size = newSize;
}


template <typename T>
void Vector <T> :: insert(size_t index, const T& value)
{
	if (index <= m_size)
	{
		//�������� ����� 
		T* new_arr_begin = allocate_memory(m_capacity);
		T* new_arr_end = allocate_memory(m_capacity);
		//�������� �� ������� ��������� �� arr[0] �� arr[index-1] � new_arr_begin
		copy_function(arr,arr+index-1,new_arr_begin);
		//�������� �� ������� ��������� �� ���������� �� arr[index-1] �� arr[m_size-1] � new_arr_end 
		copy_function(arr+index-1,arr+m_size,new_arr_end);
		++m_capacity;
		//�������� �� ���� ����� �� ������
		arr = allocate_memory(m_capacity);
		//arr[index-1] ��������� �������� value
		new((void*)(arr+index-1)) T(value);
		//������� �����������
		copy_function(new_arr_begin,new_arr_begin+index-1,arr);
		copy_function(new_arr_end,new_arr_end+m_size-(index-1),arr+index);
		++m_size;
		//������������� ������� 
		operator delete (new_arr_begin);
		operator delete (new_arr_end);
	}
	else cout<<"error";
}	


template <typename T>
void Vector <T> :: erase (size_t index)
{ 
	//������� ������ �� arr[index] �� �rr[m_size-1] ���� ����� ������� ��������� ���������� �� ���������
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
	//����� ��������� ������ �� � ����� �� ���� ��������
	size_t new_capacity = m_size;
	T* new_arr = allocate_memory(new_capacity);
	copy_function(arr, arr + m_size, new_arr);
	//������������� �������
	delete_function(arr, arr + m_size);
	operator delete(arr);
	arr = new_arr;
	m_capacity = new_capacity;
	//������������ ����� �����
	construct_function(arr, arr + m_capacity);	
}

template <typename T>
void Vector <T> :: swap(Vector& v)
{
   if (this == &v)
   {
	   return;
   }
   //�������� ����� �� ��� ����� new_arr
   T* new_arr = allocate_memory(m_capacity);
   size_t new_size;
   size_t new_capacity;
   //�������� ������� ����� arr ��� ���� ��������� new_arr 
   copy_function(arr,arr+m_size,new_arr);
   //�������� �� ����� ����� v � ������ arr
   copy_function(v.arr,v.arr+m_size,arr);
   //�������� �� new_arr (����� �� ������ �� ����������� �� arr) � v
   copy_function(new_arr,new_arr+m_size,v.arr);
   //������� �� ����������� �� arr � v
   new_capacity = m_capacity;
   m_capacity = v.m_capacity;
   v.m_capacity = new_capacity;
   new_size = m_size;
   m_size = v.m_size;
   v.m_size = new_size;
   //��������� ������� �� new_arr � ������������� �������
   delete_function(new_arr,new_arr+m_size);
   operator delete(new_arr);
}

template <typename T>
void Vector <T> :: reserve(size_t newCapacity)
{
	if (newCapacity > m_capacity) {
		//�������� ����� �� ����� ���������
		T* new_arr = allocate_memory(newCapacity);
		//�������� ������� ���������
		copy_function(arr, arr + m_size, new_arr);
		//��������� ������������� � ������������� �������
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

#endif

