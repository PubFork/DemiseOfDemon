#ifndef M3DG_MODEL_M3DG_CAMERA_H
#define M3DG_MODEL_M3DG_CAMERA_H

namespace m3dg {
namespace model {

// 32byte
struct Camera {
enum Type {
	PERSPECTIVE = 0,
	ORTHOGRAPHIC
};

Camera() {
	type = PERSPECTIVE;
	fovY = 0.5f;
	aspect = 1;
	width = 0;
	height = 0;
	nearZ = 1;
	farZ = 100;
	reserved = 0;
};

Type	type;
float	fovY;
float	aspect;
float	width;
float	height;
float	nearZ;
float	farZ;
float	reserved;

};

}
}

#endif
