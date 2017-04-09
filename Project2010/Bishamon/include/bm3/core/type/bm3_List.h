#ifndef BM3_SDK_INC_BM3_CORE_TYPE_BM3_LIST_H
#define BM3_SDK_INC_BM3_CORE_TYPE_BM3_LIST_H

#include "../../system/bm3_Config.h"
#include "../bm3_MemoryManager.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

/// @brief Listクラス
template<typename T>
class List{

private:

	// 注意：
	// prior_, next_ のメモリー位置を合わせる

	struct Item{
		Item(){;}
		Item(const T &value) : value_(value){;}

		Item	*prior_;
		Item	*next_;
		//
		T		value_;
	};

	struct Mark{
		Item	*prior_;
		Item	*next_;
	};

public:
	struct Iterator{
		Item *item_;

		Iterator(){							;	}
		Iterator(Item *item) : item_(item){	;	}

		T & operator *(){					return item_->value_;	}
		const T & operator *() const{		return item_->value_;	}

		Iterator	operator ++(int){	Iterator i = *this;	item_ = item_->next_;	return i;	}
		Iterator	operator --(int){	Iterator i = *this;	item_ = item_->prior_;	return i;	}
		Iterator &	operator ++(){		item_ = item_->next_;	return *this;	}
		Iterator &	operator --(){		item_ = item_->prior_;	return *this;	}

		bool		operator==(const Iterator &i) const{	return item_ == i.item_;	}
		bool		operator!=(const Iterator &i) const{	return item_ != i.item_;	}
	};

	struct ConstIterator{
		const Item *item_;

		ConstIterator(){								;	}
		ConstIterator(const Item *item) : item_(item){	;	}

		const T & operator *() const{		return item_->value_;	}

		ConstIterator	operator ++(int){	ConstIterator i = *this;	item_ = item_->next_;	return i;	}
		ConstIterator	operator --(int){	ConstIterator i = *this;	item_ = item_->prior_;	return i;	}
		ConstIterator &	operator ++(){		item_ = item_->next_;	return *this;	}
		ConstIterator &	operator --(){		item_ = item_->prior_;	return *this;	}

		bool		operator==(const ConstIterator &i) const{	return item_ == i.item_;	}
		bool		operator!=(const ConstIterator &i) const{	return item_ != i.item_;	}
	};

	List(){
		Initialize();
	}

	~List(){
		Clear();
	}

	Iterator		Begin(){			return Iterator(head_.next_);									}
	Iterator		End(){				return Iterator(reinterpret_cast<Item *>(&tail_));				}
	ConstIterator	ConstBegin() const{	return ConstIterator(head_.next_);								}
	ConstIterator	ConstEnd() const{	return ConstIterator(reinterpret_cast<const Item *>(&tail_));	}

	void	PushBack(const T &value){

		Item *i = (Item *)MemoryManager::instance().General_Alloc(sizeof(Item));

		if(i){
			new(i) Item();

			Item *end	= tail_.prior_;

			i->prior_	= end;
			i->next_	= reinterpret_cast<Item *>(&tail_);
			i->value_	= value;

			tail_.prior_	= i;
			end->next_		= i;

			++ size_;
		}
	}

	void	Clear(){

		Iterator i   = Begin();
		Iterator end = End();

		while(i != end){
			i = Erase(i);
		}
	}

	Iterator		Erase(Iterator i){

		Item *prior = i.item_->prior_;
		Item *next  = i.item_->next_;

		i.item_->value_.~T();

		MemoryManager::instance().General_Free(i.item_);

		prior->next_	= next;
		next->prior_	= prior;

		-- size_;

		return Iterator(next);
	}

	bool	Empty() const{	return size_ == 0;	}
	int		Size() const{	return size_;		}

private:
	int		size_;

	Mark	head_;
	Mark	tail_;

	void	Initialize(){

		head_.prior_	= NULL;
		head_.next_		= reinterpret_cast<Item *>(&tail_);
		tail_.prior_	= reinterpret_cast<Item *>(&head_);
		tail_.next_		= NULL;

		size_			= 0;
	}
};

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_TYPE_BM3_LIST_H
