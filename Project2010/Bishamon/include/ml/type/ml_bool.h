#ifndef LIBMATCHLOCK_INC_ML_TYPE_ML_BOOL_H
#define LIBMATCHLOCK_INC_ML_TYPE_ML_BOOL_H

namespace ml{

///	@addtogroup	type
///	@{

///	@addtogroup	type_bool
///	@{

/// true‚ğ¦‚·Œ^
struct true_type{
	enum{
		value = true
	};
};

/// false‚ğ¦‚·Œ^
struct false_type{
	enum{
		value = false
	};
};

/// ƒu[ƒ‹’l‚ğ¦‚·Œ^
template<bool>
struct	bool_type;

/// true‚ğ¦‚·Œ^
template<>
struct bool_type<true>{
	typedef	true_type	type;
};

/// false‚ğ¦‚·Œ^
template<>
struct bool_type<false>{
	typedef	false_type	type;
};

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_TYPE_ML_BASIC_H
