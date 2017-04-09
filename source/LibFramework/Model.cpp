#include "Model.h"
#include "dxlib.h"

class ModelImpl {
public:
	VERTEX3D *_vertex;
	int _polygon_num;
	int _texture;
};

Model::Model( ) {
	_impl = ModelImplPtr( new ModelImpl );

	_impl->_vertex = NULL;
	_impl->_polygon_num = 0;
}

Model::~Model( ) {
	if ( _impl->_vertex ) {
		delete [] _impl->_vertex;
	}
}

void Model::setTexture( int id ) {
	_impl->_texture = id;
}

void Model::alloc( int polygon_num ) {
	_impl->_polygon_num = polygon_num;
	if ( _impl->_vertex != NULL ) {
		delete [] _impl->_vertex;
	}
	_impl->_vertex = new VERTEX3D[ polygon_num * 3 ];
}

void Model::setPolygonNum( int num ) {
	_impl->_polygon_num = num;
}

void Model::draw( bool trans ) const {
	DrawPolygon3D( _impl->_vertex, _impl->_polygon_num, _impl->_texture, trans ? TRUE : FALSE );
}

void Model::translate( Vector move ) {
	for ( int i = 0; i < _impl->_polygon_num * 3; i++ ) {
		_impl->_vertex[ i ].pos.y += ( float )move.x;
		_impl->_vertex[ i ].pos.y += ( float )move.y;
		_impl->_vertex[ i ].pos.y += ( float )move.z;
	}
}

void Model::set( int n, VERTEX vertex ) {

	VERTEX3D vtx;
	vtx.pos = VGet( ( float )vertex.pos.x, ( float )vertex.pos.y, ( float )vertex.pos.z );
	vtx.norm = VGet( 0, 0, 0 );
	vtx.dif  = GetColorU8( 255, 255, 255, 255 );
	vtx.spc  = GetColorU8( 0, 0, 0, 0 );
	vtx.u    = ( float )vertex.u;
	vtx.v    = ( float )vertex.v;
	vtx.su   = 0.0f;
	vtx.sv   = 0.0f;

	_impl->_vertex[ n ] = vtx;
}