#include <iostream>
#include <memory>

template<class T, typename Alloc = std::allocator<T>>
class Vector
{
  private:
  
    /* sz = 0 так как реально элементов при создании по умолчанию не кладем */
    size_t sz = 0;
	/* cap = 10 всегда по умолчанию */ 
	size_t cap = 10;
    T* arr = new T[10];
	Alloc alloc;
	
	typedef typename std::allocator_traits<Alloc> traits;
	//using traits = std::allocator_traits<Alloc>;
  public:

    Vector();
	/* 
		Вектор одинаковых элементов размера size == capacity
		Если не передавать свой аллокатор, будет создан по умолчанию
	*/ 
	Vector(size_t size, const T& value, const Alloc& alloc = Alloc()) : alloc(alloc) 
	{
		reserve(size);
		
		for(size_t i = 0; i < size; ++i)
		{
		    new(arr+i) T(value);
		//	alloc.construct(arr+i, value);
		//	traits::construct(alloc, arr+i, value);
			++sz;
		}
	}
	
	
	Vector<T, Alloc>& operator=(const Vector<T, Alloc>& v)
	{
	    std::cout<<"operator="<<'\n';
		for(size_t i = 0; i < sz; ++i)
		{
			traits::destroy(alloc, arr+i);
		}
		traits::deallocate(alloc, arr);
		
		if(traits::propagate_on_container_copy_assignment::value && alloc != v.alloc) 
		{
			alloc = v.alloc;
		}
		
		try {
			reserve(v.sz);
			for(size_t i = 0; i < v.sz; ++i)
			{
			traits::construct(alloc, arr+i, v[i]);
			}
		} catch (...) {}
	}
	
  
    void reserve(size_t n)
	{
		if(n < cap) return;
		
		/* 
			Выделение памяти, создаем непроинициализированный массив размера [n * sizeof(T)] для элементов типа int8_t
			Кастуем указатель на int8_t к reinterpret_cast<T*>
		*/
		
		T* newarr = traits::allocate(alloc, n);
		//T* newarr = reinterpret_cast<T*>(new int8_t[n * sizeof(T)]);
		
		/*
			Вызовы конструкторов через placement new от страрого массива  
			std::uninitialized_copy(откуда, докуда, куда);
			std::uninitialized_copy - вызывет конструкторы проверяя на исключения при конструировании
			Если исключение возникло сам вызывает деструкторы от того что уже сконструированно и кидает исключение(bad_alloc?)
		*/
		
		try {
			std::uninitialized_copy(arr, arr + sz, newarr);
		} catch(...) {
			traits::deallocate(alloc, newarr, n);
			//delete[] reinterpret_cast<T*>(arr);
			throw;
		}
		
		/* Явный вызов деструкторов для элементов старого массива  */
		for(size_t i=0; i < sz; ++i)
		{
			traits::destroy(alloc, arr + i);
			//(arr + i) -> ~T();
		}
		
		/* Освобождение памяти */
		//traits::deallocate(alloc, arr);
		delete[] reinterpret_cast<int8_t*>(arr);
		
		/* ... */
		arr = newarr;
		cap = n;
	}
	
	
	void resize(size_t n)
	{	
		/* Вызываем деструкторы с конца до size */
	    for(size_t i=sz; n < i; --i)
		{
			traits::destroy(alloc, (arr + sz) -i);
			//((arr + sz) -i) -> ~T();
		}
		
		/* Новый size */ 
		sz = (sz - (sz - n));
	}
	
  // Возможны ошибки
	void shrink_to_fit()
	{
		T* newarr = traits::allocate(alloc, sz);
	    // T* newarr = reinterpret_cast<T*>(new int8_t[sz]);
	    
	    try {
			std::uninitialized_copy(arr, arr + sz, newarr);
		} catch(...) {
			//traits::deallocate(alloc, arr);
			delete[] reinterpret_cast<T*>(arr);
			throw;
		}
		
		for(size_t i=0; i < sz; ++i)
		{
			traits::destroy(alloc, (arr + sz) -i);
			// (arr + i) -> ~T();
		}
		
    
		// Освобождение памяти 
		traits::deallocate(alloc, arr, sz);
		//delete[] reinterpret_cast<int8_t*>(arr);
		
		cap = sz;
		arr = newarr;
	}
	
	
	void push_back(const T& val)
	{
	    std::cout<<"copy"<<'\n';
		/* Memory allocation */ 
		if(sz == cap) { reserve(2 * cap); }
	
		/* placement new, вызываем конструкторы от старых элементов */ 
		traits::construct(alloc, arr + sz, val);
		//new(arr + sz) T(val);
		++sz;
	}
	
	
	void push_back(T&& val)
	{
	    //std::cout<<"Move"<<'\n';
	    /* Memory allocation */
		if(sz == cap) { reserve(2 * cap); }
	
		/* placement new, вызываем конструкторы от старых элементов */
		traits::construct(alloc, arr + sz, std::move(val));
		//new(arr + sz) T(std::move(val));
		++sz;
	}
  
  /* 
		Если в векторе лежат сложные типы, то можно не вставлять объект, 
		а пробросить параметры его конструктора(объект сам построится сэкономив на копировании 
	*/ 
	template<typename... Args>
	void emplace_back(const Args&&... args)
	{
	    if(sz == cap) { reserve(2 * cap); }
	
		traits::construct(alloc, arr + sz, std::forward<Args>(args)...);
    /* placement new, вызываем конструкторы от старых элементов */
		//new(arr + sz) T(args...);
		++sz;
	}
	
	
	void pop_back()
	{
	    --sz;
	    traits::destroy(alloc, arr+sz);
		// (arr + i) -> ~T();
	}
	
	
    bool is_empty() const { return sz == 0; }
    size_t size() const { return sz; }
    size_t capacity() const { return cap; }
    
	/*  */ 
    T& operator[](size_t index)
    { 
	if(i<0 || size() <= i)
	{
		throw out_of_range{"Out of range in Vector::operator[]"};
	}
	return arr[index];
    }
	
	
    const T& operator[](size_t index) const
    {
	if(i<0 || size() <= i)
	{
		throw out_of_range{"Out of range in Vector::operator[]"};
	}
	return arr[index]; 
    }
	
	T& at(size_t index)
	{
		if(index >= sz) throw std::out_of_range("....");
		return arr[index];
	}
};
