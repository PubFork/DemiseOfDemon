#ifndef LIBMATCHLOCK_INC_ML_TYPE_ML_COLOR_H
#define LIBMATCHLOCK_INC_ML_TYPE_ML_COLOR_H

#include "../detail/ml_color_detail.h"
#include "../internal/ml_type_internal.h"
#include "../../math/ml_arithmetic.h"
#include "../../math/ml_random.h"

namespace ml{

///	@addtogroup	type
///	@{

///	@addtogroup	type_color
///	@{

///	@brief	�F�N���X(RGB)
template<typename T>
class color_rgb : public detail::color_rgb<T>{

public:
	///	@brief			�R���X�g���N�^
	color_rgb(){
		set(T(), T(), T());
	}

	///	@brief			�R���X�g���N�^
	///	@param[in]	r	��
	///	@param[in]	g	��
	///	@param[in]	b	��
	color_rgb(T r, T g, T b){
		set(r, g, b);
	}

//	///	@brief			�R���X�g���N�^
//	///	@param[in]	v	�F
//	color_rgb(const detail::color_rgb<T> &v){
//		static_cast<detail::color_rgb &>(*this) = v;
//	}

	///	@cond internal
	explicit color_rgb(internal::not_initialize){;}
	///	@endcond

	///	@brief			�e�v�f��ݒ肷��
	///	@param[in]	r	��
	///	@param[in]	g	��
	///	@param[in]	b	��
	///	@return			*this
	color_rgb &	set(T r, T g, T b){
		detail::color_rgb<T>::e_.r_ = r;
		detail::color_rgb<T>::e_.g_ = g;
		detail::color_rgb<T>::e_.b_ = b;
		return *this;
	}

	///	@brief			�e�v�f���w�肵���l�Ŋۂ߂��񂾒l��ݒ肷��
	///	@param[in]	min	�ŏ��l
	///	@param[in]	max	�ő�l
	///	@return			*this
	color_rgb &	clamp(T min, T max){
		detail::color_rgb<T>::e_.r_ = ml::clamp(detail::color_rgb<T>::e_.r_, min, max);
		detail::color_rgb<T>::e_.g_ = ml::clamp(detail::color_rgb<T>::e_.g_, min, max);
		detail::color_rgb<T>::e_.b_ = ml::clamp(detail::color_rgb<T>::e_.b_, min, max);
		return *this;
	}

	///	@brief				�e�v�f�������_���ݒ肷��
	///	@param[in]	r		�����_��
	///	@param[in]	base	�x�[�X
	///	@param[in]	range	�����_����
	///	@return				*this
	color_rgb &	random_range_rgb(ml::random &r, const color_rgb &base, const color_rgb &range){
		detail::color_rgb<T>::e_.r_ = r.f32_range(base.e_.r_, range.e_.r_);
		detail::color_rgb<T>::e_.g_ = r.f32_range(base.e_.g_, range.e_.g_);
		detail::color_rgb<T>::e_.b_ = r.f32_range(base.e_.b_, range.e_.b_);

		return *this;
	}
};

///	@brief	�F�N���X(ARGB)
template<typename T>
class color_argb : public detail::color_argb<T>{

public:
	///	@brief			�R���X�g���N�^
	color_argb(){
		set(T(), T(), T(), T());
	}

	///	@brief			�R���X�g���N�^
	///	@param[in]	a	�A���t�@
	///	@param[in]	r	��
	///	@param[in]	g	��
	///	@param[in]	b	��
	color_argb(T a, T r, T g, T b){
		set(a, r, g, b);
	}

//	///	@brief			�R���X�g���N�^
//	///	@param[in]	v	�F
//	color_argb(const detail::color_argb<T> &v){
//		static_cast<detail::color_argb &>(*this) = v;
//	}

	///	@cond internal
	explicit color_argb(internal::not_initialize){;}
	///	@endcond

	///	@brief			�e�v�f��ݒ肷��
	///	@param[in]	a	�A���t�@
	///	@param[in]	r	��
	///	@param[in]	g	��
	///	@param[in]	b	��
	///	@return			*this
	color_argb &	set(T a, T r, T g, T b){
		detail::color_argb<T>::e_.a_ = a;
		detail::color_argb<T>::e_.r_ = r;
		detail::color_argb<T>::e_.g_ = g;
		detail::color_argb<T>::e_.b_ = b;
		return *this;
	}

	///	@brief			�e�v�f���w�肵���l�Ŋۂ߂��񂾒l��ݒ肷��
	///	@param[in]	min	�ŏ��l
	///	@param[in]	max	�ő�l
	///	@return			*this
	color_argb &	clamp(T min, T max){
		detail::color_argb<T>::e_.a_ = ml::clamp(detail::color_argb<T>::e_.a_, min, max);
		detail::color_argb<T>::e_.r_ = ml::clamp(detail::color_argb<T>::e_.r_, min, max);
		detail::color_argb<T>::e_.g_ = ml::clamp(detail::color_argb<T>::e_.g_, min, max);
		detail::color_argb<T>::e_.b_ = ml::clamp(detail::color_argb<T>::e_.b_, min, max);
		return *this;
	}

	///	@brief				�e�v�f�������_���ݒ肷��
	///	@param[in]	r		�����_��
	///	@param[in]	base	�x�[�X
	///	@param[in]	range	�����_����
	///	@return				*this
	color_argb &	random_range_rgb(ml::random &r, const color_argb &base, const color_argb &range){
		detail::color_argb<T>::e_.r_ = r.f32_range(base.e_.r_, range.e_.r_);
		detail::color_argb<T>::e_.g_ = r.f32_range(base.e_.g_, range.e_.g_);
		detail::color_argb<T>::e_.b_ = r.f32_range(base.e_.b_, range.e_.b_);
		detail::color_argb<T>::e_.a_ = 1.0f;

		return *this;
	}
};


///	@brief	�F�N���X(RGBA)
template<typename T>
class color_rgba : public detail::color_rgba<T>{

public:
	///	@brief			�R���X�g���N�^
	color_rgba(){
		set(T(), T(), T(), T());
	}

	///	@brief			�R���X�g���N�^
	///	@param[in]	a	�A���t�@
	///	@param[in]	r	��
	///	@param[in]	g	��
	///	@param[in]	b	��
	color_rgba(T r, T g, T b, T a){
		set(r, g, b, a);
	}

//	///	@brief			�R���X�g���N�^
//	///	@param[in]	v	�F
//	color_rgba(const detail::color_rgba<T> &v){
//		static_cast<detail::color_rgba &>(*this) = v;
//	}

	///	@cond internal
	explicit color_rgba(internal::not_initialize){;}
	///	@endcond

	///	@brief			�e�v�f��ݒ肷��
	///	@param[in]	a	�A���t�@
	///	@param[in]	r	��
	///	@param[in]	g	��
	///	@param[in]	b	��
	///	@return			*this
	color_rgba &	set(T r, T g, T b, T a){
		detail::color_rgba<T>::e_.r_ = r;
		detail::color_rgba<T>::e_.g_ = g;
		detail::color_rgba<T>::e_.b_ = b;
		detail::color_rgba<T>::e_.a_ = a;
		return *this;
	}

	///	@brief			�e�v�f���|�����킹��
	///	@param[in]	a	�A���t�@
	///	@param[in]	r	��
	///	@param[in]	g	��
	///	@param[in]	b	��
	///	@return			*this
	color_rgba &	mul(T r, T g, T b, T a){
		detail::color_rgba<T>::e_.r_ *= r;
		detail::color_rgba<T>::e_.g_ *= g;
		detail::color_rgba<T>::e_.b_ *= b;
		detail::color_rgba<T>::e_.a_ *= a;
		return *this;
	}

	///	@brief			�e�v�f���|�����킹��
	///	@param[in]	c	�J���[
	///	@return			*this
	color_rgba &	mul(color_rgba &c){
		return this->mul(c.e_.r_, c.e_.g_, c.e_.b_, c.e_.a_);
	}

	///	@brief			�e�v�f�Ɋ|�����킹��
	///	@param[in]	c	�J���[
	///	@return			*this
	color_rgba &	mul(T &c){
		return this->mul(c, c, c, c);
	}

	///	@brief			�e�v�f���w�肵���l�Ŋۂ߂��񂾒l��ݒ肷��
	///	@param[in]	min	�ŏ��l
	///	@param[in]	max	�ő�l
	///	@return			*this
	color_rgba &	clamp(T min, T max){
		detail::color_rgba<T>::e_.r_ = ml::clamp(detail::color_rgba<T>::e_.r_, min, max);
		detail::color_rgba<T>::e_.g_ = ml::clamp(detail::color_rgba<T>::e_.g_, min, max);
		detail::color_rgba<T>::e_.b_ = ml::clamp(detail::color_rgba<T>::e_.b_, min, max);
		detail::color_rgba<T>::e_.a_ = ml::clamp(detail::color_rgba<T>::e_.a_, min, max);
		return *this;
	}

	///	@brief				�e�v�f�������_���ݒ肷��
	///	@param[in]	r		�����_��
	///	@param[in]	base	�x�[�X
	///	@param[in]	range	�����_����
	///	@return				*this
	color_rgba &	random_range_rgb(ml::random &r, const color_rgba &base, const color_rgba &range){

		detail::color_rgba<T>::e_.r_ = r.f32_range(base.e_.r_, range.e_.r_);
		detail::color_rgba<T>::e_.g_ = r.f32_range(base.e_.g_, range.e_.g_);
		detail::color_rgba<T>::e_.b_ = r.f32_range(base.e_.b_, range.e_.b_);
		detail::color_rgba<T>::e_.a_ = 1.0f;

		return *this;
	}

	///	@brief				�e�v�f�����ׂă[�����ǂ���
	///	@return				�^�U
	bool	is_unit() const{
		if(0.0f!=detail::color_rgba<T>::e_.r_)	return false;
		if(0.0f!=detail::color_rgba<T>::e_.g_)	return false;
		if(0.0f!=detail::color_rgba<T>::e_.b_)	return false;

		return true;
	}

	///	@brief				�e�v�f�����ׂă[�����ǂ���
	///	@return				�^�U
	bool	is_equal(const color_rgba &v) const{
		if(v.e_.r_!=detail::color_rgba<T>::e_.r_)	return false;
		if(v.e_.g_!=detail::color_rgba<T>::e_.g_)	return false;
		if(v.e_.b_!=detail::color_rgba<T>::e_.b_)	return false;

		return true;
	}

};

///	@}

///	@}

namespace ni{

///	@addtogroup	type
///	@{

///	@addtogroup	type_color
///	@{

///	@brief	�f�t�H���g�R���X�g���N�^�ŏ��������������Ȃ��F�N���X(RGB)
template<typename T>
class color_rgb : public ml::color_rgb<T>{

public:
	color_rgb() : ml::color_rgb<T>(internal::not_initialize()){;}
	color_rgb(T r, T g, T b) : ml::color_rgb<T>(r, g, b){;}
	color_rgb(const detail::color_rgb<T> &v){
		static_cast<detail::color_rgb<T> &>(*this) = v;
	}
};

///	@brief	�f�t�H���g�R���X�g���N�^�ŏ��������������Ȃ��F�N���X(ARGB)
template<typename T>
class color_argb : public ml::color_argb<T>{

public:
	color_argb() : ml::color_argb<T>(internal::not_initialize()){;}
	color_argb(T a, T r, T g, T b) : ml::color_argb<T>(a, r, g, b){;}
	color_argb(const detail::color_argb<T> &v){
		static_cast<detail::color_argb<T> &>(*this) = v;
	}
};

///	@brief	�f�t�H���g�R���X�g���N�^�ŏ��������������Ȃ��F�N���X(RGBA)
template<typename T>
class color_rgba : public ml::color_rgba<T>{

public:
	color_rgba() : ml::color_rgba<T>(internal::not_initialize()){;}
	color_rgba(T r, T g, T b, T a) : ml::color_rgba<T>(r, g, b, a){;}
	color_rgba(const detail::color_rgba<T> &v){
		static_cast<detail::color_rgba<T> &>(*this) = v;
	}
};

///	@}

///	@}

}	// namespace ni

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_TYPE_ML_COLOR_H
