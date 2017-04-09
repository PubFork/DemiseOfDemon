#ifndef BM3_SDK_INC_BM3_CORE_FILEFORMAT_BMB_UPDATEBMBINFO_H
#define BM3_SDK_INC_BM3_CORE_FILEFORMAT_BMB_UPDATEBMBINFO_H

#include "../../system/bm3_Config.h"
#include "../bm3_Const.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace fileformat{
namespace bmb{

class UpdateBmbInfo{
public:
	virtual ~UpdateBmbInfo(){}
	virtual void Update(void *res, NodeTypeConst node_type) = 0;
};
	

}	// namespace bmb
}	// namespace fileformat
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// BM3_SDK_INC_BM3_CORE_FILEFORMAT_BMB_UPDATEBMBINFO_H
