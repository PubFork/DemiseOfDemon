#ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_ARRAY_H
#define LIBMATCHLOCK_INC_ML_UTILITY_ML_ARRAY_H

namespace ml{

///	@addtogroup	utility
///	@{

///	@addtogroup	utility_array
///	@{

// http://www.tietew.jp/cppll/archive/9004

///	@cond internal

namespace internal{

template<int N>
struct array_length{
	enum{
		length = N
	};
};

}	// namespace internal

///	@endcond

static const int dummy_ = 1234;	// �x���폜�̂���

///	@brief			�z��̒��������߂�
///	@param[in]	a	���͔z��
///	@return			�z��
template<typename T, int N>
inline internal::array_length<N> & array(T (&/*a*/)[N]){

//	(void *)a;

	return *((internal::array_length<N> *)dummy_);
}

///	@brief			�z��̐擪�����߂�
///	@param[in]	a	���͔z��
///	@return			�z��̐擪
template<typename T, int N>
inline T * array_begin(T (&a)[N]){

	return a;
}

///	@brief			�z��̍Ō�������߂�
///	@param[in]	a	���͔z��
///	@return			�z��̍Ō��
template<typename T, int N>
inline T * array_end(T (&a)[N]){

	return a + array(a).length;
}

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_ARRAY_H
