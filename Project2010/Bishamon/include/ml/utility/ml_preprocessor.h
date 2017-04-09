#ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_PREPROCESSOR_H
#define LIBMATCHLOCK_INC_ML_UTILITY_ML_PREPROCESSOR_H

namespace ml{

///	@addtogroup	utility
///	@{

///	@addtogroup	utility_preprocessor
///	@{

///	@brief			������
///	@param[in]	a	�g�[�N��
///	@return			������
#define	ML_PP_STR(a)				ML_PP_STR_INTERNAL(a)

///	@brief			���C�h������
///	@param[in]	a	�g�[�N��
///	@return			���C�h������
#define	ML_PP_WSTR(a)				ML_PP_WIDESTRING(ML_PP_STR(a))

///	@brief			�g�[�N������
///	@param[in]	a	�g�[�N��
///	@param[in]	b	�g�[�N��
///	@return			�g�[�N��
#define	ML_PP_CAT(a, b)				ML_PP_CAT_INTERNAL(a, b)

///	@brief			�����񃊃e���������C�h�����񃊃e�����ɂ���
///	@param[in]	a	������
///	@return			���C�h������
#define	ML_PP_WIDESTRING(a)			ML_PP_CAT(L, a)

///	@brief				���j�[�N��ID�����߂�
///	@param[in]	base	���̓g�[�N��
///	@return				�g�[�N��
#define	ML_PP_UNIQUE_ID(base)		ML_PP_CAT(base, __LINE__)

///	@cond internal

namespace internal{

#define	ML_PP_STR_INTERNAL(a)		#a
#define	ML_PP_CAT_INTERNAL(a, b)	a##b

}	// namespace internal

///	@endcond

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_PREPROCESSOR_H
