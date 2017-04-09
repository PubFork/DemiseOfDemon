#ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_STRING_H
#define LIBMATCHLOCK_INC_ML_UTILITY_ML_STRING_H

#include "ml_assert.h"
#include "../type/traits/ml_is_numeric.h"

#include <string>
#include <cstring>
#if defined(__GNUC__) && (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)
	#include <cstdlib>
#else
	#include <stdlib.h>
#endif
#include <wchar.h>

namespace ml{

///	@addtogroup	utility
///	@{

///	@addtogroup	utility_string
///	@{

///	@brief				�������long�l�ɕϊ�
///	@param[in]	s		�ϊ��Ώە�����
///	@param[in]	endptr	�ϊ��s�\�ȕ�����ւ̃|�C���^�̊i�[��
///	@param[in]	radix	�
///	@return				�l
///	@note				�W��C���C�u�����݊��ŕ�����̌^���ӎ����Ȃ��Ă����삳���邽�߂Ɏ�������Ă���
long strtol(const wchar_t *s, wchar_t **endptr, int radix);

///	@brief				�������double�l�ɕϊ�
///	@param[in]	s		�ϊ��Ώە�����
///	@param[in]	endptr	�ϊ��s�\�ȕ�����ւ̃|�C���^�̊i�[��
///	@return				�l
///	@note				�W��C���C�u�����݊��ŕ�����̌^���ӎ����Ȃ��Ă����삳���邽�߂Ɏ�������Ă���
double strtod(const wchar_t *s, wchar_t **endptr);

///	@brief				�������long�l�ɕϊ�
///	@param[in]	s		�ϊ��Ώە�����
///	@param[in]	endptr	�ϊ��s�\�ȕ�����ւ̃|�C���^�̊i�[��
///	@param[in]	radix	�
///	@return				�l
///	@note				�W��C���C�u�����݊��ŕ�����̌^���ӎ����Ȃ��Ă����삳���邽�߂Ɏ�������Ă���
inline long strtol(const char *s, char **endptr, int radix){
	ML_PTR_ASSERT(s);
	return std::strtol(s, endptr, radix);
//	return strtoll(s, endptr, radix);
}

///	@brief				�������long�l�ɕϊ�
///	@param[in]	s		�ϊ��Ώە�����
///	@param[in]	endptr	�ϊ��s�\�ȕ�����ւ̃|�C���^�̊i�[��
///	@param[in]	radix	�
///	@return				�l
///	@note				�W��C���C�u�����݊��ŕ�����̌^���ӎ����Ȃ��Ă����삳���邽�߂Ɏ�������Ă���
inline long strtol(const std::string &s, char **endptr, int radix){
	return strtol(s.c_str(), endptr, radix);
}

///	@brief				�������long�l�ɕϊ�
///	@param[in]	s		�ϊ��Ώە�����
///	@param[in]	endptr	�ϊ��s�\�ȕ�����ւ̃|�C���^�̊i�[��
///	@param[in]	radix	�
///	@return				�l
///	@note				�W��C���C�u�����݊��ŕ�����̌^���ӎ����Ȃ��Ă����삳���邽�߂Ɏ�������Ă���
inline long strtol(const std::wstring &s, wchar_t **endptr, int radix){
	return strtol(s.c_str(), endptr, radix);
}

///	@brief				�������double�l�ɕϊ�
///	@param[in]	s		�ϊ��Ώە�����
///	@param[in]	endptr	�ϊ��s�\�ȕ�����ւ̃|�C���^�̊i�[��
///	@return				�l
///	@note				�W��C���C�u�����݊��ŕ�����̌^���ӎ����Ȃ��Ă����삳���邽�߂Ɏ�������Ă���
inline double strtod(const char *s, char **endptr){
	ML_PTR_ASSERT(s);
	return std::strtod(s, endptr);
//	return strtold(s, endptr);
}

///	@brief				�������double�l�ɕϊ�
///	@param[in]	s		�ϊ��Ώە�����
///	@param[in]	endptr	�ϊ��s�\�ȕ�����ւ̃|�C���^�̊i�[��
///	@return				�l
///	@note				�W��C���C�u�����݊��ŕ�����̌^���ӎ����Ȃ��Ă����삳���邽�߂Ɏ�������Ă���
inline double strtod(const std::string &s, char **endptr){
	return strtod(s.c_str(), endptr);
}

///	@brief				�������double�l�ɕϊ�
///	@param[in]	s		�ϊ��Ώە�����
///	@param[in]	endptr	�ϊ��s�\�ȕ�����ւ̃|�C���^�̊i�[��
///	@return				�l
///	@note				�W��C���C�u�����݊��ŕ�����̌^���ӎ����Ȃ��Ă����삳���邽�߂Ɏ�������Ă���
inline double strtod(const std::wstring &s, wchar_t **endptr){
	return strtod(s.c_str(), endptr);
}

///	@brief				������̕������̎擾
///	@param[in]	s		������
///	@return				������̕�����
///	@note				�W��C���C�u�����݊��ŕ�����̌^���ӎ����Ȃ��Ă����삳���邽�߂Ɏ�������Ă���
inline size_t strlen(const char *s){
	ML_PTR_ASSERT(s);
	return std::strlen(s);
}

///	@brief				������̕������̎擾
///	@param[in]	s		������
///	@return				������̕�����
///	@note				�W��C���C�u�����݊��ŕ�����̌^���ӎ����Ȃ��Ă����삳���邽�߂Ɏ�������Ă���
inline size_t strlen(const wchar_t *s){
	ML_PTR_ASSERT(s);
#ifdef _FCHAR_WCHAR
	return std::strlen((const char *)s);
#else
	return ::wcslen(s);
#endif
}

///	@brief				������̕������̎擾
///	@param[in]	s		������
///	@return				������̕�����
///	@note				�W��C���C�u�����݊��ŕ�����̌^���ӎ����Ȃ��Ă����삳���邽�߂Ɏ�������Ă���
inline size_t strlen(const std::string &s){
	return strlen(s.c_str());
}

///	@brief				������̕������̎擾
///	@param[in]	s		������
///	@return				������̕�����
///	@note				�W��C���C�u�����݊��ŕ�����̌^���ӎ����Ȃ��Ă����삳���邽�߂Ɏ�������Ă���
inline size_t strlen(const std::wstring &s){
	return strlen(s.c_str());
}

///	@brief				������̔�r
///	@param[in]	s0		�Ώە�����
///	@param[in]	s1		�Ώە�����
///	@return				��r����
///	@note				�W��C���C�u�����݊��ŕ�����̌^���ӎ����Ȃ��Ă����삳���邽�߂Ɏ�������Ă���
inline int strcmp(const char *s0, const char *s1){
	ML_PTR_ASSERT(s0);
	ML_PTR_ASSERT(s1);
	return std::strcmp(s0, s1);
}

///	@brief				������̔�r
///	@param[in]	s0		�Ώە�����
///	@param[in]	s1		�Ώە�����
///	@return				��r����
///	@note				�W��C���C�u�����݊��ŕ�����̌^���ӎ����Ȃ��Ă����삳���邽�߂Ɏ�������Ă���
inline int strcmp(const wchar_t *s0, const wchar_t *s1){
	ML_PTR_ASSERT(s0);
	ML_PTR_ASSERT(s1);
#ifdef _FCHAR_WCHAR
	return std::strcmp((const char *)s0, (const char *)s1);
#else
	return ::wcscmp(s0, s1);
#endif
}

///	@brief				������̔�r
///	@param[in]	s0		�Ώە�����
///	@param[in]	s1		�Ώە�����
///	@return				��r����
///	@note				�W��C���C�u�����݊��ŕ�����̌^���ӎ����Ȃ��Ă����삳���邽�߂Ɏ�������Ă���
inline int strcmp(const std::string &s0, const std::string &s1){
	return strcmp(s0.c_str(), s1.c_str());
}

///	@brief				������̔�r
///	@param[in]	s0		�Ώە�����
///	@param[in]	s1		�Ώە�����
///	@return				��r����
///	@note				�W��C���C�u�����݊��ŕ�����̌^���ӎ����Ȃ��Ă����삳���邽�߂Ɏ�������Ă���
inline int strcmp(const std::wstring &s0, const std::wstring &s1){
	return strcmp(s0.c_str(), s1.c_str());
}

///	@brief				�}���`�o�C�g������̃��C�h������
///	@param[in]	s		������
///	@return				�ϊ��㕶����
std::wstring to_wstring(const std::string &s);

///	@brief				�}���`�o�C�g������̃��C�h������
///	@param[in]	s		������
///	@return				�ϊ��㕶����
std::wstring to_wstring(const char *s);

///	@brief				���C�h������̃}���`�o�C�g������
///	@param[in]	s		������
///	@return				�ϊ��㕶����
std::string to_string(const std::wstring &s);

///	@brief				���C�h������̃}���`�o�C�g������
///	@param[in]	s		������
///	@return				�ϊ��㕶����
std::string to_string(const wchar_t *s);

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_STRING_H
