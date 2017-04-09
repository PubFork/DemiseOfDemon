#ifndef BM3_SDK_INC_BM3_UTILITY_BM3_HASH_H
#define BM3_SDK_INC_BM3_UTILITY_BM3_HASH_H

#include "../system/bm3_Config.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

/*!
	@brief	�n�b�V�����X�g���Ǘ�����N���X
*/
class HashInterface{

public:
	virtual ~HashInterface();

	/*!
		@brief	�n�b�V�����X�g�ɒǉ�����
		@param	name	�n�b�V���̃L�[�ƂȂ閼�O
		@param	value	�n�b�V���l
		@return	�Ȃ�
	*/
	virtual void	Add(const wchar_t *name, void *value) = 0;
	/*!
		@brief	�n�b�V�����X�g����폜����
		@param	name	�n�b�V���̃L�[�ƂȂ閼�O
		@return	�Ȃ�
	*/
	virtual void	Erase(const wchar_t *name) = 0;
	/*!
		@brief	�n�b�V�����X�g�����ׂď���
		@return	�Ȃ�
	*/
	virtual void	Clear() = 0;

	/*!
		@brief	�n�b�V�����X�g�ɑ��݂��邩�ǂ����𒲂ׂ�
		@param	name	�n�b�V���̃L�[�ƂȂ閼�O
		@return	���݂���Ƃ�true,���݂��Ȃ����false
	*/
	virtual bool	IsExist(const wchar_t *name) const = 0;
	/*!
		@brief	�n�b�V�����X�g�̒l���擾����
		@param	name			�n�b�V���̃L�[�ƂȂ閼�O
		@param	default_value	�n�b�V�����猩����Ȃ������Ƃ��̒l
		@return	�n�b�V���̒l
	*/
	virtual void *	Value(const wchar_t *name, void *default_value) const = 0;
};

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_UTILITY_BM3_HASH_H
