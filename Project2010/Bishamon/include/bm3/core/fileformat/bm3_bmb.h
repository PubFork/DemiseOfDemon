#ifndef BM3_SDK_INC_BM3_CORE_FILEFORMAT_BMB_H
#define BM3_SDK_INC_BM3_CORE_FILEFORMAT_BMB_H

#include <ml/type/ml_basic.h>
#include <ml/type/ml_vector3d.h>
#include <ml/type/ml_color.h>
#include <ml/utility/ml_assert.h>
#include <ml/utility/ml_make_sign.h>
#include "bm3/core/bm3_Const.h"
#include "bm3/core/bm3_CoreType.h"
#include "bm3/core/resource/policy/bm3_res_policy_StaticChildArray.h"
#include "bm3/utility/internal/bm3_MacroBegin.h"
#include "bm3_bmb_NodeHeader.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace fileformat{
namespace bmb{

struct Header{
	enum{
		//
		Sign						= ML_MAKE_SIGN_32('B','M','B','I'),
		ReverseSign					= ML_MAKE_SIGN_R32('B','M','B','I'),
		//
		Version						= 0x00000037,				// 0x00000037: �e�̃J���[�p����ǉ�
																// 0x00000036: �e�N�X�`���N���b�v�̃����_���A�r���{�[�h�|�����C���A���f���̃��C�g�̃I���I�t�ǉ�
																// 0x00000035: �Q�ƃm�[�h�ǉ�
																// 0x00000034: ���΃p�X���ǉ�
																// 0x00000033: Version1_5�^�O�ǉ�
																// 0x00000032: �Q�t�B�[���h�ɏW���p�����[�^�ǉ��Abm2�݊��t���O���f�t�H���g��true�ɕύX
																// 0x00000031: ���[�g�Ɏ����ǉ��i�G�t�F�N�g�S�̂̎����j
																// 0x00000030: �w�i�摜�m�[�h�ǉ�
																// 0x0000002F: �a�l�Q�݊����[�h�ǉ�
																// 0x0000002E: �G�t�F�N�g�S�̃\�[�g�ǉ�
																// 0x0000002D: Header member �s��C��
																// 0x0000002C: �G�t�F�N�g�S�̎����ϐ��ǉ�
																// 0x0000002B: �œK���r�b�g�t�B�[���h�ǉ�
																// 0x0000002A: �t�B�[���h�ɃS�[���@�\�ǉ�
																// 0x00000029: �t�H�O�ǉ�
																// 0x00000028: Facing�^�C�v�ǉ�
																// 0x00000027: IDE�ɃK�C�h��`�悷��悤�ɏC���A�}�X�p�[�e�B�N���̐����`��ɖ@�������t���O��ǉ�
																// 0x00000026: �}�X�p�[�e�B�N���ɃJ���[�J�[�u���ǉ�����Ă����̂��폜
																// 0x00000025: �}�X�p�[�e�B�N���ɐe�̌p���֌W�A�m�C�Y�ɕp�x�A�������Ԃ�ǉ�
																// 0x00000024: �}�X�p�[�e�B�N���Ƀe�N�X�`���A�j���A�������s�[�gOn/Off�@�\�A
																// 0x00000023: �}�X�p�[�e�B�N���ɃA���t�@�A�j���[�V�����A�X�P�[�������_���A�J���[�����_���A�����`��A�����J�n���Ԃ�ǉ�
																// 0x00000022: �A���t�@�e�X�g�ǉ��A�}�X�p�[�e�B�N���ɐ[�x�o�b�t�@�������ݒǉ�
																// 0x00000021: �G�~�b�^��Z�\�[�g�ǉ�,�t�B�[���h�̕\���E��\����ǉ�
																// 0x00000020: �}�X�p�[�e�B�N���ǉ�
																// 0x0000001F: ���A�Q�ɃX�P�[���̉e����ǉ�
																// 0x0000001E: �����`��A�S�[���t�B�[���h�ɃX�P�[����ǉ�
																// 0x0000001D: �r������Đ�������@�\�ǉ�
																// 0x0000001C: �t�B�[���h�S�[���ǉ�
																// 0x0000001B: �t�B�[���h���A�Q�ǉ�
																// 0x0000001A: �k���G�~�b�^�Ƀu�����h�ǉ��A�p���֌W�ɃA���t�@��ǉ�
																// 0x00000019: �����R�[�h�ϊ��@�\�ǉ��A���W�n�ϊ��@�\�ǉ�
																// 0x00000018: �X�g���C�v�̊e�֐߂̃����_���𓯊��ł���悤�ɏC��
																// 0x00000017: �V���v���p�[�e�B�N���ǉ�
																// 0x00000016: �A���t�@�̃����_���A�F�`�����l���������ǉ�
																// 0x00000015: �����`�󃂃f�����ǉ�
																// 0x00000014: ���[�g�̃��[�U�[�f�[�^��ێ�����悤�ɂ���
																// 0x00000013: ���[�U�[�f�[�^���ǉ�
																// 0x00000012: �\�t�g�n���ǉ�
																// 0x00000011: �e�N�X�`�����C���[�Q���ǉ�
																// 0x00000010: �t�B�[���h�̕��ƃR���W��������U����
																// 0x0000000F: ���f���e�N�X�`���̃A�h���b�V���O�A�t�B���^���ǉ�
																// 0x0000000E: ���f���e�N�X�`���̃X�N���[�����ǉ�
																// 0x0000000D: �G�~�b�^�[�̏���ǉ�
																// 0x0000000C: �t�B�[���h�ǉ�
																// 0x0000000B: ���f���̏ڍ׍��ڒǉ�
		//
		DataFlag_is_little_endian	= 1<<0,
		RuntimeFlag_is_mapped		= 1<<0
	};

	struct EmitterInfo{
		EmitterTypeConst	type_;
		ml::ui32			resource_offset_;
	};

	// +0x00
	ml::ui32	sign_;
	ml::ui32	version_;
	ml::ui8		data_flag_;
	ml::ui8		runtime_flag_;
	ml::ui8		pad_8_[2];
	ml::ui32	node_offset_;

	// +0x10
	ml::ui32	texture_count_;
	ml::ui32	texture_path_offset_;
	ml::ui32	texture_name_offset_;
	ml::ui32	pad1_;

	// +0x20
	ml::ui32	model_count_;
	ml::ui32	model_path_offset_;
	ml::ui32	model_name_offset_;
	ml::ui32	pad2_;

	// +0x30
	ml::ui32	model_texture_count_;
	ml::ui32	model_texture_path_offset_;
	ml::ui32	model_texture_name_offset_;
	ml::ui32	pad3_;

	// +0x40
	ml::ui32	bmsln_count_;
	ml::ui32	bmsln_path_offset_;
	ml::ui32	bmsln_name_offset_;
	ml::ui32	pad4_;
	
	// +0x50
	ml::ui32	emitter_count_;
	ml::ui32	emitter_offset_;
	ml::ui32	pad5_;
	ml::ui32	pad6_;

	// +0x60
	ml::ui32	root_userdata_offset_;
	ml::ui32	root_advanced_frame_offset_;
	ml::si32	root_total_lifetime_offset_;
	ml::si32	root_whole_z_sort_offset_;

	// +0x70
	ml::ui32	root_version1_5_offset_;
	ml::ui32	root_life_offset_;
	ml::ui32	string_offset_;
	ml::ui32	binary_offset_;
};

ML_STATIC_ASSERT((sizeof(Header) & 15) == 0);

}	// namespace bmb
}	// namespace fileformat
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#include "../../utility/internal/bm3_MacroEnd.h"

#include "bm3_bmb_pack.h"

#endif	// BM3_SDK_INC_BM3_CORE_FILEFORMAT_BMB_H
