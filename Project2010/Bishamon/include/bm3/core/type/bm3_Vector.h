#ifndef BM3_VECTOR_H
#define BM3_VECTOR_H

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

/// @brief VectorƒNƒ‰ƒX
template<typename T>
class Vector{
	struct Item{
		T	value_;
		
		Item(){}
		Item(const T &value) : value_(value){}
	};
public:
	
	struct Iterator{
		Item		*item_;
		
		Iterator(){							;	}
		Iterator(Item *item) : item_(item){	;	}

		T & operator *(){					return item_->value_;	}
		const T & operator *() const{		return item_->value_;	}

		Iterator	operator ++(int){	Iterator i = *this;	item_ = item_++;	return i;	}
		Iterator	operator --(int){	Iterator i = *this;	item_ = item_--;	return i;	}
		Iterator &	operator ++(){		item_++;	return *this;	}
		Iterator &	operator --(){		item_--;	return *this;	}

		bool		operator==(const Iterator &i) const{	return item_ == i.item_;	}
		bool		operator!=(const Iterator &i) const{	return item_ != i.item_;	}
	};

	struct ConstIterator{
		const Item		*item_;

		ConstIterator(){							;	}
		ConstIterator(const T *item) : item_(item){	;	}

		T & operator *(){					return item_->value_;	}
		const T & operator *() const{		return item_->value_;	}

		ConstIterator	operator ++(int){	Iterator i = *this;	item_ = item_++;	return i;	}
		ConstIterator	operator --(int){	Iterator i = *this;	item_ = item_--;	return i;	}
		ConstIterator &	operator ++(){		item_++;	return *this;	}
		ConstIterator &	operator --(){		item_--;	return *this;	}

		bool		operator==(const ConstIterator &i) const{	return item_ == i.item_;	}
		bool		operator!=(const ConstIterator &i) const{	return item_ != i.item_;	}
	};
	
	Vector() :
		array_(NULL), size_(0){
	}
	~Vector(){
		clear();
	}
	
	Iterator	Begin(){			return Iterator(&array_[0]);			}
	Iterator	End(){				return Iterator(&array_[size_-1]);		}
	ConstIterator	Begin() const{	return ConstIterator(&array_[0]);		}
	ConstIterator	End()	const{	return ConstIterator(&array_[size_-1]);	}

	void		set(int id, T v){	array_[id].value_	= v;	}
	T			at(int id) const{	return array_[id].value_;	}

	bool		resize(int size, T v){
		if(array_){
			MemoryManager::instance().General_Free(array_);
		}
		array_	= (Item*)(MemoryManager::instance().General_Alloc(size * sizeof(Item)));
		if(array_){
			size_	= size;
			for(int i=0 ;i<size ; ++i){	new(&array_[i]) Item(v);	}
		}
		else{
			return false;
		}
		return true;
	}
	void		assign(int size, T v){
		for(int i=0 ;i<size ; ++i){	array_[i].value_	= v;	}
	}

	void		clear(){
		for(int i=0 ;i<size_ ; ++i){	array_[i].~Item();	}
		MemoryManager::instance().General_Free(array_);
		array_	= NULL;
		size_	= 0;
	}
	
	bool		empty() const{ return (size_==0);	}
	int			size()	const{ return size_;		}
private:
	Item*	array_;
	int	size_;
};

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3


#endif	//BM3_VECTOR_H

