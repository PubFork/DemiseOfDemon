#ifndef BM3_SDK_INC_BM3_CORE_FILEFORMAT_BMB_COMMON_H
#define BM3_SDK_INC_BM3_CORE_FILEFORMAT_BMB_COMMON_H

#include "../../system/bm3_Config.h"
#include "../bm3_CoreTypeSwitch.h"
#include "bm3_bmb_UpdateBmbInfo.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace fileformat{
namespace bmb{

struct NodeHeader;

/*!
	@brief	bmb�C���[�W������������i�|�C���^�̕t���ւ��j
	@param	image	bmb�C���[�W�̐擪�|�C���^
	@return	�Ȃ�
*/
void Initialize(void *image);
/*!
	@brief	bmb�t�@�C���̑Ó������`�F�b�N����
	@param	image	bmb�C���[�W�̐擪�|�C���^
	@return	bmb�t�@�C���̑Ó����ǂ���
*/
bool IsValidateData(const void *image);
/*!
	@brief	�������ς݂��ǂ�����Ԃ�
	@param	image	bmb�C���[�W�̐擪�|�C���^
	@return	�������ς݂��ǂ���
*/
bool IsInitializedData(const void *image);
/*!
	@brief	���[�g�m�[�h�̃|�C���^��Ԃ�
	@param	image	bmb�C���[�W�̐擪�|�C���^
	@return	���[�g�m�[�h�̃|�C���^
*/
const void *RootNodeRes(const void *image);
/*!
	@brief	�g�p���Ă���e�N�X�`���̖�����Ԃ�
	@param	image	bmb�C���[�W�̐擪�|�C���^
	@return	�g�p���Ă���e�N�X�`���̖���
*/
int TextureCount(const void *image);
/*!
	@brief	�g�p���Ă��郂�f���f�[�^�̐���Ԃ�
	@param	image	bmb�C���[�W�̐擪�|�C���^
	@return	�g�p���Ă��郂�f���f�[�^�̐���
*/
int ModelCount(const void *image);
/*!
	@brief	���f���f�[�^�Ŏg�p���Ă���e�N�X�`���̐���Ԃ�
	@param	image	bmb�C���[�W�̐擪�|�C���^
	@return	���f���f�[�^�Ŏg�p���Ă���e�N�X�`���̐�
*/
int ModelTextureCount(const void *image);
/*!
	@brief	�Q�ƃm�[�h��bmsln�̐���Ԃ�
	@param	image	bmb�C���[�W�̐擪�|�C���^
	@return	�Q�ƃm�[�h��bmsln�̐�
*/
int BmslnCount(const void *image);
/*!
	@brief	�G�~�b�^�[�m�[�h�̐���Ԃ�
	@param	image	bmb�C���[�W�̐擪�|�C���^
	@return	�G�~�b�^�[�m�[�h�̐�
*/
int EmitterCount(const void *image);

/*!
	@brief	�e�N�X�`���̖��O��Ԃ�
	@param	image	bmb�C���[�W�̐擪�|�C���^
	@param	index	�e�N�X�`���̃C���f�b�N�X
	@return	�e�N�X�`���̖��O
*/
const wchar_t *TextureNameFromIndex(const void *image, int index);
/*!
	@brief	bmsln����e�N�X�`���ւ̑��΃p�X����Ԃ�
	@param	image	bmb�C���[�W�̐擪�|�C���^
	@param	index	�e�N�X�`���̃C���f�b�N�X
	@return	bmsln����e�N�X�`���ւ̑��΃p�X��
*/
const wchar_t *TexturePathFromIndex(const void *image, int index);
/*!
	@brief	���f���̖��O��Ԃ�
	@param	image	bmb�C���[�W�̐擪�|�C���^
	@param	index	���f���̃C���f�b�N�X
	@return	���f���̖��O
*/
const wchar_t *ModelNameFromIndex(const void *image, int index);
/*!
	@brief	bmsln���烂�f���ւ̑��΃p�X����Ԃ�
	@param	image	bmb�C���[�W�̐擪�|�C���^
	@param	index	���f���̃C���f�b�N�X
	@return	bmsln���烂�f���ւ̑��΃p�X��
*/
const wchar_t *ModelPathFromIndex(const void *image, int index);
/*!
	@brief	���f���e�N�X�`���̖��O��Ԃ�
	@param	image	bmb�C���[�W�̐擪�|�C���^
	@param	index	���f���e�N�X�`���̃C���f�b�N�X
	@return	���f���e�N�X�`���̖��O
*/
const wchar_t *ModelTextureNameFromIndex(const void *image, int index);
/*!
	@brief	bmsln���烂�f���e�N�X�`���ւ̑��΃p�X����Ԃ��i���f���e�N�X�`���̓��f���Ɠ����ꏊ�ɂ���K�v������j
	@param	image	bmb�C���[�W�̐擪�|�C���^
	@param	index	���f���̃C���f�b�N�X
	@return	bmsln���烂�f���e�N�X�`���ւ̑��΃p�X��
*/
const wchar_t *ModelTexturePathFromIndex(const void *image, int index);
/*!
	@brief	bmsln�̖��O��Ԃ�
	@param	image	bmb�C���[�W�̐擪�|�C���^
	@param	index	bmsln�̃C���f�b�N�X
	@return	bmsln�̖��O
*/
const wchar_t *BmslnNameFromIndex(const void *image, int index);
/*!
	@brief	bmsln����Q�ƃm�[�h�p��bmsln�ւ̑��΃p�X����Ԃ�
	@param	image	bmb�C���[�W�̐擪�|�C���^
	@param	index	�Q�ƃm�[�h�p��bmsln�̃C���f�b�N�X
	@return	bmsln����Q�ƃm�[�h�p��bmsln�ւ̑��΃p�X��
*/
const wchar_t *BmslnPathFromIndex(const void *image, int index);

/*!
	@brief	���[�g�̃��[�U�[�f�[�^�ւ̃|�C���^��Ԃ�
	@param	image	bmb�C���[�W�̐擪�|�C���^
	@return	���[�g�̃��[�U�[�f�[�^�ւ̃|�C���^
*/
const UserDataType *RootUserData(const void *image);
/*!
	@brief	�J��グ�t���[����Ԃ�
	@param	image	bmb�C���[�W�̐擪�|�C���^
	@return	�J��グ�t���[��
*/
int RootAdvancedFrame(const void *image);
/*!
	@brief	bm2�݊��t���O���Z�b�g����Ă��邩�ǂ�����Ԃ�
	@param	image	bmb�C���[�W�̐擪�|�C���^
	@return	bm2�݊��t���O���Z�b�g����Ă��邩�ǂ���
*/
bool RootInterChangedBm2(const void *image);
/*!
	@brief	version1.5�t���O���Z�b�g����Ă��邩�ǂ�����Ԃ�
	@param	image	bmb�C���[�W�̐擪�|�C���^
	@return	version1.5�t���O���Z�b�g����Ă��邩�ǂ���
*/
bool RootVersion1_5(const void *bmb_image);
/*!
	@brief	�G�t�F�N�g�S�̂̎�����Ԃ�
	@param	image	bmb�C���[�W�̐擪�|�C���^
	@return	�G�t�F�N�g�S�̂̎���
	@detail	�G�t�F�N�g�S�̂̎�����Ԃ��B
			�����_�����ݒ肳��Ă���ꍇ�̓����_�����ő�l��������ꍇ������Ԃ�
*/
int TotalLifeTime(const void *bmb_image);
/*!
	@brief	�G�t�F�N�g��Z�\�[�g���L�����ǂ�����Ԃ�
	@param	image	bmb�C���[�W�̐擪�|�C���^
	@return	�G�t�F�N�g��Z�\�[�g���L�����ǂ���
*/
bool IsZSort(const void *bmb_image);
/*!
	@brief	���[�g�m�[�h�̎�����Ԃ�
	@param	image	bmb�C���[�W�̐擪�|�C���^
	@return	���[�g�m�[�h�̎���
	@detail	���[�g�m�[�h�̎�����Ԃ��B
			0�̏ꍇ�͐ݒ肪�������̂Ƃ���
*/
int RootLife(const void *bmb_image);

/*!
	@brief	bmb�̒��g������������
	@param	info	bmb�X�V���
	@param	image	bmb�C���[�W�̐擪�|�C���^
	@return	�Ȃ�
	@detail	UpdateBmbInfo���p�������N���X���쐬����B
			�Ăяo���ꂽ�֐�����f�[�^�̓��e������������B
*/
void UpdateBmb(UpdateBmbInfo &info, void *bmb_image);
	
struct ChildNodeMapping{
	static int Execute(NodeHeader &node_header, const wchar_t *string_chunk, const void *binary_chunk);
};

}	// namespace bmb
}	// namespace fileformat
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// BM3_SDK_INC_BM3_CORE_FILEFORMAT_BMB_COMMON_H
