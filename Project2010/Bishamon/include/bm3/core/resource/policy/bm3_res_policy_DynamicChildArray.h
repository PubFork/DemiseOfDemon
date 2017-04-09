#ifndef BM3_SDK_INC_BM3_CORE_RESOURCE_POLICY_BM3_RES_POLICY_DYNAMICCHILDARRAY_H
#define BM3_SDK_INC_BM3_CORE_RESOURCE_POLICY_BM3_RES_POLICY_DYNAMICCHILDARRAY_H

#include "../../../system/bm3_Config.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace res{

#if defined(BM3_TARGET_IDE)

template<typename ChildType>
class DynamicChildArray{
public:
	void	Add(const ChildType &child){
		data_.push_back(&child);
	}

	void	Insert(int index, const ChildType &child){

		ChildTypeArrayI i = data_.begin();

		std::advance(i, index);

		data_.insert(i, &child);
	}

	void	Remove(int index){

		ChildTypeArrayI i = data_.begin();

		std::advance(i, index);

		data_.erase(i);
	}

	void	Clear(){

		data_.swap(ChildTypeArray());

		ML_ASSERT(Count() == 0);
	}

	int		Count() const{
		return data_.size();
	}

	const ChildType &	Child(int index) const{
		ML_ASSERT(index >= 0);
		ML_ASSERT(index <  Count());
		return *data_[index];
	}

private:
	typedef	std::vector<const ChildType *>		ChildTypeArray;
	typedef	typename ChildTypeArray::iterator	ChildTypeArrayI;

	ChildTypeArray	data_;
};

#endif	// #if defined(BM3_TARGET_IDE)

}	// namespace res
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_RESOURCE_POLICY_BM3_RES_POLICY_DYNAMICCHILDARRAY_H
