#include "Object.h"

Object::Object( ) {

}

Object::~Object( ) {

}

ObjectPtr Object::getThis( ) {
	return shared_from_this( );
}
