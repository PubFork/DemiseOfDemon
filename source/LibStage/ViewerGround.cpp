#include "ViewerGround.h"
#include "Map.h"
#include "Model.h"
#include "dxlib.h"

const int TEXNUM_PLAIN =  0;
const int TEXNUM_BENT  =  2;
const int TEXNUM_STONE = 48;
const int TEXNUM_CLIFF_TOP = 6;
const int TEXNUM_CLIFF_MID = 4;
const int TEXNUM_CLIFF_BTM = 22;
const int TEXNUM_VINE_ONE = 20;
const int TEXNUM_VINE_TOP = 38;
const int TEXNUM_VINE_MID = 36;
const int TEXNUM_VINE_BTM = 18;

class ModelCreator {
public:
	ModelCreator( ) { }
	virtual ~ModelCreator( ) { }
	void create(

		MapConstPtr map,
		Model * model ) {
		
		_map = map;
		_model = model;
		_data = new DATA[ MAP_SIZE * MAP_SIZE ];

		createAllData( );
		createVertex( );

		delete [] _data;
	}
private:
	struct DATA {
		Vector pos[ 4 * ( 30 + 1 ) ];
		int wall[ 4 ];
		int height;
		int tnum_top;
		int tnum_top_dir;
		int tnum_wall[ 4 ][ 30 ];
		int vine[ 4 ];
	} * _data;
	
	MapConstPtr _map;
	Model * _model;

	int _vertex_pos;
	
private:
	void setTNumTop( ) {
		// ëêÇÇµÇ´Ç¬ÇﬂÇÈ
		for ( int x = 0; x < MAP_SIZE; x++ ) {
			for ( int z = 0; z < MAP_SIZE; z++ ) {
				int tn = TEXNUM_PLAIN;
				if ( GetRand( 10 ) == 0 ) {
					tn = TEXNUM_BENT;
				}
				setTNumTopOne( x, z, tn + ( z % 2 ) + ( x / 2 % 2 ) * 8, 0 );
			}
		}
		
		// ï‡ÇØÇÈÇ∆Ç±ÇÎÇÕêŒèÙÇ…Ç∑ÇÈ
		for ( int x = 0; x < MAP_SIZE; x++ ) {
			for ( int z = x % 2; z < MAP_SIZE; z++ ) {
				if ( _map->getAttribute( x, z ) == ATTRIBUTE_GRASS ) {
					continue;
				}
				if ( !isFlat( x, z ) ) {
					continue;
				}
				if ( isStone( x, z ) ) {
					continue;
				}
				// Ç®ÇØÇÈÇ©ÅH
				if ( GetRand( 1 ) ) {
					if ( isFlat( x, z + 1 ) ) {
						if ( !setStoneH( x, z ) ) {
							setStoneV( x, z );
						}
					}
				} else {
					if ( isFlat( x + 1, z ) ) {
						if ( !setStoneV( x, z ) ) {
							setStoneH( x, z );
						}
					}
				}
			}
		}
	}
	
	void setTNumTopOne( int x, int z, int tnum, int dir ) {
		if ( x < 0 || x >= MAP_SIZE ||
			 z < 0 || z >= MAP_SIZE ) return;

		_data[ x + z * MAP_SIZE ].tnum_top_dir = dir;
		_data[ x + z * MAP_SIZE ].tnum_top = tnum;
	}

	bool setStoneH( int x, int z ) {
		if ( _map->getHeight( x, z ) != _map->getHeight( x, z + 1 ) ||
			 _map->getAttribute( x, z + 1 ) == ATTRIBUTE_GRASS ||
			 isStone( x, z + 1 ) ) {
			return false;
		}

		int tnum = TEXNUM_STONE;
		if ( GetRand( 10 ) == 0 ) {
			tnum += GetRand( 7 );
		}
		setTNumTopOne( x, z + 0, tnum    , 3 );
		setTNumTopOne( x, z + 1, tnum + 8, 3 );
		return true;
	}
	
	bool setStoneV( int x, int z  ) {
		if ( _map->getHeight( x, z ) != _map->getHeight( x + 1, z ) ||
			 _map->getAttribute( x + 1, z ) == ATTRIBUTE_GRASS ||
			 isStone( x + 1, z ) ) {
			return false;
		}
		int tnum = TEXNUM_STONE;
		if ( GetRand( 10 ) == 0 ) {
			tnum += GetRand( 7 );
		}
		setTNumTopOne( x + 0, z, tnum    , 0 );
		setTNumTopOne( x + 1, z, tnum + 8, 0 );
		return true;
	}

	bool isStone( int x, int z ) {
		if ( x < 0 || x >= MAP_SIZE ||
			 z < 0 || z >= MAP_SIZE ) return false;
		return _data[ x + z * MAP_SIZE ].tnum_top >= TEXNUM_STONE;
	}
	
	bool isFlat( int x, int z ) {
		int height = _map->getHeight( x, z );
		for ( int i = 0; i < 9; i++ ) {
			int mx = x + i % 3 - 1;
			int mz = z + i / 3 - 1;
			if ( height < _map->getHeight( mx, mz ) ) { 
				return false;
			}
		}

		return true;
	}

	void setTNumWall( ) {
		for ( int x = 0; x < MAP_SIZE; x++ ) {
			for ( int z = 0; z < MAP_SIZE; z++ ) {
				DATA &data = _data[ x + z * MAP_SIZE ];
				for ( int j = 0; j < 4; j++ ) {
					for ( int i = 0; i < data.wall[ j ]; i++ ) {
						int tnum = TEXNUM_CLIFF_BTM;
						if ( i < data.wall[ j ] - 1 ) {
							if ( i == data.vine[ j ] - 1 ) {
								tnum = TEXNUM_VINE_BTM;
							} else if ( i < data.vine[ j ] - 1 ) {
								tnum = TEXNUM_VINE_MID;
							} else {
								tnum = TEXNUM_CLIFF_MID;
							}
						}
						if ( i == 0 ) {
							if ( data.vine[ j ] == 1 ) {
								tnum = TEXNUM_VINE_ONE;
							} else if ( data.vine[ j ] > 1 ) {
								tnum = TEXNUM_VINE_TOP;
							} else {
								tnum = TEXNUM_CLIFF_TOP;
							}
						}
						data.tnum_wall[ j ][ i ] = tnum + ( z + x + j % 2 ) % 2 + ( i + data.height ) % 2 * 8;
					}
				}
			}
		}
	}

	void createAllData( ) {
		for ( int x = 0; x < MAP_SIZE; x++ ) {
			for ( int z = 0; z < MAP_SIZE; z++ ) {
				createData( x, z );
			}
		}
		setVive( );
		setTNumTop( );
		setTNumWall( );
	}

	void setVive( ) {
		for ( int x = 0; x < MAP_SIZE - 1; x += 1 ) {
			for ( int z = 0; z < MAP_SIZE - 1; z += 1 ) {
				for ( int j = 0; j < 4; j++ ) {
					if ( ( x + z ) % 2 == 0 ) continue;
					if ( GetRand( 10 ) ) continue;
					int wall = _data[ x + z * MAP_SIZE ].wall[ j ];
					if ( wall > _data[ x + j % 2 + ( z + 1 - j % 2 ) * MAP_SIZE ].wall[ j ] ) {
						wall = _data[ x + j % 2 + ( z + 1 - j % 2 ) * MAP_SIZE ].wall[ j ];
					}
					if ( wall > 0 ) {
						int v = GetRand( wall - 1 );
						_data[ x + z * MAP_SIZE ].vine[ j ] = v;
						_data[ x + j % 2 + ( z + 1 - j % 2 ) * MAP_SIZE ].vine[ j ] = v;
					}
				}
			}
		}
	}

	void createData( int x, int z ) {
		DATA &data = _data[ x + z * MAP_SIZE ];

		data.height = _map->getHeight( x, z );

		data.wall[ 0 ] = data.height - _map->getHeight( x - 1, z );
		data.wall[ 1 ] = data.height - _map->getHeight( x, z - 1 );
		data.wall[ 2 ] = data.height - _map->getHeight( x, z + 1 );
		data.wall[ 3 ] = data.height - _map->getHeight( x + 1, z );

		for ( int i = 0; i < 4; i++ ) {
			if ( data.wall[ i ] < 0 ) {
				data.wall[ i ] = 0;
			}
		}

		for ( int j = 0; j < 4; j++ ) {
			for ( int i = 0; i < 30; i++ ) {
				data.pos[ j + i * 4 ] = Vector( x + j / 2, data.height - i, z + j % 2 );
			}
			data.vine[ j ] = 0;
		}
	}

	void createVertex( ) {
		int polygon_num = 0;
		for ( int i = 0; i < MAP_SIZE * MAP_SIZE; i++ ) {
			if ( _data[ i ].height == 0 ) {
				continue;
			}
			polygon_num += 2; // TOP
			for ( int j = 0; j < 4; j++ ) {
				polygon_num += _data[ i ].wall[ j ] * 2; // WALL
			}
		}

		_model->alloc( polygon_num );

		_vertex_pos = 0;
		
		for ( int i = 0; i < MAP_SIZE * MAP_SIZE; i++ ) {
			if ( _data[ i ].height == 0 ) {
				continue;
			}

			DATA& data = _data[ i ];

			createTopPlane( data );
			createWallPlane( data );
		}
	}

	void createTopPlane( const DATA& data ) {
		Vector pos[ 4 ] = {
			data.pos[ 0 ], data.pos[ 1 ], data.pos[ 2 ], data.pos[ 3 ],
		};

		Model::VERTEX vertex[ 4 ];
		for ( int i = 0; i < 4; i++ ) {
			switch ( data.tnum_top_dir ) {
			case 0:
				vertex[ i ] = Model::VERTEX( pos[ i ], data.tnum_top % 8 * 0.125f + i % 2         * 0.12499f, data.tnum_top / 8 * 0.125f + i / 2         * 0.12499f );
				break;
			case 1:
				vertex[ i ] = Model::VERTEX( pos[ i ], data.tnum_top % 8 * 0.125f + i / 2         * 0.12499f, data.tnum_top / 8 * 0.125f + ( 1 - i % 2 ) * 0.12499f );
				break;
			case 2:
				vertex[ i ] = Model::VERTEX( pos[ i ], data.tnum_top % 8 * 0.125f + ( 1 - i % 2 ) * 0.12499f, data.tnum_top / 8 * 0.125f + ( 1 - i / 2 ) * 0.12499f );
				break;
			case 3:
				vertex[ i ] = Model::VERTEX( pos[ i ], data.tnum_top % 8 * 0.125f + ( 1 - i / 2 ) * 0.12499f, data.tnum_top / 8 * 0.125f + i % 2         * 0.12499f );
				break;
			}
		}
		_model->set( _vertex_pos++, vertex[ 0 ] );
		_model->set( _vertex_pos++, vertex[ 1 ] );
		_model->set( _vertex_pos++, vertex[ 2 ] );
		_model->set( _vertex_pos++, vertex[ 1 ] );
		_model->set( _vertex_pos++, vertex[ 3 ] );
		_model->set( _vertex_pos++, vertex[ 2 ] );
	}
	
	void createWallPlane( const DATA& data ) {
		for ( int i = 0; i < data.wall[ 0 ]; i++ ) createWallPlaneLeft( data, i );
		for ( int i = 0; i < data.wall[ 1 ]; i++ ) createWallPlaneRight( data, i );
		for ( int i = 0; i < data.wall[ 2 ]; i++ ) createWallPlaneBackLeft( data, i );
		for ( int i = 0; i < data.wall[ 3 ]; i++ ) createWallPlaneBackRight( data, i );
	}

	void createWallPlaneLeft( const DATA& data, int hc ) {
		Vector pos[ 4 ] = {
			data.pos[ 1 + hc * 4 ], data.pos[ 0 + hc * 4 ], data.pos[ 5 + hc * 4 ], data.pos[ 4 + hc * 4 ],
		};
		createWallPlaneOne( pos, data.tnum_wall[ 0 ][ hc ] );
	}

	void createWallPlaneRight( const DATA& data, int hc ) {
		Vector pos[ 4 ] = {
			data.pos[ 0 + hc * 4 ], data.pos[ 2 + hc * 4 ], data.pos[ 4 + hc * 4 ], data.pos[ 6 + hc * 4 ],
		};
		createWallPlaneOne( pos, data.tnum_wall[ 1 ][ hc ] );
	}
	
	void createWallPlaneBackLeft( const DATA& data, int hc ) {
		Vector pos[ 4 ] = {

			data.pos[ 3 + hc * 4 ], data.pos[ 1 + hc * 4 ], data.pos[ 7 + hc * 4 ], data.pos[ 5 + hc * 4 ],
		};
		createWallPlaneOne( pos, data.tnum_wall[ 2 ][ hc ] );
	}

	void createWallPlaneBackRight( const DATA& data, int hc ) {
		Vector pos[ 4 ] = {
			data.pos[ 2 + hc * 4 ], data.pos[ 3 + hc * 4 ], data.pos[ 6 + hc * 4 ], data.pos[ 7 + hc * 4 ],
		};
		createWallPlaneOne( pos, data.tnum_wall[ 3 ][ hc ] );
	}
	
	void createWallPlaneOne( Vector pos[ 4 ], int tnum ) {
		Model::VERTEX vertex[ 4 ];
		for ( int i = 0; i < 4; i++ ) {
			vertex[ i ] = Model::VERTEX( pos[ i ], ( tnum % 8 + i % 2 ) * 0.125f, ( tnum / 8 + i / 2 ) * 0.125f );
		}
		_model->set( _vertex_pos++, vertex[ 0 ] );
		_model->set( _vertex_pos++, vertex[ 1 ] );
		_model->set( _vertex_pos++, vertex[ 2 ] );
		_model->set( _vertex_pos++, vertex[ 1 ] );
		_model->set( _vertex_pos++, vertex[ 3 ] );
		_model->set( _vertex_pos++, vertex[ 2 ] );
	}
};

ViewerGround::ViewerGround( MapConstPtr map, bool outter ) {
	const char * FILE[ 2 ] = {
		"resource/image/map/ground_inner.png",
		"resource/image/map/ground_outter.png",
	};
	ModelCreator( ).create( map, this );
	int n = 0;
	if ( outter ) {
		n = 1;
	}
	int id = LoadGraph( FILE[ n ] );
	setTexture( id );
}

ViewerGround::~ViewerGround( ) {
}
