#ifndef BM3_SDK_INC_BM3_CORE_TYPE_BM3_USERDATA_H
#define BM3_SDK_INC_BM3_CORE_TYPE_BM3_USERDATA_H

#include <ml/utility/ml_assert.h>
#include "../../system/bm3_Config.h"
#include "../bm3_CoreType.h"
#include "../bm3_Const.h"
#include "../bm3_CoreTypeSwitch.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

#if defined(BM3_TARGET_IDE)
namespace ide{

/// @brief UserDataƒNƒ‰ƒX
class UserData{

public:
	typedef	std::vector<ide::UserDataElement>	UserDataElementArray;
	typedef	UserDataElementArray::iterator	UserDataElementArrayI;

	UserData(){
		;
	}

	UserDataElementArrayI	AddElement(const StringType &name, UserDataElementTypeConst type){
		element_array_.push_back(ide::UserDataElement(name, type));

		return element_array_.end() - 1;
	}

	void	RemoveElement(UserDataElementArrayI i){

		element_array_.erase(i);
	}

	void	RemoveElement(int index){

		ML_ASSERT(index >= 0);
		ML_ASSERT(index <  ElementCount());

		UserDataElementArrayI i = element_array_.begin();
		std::advance(i, index);

		RemoveElement(i);
	}

	void	Clear(){

		element_array_.clear();
	}

	int		ElementIndex(UserDataElementArrayI i){

		return std::distance(element_array_.begin(), i);
	}

	int						ElementCount() const{		return int(element_array_.size());	}
	UserDataElementArrayI	ElementBegin(){				return element_array_.begin();		}
	UserDataElementArrayI	ElementEnd(){				return element_array_.end();		}
	const ide::UserDataElement &	Element(int index) const{
		ML_ASSERT((index >= 0) && (index < ElementCount()));
		return element_array_[index];
	}

private:
	UserDataElementArray	element_array_;
};

}	// namespace ide
#endif	// #if defined(BM3_TARGET_IDE)

namespace embedded{
class UserData{

public:
	typedef	const embedded::UserDataElement *	UserDataElementArrayI;

	int						ElementCount() const{		return element_count_;					}
	UserDataElementArrayI	ElementBegin()		 const{		return element_begin_;					}
	UserDataElementArrayI	ElementEnd() const{			return element_begin_ + element_count_;	}
	const embedded::UserDataElement &	Element(int index) const{
		ML_ASSERT((index >= 0) && (index < ElementCount()));
		return element_begin_[index];
	}

	void	Mapping(const void *binary_start, const void *string_start){

		element_begin_ =
			(element_begin_offset_ == ml::ptr_size_ui(-1)) ?
				NULL :
				reinterpret_cast<UserDataElementArrayI>(
					ml::ptr_size_ui(binary_start) + element_begin_offset_
				);

		embedded::UserDataElement *i	= const_cast<embedded::UserDataElement *>(ElementBegin());
		embedded::UserDataElement *end	= const_cast<embedded::UserDataElement *>(ElementEnd());
		for(;i != end;++ i){
			i->Mapping(binary_start, string_start);
		}
	}

	void	SetElementCount(int c){					element_count_ = c;					}
	void	SetElementOffset(ml::ptr_size_ui o){	element_begin_offset_ = o;			}

private:
	ml::si32			element_count_;

	union{
		UserDataElementArrayI	element_begin_;
		ml::ptr_size_ui			element_begin_offset_;
	};
};

}	// namespace embedded

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_TYPE_BM3_USERDATA_H
