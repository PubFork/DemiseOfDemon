#ifndef LIBMATCHLOCK_INC_ML_TYPE_TRAITS_ML_IS_SAME_H
#define LIBMATCHLOCK_INC_ML_TYPE_TRAITS_ML_IS_SAME_H

namespace ml{

///	@addtogroup	type
///	@{

///	@addtogroup	type_trait
///	@{

///	@brief	整数値型かどうかを判定する
template<typename T, typename U>
struct is_same{
	enum{
		value = false
	};
};

///	@cond internal

template<typename T>
struct is_same<T, T>{
	enum{
		value = true
	};
};

///	@endcond

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_TYPE_TRAITS_ML_IS_FLOAT_H
