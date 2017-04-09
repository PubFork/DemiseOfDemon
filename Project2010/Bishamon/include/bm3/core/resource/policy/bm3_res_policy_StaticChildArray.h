#ifndef BM3_SDK_INC_BM3_CORE_RESOURCE_POLICY_BM3_RES_POLICY_STATICCHILDARRAY_H
#define BM3_SDK_INC_BM3_CORE_RESOURCE_POLICY_BM3_RES_POLICY_STATICCHILDARRAY_H

#include "../../../system/bm3_Config.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace res{

template<typename ChildType>
class StaticChildArray{

public:
	int					Count() const{
		return count_;
	}

	const ChildType &	Child(int index) const{
		ML_ASSERT(index >= 0);
		ML_ASSERT(index <  count_);

		return *child_array_[index];
	}

	ChildType &	Child(int index){
		ML_ASSERT(index >= 0);
		ML_ASSERT(index <  count_);

		return *child_array_[index];
	}

	void	Mapping(const void *start){

		child_array_ =
			(child_array_offset_ == ml::ptr_size_ui(-1)) ?
				NULL :
				reinterpret_cast<ChildType **>(
					ml::ptr_size_ui(start) + child_array_offset_
				);
	}
	void	SetChild(int index, ChildType *c){
		child_array_[index] = c;
	}

	void	SetCount(int c){						count_ = c;					}
	void	SetChildArrayOffset(ml::ptr_size_ui o){	child_array_offset_ = o;	}

private:
	int					count_;

	union{
		ChildType **		child_array_;
		ml::ptr_size_ui		child_array_offset_;
	};
};

}	// namespace res
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_RESOURCE_POLICY_BM3_RES_POLICY_STATICCHILDARRAY_H
