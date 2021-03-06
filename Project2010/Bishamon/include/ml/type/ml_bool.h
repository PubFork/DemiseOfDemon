#ifndef LIBMATCHLOCK_INC_ML_TYPE_ML_BOOL_H
#define LIBMATCHLOCK_INC_ML_TYPE_ML_BOOL_H

namespace ml{

///	@addtogroup	type
///	@{

///	@addtogroup	type_bool
///	@{

/// trueを示す型
struct true_type{
	enum{
		value = true
	};
};

/// falseを示す型
struct false_type{
	enum{
		value = false
	};
};

/// ブール値を示す型
template<bool>
struct	bool_type;

/// trueを示す型
template<>
struct bool_type<true>{
	typedef	true_type	type;
};

/// falseを示す型
template<>
struct bool_type<false>{
	typedef	false_type	type;
};

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_TYPE_ML_BASIC_H
