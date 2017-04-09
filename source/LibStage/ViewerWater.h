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
	void addPos( int x, int y, int z );		// 出現位置の指定
	void create( );							// 波の作成
	void setAmplitude( double index );		// 振幅の設定
	void setPeriodSpeed( double speed );	// 周期の速度
	void loadImage( const char* str );		// 使用する画像をロード
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
		float period;		// 周期
		float amplitude;	// 振幅

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

