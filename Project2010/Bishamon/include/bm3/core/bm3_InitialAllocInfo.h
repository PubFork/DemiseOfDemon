#ifndef BM3_SDK_INC_BM3_CORE_BM3_INITIALALLOCINFO_H
#define BM3_SDK_INC_BM3_CORE_BM3_INITIALALLOCINFO_H

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class InitialAllocInfo{

public:
	InitialAllocInfo(int parent_max_gen_count) :
		parent_max_gen_count_(parent_max_gen_count){
		;
	}

	int					ParentMaxGenCount(){	return parent_max_gen_count_;	}

private:
	int					parent_max_gen_count_;
};

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BM3_INITIALALLOCATEINFO_H
