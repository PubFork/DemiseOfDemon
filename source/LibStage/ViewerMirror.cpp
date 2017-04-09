#include "ViewerMirror.h"

// 鏡に映る映像の取得に使用するスクリーンの解像度
const int MIRROR_SCREEN_W = 1920;
const int MIRROR_SCREEN_H = 1280;

ViewerMirror::ViewerMirror( ) {

	// 鏡処理を行うための頂点シェーダーとピクセルシェだーを読み込む
	_vertex_shader = LoadVertexShader( "resource/shader/MirrorVS.vso" );
	_pixel_shader = LoadPixelShader( "resource/shader/MirrorPS.pso" );

	// 鏡に映る映像の取得に使用するスクリーンの作成
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

	// 鏡の面の法線を算出
	MirrorNormal = VNorm( VCross( VSub( MirrorWorldPosP[ 1 ], MirrorWorldPosP[ 0 ] ), VSub( MirrorWorldPosP[ 2 ], MirrorWorldPosP[ 0 ] ) ) );

	// 鏡の面からカメラの座標までの最短距離、鏡の面からカメラの注視点までの最短距離を算出
	EyeLength    = Plane_Point_MinLength( MirrorWorldPosP[ 0 ], MirrorNormal, CameraEyePos    );
	TargetLength = Plane_Point_MinLength( MirrorWorldPosP[ 0 ], MirrorNormal, CameraTargetPos );

	// 鏡に映る映像を描画する際に使用するカメラの座標とカメラの注視点を算出
	MirrorCameraEyePos    = VAdd( CameraEyePos,    VScale( MirrorNormal, -EyeLength    * 2.0f ) );
	MirrorCameraTargetPos = VAdd( CameraTargetPos, VScale( MirrorNormal, -TargetLength * 2.0f ) );

	// 計算で得られたカメラの座標とカメラの注視点の座標をカメラの設定する
	SetCameraPositionAndTarget_UpVecY( MirrorCameraEyePos, MirrorCameraTargetPos );

	// 鏡に映る映像の中での鏡の四隅の座標を算出( 同次座標 )
	for( int i = 0; i < 4; i ++ )
	{
		_screen_pos[ i ] = ConvWorldPosToScreenPosPlusW( MirrorWorldPosP[ i ] );
	}
}

// 座標指定用関数
void ViewerMirror::setPos( Vector pos, int width, int depth ) {
	_world_pos[ 0 ] = VGet( ( float )pos.x, ( float )pos.y, ( float )pos.z + depth );
	_world_pos[ 1 ] = VGet( ( float )pos.x + width, ( float )pos.y, ( float )pos.z + depth );
	_world_pos[ 2 ] = VGet( ( float )pos.x, ( float )pos.y, ( float )pos.z );
	_world_pos[ 3 ] = VGet( ( float )pos.x + width, ( float )pos.y, ( float )pos.z );

}


void ViewerMirror::render( ) {

	// 鏡の描画に使用する頂点のセットアップ
	for( int i = 0; i < 4; i ++ )
	{
		// 座標をセット
		_vertex[ i ].pos = _world_pos[ i ];

		// Diffuse Color をセット
		_vertex[ i ].dif = GetColorU8( 200, 200, 200, 150 );

		// 補助座標に鏡に映る視点での射影座標を代入する
		_vertex[ i ].spos.x = _screen_pos[ i ].x / MIRROR_SCREEN_W;
		_vertex[ i ].spos.y = _screen_pos[ i ].y / MIRROR_SCREEN_H;
		_vertex[ i ].spos.z = _screen_pos[ i ].z;
		_vertex[ i ].spos.w = _screen_pos[ i ].w;
	}
	_vertex[ 4 ] = _vertex[ 2 ];
	_vertex[ 5 ] = _vertex[ 1 ];

	// 描画ブレンドモードをセット
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, 255 ) ;

	// 使用する頂点シェーダーをセット
	SetUseVertexShader( _vertex_shader );

	// 使用するピクセルシェーダーをセット
	SetUsePixelShader( _pixel_shader );

	// 使用するテクスチャをセット
	SetUseTextureToShader( 0, _handle );

	// 鏡を描画
	DrawPolygon3DToShader( _vertex, 2 );

	// 描画ブレンドモードを元に戻す
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 ) ;

	// シェーダーを消す
	SetUseVertexShader( -1 );
	SetUsePixelShader( -1 );

}

void ViewerMirror::drawBegin( const Vector eye, const Vector focus ) {
	// カメラの設定
	_eye = VGet( ( float )eye.x, ( float )eye.y, ( float )eye.z );
	_focus = VGet( ( float )focus.x, ( float )focus.y, ( float )focus.z );

	// 描画先を鏡に映る映像の取得に使用する画像を描画するスクリーンにする
	SetDrawScreen( _handle );
	ClearDrawScreen( );

	// 鏡に映る映像を描画する際に使用するカメラの設定を行う
	setupCamera( _eye, _focus );
}

void ViewerMirror::drawEnd( ) {
	// 裏画面を描画対象にする
	SetDrawScreen( DX_SCREEN_BACK );

	// カメラを元に戻す
	SetCameraNearFar( 10.0f, 200.0f );
	SetCameraPositionAndTarget_UpVecY( VGet( ( float )_eye.x, ( float )_eye.y, ( float )_eye.z ), VGet( ( float )_focus.x, ( float )_focus.y, ( float )_focus.z ) );
	// 描画する
	render( );
}
