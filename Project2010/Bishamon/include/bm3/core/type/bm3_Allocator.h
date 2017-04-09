#ifndef BM3_SDK_INC_BM3_CORE_TYPE_BM3_ALLOCATOR_H
#define BM3_SDK_INC_BM3_CORE_TYPE_BM3_ALLOCATOR_H

#include "../bm3_MemoryManager.h"
#include <ml/utility/ml_assert.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

// allocate がwarning に掛かるため
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4100)
#endif

/// @brief Allocatorクラス
template<typename T>
class Allocator{
public:
	typedef size_t		size_type;
	typedef ptrdiff_t	difference_type;
	typedef T *			pointer;
	typedef const T *	const_pointer;
	typedef T &			reference;
	typedef const T &	const_reference;
	typedef T			value_type;

	template<typename U>
	struct rebind{
		typedef Allocator<U> other;
	};

	Allocator() throw(){
		;
	}

	Allocator(const Allocator &) throw(){
		;
	}

	template <class U>
	Allocator(const Allocator<U> &) throw(){
		;
	}

	~Allocator() throw(){
		;
	}
//	pointer allocate(size_type num, Allocator<void>::const_pointer hint = 0)
	pointer allocate(size_type num, const void *hint = 0)
	{
		DummyForNoUseVarWarning(hint);

		return reinterpret_cast<pointer>(MemoryManager::instance().General_Alloc(num * sizeof(T)));
	}

	void construct(pointer p, const T& value){
		ML_PTR_ASSERT(p);
		new(reinterpret_cast<void *>(p)) T(value);
	}

	void deallocate(pointer p, size_type /*num*/){

//		ML_PTR_ASSERT(p);	// freeの中でNULLチェックをしているのでいらない
		return MemoryManager::instance().General_Free(reinterpret_cast<void *>(p));
	}

	void destroy(pointer p){
		ML_PTR_ASSERT(p);
		p->~T();
	}

	pointer address(reference value) const{
		return &value;
	}

	const_pointer address(const_reference value) const{
		return &value;
	}

	size_type max_size() const throw(){
		return MemoryManager::instance().General_MaxSize() / sizeof(T);
	}

private:
	inline void DummyForNoUseVarWarning(const void *){
		;
	}
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif


BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_TYPE_BM3_ALLOCATOR_H
