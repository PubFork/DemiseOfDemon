#ifndef BM3_SDK_INC_BM3_CORE_TYPE_BM3_KEYFRAME_H
#define BM3_SDK_INC_BM3_CORE_TYPE_BM3_KEYFRAME_H

#include <ml/utility/ml_assert.h>
#include "../../system/bm3_Config.h"
#include "../bm3_Const.h"
#include "../bm3_CoreType.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace internal{
template<typename T>	struct KeyframeValueElement;
template<>	struct KeyframeValueElement<float>{						enum{	Count = 1	};	};
template<>	struct KeyframeValueElement<ml::vector3d>{				enum{	Count = 3	};	};
//template<>	struct KeyframeValueElement<ml::color_rgba<float> >{	enum{	Count = 4	};	};
template<>	struct KeyframeValueElement<ml::color_rgba<float> >{	enum{	Count = 3	};	};

}	// namespace internal

/// @brief KeyframeƒNƒ‰ƒX
template<typename T>
class Keyframe{

public:
	enum{
		ValueElementCount = internal::KeyframeValueElement<T>::Count
	};

	Keyframe() :
		value_(T()){
#if defined(BM3_TARGET_IDE)
		selected_keyframe_	= false;
		for(int i=0 ; i<internal::KeyframeValueElement<T>::Count ; ++i){
			selected_keyframe_value_[i]	= false;
		}
#endif
	}

	KeyframeTypeConst	KeyframeType(int element_no) const{			ML_ASSERT((element_no >= 0) && (element_no < ValueElementCount));
																	return element_info_[element_no].keyframe_type_;										}

	float				SlopeL(int element_no) const{				ML_ASSERT((element_no >= 0) && (element_no < ValueElementCount));
																	return element_info_[element_no].slope_l_;												}

	float				SlopeR(int element_no) const{				ML_ASSERT((element_no >= 0) && (element_no < ValueElementCount));
																	return element_info_[element_no].slope_r_;												}

	const KeyframeElementInfo *	ElementInfoArray() const{			return element_info_;																	}

	const T &			Value() const{								return value_;																			}

	bool	IsBreakSlope(int element_no) const{						ML_ASSERT((element_no >= 0) && (element_no < ValueElementCount));
																	return (element_info_[element_no].keyframe_type_ == KeyframeTypeConst_BreakSpline);		}

	bool	IsSpline(int element_no) const{							ML_ASSERT((element_no >= 0) && (element_no < ValueElementCount));
																	return (element_info_[element_no].keyframe_type_ == KeyframeTypeConst_Spline     ) ||
																		   (element_info_[element_no].keyframe_type_ == KeyframeTypeConst_BreakSpline);		}

	void	SetKeyframeType(int element_no, KeyframeTypeConst t){	ML_ASSERT((element_no >= 0) && (element_no < ValueElementCount));
																	element_info_[element_no].keyframe_type_	= t;										}

	void	SetSlopeL(int element_no, float s){						ML_ASSERT((element_no >= 0) && (element_no < ValueElementCount));
																	element_info_[element_no].slope_l_			= s;
																	if(IsBreakSlope(element_no) == false){
																		element_info_[element_no].slope_r_ = s;
																	}																						}

	void	SetSlopeR(int element_no, float s){						ML_ASSERT((element_no >= 0) && (element_no < ValueElementCount));
																	element_info_[element_no].slope_r_			= s;
																	if(IsBreakSlope(element_no) == false){
																		element_info_[element_no].slope_l_ = s;
																	}																						}

	void	SetValue(const T &v){									value_				= v;																}

#if defined(BM3_TARGET_IDE)
	bool	SelectedKeyframe(){										return selected_keyframe_;		}
	void	SetSelectedKeyframe(bool value){						selected_keyframe_	= value;	}
	
	bool	SelectedKeyframeValue(int element_no){					return selected_keyframe_value_[element_no];	}
	void	SetSelectedKeyframeValue(int element_no, bool value){	selected_keyframe_value_[element_no]= value;	}
#endif

private:
	KeyframeElementInfo	element_info_[ValueElementCount];
	T					value_;
	
#if defined(BM3_TARGET_IDE)
	bool				selected_keyframe_;
	bool				selected_keyframe_value_[internal::KeyframeValueElement<T>::Count];
#endif
};

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_TYPE_BM3_KEYFRAME_H
