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

/// @brief	�X�P�[���`��^�C�v
struct DrawScale{
	enum Type{
		Nothing,		///< �������Ȃ�
		X,				///< X
		XY,				///< XY
		XYZ,			///< XYZ
		//
		MAX
	};
};

/// @brief	��]�`��^�C�v
struct DrawRotate{
	enum Type{
		Nothing,				///< �������Ȃ�
//		Billboard,				///< �r���{�[�h				<== ���̂Ƃ��떢���������ǕK�v�ɂȂ�͂�
		Billboard_Z,			///< �r���{�[�h��Z����]
		Polyline,				///< �|�����C��
		XYZ,					///< XYZ
		Directional,			///< �f�B���N�V���i��
		DirectionalPolyline,	///< �f�B���N�V���i���|�����C��
		Facing,					///< �t�F�C�V���O
		//
		MAX
	};
};

/// @brief	�F�`��^�C�v
struct DrawColor{
	enum Type{
		Nothing,			///< �������Ȃ�
		_1,				///< �P�F
		_2,				///< �Q�F
		_4,				///< �S�F
		//
		MAX
	};
};

/// @brief	�e�N�X�`���`��^�C�v
struct DrawTexture{
	enum Type{
		Nothing,			///< �e�N�X�`���Ȃ�
		Normal,			///< �ӂ��i�Œ�j
		Crop,
		Scroll,
		Pattern,
		Transform,
		//
		MAX
	};
};

/// @brief	�`��X�e�[�g�^�C�v
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

// �e�N�X�`���[���C���[
struct TextureLayer{
	enum Type{
		_1	= 0,
		_2,
//		_3,						///< �\��n
//		_4,						///< �\��n
		//
		Max
	};
};

/// @brief	�ϊ��s��
struct TransformMatrix{
	enum Type{
		World,
		//
		Max
	};
};

/// @brief	�e�N�X�`���J�e�S��
struct TextureCategory{
	enum Type{
		Effect,
		Model
	};
};

/// @brief	�������[�J�e�S��
struct MemoryCategory{
	enum Type{
		General,
		Shader
	};
};

// �e�N�X�`���R���e�i
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
		FixColor	= 1 <<	1,	///< �J���[���P�F�̂݁i�����_���A�A�j���Ȃ��j
		FixRotate	= 1 <<	2,	///< ��]���Œ�i�����_���A�A�j���Ȃ��j
		FixQuad		= 1 <<	3,	///< 4���_�����m�ɋ�`�i�����_���A�A�j���Ȃ��j
		FixQuadPivot= 1 <<	4	///< �s�{�b�g���L�邩������
	};
};
	

enum{
	DummyFill	= 0x19770322
};

/// @brief	�F
typedef	ml::ni::color_rgba<float>	ColorType;

/// @brief	�e�N�X�`�����W
struct TextureCoordinate{
	enum{
		element_size	= 2
	};

	float	u_;
	float	v_;
};

/// @brief	�e�N�X�`���[�N���b�v
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

/// @brief	�e�N�X�`���[�X�N���[��
struct TextureScroll{
	TextureScroll() :
		v_(0.0f),
		h_(0.0f){;}

	float	v_;
	float	h_;
};

/// @brief	�e�N�X�`���[�p�^�[��
struct TexturePattern{
	TexturePattern() :
		frame_count_(0){;}

	TexturePattern(int frame_count, float left, float top, float width, float height) :
		frame_count_(frame_count),
		crop_(left, top, width, height){;}

	int			frame_count_;
	TextureCrop	crop_;
};

/// @brief	���[�U�[�f�[�^�v�f
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


/// @brief	�L�[�t���[���v�f���
struct	KeyframeElementInfo{
	KeyframeElementInfo() :
		keyframe_type_(KeyframeTypeConst_Linear),
		slope_l_(0.0f),
		slope_r_(0.0f){;}

	KeyframeTypeConst	keyframe_type_;
	float				slope_l_;
	float				slope_r_;
};

/// @brief	�v���~�e�B�u���������
struct PrimitiveInitializeInfo{

	ml::ui32	random_seed_;
};

/// @brief	�v���~�e�B�u�X�V���
struct PrimitiveUpdateInfo{

	float		life_rate_;
	ml::ui32	random_seed_;
};

/// @brief	�v���~�e�B�u�������
struct PrimitivePrepareInfo{

	float		life_rate_;
	ml::ui32	random_seed_;
};

/// @brief	�v���~�e�B�u�`����
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
