#include "Item.h"

Item::Item( ) {
	_expired =  true;
}

Item::~Item( ) {
}

bool Item::isExpired( ) {
	return _expired;
}

void Item::delMyself( ) {
	_expired = false;
}