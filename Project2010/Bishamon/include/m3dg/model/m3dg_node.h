#ifndef M3DG_MODEL_M3DG_NODE_H
#define M3DG_MODEL_M3DG_NODE_H

namespace m3dg{
namespace model{

// 
struct Node {

enum {
	NODE_NULL = 0,
	NODE_CAMERA,
	NODE_CONTROLLER,
	NODE_GEOMETRY
};

Node() {
	type = NODE_NULL;
	index = -1;
	own = 0;
	children = 0;
	childCount = 0;
	parent = -1;
	reserved[0] = 0;
	reserved[1] = 0;
	name[0] = 0;
	scale = ml::vector3d(1, 1, 1, 0);
}

int				type;
int				index;
int				own;
int				children;
int				childCount;
int				parent;
int				reserved[2];

ml::vector3d	translation;
ml::vector3d	rotation;
ml::vector3d	scale;
ml::matrix44	local;
ml::matrix44	world;
char			name[256];

};

}
}

#endif
