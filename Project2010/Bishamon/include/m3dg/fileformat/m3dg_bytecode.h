#ifndef M3DG_FILEFORMAT_M3DG_BYTECODE_H
#define M3DG_FILEFORMAT_M3DG_BYTECODE_H

namespace m3dg{
namespace fileformat{

enum VertexFormatConst{
	//
	VertexFormatConst_BitP	= 1<<0,
	VertexFormatConst_BitN	= 1<<1,
	VertexFormatConst_BitC	= 1<<2,
	VertexFormatConst_BitT	= 1<<3,
	//
	VertexFormatConst_PN	= VertexFormatConst_BitP | VertexFormatConst_BitN,
	VertexFormatConst_PNT	= VertexFormatConst_BitP | VertexFormatConst_BitN                          | VertexFormatConst_BitT,
	VertexFormatConst_PNC	= VertexFormatConst_BitP | VertexFormatConst_BitN | VertexFormatConst_BitC,
	VertexFormatConst_PNCT	= VertexFormatConst_BitP | VertexFormatConst_BitN | VertexFormatConst_BitC | VertexFormatConst_BitT,

	//
	VertexFormatConst_FORCE32	= 0xFFFFFFFF
};

enum ByteCodeOpecodeConst{
	//																�T�C�Y	�v�f�̌^
	//																		F:float
	//																		I:Int
	//																		B:Binary
	//																		S:String
	//																		W:Work

	// �����I��
	ByteCodeOpecodeConst_End,									//	0

	// �}�e���A���֌W
	ByteCodeOpecodeConst_SetMaterial,							//	17		F,F,F,F	AmbientColor
																//			F,F,F,F	DiffuseColor
																//			F,F,F,F	SpecularColor
																//			F,F,F,F	EmissionColor
																//			F		Shininess

	// ���_�z��̕`��
	ByteCodeOpecodeConst_DrawArray_PN,							//	7		I,B,B,B,F,B,B
																//				0:���_��
																//				1:���_�C���f�b�N�X
																//				2:�ʒu�f�[�^
																//				3:�@���f�[�^
																//				4:���ׂĂ̎O�p�`�ʐ�
																//				5:�O�p�`���Z�ʐ�
																//				6:�o�E���f�B���O�X�t�B�A
	ByteCodeOpecodeConst_DrawArray_PNT,							//	9		I,B,B,B,B,S,F,B,B
																//				0:���_��
																//				1:���_�C���f�b�N�X
																//				2:�ʒu�f�[�^
																//				3:�@���f�[�^
																//				4:�e�N�X�`���f�[�^
																//				5:�e�N�X�`����
																//				6:���ׂĂ̎O�p�`�ʐ�
																//				7:�O�p�`���Z�ʐ�
																//				8:�o�E���f�B���O�X�t�B�A
	ByteCodeOpecodeConst_DrawArray_PNC,							//	8		I,B,B,B,B,F,B,B
																//				0:���_��
																//				1:���_�C���f�b�N�X
																//				2:�ʒu�f�[�^
																//				3:�@���f�[�^
																//				4:�J���[
																//				5:���ׂĂ̎O�p�`�ʐ�
																//				6:�O�p�`���Z�ʐ�
																//				7:�o�E���f�B���O�X�t�B�A
	ByteCodeOpecodeConst_DrawArray_PNCT,						//	10		I,B,B,B,B,S,B,F,B,B
																//				0:���_��
																//				1:���_�C���f�b�N�X
																//				2:�ʒu�f�[�^
																//				3:�@���f�[�^
																//				4:�J���[
																//				5:�e�N�X�`���f�[�^
																//				6:�e�N�X�`����
																//				7:���ׂĂ̎O�p�`�ʐ�
																//				8:�O�p�`���Z�ʐ�
																//				9:�o�E���f�B���O�X�t�B�A

	// �C���^�[���[�u���ꂽ���_�z��̕`��(DirectX9)
	ByteCodeOpecodeConst_DrawInterleavedArray_DirectX9,			//	9		I,I,B,B,I,S,F,B,B
																//				0:VertexFormatConst
																//				1:���_��
																//				2:���_�C���f�b�N�X
																//				3:���_�f�[�^
																//				4:���_�X�g���C�h
																//				5:�e�N�X�`����
																//				6:���ׂĂ̎O�p�`�ʐ�
																//				7:�O�p�`���Z�ʐ�
																//				8:�o�E���f�B���O�X�t�B�A

	// �C���^�[���[�u���ꂽ���_�z��o�b�t�@�ł̕`��(DirectX9)
	ByteCodeOpecodeConst_DrawInterleavedArrayBuffer_DirectX9,	//	11		I,I,B,B,I,S,W,W,F,B,B
																//				0:VertexFormatConst
																//				1:���_��
																//				2:���_�C���f�b�N�X
																//				3:���_�f�[�^
																//				4:���_�X�g���C�h
																//				5:�e�N�X�`����
																//				6:���_�o�b�t�@
																//				7:�C���f�b�N�X�o�b�t�@
																//				8:���ׂĂ̎O�p�`�ʐ�
																//				9:�O�p�`���Z�ʐ�
																//				10:�o�E���f�B���O�X�t�B�A
	//
	ByteCodeOpecodeConst_Max,
	//
	ByteCodeOpecodeConst_FORCE32		= 0xFFFFFFFF
};

enum ByteCodeOperandSizeConst{
	ByteCodeOperandSizeConst_End									= 0,
	ByteCodeOperandSizeConst_SetMaterial							= 17,
	ByteCodeOperandSizeConst_DrawArray_PN							= 7,
	ByteCodeOperandSizeConst_DrawArray_PNT							= 9,
	ByteCodeOperandSizeConst_DrawArray_PNC							= 8,
	ByteCodeOperandSizeConst_DrawArray_PNCT							= 10,
	ByteCodeOperandSizeConst_DrawInterleavedArray_DirectX9			= 9,
	ByteCodeOperandSizeConst_DrawInterleavedArrayBuffer_DirectX9	= 11
};

}	// namespace fileformat
}	// namespace m3dg

#endif	// #ifndef M3DG_FILEFORMAT_M3DG_BYTECODE_H
