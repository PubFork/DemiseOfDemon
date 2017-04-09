#ifndef BM3_SDK_INC_BM3_CORE_BM3_EFFECT_INL_H
#define BM3_SDK_INC_BM3_CORE_BM3_EFFECT_INL_H

#include "bm3_RenderStateManager.h"
#include <ml/utility/ml_simpletimer.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

template<typename ResRootType>
Effect<ResRootType>::Effect(){

	;
}
template<typename ResRootType>
Effect<ResRootType>::~Effect(){

	obj_root_.Finalize();
}

#if defined(BM3_TARGET_IDE)
template<typename ResRootType>
bool	Effect<ResRootType>::Initialize(const ResRootType &res_root){

	return obj_root_.Initialize(res_root);
}
#endif

template<typename ResRootType>
bool	Effect<ResRootType>::Initialize(const ResRootType &res_root, const fileformat::bmb::Header *header, const float gen_scale){

	ML_INITIALIZE_TIMER_SEP;
	ML_INITIALIZE_TIMER;
	return obj_root_.Initialize(res_root, header, gen_scale);
}

template<typename ResRootType>
bool	Effect<ResRootType>::Initialize(InitInfo &init_info){
	return obj_root_.Initialize(init_info);
}

template<typename ResRootType>
template<typename UpdateInfo>
void	Effect<ResRootType>::Update(UpdateInfo &info){

	ML_UPDATE_TIMER_SEP;
	ML_UPDATE_TIMER;
	ML_ASSERT(obj_root_.IsInitialized());

	obj_root_.Update(info);
}

template<typename ResRootType>
void	Effect<ResRootType>::MakeVertex(DrawInfo &info){

	ML_MAKEVERTEX_TIMER_SEP;
	ML_MAKEVERTEX_TIMER;
	ML_ASSERT(obj_root_.IsInitialized());

	obj_root_.MakeVertex(info);
}

template<typename ResRootType>
void	Effect<ResRootType>::Draw(DrawInfo &info){

	ML_DRAWTIMER_SEP;
	ML_ASSERT(obj_root_.IsInitialized());

	RenderStateManager::instance().BeginRender(info);
	{
		RenderStateManager::instance().SetWorldMatrix(ml::matrix44());
		RenderStateManager::instance().SetCameraDirection(info.InverseViewMatrix().row(ml::matrix44::row::z));
#if defined(BM3_TARGET_IDE)
		// ÉcÅ[ÉãÇ≈Ç‡íºÇ…make vertex ÇåƒÇ‘Ç◊Ç´
		obj_root_.MakeVertex(info);
#endif
		obj_root_.Draw(info);
	}
	RenderStateManager::instance().EndRender(info);
}

template<typename ResRootType>
int		Effect<ResRootType>::VertexBufferSize() const{

	return obj_root_.TotalVertexBufferSize();

}

template<typename ResRootType>
void	Effect<ResRootType>::SetMatrix(const ml::matrix44 &matrix){

	obj_root_.SetMatrix(matrix);
}

template<typename ResRootType>
void	Effect<ResRootType>::SetColorScale(const ColorType &color_scale){

	obj_root_.SetColorScale(color_scale);
}

template<typename ResRootType>
void	Effect<ResRootType>::SetOffsetPosition(const ml::vector3d &offset_position){

	obj_root_.SetOffsetPosition(offset_position);
}

template<typename ResRootType>
bool	Effect<ResRootType>::IsExpired() const{

	return obj_root_.IsExpired();
}

template<typename ResRootType>
void	Effect<ResRootType>::RequestRetire(){

	obj_root_.RequestRetire();
}

template<typename ResRootType>
void	Effect<ResRootType>::Reset(ResetInfo &info){

	obj_root_.Reset(info);
}

template<typename ResRootType>
void	Effect<ResRootType>::RegistryCreatePrimitiveCallback(void (*callback)(CreatePrimitiveCallbackInfo &info, void *data), void *data){

	obj_root_.RegistryCreatePrimitiveCallback(callback, data);
}

template<typename ResRootType>
void	Effect<ResRootType>::RegistryDestoryPrimitiveCallback(void (*callback)(DestoryPrimitiveCallbackInfo &info, void *data), void *data){

	obj_root_.RegistryDestoryPrimitiveCallback(callback, data);
}

template<typename ResRootType>
int		Effect<ResRootType>::NodeCount(){
	return obj_root_.NodeCount();
}
template<typename ResRootType>
void	Effect<ResRootType>::NodeVertexInfo(int emitter_id, VertexInfo &info, int vertex_buffer_id){
	obj_root_.NodeVertexInfo(emitter_id, info, vertex_buffer_id);
}

template<typename ResRootType>
int	Effect<ResRootType>::SwitchEffect(const void *bmb_image){

	return obj_root_.SwitchEffect(*((ResRootType *)(fileformat::bmb::RootNodeRes(bmb_image))), ml::offset_pointer<fileformat::bmb::Header*>(bmb_image, 0));
}

template<typename ResRootType>
int	Effect<ResRootType>::MaxVertexBufferSize() const{
	return obj_root_.MaxVertexBufferSize();
}


template<typename ResRootType>
void	Effect<ResRootType>::DestroyResource(){
	obj_root_.DestroyResource();
}

template<typename ResRootType>
void	Effect<ResRootType>::RestoreResource(RestoreInfo &info){
	obj_root_.RestoreResource(info);
}

#ifdef BM3_PLATFORM_playstation3
	
template<typename ResRootType>
template<typename UpdateInfo>
void	Effect<ResRootType>::RawSpuUpdate(UpdateInfo &info){

	ML_UPDATE_TIMER_SEP;
	ML_UPDATE_TIMER;
	ML_ASSERT(obj_root_.IsInitialized());

	obj_root_.RawSpuUpdate(info);
}

template<typename ResRootType>
template<typename UpdateInfo>
void	Effect<ResRootType>::RawSpuUpdateField(UpdateInfo &info){

	ML_UPDATE_TIMER_SEP;
	ML_UPDATE_TIMER;
	ML_ASSERT(obj_root_.IsInitialized());

	obj_root_.RawSpuUpdateField(info);
}

template<typename ResRootType>
void	Effect<ResRootType>::RawSpuMakeVertex(DrawInfo &info){

	ML_MAKEVERTEX_TIMER_SEP;
	ML_MAKEVERTEX_TIMER;
	ML_ASSERT(obj_root_.IsInitialized());

	obj_root_.RawSpuMakeVertex(info);
}
	
#endif

#ifdef BM3_PLATFORM_xbox360	
template<typename ResRootType>
BOOL Effect<ResRootType>::IsVertexBufferBusy(){
	return obj_root_.IsVertexBufferBusy();
}
#endif


BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BM3_EFFECT_INL_H
