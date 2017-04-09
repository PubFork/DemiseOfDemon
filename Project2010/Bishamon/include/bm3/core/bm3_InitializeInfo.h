#ifndef BM3_SDK_INC_BM3_CORE_BM3_INITIALIZEINFO_H
#define BM3_SDK_INC_BM3_CORE_BM3_INITIALIZEINFO_H

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace obj{
class RootNodeBase;
class NodeBase;
}	// namespace obj

/// @brief InitializeInfoƒNƒ‰ƒX
class InitializeInfo{

public:
	InitializeInfo(obj::RootNodeBase *root, const float gen_scale) :
		root_(root),
		gen_scale_(gen_scale){
		;
	}

	obj::RootNodeBase	*Root()		{	return root_;		}
	float				GenScale()	{	return gen_scale_;	}

private:
	obj::RootNodeBase	*root_;
	float				gen_scale_;
};

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BM3_INITIALIZEINFO_H
