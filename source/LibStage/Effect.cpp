#include "Effect.h"
#include "Camera.h"
#include "Camera.h"
#include "Framework.h"
#include "DxLib.h"

Effect::Effect( ) {
	_num = 0;
}

Effect::~Effect( ) {
}

int Effect::getNum( ) const {
	return _num;
}

void Effect::update( ) {
	_num = 0;
}

int Effect::getMaxNum( ) const {
	return MAX_NUM;
}

Effect::DATA Effect::getData( int index ) const {
	return _data[ index ];
}

void Effect::play( EFFECT effect, Vector& pos, float scale, int time ) {
	if ( _num >= MAX_NUM ) {
		return;
	}

	DATA data;
	data.tag = effect;
	data.pos = pos;
	data.scale = scale;
	data.time = time;
	_data[ _num ] = data;
	_num++;
}
