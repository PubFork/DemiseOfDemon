#ifndef BM3_SDK_INC_BM3_UTILITY_BM3_STANDARDHASH_H
#define BM3_SDK_INC_BM3_UTILITY_BM3_STANDARDHASH_H

#if defined(BM3_PLATFORM_nw4c)
#pragma diag_suppress 1300
#pragma diag_suppress 1301
#endif

#include "../system/bm3_Config.h"
#include "bm3_Hash.h"
#include <ml/utility/ml_string.h>
#include <list>
#include <map>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace internal{
struct StandardHashComp{
	bool operator()(const wchar_t *l, const wchar_t *r) const{
		return ml::strcmp(l, r) < 0;
	}
};
}	// namespace internal

/*!
	@brief	ハッシュリストを管理するクラス
	@note	HashInterfaceを使ったサンプル。STLを使用している。
			メモリ管理を完全に行うにはこのクラスは適さない。
*/
class StandardHash : public HashInterface{

public:
	StandardHash();
	~StandardHash();

	/*!
		@brief	ハッシュリストに追加する
		@param	name	ハッシュのキーとなる名前
		@param	value	ハッシュ値
		@return	なし
	*/
	void	Add(const wchar_t *name, void *value);
	/*!
		@brief	ハッシュリストから削除する
		@param	name	ハッシュのキーとなる名前
		@return	なし
	*/
	void	Erase(const wchar_t *name);
	/*!
		@brief	ハッシュリストをすべて消す
		@return	なし
	*/
	void	Clear();

	/*!
		@brief	ハッシュリストに存在するかどうかを調べる
		@param	name	ハッシュのキーとなる名前
		@return	存在するときtrue,存在しなければfalse
	*/
	bool	IsExist(const wchar_t *name) const;
	/*!
		@brief	ハッシュリストの値を取得する
		@param	name			ハッシュのキーとなる名前
		@param	default_value	ハッシュから見つからなかったときの値
		@return	ハッシュの値
	*/
	void *	Value(const wchar_t *name, void *default_value) const;

private:
	typedef	std::map<const wchar_t *, void *, internal::StandardHashComp>	ValueMap;
	typedef	ValueMap::iterator												ValueMapI;
	typedef	ValueMap::const_iterator										ValueMapCI;

	typedef	std::list<std::wstring>											StringPool;

	ValueMap	value_map_;		//!< ハッシュリスト
	StringPool	string_pool_;	
};

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_UTILITY_BM3_STANDARDHASH_H
