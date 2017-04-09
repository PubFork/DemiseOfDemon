#ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_PRIM_PRIMITIVE_COMMON_H
#define BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_PRIM_PRIMITIVE_COMMON_H

#include "../bm3_CoreType.h"
#include "../bm3_Const.h"
#include "../object/bm3_obj_NodeBase.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

//class obj::NodeBase;	

namespace prim{

/// @brief MatrixParamクラス
struct MatrixParam{
	ml::ni::vector3d	value_;
	ml::ni::vector3d	speed_;
	ml::ni::vector3d	acceleration_;

	void Initialize0(){
		value_.set_unit();
		speed_.set_unit();
		acceleration_.set_unit();
	}

	BM3_FORCEINLINE float	ValueX() const{	return value_.e_.x_;	}
	BM3_FORCEINLINE float	ValueY() const{	return value_.e_.y_;	}
	BM3_FORCEINLINE float	ValueZ() const{	return value_.e_.z_;	}
};

struct ScalarParam{
	float				value_;
	float				speed_;
	float				acceleration_;

	void Initialize0(){
		value_			= 0;
		speed_			= 0;
		acceleration_	= 0;
	}

	BM3_FORCEINLINE float	ValueX() const{	return value_;	}
	BM3_FORCEINLINE float	ValueY() const{	return value_;	}
	BM3_FORCEINLINE float	ValueZ() const{	return value_;	}
};

struct ColorParam{
	ColorType			value_;
};

/// @brief	テクスチャアニメーション処理の構造体
struct TextureParam{
	int					frame_;		//!< クロップのＩＤ
	float				step_;		//!< クロップ切り替えＩＤ
	float				h_scroll_;	//!< 水平方向のスクロール座標
	float				v_scroll_;	//!< 垂直方向のスクロール座標
	TextureCrop			value_;		//!< クロップの領域
};

/// @brief ModelTextureParam構造体
struct ModelTextureParam{
	float				h_scroll_;
	float				v_scroll_;
};

/// @brief TransformParam構造体
struct TransformParam{
	ml::ni::vector3d	left_top_;
	ml::ni::vector3d	right_top_;
	ml::ni::vector3d	left_bottom_;
	ml::ni::vector3d	right_bottom_;
};

/// @brief GenerateShape構造体
struct GenerateShape{
	ml::ni::vector3d	position_;
	ml::ni::vector3d	direction_;
};

/// @brief ParentParamクラス
struct ParentParam{
public:
	void	SetBase(obj::NodeBase *base){	base_			= base;		}

	obj::NodeBase		&Base()			const { return *base_;		}
	ColorType&	ParentColor()	const { return base_->BlendColor();	}

private:
	obj::NodeBase*		base_;
};

}	// namespace prim


BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_PRIM_PRIMITIVE_COMMON_H
