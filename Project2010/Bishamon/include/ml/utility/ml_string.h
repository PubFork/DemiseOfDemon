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

///	@brief				文字列をlong値に変換
///	@param[in]	s		変換対象文字列
///	@param[in]	endptr	変換不可能な文字列へのポインタの格納先
///	@param[in]	radix	基数
///	@return				値
///	@note				標準Cライブラリ互換で文字列の型を意識しなくても動作させるために実装されている
long strtol(const wchar_t *s, wchar_t **endptr, int radix);

///	@brief				文字列をdouble値に変換
///	@param[in]	s		変換対象文字列
///	@param[in]	endptr	変換不可能な文字列へのポインタの格納先
///	@return				値
///	@note				標準Cライブラリ互換で文字列の型を意識しなくても動作させるために実装されている
double strtod(const wchar_t *s, wchar_t **endptr);

///	@brief				文字列をlong値に変換
///	@param[in]	s		変換対象文字列
///	@param[in]	endptr	変換不可能な文字列へのポインタの格納先
///	@param[in]	radix	基数
///	@return				値
///	@note				標準Cライブラリ互換で文字列の型を意識しなくても動作させるために実装されている
inline long strtol(const char *s, char **endptr, int radix){
	ML_PTR_ASSERT(s);
	return std::strtol(s, endptr, radix);
//	return strtoll(s, endptr, radix);
}

///	@brief				文字列をlong値に変換
///	@param[in]	s		変換対象文字列
///	@param[in]	endptr	変換不可能な文字列へのポインタの格納先
///	@param[in]	radix	基数
///	@return				値
///	@note				標準Cライブラリ互換で文字列の型を意識しなくても動作させるために実装されている
inline long strtol(const std::string &s, char **endptr, int radix){
	return strtol(s.c_str(), endptr, radix);
}

///	@brief				文字列をlong値に変換
///	@param[in]	s		変換対象文字列
///	@param[in]	endptr	変換不可能な文字列へのポインタの格納先
///	@param[in]	radix	基数
///	@return				値
///	@note				標準Cライブラリ互換で文字列の型を意識しなくても動作させるために実装されている
inline long strtol(const std::wstring &s, wchar_t **endptr, int radix){
	return strtol(s.c_str(), endptr, radix);
}

///	@brief				文字列をdouble値に変換
///	@param[in]	s		変換対象文字列
///	@param[in]	endptr	変換不可能な文字列へのポインタの格納先
///	@return				値
///	@note				標準Cライブラリ互換で文字列の型を意識しなくても動作させるために実装されている
inline double strtod(const char *s, char **endptr){
	ML_PTR_ASSERT(s);
	return std::strtod(s, endptr);
//	return strtold(s, endptr);
}

///	@brief				文字列をdouble値に変換
///	@param[in]	s		変換対象文字列
///	@param[in]	endptr	変換不可能な文字列へのポインタの格納先
///	@return				値
///	@note				標準Cライブラリ互換で文字列の型を意識しなくても動作させるために実装されている
inline double strtod(const std::string &s, char **endptr){
	return strtod(s.c_str(), endptr);
}

///	@brief				文字列をdouble値に変換
///	@param[in]	s		変換対象文字列
///	@param[in]	endptr	変換不可能な文字列へのポインタの格納先
///	@return				値
///	@note				標準Cライブラリ互換で文字列の型を意識しなくても動作させるために実装されている
inline double strtod(const std::wstring &s, wchar_t **endptr){
	return strtod(s.c_str(), endptr);
}

///	@brief				文字列の文字数の取得
///	@param[in]	s		文字列
///	@return				文字列の文字数
///	@note				標準Cライブラリ互換で文字列の型を意識しなくても動作させるために実装されている
inline size_t strlen(const char *s){
	ML_PTR_ASSERT(s);
	return std::strlen(s);
}

///	@brief				文字列の文字数の取得
///	@param[in]	s		文字列
///	@return				文字列の文字数
///	@note				標準Cライブラリ互換で文字列の型を意識しなくても動作させるために実装されている
inline size_t strlen(const wchar_t *s){
	ML_PTR_ASSERT(s);
#ifdef _FCHAR_WCHAR
	return std::strlen((const char *)s);
#else
	return ::wcslen(s);
#endif
}

///	@brief				文字列の文字数の取得
///	@param[in]	s		文字列
///	@return				文字列の文字数
///	@note				標準Cライブラリ互換で文字列の型を意識しなくても動作させるために実装されている
inline size_t strlen(const std::string &s){
	return strlen(s.c_str());
}

///	@brief				文字列の文字数の取得
///	@param[in]	s		文字列
///	@return				文字列の文字数
///	@note				標準Cライブラリ互換で文字列の型を意識しなくても動作させるために実装されている
inline size_t strlen(const std::wstring &s){
	return strlen(s.c_str());
}

///	@brief				文字列の比較
///	@param[in]	s0		対象文字列
///	@param[in]	s1		対象文字列
///	@return				比較結果
///	@note				標準Cライブラリ互換で文字列の型を意識しなくても動作させるために実装されている
inline int strcmp(const char *s0, const char *s1){
	ML_PTR_ASSERT(s0);
	ML_PTR_ASSERT(s1);
	return std::strcmp(s0, s1);
}

///	@brief				文字列の比較
///	@param[in]	s0		対象文字列
///	@param[in]	s1		対象文字列
///	@return				比較結果
///	@note				標準Cライブラリ互換で文字列の型を意識しなくても動作させるために実装されている
inline int strcmp(const wchar_t *s0, const wchar_t *s1){
	ML_PTR_ASSERT(s0);
	ML_PTR_ASSERT(s1);
#ifdef _FCHAR_WCHAR
	return std::strcmp((const char *)s0, (const char *)s1);
#else
	return ::wcscmp(s0, s1);
#endif
}

///	@brief				文字列の比較
///	@param[in]	s0		対象文字列
///	@param[in]	s1		対象文字列
///	@return				比較結果
///	@note				標準Cライブラリ互換で文字列の型を意識しなくても動作させるために実装されている
inline int strcmp(const std::string &s0, const std::string &s1){
	return strcmp(s0.c_str(), s1.c_str());
}

///	@brief				文字列の比較
///	@param[in]	s0		対象文字列
///	@param[in]	s1		対象文字列
///	@return				比較結果
///	@note				標準Cライブラリ互換で文字列の型を意識しなくても動作させるために実装されている
inline int strcmp(const std::wstring &s0, const std::wstring &s1){
	return strcmp(s0.c_str(), s1.c_str());
}

///	@brief				マルチバイト文字列のワイド文字列化
///	@param[in]	s		文字列
///	@return				変換後文字列
std::wstring to_wstring(const std::string &s);

///	@brief				マルチバイト文字列のワイド文字列化
///	@param[in]	s		文字列
///	@return				変換後文字列
std::wstring to_wstring(const char *s);

///	@brief				ワイド文字列のマルチバイト文字列化
///	@param[in]	s		文字列
///	@return				変換後文字列
std::string to_string(const std::wstring &s);

///	@brief				ワイド文字列のマルチバイト文字列化
///	@param[in]	s		文字列
///	@return				変換後文字列
std::string to_string(const wchar_t *s);

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_STRING_H
