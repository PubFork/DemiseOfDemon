#ifndef M3DG_MODEL_M3DG_CONTROLLER_H
#define M3DG_MODEL_M3DG_CONTROLLER_H

namespace m3dg{
namespace model{

// Controller
struct Controller {

static const int JOINT_MAX = 32;

Controller() {}

int				geometryIndex;
int				jointCount;
int				padding[2];

ml::matrix44	bindShapeMatrix;
ml::matrix44	inverseBindMatrices[JOINT_MAX];
int				jointIndices[JOINT_MAX];

};

}
}

#endif
