#ifndef LIBMATCHLOCK_INC_ML_TYPE_ML_TYPE2TYPE_H
#define LIBMATCHLOCK_INC_ML_TYPE_ML_TYPE2TYPE_H

namespace ml{

///	@addtogroup	type
///	@{

///	@addtogroup	type_type2type
///	@{

///	@brief	å^Ç©ÇÁå^ÇçÏÇÈ
template<typename T>
struct type2type{
	typedef T src_type;			///<	å≥Ç∆Ç»Ç¡ÇΩå^
};

///	@brief	óÒãìëÃÇ©ÇÁå^ÇçÏÇÈ
template<typename T, T m>
struct enum2type{
	typedef T src_type;			///<	å≥Ç∆Ç»Ç¡ÇΩå^
	enum{
		value = m				///<	å≥Ç∆Ç»Ç¡ÇΩóÒãìëÃÇÃÉÅÉìÉoÅ[ÇÃíl
	};
};

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_TYPE_ML_TYPE2TYPE_H
