#include "ModuleEditor.h"
#include "framework.h"
#include "CameraMapEditor.h"
#include "Grid.h"
#include "Map.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "MEObject.h"
#include "MapEditor.h"

ModuleEditor::ModuleEditor( MapPtr map, MEObjectPtr object )
: ModuleBase( map ) {
	_grid = GridPtr( new Grid( _map ) );
	_object = object;

	_last_mouse_x = 0;
	_last_mouse_y = 0;

	_mode = MODE_HEIGHT;
	_height = 11;
	_range = 1;
	_attribute = ATTRIBUTE_STONE;

	_grid_height = 10.0;
	_grid->setHeight( _grid_height );
	_object->setHeight( _grid_height );
}


ModuleEditor::~ModuleEditor( ) {

}

void ModuleEditor::update( ) {

	KeyboardPtr keyboard = Keyboard::getTask( );
	MousePtr mouse = Mouse::getTask( );
	MapEditorPtr editor = MapEditor::getTask( );
	CameraMapEditorPtr camera = editor->getCamera( );

	// 情報
	const char *KEY_NUMBER[ 20 ] = {
		"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", 
		"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P"
	};
	const int HEIGHT[ 20 ] = {
		 0, 11, 12, 13, 14, 15, 16, 17, 18, 19,
		20, 21, 22, 23, 24, 25, 26, 27, 28, 29
	};
	for ( int i = 0; i < 20; i++ ) {
		if ( keyboard->isPushKey( KEY_NUMBER[ i ] ) ) {
			if ( _mode != MODE_WATER ) {
				_mode = MODE_HEIGHT;
			}
			_height = HEIGHT[ i ];
		}
	}
	const char *NUM_NUMBER[ 10 ] = {
		"NUM0", "NUM1", "NUM2", "NUM3", "NUM4", "NUM5", "NUM6", "NUM7", "NUM8", "NUM9", 
	};
	for ( int i = 0; i < 10; i++ ) {
		if ( keyboard->isPushKey( NUM_NUMBER[ i ] ) ) {
			_range = i;
			if ( _range < 1 ) {
				_range = 1;
			}
		}
	}
	
	if ( keyboard->isPushKey( "+" ) ) {
		_grid_height += 1;
		_grid->setHeight( _grid_height );
		_object->setHeight( _grid_height );
	}
	if ( keyboard->isPushKey( "-" ) ) {
		_grid_height -= 1;
		_grid->setHeight( _grid_height );
		_object->setHeight( _grid_height );
	}
	if ( keyboard->isPushKey( "V" ) ) {
		_attribute = ATTRIBUTE_STONE;
		_mode = MODE_ATTRIBUTE;
	}
	if ( keyboard->isPushKey( "B" ) ) {
		_attribute = ATTRIBUTE_GRASS;
		_mode = MODE_ATTRIBUTE;
	}
	//水
	if ( keyboard->isPushKey( "N" ) ) {
		_mode = MODE_WATER;
	}
	if ( keyboard->isPushKey( "M" ) ) {
		_attribute = ATTRIBUTE_STONE;
		_mode = MODE_HEIGHT;
	}

	if ( keyboard->isPushKey( "C" ) ) {
		camera->appearanceFocus( );
		_map->setCamera( camera->getEye( ), camera->getFocus( ) );
	}
	//カメラターゲット変更
	if ( keyboard->isPushKey( "X" ) ) {
		FrameworkPtr fw = Framework::getInstance( );
		Vector pos = fw->getCursor( _last_mouse_x, _last_mouse_y );
		camera->setFocus( camera->getEye( ), pos );
	}

	if ( keyboard->isHoldKey( "ARROW_LEFT" ) ) {
		camera->moveHorizontal( -1.0 );
	}
	if ( keyboard->isHoldKey( "ARROW_RIGHT" ) ) {
		camera->moveHorizontal(  1.0 );
	}
	if ( keyboard->isHoldKey( "ARROW_UP" ) ) {
		camera->moveVertical(  1.0 );
	}
	if ( keyboard->isHoldKey( "ARROW_DOWN" ) ) {
		camera->moveVertical( -1.0 );
	}
	if ( mouse->getWheel( ) != 0 ) {
		camera->moveZoom( mouse->getWheel( ) * 2 );
	}
	if ( mouse->isRight( ) ) {
		camera->moveYaw( ( mouse->getX( ) - _last_mouse_x ) * 0.005 );
		camera->movePitch( ( mouse->getY( )- _last_mouse_y ) * 0.005 );
	}
	
	_last_mouse_x = mouse->getX( );
	_last_mouse_y = mouse->getY( );
	camera->update( );

	FrameworkPtr fw = Framework::getInstance( );
	Vector pos = fw->getCursor( _last_mouse_x, _last_mouse_y );
	int mx = ( int )pos.x;
	int mz = ( int )pos.z;
	_grid->draw( );
	if ( mx >= 0 && mx < MAP_SIZE &&
		 mz >= 0 && mz < MAP_SIZE ) {

		int h = _height;
		if ( _mode == MODE_ATTRIBUTE ) {
			h = -1;
		}
		_grid->drawCursor( mx, mz, h, _range, _mode );

		if (  mouse->isLeft( ) ) {
			modifiSector( mx, mz );
		}
		if ( _range == 1 ) {
			unsigned char object = _map->getObject( mx, mz );

			// オブジェクト削除
			if ( keyboard->isPushKey( "A" ) ) {
				object = OBJECT_NONE;
				_map->setObject( mx, mz, OBJECT_NONE );
			}

			bool peripheral = false;

			// 扉配置
			if ( keyboard->isPushKey( "D" ) ) {
				switch ( object ) {
				case OBJECT_DOOR_PX:
					object = OBJECT_DOOR_PZ;
					break;
				case OBJECT_DOOR_MX:
					object = OBJECT_DOOR_MZ;
					break;
				case OBJECT_DOOR_MZ:
					object = OBJECT_DOOR_PX;
					break;
				case OBJECT_DOOR_PZ:
					object = OBJECT_DOOR_MX;
					break;
				default:
					object = OBJECT_DOOR_PZ;
					break;
				}
				peripheral = true;
			}

			bool only = false;
			// サイクロプス配置
			if ( keyboard->isPushKey( "F" ) ) {
				object = OBJECT_ENEMY_CYCLOPS;
				peripheral = true;
				only = true;
			}
			// 宝箱配置
			if ( keyboard->isPushKey( "S" ) ) {
				object = OBJECT_DEEDBOX;
			}
			// ゴブリン配置
			if ( keyboard->isPushKey( "G" ) ) {
				object = OBJECT_ENEMY_GOBLIN;
			}
			//　レッドゴブリン配置
			if ( keyboard->isPushKey( "H" ) ) {
				object = OBJECT_ENEMY_REDGOBLIN;
			}
			// ゴースト配置
			if ( keyboard->isPushKey( "J" ) ) {
				object = OBJECT_ENEMY_GHOST;
			}
			//　鎧騎士配置
			if ( keyboard->isPushKey( "K" ) ) {
				object = OBJECT_ENEMY_ARMOR;
			}
			// 武器レベルアップアイテム配置
			if ( keyboard->isPushKey( "L" ) ) {
				object = OBJECT_ITEM_GEM;
			}
			if ( object != OBJECT_NONE ) {
				if ( peripheral ) {
					int height = _map->getHeight( mx, mz );
					for ( int i = 0; i < 9; i++ ) {
						int ax = i % 3 - 1;
						int az = i / 3 - 1;
						//・周囲９マスの高さをあわせる
						_map->setHeight( mx + ax, mz + az, height );
						//・周囲９マスを移動可能にする（STONE化)
						_map->setAttribute( mx + ax, mz + az, ATTRIBUTE_STONE );
						//・周囲９マスのobject削除
						_map->setObject( mx + ax, mz + az, OBJECT_NONE );

						_grid->updateBox( mx + ax, mz + az );
					}
				}
				if ( only ) {
					for ( int i = 0; i < ( MAP_SIZE- 1 ) * ( MAP_SIZE - 1 ); i++ ) {
						int ax = i % MAP_SIZE - 1;
						int az = i / MAP_SIZE - 1;
						// サイクロプスを一つだけ配置
						if ( _map->getObject( ax, az ) == OBJECT_ENEMY_CYCLOPS ) {
							_map->setObject( ax, az, OBJECT_NONE );
						}
					}
				}

				_map->setObject( mx, mz, object );
			}
		}
	}
	
	fw->drawString( 0, 0, "x:%d z:%d", mx, mz );
	fw->drawString( 0, 12, "eye   x:% 2d y:% 2d z:% 2d", ( int )camera->get_Focus_pos_x( ), ( int )camera->get_Focus_pos_y( ), ( int )camera->get_Focus_pos_z( ) );
	fw->drawString( 0, 24, "focus x:% 2d y:% 2d z:% 2d", ( int )camera->get_Focus_target_x( ), ( int )camera->get_Focus_target_y( ), ( int )camera->get_Focus_target_z( ) );
}

void ModuleEditor::modifiSector( int mx, int mz ) {
	switch ( _mode )  {
	case MODE_HEIGHT:
		for ( int i = 0; i < _range * _range; i++ ) {
			int ax = i % _range;
			int az = i / _range;
			_map->setHeight( mx + ax, mz + az, _height );
			_grid->updateBox( mx + ax, mz + az );
			removeWildObject( mx + ax, mz + az);
		}
		break;
	case MODE_ATTRIBUTE:
		for ( int i = 0; i < _range * _range; i++ ) {
			int ax = i % _range;
			int az = i / _range;
			_map->setAttribute( mx + ax, mz + az, _attribute );
			_grid->updateBox( mx + ax, mz + az );
		}
		break;
	case MODE_WATER://水
		for ( int i = 0; i < _range * _range; i++ ) {
			int ax = i % _range;
			int az = i / _range;
			_map->setWaterHeight( mx + ax, mz + az, _height );
			_grid->updateWater( mx + ax, mz + az );
		}
		break;
	}
}

void ModuleEditor::removeWildObject( int mx, int mz ) {
	for ( int i = 0; i < 9; i++ ) {
		int x = mx + i % 3;
		int z = mz + i / 3;
		unsigned char object = _map->getObject( x, z );
		if ( object == OBJECT_ENEMY_CYCLOPS ||
			 object == OBJECT_DOOR_PX ||
			 object == OBJECT_DOOR_MX ||
			 object == OBJECT_DOOR_PZ ||
			 object == OBJECT_DOOR_MZ ) {
				_map->setObject( x, z, OBJECT_NONE );
		}
	}
}

