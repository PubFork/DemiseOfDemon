#ifndef LIBMATCHLOCK_INC_ML_TYPE_ML_TYPE2TYPE_H
#define LIBMATCHLOCK_INC_ML_TYPE_ML_TYPE2TYPE_H

namespace ml{

///	@addtogroup	type
///	@{

///	@addtogroup	type_type2type
///	@{

///	@brief	型から型を作る
template<typename T>
struct type2type{
	typedef T src_type;			///<	元となった型
};

///	@brief	列挙体から型を作る
template<typename T, T m>
struct enum2type{
	typedef T src_type;			///<	元となった型
	enum{
		value = m				///<	元となった列挙体のメンバーの値
	};
};

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_TYPE_ML_TYPE2TYPE_H
