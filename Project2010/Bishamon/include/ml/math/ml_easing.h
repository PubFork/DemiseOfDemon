#ifndef LIBMATCHLOCK_INC_MATH_ML_EASING_H
#define LIBMATCHLOCK_INC_MATH_ML_EASING_H

#include "../utility/ml_assert.h"
#include "ml_arithmetic.h"
#include "ml_triangular.h"

namespace ml{

///	@addtogroup	math
///	@{

///	@addtogroup	math_easing
///	@{

///	@brief				IN�C�[�W���O
///	@param[in]	rate	����(0.0�`1.0)
///	@return				�Z�o���ꂽ�l
template<int Level>
inline float ease_in(float rate){

	ML_ASSERT((rate >= 0.0f) && (rate <= 1.0f));

	return expansion_pow<Level>(rate);
}

///	@brief				OUT�C�[�W���O
///	@param[in]	rate	����(0.0�`1.0)
///	@return				�Z�o���ꂽ�l
template<int Level>
inline float ease_out(float rate){

	ML_ASSERT((rate >= 0.0f) && (rate <= 1.0f));

	return 1.0f - expansion_pow<Level>(1.0f - rate);
}

///	@brief				IN/OUT�C�[�W���O
///	@param[in]	rate	����(0.0�`1.0)
///	@return				�Z�o���ꂽ�l
template<int Level>
inline float ease_in_out(float rate){

	ML_ASSERT((rate >= 0.0f) && (rate <= 1.0f));

	return
		(rate <= 0.5f) ?
			ease_in<Level>( (rate       ) * 2.0f) * 0.5f :
			ease_out<Level>((rate - 0.5f) * 2.0f) * 0.5f + 0.5f;
}

///	@brief				OUT/IN�C�[�W���O
///	@param[in]	rate	����(0.0�`1.0)
///	@return				�Z�o���ꂽ�l
template<int Level>
inline float ease_out_in(float rate){

	ML_ASSERT((rate >= 0.0f) && (rate <= 1.0f));

	return
		(rate <= 0.5f) ?
			ease_out<Level>((rate       ) * 2.0f) * 0.5f :
			ease_in<Level>( (rate - 0.5f) * 2.0f) * 0.5f + 0.5f;
}

///	@brief				IN�C�[�W���O
///	@param[in]	rate	����(0.0�`1.0)
///	@return				�Z�o���ꂽ�l
inline float ease_in_sin(float rate){

	ML_ASSERT((rate >= 0.0f) && (rate <= 1.0f));

	return 1.0f - cosf(rate * 3.14159265358979323846f * 0.5f);
}

///	@brief				IN�C�[�W���O
///	@param[in]	rate	����(0.0�`1.0)
///	@return				�Z�o���ꂽ�l
inline float ease_out_sin(float rate){

	ML_ASSERT((rate >= 0.0f) && (rate <= 1.0f));

	return sinf(rate * 3.14159265358979323846f * 0.5f);
}

///	@brief				IN/OUT�C�[�W���O
///	@param[in]	rate	����(0.0�`1.0)
///	@return				�Z�o���ꂽ�l
inline float ease_in_out_sin(float rate){

	ML_ASSERT((rate >= 0.0f) && (rate <= 1.0f));

	return
		(rate <= 0.5f) ?
			ease_in_sin( (rate       ) * 2.0f) * 0.5f :
			ease_out_sin((rate - 0.5f) * 2.0f) * 0.5f + 0.5f;
}

///	@brief				OUT/IN�C�[�W���O
///	@param[in]	rate	����(0.0�`1.0)
///	@return				�Z�o���ꂽ�l
inline float ease_out_in_sin(float rate){

	ML_ASSERT((rate >= 0.0f) && (rate <= 1.0f));

	return
		(rate <= 0.5f) ?
			ease_out_sin((rate       ) * 2.0f) * 0.5f :
			ease_in_sin( (rate - 0.5f) * 2.0f) * 0.5f + 0.5f;
}

///	@brief				IN�C�[�W���O
///	@param[in]	rate	����(0.0�`1.0)
///	@return				�Z�o���ꂽ�l
inline float ease_in_circle(float rate){

	ML_ASSERT((rate >= 0.0f) && (rate <= 1.0f));

	return 1.0f - sqrtf(1.0f - rate * rate);
}

///	@brief				IN�C�[�W���O
///	@param[in]	rate	����(0.0�`1.0)
///	@return				�Z�o���ꂽ�l
inline float ease_out_circle(float rate){

	ML_ASSERT((rate >= 0.0f) && (rate <= 1.0f));

	rate = 1.0f - rate;

	return sqrtf(1.0f - rate * rate);
}

///	@brief				IN/OUT�C�[�W���O
///	@param[in]	rate	����(0.0�`1.0)
///	@return				�Z�o���ꂽ�l
inline float ease_in_out_circle(float rate){

	ML_ASSERT((rate >= 0.0f) && (rate <= 1.0f));

	return
		(rate <= 0.5f) ?
			ease_in_circle( (rate       ) * 2.0f) * 0.5f :
			ease_out_circle((rate - 0.5f) * 2.0f) * 0.5f + 0.5f;
}

///	@brief				OUT/IN�C�[�W���O
///	@param[in]	rate	����(0.0�`1.0)
///	@return				�Z�o���ꂽ�l
inline float ease_out_in_circle(float rate){

	ML_ASSERT((rate >= 0.0f) && (rate <= 1.0f));

	return
		(rate <= 0.5f) ?
			ease_out_circle((rate       ) * 2.0f) * 0.5f :
			ease_in_circle( (rate - 0.5f) * 2.0f) * 0.5f + 0.5f;
}


namespace fast{

///	@brief				IN�C�[�W���O
///	@param[in]	rate	����(0.0�`1.0)
///	@return				�Z�o���ꂽ�l
template<int Level>
inline float ease_in(float rate){

	ML_ASSERT((rate >= 0.0f) && (rate <= 1.0f));

	return ml::ease_in<Level>(rate);
}

///	@brief				OUT�C�[�W���O
///	@param[in]	rate	����(0.0�`1.0)
///	@return				�Z�o���ꂽ�l
template<int Level>
inline float ease_out(float rate){

	ML_ASSERT((rate >= 0.0f) && (rate <= 1.0f));

	return ml::ease_out<Level>(rate);
}

///	@brief				IN/OUT�C�[�W���O
///	@param[in]	rate	����(0.0�`1.0)
///	@return				�Z�o���ꂽ�l
template<int Level>
inline float ease_in_out(float rate){

	ML_ASSERT((rate >= 0.0f) && (rate <= 1.0f));

	return ml::ease_in_out<Level>(rate);
}

///	@brief				OUT/IN�C�[�W���O
///	@param[in]	rate	����(0.0�`1.0)
///	@return				�Z�o���ꂽ�l
template<int Level>
inline float ease_out_in(float rate){

	ML_ASSERT((rate >= 0.0f) && (rate <= 1.0f));

	return ml::ease_out_in<Level>(rate);
}

///	@brief				IN�C�[�W���O
///	@param[in]	rate	����(0.0�`1.0)
///	@return				�Z�o���ꂽ�l
inline float ease_in_sin(float rate){

	ML_ASSERT((rate >= 0.0f) && (rate <= 1.0f));

	return 1.0f - fast::cosf(rate * 3.14159265358979323846f * 0.5f);
}

///	@brief				IN�C�[�W���O
///	@param[in]	rate	����(0.0�`1.0)
///	@return				�Z�o���ꂽ�l
inline float ease_out_sin(float rate){

	ML_ASSERT((rate >= 0.0f) && (rate <= 1.0f));

	return fast::sinf(rate * 3.14159265358979323846f * 0.5f);
}

///	@brief				IN/OUT�C�[�W���O
///	@param[in]	rate	����(0.0�`1.0)
///	@return				�Z�o���ꂽ�l
inline float ease_in_out_sin(float rate){

	ML_ASSERT((rate >= 0.0f) && (rate <= 1.0f));

	return
		(rate <= 0.5f) ?
			fast::ease_in_sin( (rate       ) * 2.0f) * 0.5f :
			fast::ease_out_sin((rate - 0.5f) * 2.0f) * 0.5f + 0.5f;
}

///	@brief				OUT/IN�C�[�W���O
///	@param[in]	rate	����(0.0�`1.0)
///	@return				�Z�o���ꂽ�l
inline float ease_out_in_sin(float rate){

	ML_ASSERT((rate >= 0.0f) && (rate <= 1.0f));

	return
		(rate <= 0.5f) ?
			fast::ease_out_sin((rate       ) * 2.0f) * 0.5f :
			fast::ease_in_sin( (rate - 0.5f) * 2.0f) * 0.5f + 0.5f;
}

///	@brief				IN�C�[�W���O
///	@param[in]	rate	����(0.0�`1.0)
///	@return				�Z�o���ꂽ�l
inline float ease_in_circle(float rate){

	ML_ASSERT((rate >= 0.0f) && (rate <= 1.0f));

	return ml::ease_in_circle(rate);
}

///	@brief				IN�C�[�W���O
///	@param[in]	rate	����(0.0�`1.0)
///	@return				�Z�o���ꂽ�l
inline float ease_out_circle(float rate){

	ML_ASSERT((rate >= 0.0f) && (rate <= 1.0f));

	return ml::ease_out_circle(rate);
}

///	@brief				IN/OUT�C�[�W���O
///	@param[in]	rate	����(0.0�`1.0)
///	@return				�Z�o���ꂽ�l
inline float ease_in_out_circle(float rate){

	ML_ASSERT((rate >= 0.0f) && (rate <= 1.0f));

	return ml::ease_in_out_circle(rate);
}

///	@brief				OUT/IN�C�[�W���O
///	@param[in]	rate	����(0.0�`1.0)
///	@return				�Z�o���ꂽ�l
inline float ease_out_in_circle(float rate){

	ML_ASSERT((rate >= 0.0f) && (rate <= 1.0f));

	return ml::ease_out_in_circle(rate);
}

}	// namespace fast

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_MATH_ML_EASING_H
