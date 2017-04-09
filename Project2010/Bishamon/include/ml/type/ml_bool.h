#ifndef LIBMATCHLOCK_INC_ML_TYPE_ML_BOOL_H
#define LIBMATCHLOCK_INC_ML_TYPE_ML_BOOL_H

namespace ml{

///	@addtogroup	type
///	@{

///	@addtogroup	type_bool
///	@{

/// true�������^
struct true_type{
	enum{
		value = true
	};
};

/// false�������^
struct false_type{
	enum{
		value = false
	};
};

/// �u�[���l�������^
template<bool>
struct	bool_type;

/// true�������^
template<>
struct bool_type<true>{
	typedef	true_type	type;
};

/// false�������^
template<>
struct bool_type<false>{
	typedef	false_type	type;
};

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_TYPE_ML_BASIC_H
