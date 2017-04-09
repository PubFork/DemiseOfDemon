#ifndef BM3_SDK_INC_BM3_CORE_FILEFORMAT_BMB_NODEHEADER_H
#define BM3_SDK_INC_BM3_CORE_FILEFORMAT_BMB_NODEHEADER_H

#include <ml/type/ml_basic.h>
#include <ml/type/ml_vector3d.h>
#include <ml/type/ml_color.h>
#include <ml/utility/ml_assert.h>
#include <ml/utility/ml_make_sign.h>
#include "../bm3_Const.h"
#include "../bm3_CoreType.h"
#include "../../core/resource/policy/bm3_res_policy_StaticChildArray.h"
#include "../../utility/internal/bm3_MacroBegin.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace fileformat{
namespace bmb{

struct NodeHeader{
	NodeTypeConst	node_type_;
	ml::si32		byte_size_;
	ml::si32		child_count_;
	ml::ui8			flag_;
	ml::ui8			pad_8_[3];

	enum{
		Flag_is_life_infinity	= 1<<0
	};

	typedef	res::StaticChildArray<NodeHeader>	ChildArrayType_;
	BM3_DECLARE_GETTER_REF(ChildArrayType_,	ChildArray);

	NodeTypeConst	Basic_Meta_NodeType() const{	return node_type_;	}
};

}	// namespace bmb
}	// namespace fileformat
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#include "../../utility/internal/bm3_MacroEnd.h"

#endif	// BM3_SDK_INC_BM3_CORE_FILEFORMAT_BMB_NODEHEADER_H
