#include "CameraMapEditor.h"
#include "Framework.h"
#include "Keyboard.h"

const double LENGTH = 100;

CameraMapEditor::CameraMapEditor( ) {
	_eye = Vector( 1, 1, 1 ).normalize( ) * LENGTH;
}

CameraMapEditor::~CameraMapEditor( ) {
}

void CameraMapEditor::update( ) {
	updateCamera( );
}

void CameraMapEditor::appearanceFocus( ) {
	Vector vec = _eye - _focus;
	vec.y = 0;
	_eye.y = vec.getLength( ) * 0.5;
	_focus_pos = _eye;
	_focus_target = _focus;
}


void CameraMapEditor::moveHorizontal( double value ) {
	Vector pos = _eye;
	pos.y = 0;

	Vector vec = _focus - pos;
	
	double tmp = vec.x;
	vec.x = vec.z;
	vec.z = -tmp;

	_focus += vec.normalize( ) * value;
	_focus.y = 0;
	_eye += vec.normalize( ) * value;
}

void CameraMapEditor:: moveYaw( double value ) {
	Vector vec = _eye - _focus;
	Vector axis( 0, -1, 0 );
	Matrix mat = Matrix::makeTransformRotation( axis, value );
	vec = mat.multiply( vec );
	_eye = _focus + vec;
}

void CameraMapEditor:: movePitch( double value ) {
	Vector vec = _eye - _focus;
	Vector axis = Vector( 0, 1, 0).cross( vec );
	Matrix mat = Matrix::makeTransformRotation( axis, value );
	vec = mat.multiply( vec );
	_eye = _focus + vec;
}

void CameraMapEditor:: moveZoom( double value ) {
	Vector vec = _eye - _focus;
	double length = vec.getLength( ) + value;
	vec = vec.normalize( ) * length;
	_eye = _focus + vec;
}

void CameraMapEditor:: moveVertical( double value ) {
	Vector pos = _eye;
	pos.y = 0;

	Vector vec = _focus - pos;
	
	_focus += vec.normalize( ) * value;
	_focus.y = 0;
	_eye += vec.normalize( ) * value;
}

void CameraMapEditor::setFocus( const Vector& focus_pos, const Vector& focus_target ) {
	_focus_pos = focus_pos;
	_focus_target = focus_target;
	_eye = _focus_pos;
	_focus = _focus_target;
}

double CameraMapEditor::get_Focus_pos_x( ) const {
	return _focus_pos.x;
}

double CameraMapEditor::get_Focus_pos_y( ) const {
	return _focus_pos.y;
}

double CameraMapEditor::get_Focus_pos_z( ) const {
	return _focus_pos.z;
}

double CameraMapEditor::get_Focus_target_x( ) const {
	return _focus_target.x;
}

double CameraMapEditor::get_Focus_target_y( ) const {
	return _focus_target.y;
}

double CameraMapEditor::get_Focus_target_z( ) const {
	return _focus_target.z;
}