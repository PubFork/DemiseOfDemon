#ifndef BM3_SDK_INC_SDK_INC_BM3_CORE_BMSLN_BM3_BMSLN_IDEHANDLE_H
#define BM3_SDK_INC_SDK_INC_BM3_CORE_BMSLN_BM3_BMSLN_IDEHANDLE_H

#include "../../system/bm3_Config.h"
#include "bm3_bmsln_HandleBase.h"
#if defined(BM3_TARGET_IDE)
#include "../resource/bm3_res_IdeNode.h"
#endif


namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE


namespace bmsln{

#if defined(BM3_TARGET_IDE)
namespace ide{

class Handle : public HandleBase{

public:

	Handle();
	virtual ~Handle();

	virtual void Initialize(const res::IdeNode *res);

	const void* Image();

protected:
	const res::IdeNode			*ide_node_;

private:

	bool IsInitialize() const;
	void Finalize();
};

}	// namespace ide
#endif	// #if defined(BM3_TARGET_IDE)

}	// namespace bmsln
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// BM3_SDK_INC_SDK_INC_BM3_CORE_BMSLN_BM3_BMSLN_IDEHANDLE_H
