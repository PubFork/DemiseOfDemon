#include "ViewerWater.h"
#include "Map.h"
#include <math.h>

ViewerWater::ViewerWater( MapConstPtr map )
: _sVertex( NULL )
, _vertex( NULL ) {
	// �v���O���}�u���V�F�[�_�[���f���̊m�F
	if ( GetValidShaderVersion( ) >= 200 ) {
		_shaderVersion = true;
		initialize( );
	} else {
		_shaderVersion = false;
	}
	_polygon_num = 0;

	loadImage( "resource/image/map/water.png" );

	for ( int i = 0; i < MAP_SIZE; i++ ) {
		for ( int j = 0; j < MAP_SIZE; j++ ) {
			int height = map->getWaterHeight( i, j );
			if ( height > 0 ) {
				addPos( i, height, j );
			}
		}
	}
	create( );
}

ViewerWater::~ViewerWater( ) {
	delete[ ] _vertex;
}

void ViewerWater::draw( ) {
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, 255 );	// ���ߓx�̐ݒ��L���ɂ���(���t���[���Ă�)

	if ( _shaderVersion ) {
		drawShader( );		// �V�F�[�_����
	} else {
		drawNoneShader( );	// �V�F�[�_�Ȃ�
	}

	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );	// ���ߓx�̐ݒ��L���ɂ���(���t���[���Ă�)
}

// �o���ʒu�̎w��
void ViewerWater::addPos( int x, int y, int z ) {
	_array.push_back( Vector( x, y, z ) );
}

// �g�̍쐬
void ViewerWater::create( ) {
	
	allocate( );	// �������[�m��

	_polygon_num = ( int )_array.size( ) * 2;	// �|���S����

	for ( int i = 0; i < ( int )_array.size( ); i++ ) {
		Vector plankPos[ 4 ];
		Vector vertex[ 6 ];
		
		double x = _array.at( i ).x;
		double y = _array.at( i ).y;
		double z = _array.at( i ).z;

		// �ʒu
		plankPos[ 0 ] = Vector( x + 0, y, z + 0 );
		plankPos[ 1 ] = Vector( x + 0, y, z + 1 );
		plankPos[ 2 ] = Vector( x + 1, y, z + 0 );
		plankPos[ 3 ] = Vector( x + 1, y, z + 1 );

		// �|���S���̌`��
		vertex[ 0 ] = plankPos[ 0 ];
		vertex[ 1 ] = plankPos[ 1 ];
		vertex[ 2 ] = plankPos[ 2 ];
			      			      
		vertex[ 3 ] = plankPos[ 1 ];
		vertex[ 4 ] = plankPos[ 3 ];
		vertex[ 5 ] = plankPos[ 2 ];

		setPlank( i * 6, vertex );
	}

	_array.clear( );	// �o�^�����z����폜
}

// �U���̐ݒ�
void ViewerWater::setAmplitude( double index ) {
	_parameter.amplitude = ( float )index;
}

// �����̑��x
void ViewerWater::setPeriodSpeed( double speed ) {
	_speed = ( float )speed;
}

// �g�p����摜�����[�h
void ViewerWater::loadImage( const char* str ) {
	_image = LoadGraph( str );
}

void ViewerWater::initialize( ) {
	_vertexFile = LoadVertexShader( "resource/shader/WaveVS.vso" );	// ���_�V�F�[�_�[��ǂݍ���
	
	_pixelFile = LoadPixelShader( "resource/shader/WavePS.pso" );	// �s�N�Z���V�F�[�_�[��ǂݍ���

	GetWindowSize( &_sizeX, &_sizeY );

	_time = 0.0f;

	_speed = 1;
}

void ViewerWater::setPlank( int num, Vector* vertex ) {
	for ( int i = 0; i < 6; i++ ) {
				
		// �e�N�X�`���[UV
		int u[ 6 ] = { 0, 1, 0, 1, 1, 0 };
		int v[ 6 ] = { 0, 0, 1, 0, 1, 1 };
		
		if ( _shaderVersion ) {
			_sVertex[ num + i ].pos  = VGet( ( float )vertex[ i ].x, ( float )vertex[ i ].y, ( float )vertex[ i ].z );
			_sVertex[ num + i ].norm = VGet( 0.0f, 0.0f, 0.0f );
			_sVertex[ num + i ].dif  = GetColorU8( 255, 255, 255, 255 );
			_sVertex[ num + i ].spc  = GetColorU8( 0, 0, 0, 0 );
			_sVertex[ num + i ].u    = ( float )u[ i ];
			_sVertex[ num + i ].v    = ( float )v[ i ];
			_sVertex[ num + i ].su   = 0.0f;
			_sVertex[ num + i ].sv   = 0.0f;
		} else {
			_vertex[ num + i ].pos  = VGet( ( float )vertex[ i ].x, ( float )vertex[ i ].y, ( float )vertex[ i ].z );
			_vertex[ num + i ].norm = VGet( 0.0f, 0.0f, 0.0f );
			_vertex[ num + i ].dif  = GetColorU8( 255, 255, 255, 255 );
			_vertex[ num + i ].spc  = GetColorU8( 0, 0, 0, 0 );
			_vertex[ num + i ].u    = ( float )u[ i ];
			_vertex[ num + i ].v    = ( float )v[ i ];
			_vertex[ num + i ].su   = 0.0f;
			_vertex[ num + i ].sv   = 0.0f;
		}
	}
}

void ViewerWater::setVertexRegister( ) {
	_parameter.period += 0.01f * _speed;
	_parameter.period = fmod( _parameter.period, ( float )PI2 );
	SetVSConstSF( 0, _parameter.period );
	SetVSConstSF( 1, _parameter.amplitude );
}

void ViewerWater::setPixelRegister( ) {
	_time += 0.01f;
	SetPSConstSF( 0, _time );
	SetPSConstSF( 1, ( float )_sizeX );
	SetPSConstSF( 2, ( float )_sizeY );
}

void ViewerWater::setTexture( ) {
	if ( _image != -1 ) {
		SetPSConstSF( 3, ( float )TRUE );
	} else {
		SetPSConstSF( 3, ( float )FALSE );
	}
	SetUseTextureToShader( 0, _image );
}
void ViewerWater::drawNoneShader( ) {
	if ( _image != -1 ) {
		DrawPolygon3D( _vertex ,_polygon_num, _image, TRUE );
	} else {
		DrawPolygon3D( _vertex ,_polygon_num, DX_NONE_GRAPH, TRUE );
	}
}

void ViewerWater::drawShader( ) {
	// �g�p���钸�_�V�F�[�_�[���Z�b�g
	SetUseVertexShader( _vertexFile );
	SetUsePixelShader( _pixelFile );

	// ���W�X�^�[�̃Z�b�g
	setVertexRegister( );
	setPixelRegister( );

	// �e�N�X�`���[�̃Z�b�g
	setTexture( );

	//�@�`��
	DrawPolygon3DToShader( _sVertex, _polygon_num );

	// �V�F�[�_�[�t�@�C���̃��Z�b�g
	SetUseVertexShader( -1 );
	SetUsePixelShader( -1 );
}

void ViewerWater::allocate( ) {
	if ( _shaderVersion ) {
		if ( _sVertex != NULL ) delete[ ] _sVertex;
		_sVertex = new VERTEX3DSHADER[ ( int )_array.size( ) * 6 ];
	} else {
		if ( _vertex != NULL ) delete[ ] _vertex;
		_vertex = new VERTEX3D[ ( int )_array.size( ) * 6 ];
	}
}