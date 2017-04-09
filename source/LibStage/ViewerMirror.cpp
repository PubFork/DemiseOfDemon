#include "ViewerMirror.h"

// ���ɉf��f���̎擾�Ɏg�p����X�N���[���̉𑜓x
const int MIRROR_SCREEN_W = 1920;
const int MIRROR_SCREEN_H = 1280;

ViewerMirror::ViewerMirror( ) {

	// ���������s�����߂̒��_�V�F�[�_�[�ƃs�N�Z���V�F���[��ǂݍ���
	_vertex_shader = LoadVertexShader( "resource/shader/MirrorVS.vso" );
	_pixel_shader = LoadPixelShader( "resource/shader/MirrorPS.pso" );

	// ���ɉf��f���̎擾�Ɏg�p����X�N���[���̍쐬
	_handle = MakeScreen( MIRROR_SCREEN_W, MIRROR_SCREEN_H, FALSE );
}


ViewerMirror::~ViewerMirror( ) {
}

void ViewerMirror::setupCamera( VECTOR CameraEyePos, VECTOR CameraTargetPos )
{
	float EyeLength, TargetLength;
	VECTOR MirrorNormal;
	VECTOR *MirrorWorldPosP;
	VECTOR MirrorCameraEyePos, MirrorCameraTargetPos;

	MirrorWorldPosP = _world_pos;

	// ���̖ʂ̖@�����Z�o
	MirrorNormal = VNorm( VCross( VSub( MirrorWorldPosP[ 1 ], MirrorWorldPosP[ 0 ] ), VSub( MirrorWorldPosP[ 2 ], MirrorWorldPosP[ 0 ] ) ) );

	// ���̖ʂ���J�����̍��W�܂ł̍ŒZ�����A���̖ʂ���J�����̒����_�܂ł̍ŒZ�������Z�o
	EyeLength    = Plane_Point_MinLength( MirrorWorldPosP[ 0 ], MirrorNormal, CameraEyePos    );
	TargetLength = Plane_Point_MinLength( MirrorWorldPosP[ 0 ], MirrorNormal, CameraTargetPos );

	// ���ɉf��f����`�悷��ۂɎg�p����J�����̍��W�ƃJ�����̒����_���Z�o
	MirrorCameraEyePos    = VAdd( CameraEyePos,    VScale( MirrorNormal, -EyeLength    * 2.0f ) );
	MirrorCameraTargetPos = VAdd( CameraTargetPos, VScale( MirrorNormal, -TargetLength * 2.0f ) );

	// �v�Z�œ���ꂽ�J�����̍��W�ƃJ�����̒����_�̍��W���J�����̐ݒ肷��
	SetCameraPositionAndTarget_UpVecY( MirrorCameraEyePos, MirrorCameraTargetPos );

	// ���ɉf��f���̒��ł̋��̎l���̍��W���Z�o( �������W )
	for( int i = 0; i < 4; i ++ )
	{
		_screen_pos[ i ] = ConvWorldPosToScreenPosPlusW( MirrorWorldPosP[ i ] );
	}
}

// ���W�w��p�֐�
void ViewerMirror::setPos( Vector pos, int width, int depth ) {
	_world_pos[ 0 ] = VGet( ( float )pos.x, ( float )pos.y, ( float )pos.z + depth );
	_world_pos[ 1 ] = VGet( ( float )pos.x + width, ( float )pos.y, ( float )pos.z + depth );
	_world_pos[ 2 ] = VGet( ( float )pos.x, ( float )pos.y, ( float )pos.z );
	_world_pos[ 3 ] = VGet( ( float )pos.x + width, ( float )pos.y, ( float )pos.z );

}


void ViewerMirror::render( ) {

	// ���̕`��Ɏg�p���钸�_�̃Z�b�g�A�b�v
	for( int i = 0; i < 4; i ++ )
	{
		// ���W���Z�b�g
		_vertex[ i ].pos = _world_pos[ i ];

		// Diffuse Color ���Z�b�g
		_vertex[ i ].dif = GetColorU8( 200, 200, 200, 150 );

		// �⏕���W�ɋ��ɉf�鎋�_�ł̎ˉe���W��������
		_vertex[ i ].spos.x = _screen_pos[ i ].x / MIRROR_SCREEN_W;
		_vertex[ i ].spos.y = _screen_pos[ i ].y / MIRROR_SCREEN_H;
		_vertex[ i ].spos.z = _screen_pos[ i ].z;
		_vertex[ i ].spos.w = _screen_pos[ i ].w;
	}
	_vertex[ 4 ] = _vertex[ 2 ];
	_vertex[ 5 ] = _vertex[ 1 ];

	// �`��u�����h���[�h���Z�b�g
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, 255 ) ;

	// �g�p���钸�_�V�F�[�_�[���Z�b�g
	SetUseVertexShader( _vertex_shader );

	// �g�p����s�N�Z���V�F�[�_�[���Z�b�g
	SetUsePixelShader( _pixel_shader );

	// �g�p����e�N�X�`�����Z�b�g
	SetUseTextureToShader( 0, _handle );

	// ����`��
	DrawPolygon3DToShader( _vertex, 2 );

	// �`��u�����h���[�h�����ɖ߂�
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 ) ;

	// �V�F�[�_�[������
	SetUseVertexShader( -1 );
	SetUsePixelShader( -1 );

}

void ViewerMirror::drawBegin( const Vector eye, const Vector focus ) {
	// �J�����̐ݒ�
	_eye = VGet( ( float )eye.x, ( float )eye.y, ( float )eye.z );
	_focus = VGet( ( float )focus.x, ( float )focus.y, ( float )focus.z );

	// �`�������ɉf��f���̎擾�Ɏg�p����摜��`�悷��X�N���[���ɂ���
	SetDrawScreen( _handle );
	ClearDrawScreen( );

	// ���ɉf��f����`�悷��ۂɎg�p����J�����̐ݒ���s��
	setupCamera( _eye, _focus );
}

void ViewerMirror::drawEnd( ) {
	// ����ʂ�`��Ώۂɂ���
	SetDrawScreen( DX_SCREEN_BACK );

	// �J���������ɖ߂�
	SetCameraNearFar( 10.0f, 200.0f );
	SetCameraPositionAndTarget_UpVecY( VGet( ( float )_eye.x, ( float )_eye.y, ( float )_eye.z ), VGet( ( float )_focus.x, ( float )_focus.y, ( float )_focus.z ) );
	// �`�悷��
	render( );
}
