#ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_MAKE_SIGN_H
#define LIBMATCHLOCK_INC_ML_UTILITY_ML_MAKE_SIGN_H

namespace ml{

///	@addtogroup	utility
///	@{

///	@addtogroup	utility_make_sign
///	@{

///	@brief				32�r�b�g�̏��������
///	@param[in]	A		���̓g�[�N��
///	@param[in]	B		���̓g�[�N��
///	@param[in]	C		���̓g�[�N��
///	@param[in]	D		���̓g�[�N��
///	@return				����
#define	ML_MAKE_SIGN_32(A,B,C,D)	\
	((A)<<(8*3)) |					\
	((B)<<(8*2)) |					\
	((C)<<(8*1)) |					\
	((D)<<(8*0))

///	@brief				32�r�b�g�̏��������
///	@param[in]	A		���̓g�[�N��
///	@param[in]	B		���̓g�[�N��
///	@param[in]	C		���̓g�[�N��
///	@param[in]	D		���̓g�[�N��
///	@return				����
#define	ML_MAKE_SIGN_R32(A,B,C,D)	\
	((D)<<(8*3)) |					\
	((C)<<(8*2)) |					\
	((B)<<(8*1)) |					\
	((A)<<(8*0))

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_MAKE_SIGN_H
