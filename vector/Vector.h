#ifndef VECTOR_H
#define VECTOR_H
#include <math.h>
template<typename T>
class Vector
{
public: 
	Vector(); //конструктор по подразбиране 
	Vector(size_t number, const T& value); //запълващ конструктор 
	Vector(const Vector& v); //конструктор за копиране 
	Vector (const Vector&& v); //конструктор за преместване
	Vector& operator=(const Vector& v); //оператор за копиращо присвояване
	void push_back(const T& value); //Добавя елемент в края 
	void pop_back(); //Премахва последния елемент 
	void resize(size_t newSize); //променя размера на динамичния масив, така че да стане newSize
	T& back()
		{
			T& y = arr[m_size-1];
			return y;  //връща стойността на елемента в края на Vector
	    }
	T& front() 
		{
			T& x = arr[0];
			return x;  //връща стойността на елемента в началото на Vector
	    }
	void insert(size_t index, const T& value); //вмъква елемент със стойност value на позиция index
	void erase(size_t index);  //изтрива елемент на позиция index
	T& operator[](size_t index)
		{
			return arr[index];  //връща псевдоним към елемент по подаден индекс
	    }
	int size() const
		{
			return m_size; //връща броя на елементите
	    }
	int capacity() const
		{
			return m_capacity;  //връща капацитета 
	    }
	bool empty() const
		{
			if (m_size && m_capacity == 0)   //проверява дали има елементи
				return true;
			else return false;
	    }
	void clear(); //изтрива цялото съдържание на Vector
	void shrink_to_fit(); //изтрива ненужния капацитет
	void swap (Vector& v); //разменя два масива от един и същи тип
	void reserve(size_t newCapacity); //Заделя памет с посочения капацитет newCapacity.
	~Vector(); 
private:
	size_t m_size;
	size_t m_capacity;
	T* arr;
	
	//Заделя памет за тип T с капацитет m_capacity
	//@param m_capcity - размер на заделената памет
	//@return указател към заделената памет
	static T* allocate_memory(size_t m_capacity)
	{
		return static_cast<T*> (operator new(sizeof(T) * m_capacity));
	}
	
	//Копира елементите от begin до end в new_place.
	//@param begin - указател към началото на източника
	//@param end - указател към края 
	//@param new_place - указател към мястото, където ще се извърши копирането
	//@return
	static void copy_function(T* begin, T* end, T* new_place)
	{
		while (begin != end) {
			//Инициализиране на конструктор за копиране с placement new
			new((void*)new_place) T(*begin);
			++begin;
			++new_place;
		}
	}
	//Конструира с конструктор по подразбиране обектите върху паметта от begin до end
	//@param begin - указател към началото
	//@param end - указател към края
	//@return
	static void construct_function(T* begin, T* end)
	{
		while (begin != end) {
			//Инициализиране на подразбиращ се конструктор с placement new
			new((void*)begin) T();
			++begin;
		}
	}
	//Конструира с конструктор, приемащ стойност fill_value обектите върху паметта от begin до end 
	//@param begin - указател към началото
	//@param end - указател към края
	//@return
	static void construct_function(T* begin, T* end, const T& fill_value)
	{
		while (begin != end) {
			//Инициализиране на конструктор за запълване с placement new
			new((void*)begin) T(fill_value);
			++begin;
		}
	}
	//Извиква деструкторите на обектите върху паметта, определна от begin до end
	//@param begin - указател към началото
	//@param end - указател към края
	static void delete_function(T* begin, T* end)
	{
		while (begin != end) {
			begin->~T();
			++begin;
		}
	}
};

//Създава празен вектор с капацитет 0 и брой елементи 0.
template <typename T>
Vector<T> :: Vector () : m_size(0), m_capacity(0), arr(nullptr)
{

}

//@param number - размер на векторът
//@param value - стойност с която да се запълни векторът
template<typename T>
Vector <T> :: Vector(size_t number, const T& value) : m_size(number), m_capacity(number),arr(allocate_memory(number))
{
	construct_function (arr, arr+m_size, value);
}

//Създава вектор, който съдържа копие на всеки от елементите на v.
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

//Копира данните от v.
template <typename T>
Vector<T>& Vector<T> :: operator=(const Vector& v)
{
      if (this == &v) 
	  {
		return *this;
	  }
//Извиква деструкторите на текущите елементи и освобождава текущо заделената памет
delete_function (arr, arr+m_size);
operator delete (arr);
m_capacity = v.m_capacity;
m_size = v.m_size;
arr = allocate_memory(v.m_capacity);
//Копира елементите
copy_function(v.arr, v.arr + v.m_size, arr);
return *this;
}


template<typename T>
void Vector<T>::push_back(const T& value)
{
	if (m_size != m_capacity) {
		//Инициализиране на конструктор за копиране с placement new
		new((void*)(arr + m_size)) T(value);
		++m_size;
		return;
	}
	//Задаваме нов капацитет с 1 по-голям от предишния и заделяме памет за него
	size_t new_capacity = m_capacity + 1;
	T* new_arr = allocate_memory(new_capacity);
	//Копиране на старите стойности
	copy_function(arr, arr + m_size, new_arr);
	//Инициализиране на конструктор за копиране с placement new
	new((void*)(new_arr + m_size)) T(value);
	//Извиква деструкторите на старите елементи и освобождава заделената памет
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
	//Заделяме памет за новия капацитет
	T* new_arr = allocate_memory(new_capacity);
	//Копираме старите стойности
	copy_function(arr, arr + m_size, new_arr);
	//Извиква деструкторите на старите елементи и освобождава заделената памет
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
		//Заделяме памет 
		T* new_arr_begin = allocate_memory(m_capacity);
		T* new_arr_end = allocate_memory(m_capacity);
		//Копиране на старите стойности от arr[0] до arr[index-1] в new_arr_begin
		copy_function(arr,arr+index-1,new_arr_begin);
		//Копиране на старите стойности на елементите от arr[index-1] до arr[m_size-1] в new_arr_end 
		copy_function(arr+index-1,arr+m_size,new_arr_end);
		++m_capacity;
		//Заделяне на нова памет за масива
		arr = allocate_memory(m_capacity);
		//arr[index-1] присвоява стойност value
		new((void*)(arr+index-1)) T(value);
		//връщаме стойностите
		copy_function(new_arr_begin,new_arr_begin+index-1,arr);
		copy_function(new_arr_end,new_arr_end+m_size-(index-1),arr+index);
		++m_size;
		//освобождаваме паметта 
		operator delete (new_arr_begin);
		operator delete (new_arr_end);
	}
	else cout<<"error";
}	


template <typename T>
void Vector <T> :: erase (size_t index)
{ 
	//Обхожда масива от arr[index] до аrr[m_size-1] като всеки елемент присвоява стойността на следващия
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
	//новия капацитет трябва да е равен на броя елементи
	size_t new_capacity = m_size;
	T* new_arr = allocate_memory(new_capacity);
	copy_function(arr, arr + m_size, new_arr);
	//Освобождаваме паметта
	delete_function(arr, arr + m_size);
	operator delete(arr);
	arr = new_arr;
	m_capacity = new_capacity;
	//Конструираме новия масив
	construct_function(arr, arr + m_capacity);	
}

template <typename T>
void Vector <T> :: swap(Vector& v)
{
   if (this == &v)
   {
	   return;
   }
   //Заделяме памет за нов масив new_arr
   T* new_arr = allocate_memory(m_capacity);
   size_t new_size;
   size_t new_capacity;
   //Копираме текущия масив arr във вече заделения new_arr 
   copy_function(arr,arr+m_size,new_arr);
   //Копиране на новия масив v в стария arr
   copy_function(v.arr,v.arr+m_size,arr);
   //Копиране на new_arr (който се състои от стойностите на arr) в v
   copy_function(new_arr,new_arr+m_size,v.arr);
   //Размяна на параметрите на arr и v
   new_capacity = m_capacity;
   m_capacity = v.m_capacity;
   v.m_capacity = new_capacity;
   new_size = m_size;
   m_size = v.m_size;
   v.m_size = new_size;
   //Изтриваме обеките на new_arr и освобождаваме паметта
   delete_function(new_arr,new_arr+m_size);
   operator delete(new_arr);
}

template <typename T>
void Vector <T> :: reserve(size_t newCapacity)
{
	if (newCapacity > m_capacity) {
		//Заделяме памет за новия капацитет
		T* new_arr = allocate_memory(newCapacity);
		//Копираме старите стойности
		copy_function(arr, arr + m_size, new_arr);
		//Извикваме деструкторите и освобождаваме паметта
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

