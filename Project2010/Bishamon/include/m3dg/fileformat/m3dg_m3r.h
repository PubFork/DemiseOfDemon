#ifndef M3DG_FILEFORMAT_M3DG_M3R_H
#define M3DG_FILEFORMAT_M3DG_M3R_H

#include <ml/utility/ml_make_sign.h>
#include <ml/type/ml_basic.h>

namespace m3dg{
namespace fileformat{
namespace m3r{

struct Header{
	enum{
		//
		Sign						= ML_MAKE_SIGN_32('M','3','R','B'),
		ReverseSign					= ML_MAKE_SIGN_R32('M','3','R','B'),
		//
		Version						= 0x0000000D,		// 0x0000000D:PS3の法線方向を逆にした
														// 0x0000000C:文字コード変換機能追加、座標系変換機能追加
														// 0x0000000B:三角形面積情報を追加
		ReverseVersion				= 0x0D000000,
		//
		DataFlag_is_little_endian	= 1<<0,
		//
		RuntimeFlag_is_initialized	= 1<<0
	};

	// +0x00
	ml::ui32	sign_;
	ml::ui32	version_;
	ml::ui8		data_flag_;
	ml::ui8		runtime_flag_;
	ml::ui8		pad_00_[2];
	ml::ui32	bytecode_offset_;

	// +0x10
	ml::ui32	texture_count_;
	ml::ui32	texture_name_offset_;
	ml::ui32	binary_offset_;
	ml::ui32	string_offset_;

	// +0x20
	ml::ui32	camera_offset_;
	ml::ui32	camera_count_;
	ml::ui32	curve_offset_;
	ml::ui32	curve_count_;

	// +0x30
	ml::ui32	animation_offset_;
	ml::ui32	animation_count_;
	ml::ui32	node_offset_;
	ml::ui32	node_count_;

	// +0x40
	ml::ui32	controller_offset_;
	ml::ui32	controller_count_;
	ml::ui8		pad_01_[8];

};

ML_STATIC_ASSERT((sizeof(Header) & 15) == 0);

}	// namespace m3r
}	// namespace fileformat
}	// namespace m3dg

#endif	// #ifndef M3DG_FILEFORMAT_M3DG_M3R_H
