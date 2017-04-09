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
	@brief	�n�b�V�����X�g���Ǘ�����N���X
	@note	HashInterface���g�����T���v���BSTL���g�p���Ă���B
			�������Ǘ������S�ɍs���ɂ͂��̃N���X�͓K���Ȃ��B
*/
class StandardHash : public HashInterface{

public:
	StandardHash();
	~StandardHash();

	/*!
		@brief	�n�b�V�����X�g�ɒǉ�����
		@param	name	�n�b�V���̃L�[�ƂȂ閼�O
		@param	value	�n�b�V���l
		@return	�Ȃ�
	*/
	void	Add(const wchar_t *name, void *value);
	/*!
		@brief	�n�b�V�����X�g����폜����
		@param	name	�n�b�V���̃L�[�ƂȂ閼�O
		@return	�Ȃ�
	*/
	void	Erase(const wchar_t *name);
	/*!
		@brief	�n�b�V�����X�g�����ׂď���
		@return	�Ȃ�
	*/
	void	Clear();

	/*!
		@brief	�n�b�V�����X�g�ɑ��݂��邩�ǂ����𒲂ׂ�
		@param	name	�n�b�V���̃L�[�ƂȂ閼�O
		@return	���݂���Ƃ�true,���݂��Ȃ����false
	*/
	bool	IsExist(const wchar_t *name) const;
	/*!
		@brief	�n�b�V�����X�g�̒l���擾����
		@param	name			�n�b�V���̃L�[�ƂȂ閼�O
		@param	default_value	�n�b�V�����猩����Ȃ������Ƃ��̒l
		@return	�n�b�V���̒l
	*/
	void *	Value(const wchar_t *name, void *default_value) const;

private:
	typedef	std::map<const wchar_t *, void *, internal::StandardHashComp>	ValueMap;
	typedef	ValueMap::iterator												ValueMapI;
	typedef	ValueMap::const_iterator										ValueMapCI;

	typedef	std::list<std::wstring>											StringPool;

	ValueMap	value_map_;		//!< �n�b�V�����X�g
	StringPool	string_pool_;	
};

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_UTILITY_BM3_STANDARDHASH_H
