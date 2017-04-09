#ifndef BM3_SDK_INC_BM3_CORE_BM3_CORETYPE_h
#define BM3_SDK_INC_BM3_CORE_BM3_CORETYPE_h

#if defined(BM3_PLATFORM_nw4c)
#pragma diag_suppress 1301
#endif

#include <ml/type/ml_color.h>
#include <ml/type/ml_basic.h>
#include <ml/type/ml_vector3d.h>
#include "bm3_Const.h"
#include "../utility/bm3_String.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

/// @brief	スケール描画タイプ
struct DrawScale{
	enum Type{
		Nothing,		///< 何もしない
		X,				///< X
		XY,				///< XY
		XYZ,			///< XYZ
		//
		MAX
	};
};

/// @brief	回転描画タイプ
struct DrawRotate{
	enum Type{
		Nothing,				///< 何もしない
//		Billboard,				///< ビルボード				<== 今のところ未実装だけど必要になるはず
		Billboard_Z,			///< ビルボードとZ軸回転
		Polyline,				///< ポリライン
		XYZ,					///< XYZ
		Directional,			///< ディレクショナル
		DirectionalPolyline,	///< ディレクショナルポリライン
		Facing,					///< フェイシング
		//
		MAX
	};
};

/// @brief	色描画タイプ
struct DrawColor{
	enum Type{
		Nothing,			///< 何もしない
		_1,				///< １色
		_2,				///< ２色
		_4,				///< ４色
		//
		MAX
	};
};

/// @brief	テクスチャ描画タイプ
struct DrawTexture{
	enum Type{
		Nothing,			///< テクスチャなし
		Normal,			///< ふつう（固定）
		Crop,
		Scroll,
		Pattern,
		Transform,
		//
		MAX
	};
};

/// @brief	描画ステートタイプ
struct RenderState{
	enum Type{
		Culling,
		DepthTest,
		DepthWrite,
		AlphaTest,
		//
		MAX
	};
};

// テクスチャーレイヤー
struct TextureLayer{
	enum Type{
		_1	= 0,
		_2,
//		_3,						///< 予定地
//		_4,						///< 予定地
		//
		Max
	};
};

/// @brief	変換行列
struct TransformMatrix{
	enum Type{
		World,
		//
		Max
	};
};

/// @brief	テクスチャカテゴリ
struct TextureCategory{
	enum Type{
		Effect,
		Model
	};
};

/// @brief	メモリーカテゴリ
struct MemoryCategory{
	enum Type{
		General,
		Shader
	};
};

// テクスチャコンテナ
struct TextureContainer{
	enum Type{
		File,
		Movie
	};
};

struct OptimizeBit{
	enum Type{
		None		= 0,
		NoField		= 1 <<	0,
		FixColor	= 1 <<	1,	///< カラーが単色のみ（ランダム、アニメなし）
		FixRotate	= 1 <<	2,	///< 回転が固定（ランダム、アニメなし）
		FixQuad		= 1 <<	3,	///< 4頂点が正確に矩形（ランダム、アニメなし）
		FixQuadPivot= 1 <<	4	///< ピボットが有るか無いか
	};
};
	

enum{
	DummyFill	= 0x19770322
};

/// @brief	色
typedef	ml::ni::color_rgba<float>	ColorType;

/// @brief	テクスチャ座標
struct TextureCoordinate{
	enum{
		element_size	= 2
	};

	float	u_;
	float	v_;
};

/// @brief	テクスチャークロップ
struct TextureCrop{
	TextureCrop() :
		left_(0.0f),
		top_(0.0f),
		width_(1.0f),
		height_(1.0f){;}

	TextureCrop(float left, float top, float width, float height) :
		left_(left),
		top_(top),
		width_(width),
		height_(height){;}

	float	left_;
	float	top_;
	float	width_;
	float	height_;
};

/// @brief	テクスチャースクロール
struct TextureScroll{
	TextureScroll() :
		v_(0.0f),
		h_(0.0f){;}

	float	v_;
	float	h_;
};

/// @brief	テクスチャーパターン
struct TexturePattern{
	TexturePattern() :
		frame_count_(0){;}

	TexturePattern(int frame_count, float left, float top, float width, float height) :
		frame_count_(frame_count),
		crop_(left, top, width, height){;}

	int			frame_count_;
	TextureCrop	crop_;
};

/// @brief	ユーザーデータ要素
namespace embedded{
struct UserDataElement{
#if 0
	UserDataElement() :
		type_(UserDataElementTypeConst_Int),
		data_(0){;}

	UserDataElement(const StringType &name, UserDataElementTypeConst type) :
		name_(name),
		type_(type){
		;
	}
#endif

	StaticString<>				name_;
	UserDataElementTypeConst	type_;

	union{
		ml::ui32		data_;
		//
		int					int_value_;
		float				float_value_;
		const wchar_t		*string_value_;
		const ml::vector3d	*vector_value_;
		const ColorType		*color_value_;
		bool				bool_value_;
	};

	const StaticString<> &		Name() const{			return name_;	}
	UserDataElementTypeConst	Type() const{			return type_;	}

	int						IntValue() const{		ML_ASSERT(type_ == UserDataElementTypeConst_Int);		return int_value_;		}
	float					FloatValue() const{		ML_ASSERT(type_ == UserDataElementTypeConst_Float);		return float_value_;	}
	const wchar_t *			StringValue() const{	ML_ASSERT(type_ == UserDataElementTypeConst_String);	return string_value_;	}
	const ml::vector3d &	VectorValue() const{	ML_ASSERT(type_ == UserDataElementTypeConst_Vector);	return *vector_value_;	}
	const ColorType &		ColorValue() const{		ML_ASSERT(type_ == UserDataElementTypeConst_Color);		return *color_value_;	}
	bool					BoolValue() const{		ML_ASSERT(type_ == UserDataElementTypeConst_Bool);		return bool_value_;		}

	void	Mapping(const void *binary_start, const void *string_start){

		name_.Mapping(string_start);

		switch(type_){
			case UserDataElementTypeConst_String:{	string_value_	= reinterpret_cast<const wchar_t *>(     ml::ptr_size_ui(string_start) + data_);	break;	}
			case UserDataElementTypeConst_Vector:{	vector_value_	= reinterpret_cast<const ml::vector3d *>(ml::ptr_size_ui(binary_start) + data_);	break;	}
			case UserDataElementTypeConst_Color:{	color_value_	= reinterpret_cast<const ColorType *>(   ml::ptr_size_ui(binary_start) + data_);	break;	}
			case UserDataElementTypeConst_Bool:{	bool_value_		= data_ != 0;															break;	}
			default:{																														break;	}
		}
	}
};
}	// namespace embedded

#if defined(BM3_TARGET_IDE)
namespace ide{
struct UserDataElement{
	UserDataElement() :
		type_(UserDataElementTypeConst_Int),
		int_value_(0),
		float_value_(0.0f),
		bool_value_(false){;}

	UserDataElement(const StringType &name, UserDataElementTypeConst type) :
		name_(name),
		type_(type),
		int_value_(0),
		float_value_(0.0f),
		bool_value_(false){;}

	StringType					name_;
	UserDataElementTypeConst	type_;

	int				int_value_;
	float			float_value_;
	StringType 		string_value_;
	ml::vector3d	vector_value_;
	ColorType		color_value_;
	bool			bool_value_;

	int						IntValue() const{		return int_value_;		}
	float					FloatValue() const{		return float_value_;	}
	const StringType &		StringValue() const{	return string_value_;	}
	const ml::vector3d &	VectorValue() const{	return vector_value_;	}
	const ColorType &		ColorValue() const{		return color_value_;	}
	bool					BoolValue() const{		return bool_value_;		}
};
}	// namespace ide
#endif


/// @brief	キーフレーム要素情報
struct	KeyframeElementInfo{
	KeyframeElementInfo() :
		keyframe_type_(KeyframeTypeConst_Linear),
		slope_l_(0.0f),
		slope_r_(0.0f){;}

	KeyframeTypeConst	keyframe_type_;
	float				slope_l_;
	float				slope_r_;
};

/// @brief	プリミティブ初期化情報
struct PrimitiveInitializeInfo{

	ml::ui32	random_seed_;
};

/// @brief	プリミティブ更新情報
struct PrimitiveUpdateInfo{

	float		life_rate_;
	ml::ui32	random_seed_;
};

/// @brief	プリミティブ準備情報
struct PrimitivePrepareInfo{

	float		life_rate_;
	ml::ui32	random_seed_;
};

/// @brief	プリミティブ描画情報
struct PrimitiveDrawInfo{

	float		life_rate_;
	ml::ui32	random_seed_;
};

struct PacketDrawInfo{
	int					all_active_child_count_;
	int					index_;
	ColorType			color_scale_;
	ml::vector3d		offset_position_;
	ml::vector3d		facing_position_;
	ColorType*			parent_color_;
};

struct ModelDrawInfo{
	float					frame_;
	float					life_frame_;	// life frame
	ml::color_rgba<float>	diffuse_color_;
};

struct UpdateFixPoint{
	typedef	int		Type;

	static Type		IntToFixPoint(int v){		return v << FixPointShift;												}
	static Type		FloatToFixPoint(float v){	return ml::numeric_cast<int>(v * float(1 << FixPointShift));			}

	static int		FixPointToInt(Type v){		return v >> FixPointShift;												}
	static float	FixPointToFloat(Type v){	return ml::numeric_cast<float>(v) * (1.0f / float(1 << FixPointShift));	}

private:
	enum{	FixPointShift	= 16	};
};

/// @brief	Whole Z Sort 1 prim info
struct PrimZInfo{
	int		emitter_index_;		// quad, model or ...
	int		draw_count_;
	void*	vertex_buffer_;
#ifdef BM3_PLATFORM_psvita
	void*	index_buffer_;
#endif
	float	z_value_;
};

/// @brief	Whole Z Sort prim list 
struct PrimZList{
	int			count_;
	int			max_count_;
	PrimZInfo	*info_list_;

	PrimZList() :
		count_(0),
		max_count_(0),
		info_list_(NULL){
	}

	void Initialize(int count, void* buffer){
		count_		= 0;
		max_count_	= count;
		info_list_	= reinterpret_cast<PrimZInfo*>(buffer);
	}
	void Reset(){	count_	= 0;	}
	PrimZInfo &Push(){	ML_ASSERT(count_<max_count_);	return info_list_[count_++];	}

	PrimZInfo	*Begin(){	return &info_list_[0];	}
	PrimZInfo	*End(){	return &info_list_[count_];	}
	void*		Buffer(){	return &info_list_[0];	}

	int Size(int count){	return count * sizeof(PrimZInfo);	}
	int Count()	const {	return count_;	}
};

struct PrimZPList{
	int			count_;
	int			max_count_;
	PrimZInfo	**info_list_;

	PrimZPList() :
		count_(0),
		max_count_(0),
		info_list_(NULL){
	}

	void Initialize(int count, void* buffer){
		count_		= 0;
		max_count_	= count;
		info_list_	= reinterpret_cast<PrimZInfo**>(buffer);
	}
	void Reset(){	count_	= 0;	}
	void Add(PrimZInfo* p){	ML_ASSERT(count_<max_count_);	info_list_[count_++] = p;	}

	PrimZInfo**	Begin(){	return &info_list_[0];	}
	PrimZInfo**	End(){		return &info_list_[count_];	}
	void*		Buffer(){	return info_list_;	}

	int Count()	const {	return count_;	}
	int Size(int count){	return count * sizeof(PrimZInfo*);	}
};

/// @brief	Whole Z Sort type info
struct WholeZInfo{
	bool		is_enabled_;
	void*		vertex_head_;
	int			prim_vtx_size_;
	int			emitter_index_;
#ifdef BM3_PLATFORM_psvita
	void*		index_head_;
	int			prim_index_size_;
#endif
};

// 
struct ResEmitterInfo{
	float				priority_;
	EmitterTypeConst	type_;
	const void*			resource_;
};
	

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// BM3_SDK_INC_BM3_CORE_BM3_CORETYPE_h
