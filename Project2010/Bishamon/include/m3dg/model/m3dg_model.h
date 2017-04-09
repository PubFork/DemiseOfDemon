#ifndef M3DG_MODEL_M3DG_MODEL_H
#define M3DG_MODEL_M3DG_MODEL_H

#if defined(BM3_PLATFORM_nw4c)
#pragma diag_suppress 161
#pragma diag_suppress 228
#pragma diag_suppress 237
#pragma diag_suppress 261
#pragma diag_suppress 381
#pragma diag_suppress 826
#pragma diag_suppress 1300
#pragma diag_suppress 2530
#endif

#include "../fileformat/m3dg_m3r.h"
#include "../m3dg_type.h"
#include "m3dg_camera.h"
#include "m3dg_animation.h"
#include "m3dg_node.h"
#include "m3dg_controller.h"

namespace m3dg{
namespace model{

void Initialize(fileformat::m3r::Header &model_header, const ModelInitializeInfo &mii);
void Finalize(fileformat::m3r::Header &model_header);

inline bool IsValidateData(const void *image){
	const fileformat::m3r::Header*	model_header	= (const fileformat::m3r::Header*)image;
	return (model_header->sign_	== fileformat::m3r::Header::Sign) &&
			(model_header->version_	== fileformat::m3r::Header::Version);
}

class Model {
public: inline Model(ml::ui8 *binary);
public: inline ~Model();

public: inline void UpdateAnimation(float time);
public: inline void UpdateAnimation(Animation *animation, float time);
public: inline void Update(ml::matrix44 &world);
public: inline void Update(Node *node, ml::matrix44 &world);
public: inline Node *GetNode(int index);
public: inline Node *GetNode(const char *name);
public: inline Node *GetNode(Node *node, const char *name);
public: int GetCameraCount() { return header->camera_count_; }
public: int GetControllerCount() { return header->controller_count_; }
public: inline const char *GetNodeName(int index, int type);
public: inline ml::matrix44 GetNodeWorldMatrix(int index, int type);
public: inline Node *SearchTypeNode(int index, int type);
public: inline Node *SearchTypeNode(Node *node, int index, int type);
public: inline bool IsTypeNode(int index, int type);
public: inline bool IsTypeNode(Node *node, int index, int type);
public: inline ml::matrix44 GetNodeWorldMatrix(Node *node, int index, int type);
public: inline ml::matrix44 GetNodeWorldMatrix(const char *name);
public: inline ml::matrix44 GetJointMatrix(int index);
public: inline Node *SearchJointNode(Node *node, int index, int type);
public: inline bool IsJointNode(Node *node, int index, int type);
public: inline float GetCameraFovY(const char *name);
public: inline float GetCameraNearZ(const char *name);
public: inline float GetCameraFarZ(const char *name);
public: Controller *GetController(); 
private: ml::ui8					*binary;		// バイナリ
private: fileformat::m3r::Header	*header;		// ヘッダ
private: Camera						*cameras;		// カメラデータ
private: ml::ui8					*curves;		// カーブデータ
private: Animation					*animations;	// アニメーション
private: Node						*nodes;			// ノードデータ
private: Controller					*controllers;	// コントローラー
};

void DrawDirect(fileformat::m3r::Header &model_header, const ModelDrawInfo &mdi, BindTextureCallback bind_texture_callback, void *user_data, void *shader_info, m3dg::model::Model *animation_model = NULL);

void InstanceDrawDirect(fileformat::m3r::Header &model_header, const ModelDrawInfo &mdi, BindTextureCallback bind_texture_callback, void *user_data, int instance_count, void* shader_info);

/*!
	@brief	モデルファイルの頂点情報を取得する
	@param	vai				書き込まれえるVertexArrayInfo構造体
	@param	model_header	モデルファイルのヘッダー
	@return なし
*/
void ForeachVertexArray(const fileformat::m3r::Header &model_header, ForeachVertexArrayCallback callback, void *user_data);
/*!
	@brief	モデルファイルの頂点情報を取得する
	@param	vai				書き込まれえるVertexArrayInfo構造体
	@param	model_header	モデルファイルのヘッダー
	@return なし
*/
void GetVertexArrayInfo(VertexArrayInfo &vai, const fileformat::m3r::Header &model_header);

void SetRenderType(fileformat::m3r::Header &model_header, const ModelDrawInfo &mdi);

void DebugDrawDirect(fileformat::m3r::Header &model_header, const ModelDrawInfo &mdi, const ml::matrix44 &m, void *user_data);


// model
inline Model::Model(ml::ui8 *binary) {
	this->binary = binary;
	// header
	header = reinterpret_cast<fileformat::m3r::Header *>(binary);
	// camera
	cameras = reinterpret_cast<Camera *>(binary + header->camera_offset_);
	// curves 可変長なので初期化がめんどい
	{
		curves = binary + header->curve_offset_;
		int offset = 0;
		for(ml::ui32 i=0; i<header->curve_count_; i++) {
			Curve *curve = reinterpret_cast<Curve *>(curves+offset);
			offset += 16; // header
			curve->keys = reinterpret_cast<Keyframe *>(curves+offset);
			offset += curve->size;
		}
	}
	// animation
	animations = reinterpret_cast<Animation *>(binary + header->animation_offset_);
	for(ml::ui32 i=0; i<header->animation_count_; i++) {
		Animation *anim = &animations[i];
		for(ml::ui32 j=0; j<16; j++) {
			// オフセットをCurve *に変換
			int address = reinterpret_cast<int>(anim->curves[j]);
			if(address >= 0) anim->curves[j] = reinterpret_cast<Curve *>(curves + address);
			else anim->curves[j] = NULL;
		}
	}
	// node
	nodes = reinterpret_cast<Node *>(binary + header->node_offset_);

	// controllers
	controllers = reinterpret_cast<Controller *>(binary + header->controller_offset_);

}
	
inline Model::~Model() {
#if 0
	if(curves) {
		delete [] curves;
		curves = NULL;
	}
#endif
}

inline void Model::UpdateAnimation(float time) {

	for(ml::ui32 i=0; i<header->animation_count_; i++) {
		Animation *anim = animations + i;
		UpdateAnimation(anim, time);
	}

}

inline Controller *Model::GetController()
{
	return controllers;
}

inline void Model::UpdateAnimation(Animation *animation, float time) {

	Node *node = GetNode(animation->target);

	if(!node) return;

	// matrix animation
	if(animation->curves[9] != NULL) {
		float tmp[16];
		float *out = animation->curves[9]->Interpolate(time, tmp);
#if 0
		ml::matrix44 m(
				out[ 0], out[ 1], out[ 2], out[ 3],
				out[ 4], out[ 5], out[ 6], out[ 7],
				out[ 8], out[ 9], out[10], out[11],
				out[12], out[13], out[14], out[15]
			);
#else
		ml::matrix44 m(
				out[ 0], out[ 4], out[ 8], out[12],
				out[ 1], out[ 5], out[ 9], out[13],
				out[ 2], out[ 6], out[10], out[14],
				out[ 3], out[ 7], out[11], out[15]
			);
#endif
		node->local = m;
		return;
	}

	// srt animation
	ml::matrix44 matrix;
	Curve **curve = animation->curves;
	float x, y, z;

	// translation
	x = ((*curve) && (*curve)->count > 0) ? (*curve)->Interpolate(time) : node->translation.e_.x_;
	curve++;
	y = ((*curve) && (*curve)->count > 0) ? (*curve)->Interpolate(time) : node->translation.e_.y_;
	curve++;
	z = ((*curve) && (*curve)->count > 0) ? (*curve)->Interpolate(time) : node->translation.e_.z_;
	curve++;

	ml::matrix44 trans;
	trans.set_translate(x, y, z);
	matrix.mul(trans);

	// rotation(Z, X, Y)
	x = ((*curve) && (*curve)->count > 0) ? (*curve)->Interpolate(time) : node->rotation.e_.x_;
	curve++;
	y = ((*curve) && (*curve)->count > 0) ? (*curve)->Interpolate(time) : node->rotation.e_.y_;
	curve++;
	z = ((*curve) && (*curve)->count > 0) ? (*curve)->Interpolate(time) : node->rotation.e_.z_;
	curve++;

	ml::matrix44 rot;
	// z
	rot.set_unit();
	rot.set_rot_z(ml::to_radian(z));
	matrix.mul(rot, matrix);
	// y
	rot.set_unit();
	rot.set_rot_y(ml::to_radian(y));
	matrix.mul(rot, matrix);
	// x
	rot.set_unit();
	rot.set_rot_x(ml::to_radian(x));
	matrix.mul(rot, matrix);

	// scaling
	x = ((*curve) && (*curve)->count > 0) ? (*curve)->Interpolate(time) : node->scale.e_.x_;
	curve++;
	y = ((*curve) && (*curve)->count > 0) ? (*curve)->Interpolate(time) : node->scale.e_.y_;
	curve++;
	z = ((*curve) && (*curve)->count > 0) ? (*curve)->Interpolate(time) : node->scale.e_.z_;
	curve++;

	ml::matrix44 scale;
	scale.set_scale(x, y, z);
	matrix.mul(scale);

	// result
	node->local = matrix;
}

inline void Model::Update(ml::matrix44 &world) {
	Update(nodes, world);
}

inline void Model::Update(Node *node, ml::matrix44 &world) {

//	node->world.mul(world, node->local);
	node->world.mul(node->local, world);

	for(int i=0; i<node->childCount; i++) {
		Node *child = nodes + node->children + i;
		Update(child, node->world);
	}

}

inline Node *Model::GetNode(int index) {
	return nodes + index;
}

inline Node *Model::GetNode(const char *name) {
	return GetNode(nodes, name);
}

inline Node *Model::GetNode(Node *node, const char *name) {
	if(!strcmp(node->name, name)) return node;
	for(int i=0; i<node->childCount; i++) {
		Node *child = nodes + node->children + i;
		Node *ret = GetNode(child, name);
		if(ret != NULL) return ret;
	}
	return NULL;
}

inline const char *Model::GetNodeName(int index, int type) {
	for(unsigned int i=0; i<header->node_count_; i++) {
		Node *node = nodes + i;
		if(node->type == type && --index < 0) {
			return node->name;
		}
	}
	return NULL;
}

inline ml::matrix44 Model::GetNodeWorldMatrix(int index, int type) {

	ml::matrix44 matrix;

	Node *node = SearchTypeNode(nodes, index, type);

	if (node) {
		return node->world;
	}

	return matrix;
}

inline Node *Model::SearchTypeNode(int index, int type) {
	return SearchTypeNode(nodes, index, type);
}

inline Node *Model::SearchTypeNode(Node *node, int index, int type) {

	if(IsTypeNode(node, index, type)) {
		return node;
	}

	for( int i = 0; i < node->childCount; i++) {
		Node *child = nodes + node->children + i;
		Node *ret = NULL;

		if (IsTypeNode(child, index, type)) {
			ret = child;
		}
		else {
			ret = SearchTypeNode(child, index, type); 
		}

		if (ret) {
			return ret;
		}
	}

	return NULL;
}

inline bool Model::IsTypeNode(Node *node, int index, int type) {

	if(node->type == type && --index < 0) {
		return true;
	}

	return false;
}

inline ml::matrix44 Model::GetJointMatrix(int index) {
	return (nodes+index)->world;
}

inline bool Model::IsJointNode(Node *node, int index, int type) {

	if(node->type == type && node->own == index) {
		return true;
	}

	return false;
}

inline ml::matrix44 Model::GetNodeWorldMatrix(const char *name) {
	Node *node = GetNode(name);
	if(node) return node->world;
	return ml::matrix44();
}

inline float Model::GetCameraFovY(const char *name) {
	Node *node = GetNode(name);
	if(node && node->index >= 0 && node->index < static_cast<int>(header->camera_count_)) return cameras[node->index].fovY;
	return 1;
}

inline float Model::GetCameraNearZ(const char *name) {
	Node *node = GetNode(name);
	if(node && node->index >= 0 && node->index < static_cast<int>(header->camera_count_)) return cameras[node->index].nearZ;
	return 1;
}

inline float Model::GetCameraFarZ(const char *name) {
	Node *node = GetNode(name);
	if(node && node->index >= 0 && node->index < static_cast<int>(header->camera_count_)) return cameras[node->index].farZ;
	return 100;
}


}	// namespace model
}	// namespace m3dg

#endif	// #ifndef M3DG_MODEL_M3DG_MODEL_H
