#pragma once

#include "smart_ptr.h"
#include "mathmatics.h"
#include "DxLib.h"
#include <vector>

PTR( Water );
PTR( Map );

class ViewerWater {
public:
	ViewerWater( MapConstPtr map );
	virtual ~ViewerWater( );
public:
	void draw( );
private:
	void addPos( int x, int y, int z );		// �o���ʒu�̎w��
	void create( );							// �g�̍쐬
	void setAmplitude( double index );		// �U���̐ݒ�
	void setPeriodSpeed( double speed );	// �����̑��x
	void loadImage( const char* str );		// �g�p����摜�����[�h
	void initialize( );
	void setPlank( int num, Vector* vertex );
	void setVertexRegister( );
	void setPixelRegister( );
	void setTexture( );
	void drawNoneShader( );
	void drawShader( );
	void allocate( );
private:
	struct Parameter {
		float period;		// ����
		float amplitude;	// �U��

		Parameter( ) {
			period = 0;
			amplitude = 0.2f;
		}
	};
private:
	VERTEX3DSHADER* _sVertex;
	VERTEX3D* _vertex;
	Parameter _parameter;
	std::vector< Vector > _array;
	bool _shaderVersion;
	int _image;
	int _vertexFile;
	int _pixelFile;
	int _polygon_num;
	int _sizeX;
	int _sizeY;
	float _time;
	float _speed;
};

