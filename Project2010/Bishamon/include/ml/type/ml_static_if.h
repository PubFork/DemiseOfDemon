#ifndef LIBMATCHLOCK_INC_ML_TYPE_ML_STATIC_IF_H
#define LIBMATCHLOCK_INC_ML_TYPE_ML_STATIC_IF_H

namespace ml{

///	@addtogroup	type
///	@{

///	@addtogroup	type_static_if
///	@{

///	@brief	ê√ìIif
template<bool COND, typename THEN, typename ELSE>
struct static_if;

///	@cond internal

template<typename THEN, typename ELSE>
struct static_if<true, THEN, ELSE> {
	typedef THEN type;
};

template<typename THEN, typename ELSE>
struct static_if<false, THEN, ELSE> {
	typedef ELSE type;
};

///	@endcond

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_TYPE_ML_STATIC_IF_H
