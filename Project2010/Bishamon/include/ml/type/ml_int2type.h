#ifndef LIBMATCHLOCK_INC_ML_TYPE_ML_INT2TYPE_H
#define LIBMATCHLOCK_INC_ML_TYPE_ML_INT2TYPE_H

namespace ml{

///	@addtogroup	type
///	@{

///	@addtogroup	type_int2type
///	@{

///	@brief	êÆêîÇ©ÇÁå^ÇçÏÇÈ
template<int n>
struct int2type{
	enum{
		value = n
	};
};

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_TYPE_ML_INT2TYPE_H
