#ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_PRIM_PRIMITIVE_PACKET_H
#define BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_PRIM_PRIMITIVE_PACKET_H

#include <ml/type/ml_basic.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace prim{

/// @brief Packet構造体
struct Packet{
/*
	ml::si32			count_;			// プリミティブ数
	ml::si32			byte_size_;		// プリミティブ一つあたりのバイトサイズ
	ml::si32			active_count_;	// アクティブなプリミティブの数
	ml::ui32			updated_frame_;	// 更新を行ったフレーム数
*/
	ml::ni::matrix44	generate_time_hierarchy_parent_matrix_;	// 生成時の親の行列
	ml::ni::vector3d	generate_time_hierarchy_parent_matrix_row_length_;	// 生成時の親の行列

	// 以下、可変個数

	// アクティブなプリミティブ列
	//	active_primitive[0] から active_primitive[active_count_ - 1] までが有効
//	PrimitiveType	*active_primitive[count_];

	// プリミティブ列
//	PrimitiveType	primitive_[count_];
};

}	// namespace prim
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_PRIM_PRIMITIVE_PACKET_H
