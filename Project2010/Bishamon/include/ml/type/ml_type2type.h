#ifndef LIBMATCHLOCK_INC_ML_TYPE_ML_TYPE2TYPE_H
#define LIBMATCHLOCK_INC_ML_TYPE_ML_TYPE2TYPE_H

namespace ml{

///	@addtogroup	type
///	@{

///	@addtogroup	type_type2type
///	@{

///	@brief	�^����^�����
template<typename T>
struct type2type{
	typedef T src_type;			///<	���ƂȂ����^
};

///	@brief	�񋓑̂���^�����
template<typename T, T m>
struct enum2type{
	typedef T src_type;			///<	���ƂȂ����^
	enum{
		value = m				///<	���ƂȂ����񋓑̂̃����o�[�̒l
	};
};

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_TYPE_ML_TYPE2TYPE_H
