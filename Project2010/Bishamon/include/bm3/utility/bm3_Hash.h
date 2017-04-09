#ifndef BM3_SDK_INC_BM3_UTILITY_BM3_HASH_H
#define BM3_SDK_INC_BM3_UTILITY_BM3_HASH_H

#include "../system/bm3_Config.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

/*!
	@brief	ハッシュリストを管理するクラス
*/
class HashInterface{

public:
	virtual ~HashInterface();

	/*!
		@brief	ハッシュリストに追加する
		@param	name	ハッシュのキーとなる名前
		@param	value	ハッシュ値
		@return	なし
	*/
	virtual void	Add(const wchar_t *name, void *value) = 0;
	/*!
		@brief	ハッシュリストから削除する
		@param	name	ハッシュのキーとなる名前
		@return	なし
	*/
	virtual void	Erase(const wchar_t *name) = 0;
	/*!
		@brief	ハッシュリストをすべて消す
		@return	なし
	*/
	virtual void	Clear() = 0;

	/*!
		@brief	ハッシュリストに存在するかどうかを調べる
		@param	name	ハッシュのキーとなる名前
		@return	存在するときtrue,存在しなければfalse
	*/
	virtual bool	IsExist(const wchar_t *name) const = 0;
	/*!
		@brief	ハッシュリストの値を取得する
		@param	name			ハッシュのキーとなる名前
		@param	default_value	ハッシュから見つからなかったときの値
		@return	ハッシュの値
	*/
	virtual void *	Value(const wchar_t *name, void *default_value) const = 0;
};

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_UTILITY_BM3_HASH_H
