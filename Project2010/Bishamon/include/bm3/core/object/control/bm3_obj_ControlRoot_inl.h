#ifndef BM3_SDK_INC_BM3_CORE_OBJECT_CONTROL_BM3_OBJ_CONTROLROOT_INL_H
#define BM3_SDK_INC_BM3_CORE_OBJECT_CONTROL_BM3_OBJ_CONTROLROOT_INL_H

#include "../../bm3_UpdateInfo.h"
#include "../../bm3_InitialAllocInfo.h"
#include "../../bm3_InitializeInfo.h"
#include "../bm3_obj_RootNodeBase.h"
#include "../../bm3_VertexInfo.h"
#include "../../bm3_VertexManager.h"
#ifdef BM3_PLATFORM_playstation3
#include "../../!playstation3/spu/bm3_RawSpuManager.h"
#endif

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace obj{
namespace control{

#define BASE_NODE	Base<ResourceType, RootNodeBase, policy::Life_Infinity, policy::ChildGenerateTiming_Nothing>

// ------------------------------------------------------------------
template<typename ResourceType>
Root<ResourceType>::Root(){

	;
}

// ------------------------------------------------------------------
template<typename ResourceType>
Root<ResourceType>::~Root(){

}
// ------------------------------------------------------------------
template<typename ResourceType>
void Root<ResourceType>::Finalize(){

	// 頂点バッファ開放のため最大サイズを設定しておく
	BASE_NODE::node_base_array_->ResetNodeCount();
	BASE_NODE::SetMaxParticleCount();

	BASE_NODE::node_base_array_->Renderer().Finalize(VertexManager::instance().VertexBufferCount());

	BASE_NODE::node_base_array_->~RootNodeBase();
	BASE_NODE::Finalize();
}

#if defined(BM3_TARGET_IDE)
// ------------------------------------------------------------------
template<typename ResourceType>
bool	Root<ResourceType>::Initialize(const ResourceType &res){

/*
	InitializeInfo info(this, NULL, 1);
	BASE_NODE::Initialize(res, info);
	InitializePrimitive(info, VertexManager::instance().VertexBufferCount());

	// ルートは無条件で初期化時に子を作る
	BASE_NODE::GenerateChild(1);
*/
	BASE_NODE::is_root_	= true;
	{
		bool result = true;
		// メモリの確保
		InitialAllocInfo info(1);
		result = BASE_NODE::Allocate(res);
		if(!result)return false;
		BASE_NODE::FixGenCount(res, info, 1.0f);
		result = BASE_NODE::AllocateParam();
		if(!result)return false;

//		BASE_NODE::node_base_array_->SortEmitterInfo(res);
	}
	{
		InitializeInfo info(BASE_NODE::node_base_array_, 1.0f);

		// root の初期化
		BASE_NODE::node_base_array_->SetEnableWholeZSort(res.Basic_Basic_WholeZSort());
		BASE_NODE::node_base_array_->SetResIdeNode(&res);
		BASE_NODE::node_base_array_->SetEnableBm2(!res.Basic_Basic_Version1_5());
		BASE_NODE::node_base_array_->SetGenScale(1.0f);
		BASE_NODE::node_base_array_->SetVertexBufferCount(VertexManager::instance().VertexBufferCount());
		BASE_NODE::node_base_array_->Initialize();
		// ノードの数を数えてメモリ確保
		{
			bool result = true;
			result = BASE_NODE::node_base_array_->AllocResEmitterInfo(BASE_NODE::CalcEmitterCount());
			if(!result)return result;
			BASE_NODE::SetResEmitterInfo(info);
			BASE_NODE::node_base_array_->SortResEmitterInfo();

			result = BASE_NODE::node_base_array_->InitializeNodeList(BASE_NODE::node_base_array_->ResourceEmitterCount());
			if(!result)return result;
		}
		// ノードの数を数えてからＺソートの初期化（メモリの確保ができないから）
		if(BASE_NODE::node_base_array_->IsEnableZSort()){
			int prim_count = MaxPrimCount();
			if(prim_count==0){
				BASE_NODE::node_base_array_->SetEnableWholeZSort(false);
			}
			else{
				bool result = BASE_NODE::node_base_array_->InitializePrimZList(prim_count);
				if(!result)return false;
			}
		}

		// 各ノードの初期化
		BASE_NODE::Initialize(res, info);

		// ルートは無条件でアクティブ
		BASE_NODE::ActivateNode(0, 1);
		BASE_NODE::ActivateNodeDependParent(0, 1, NULL);


		InitializePrimitive(info);
		BASE_NODE::is_root_	= true;
	}
	
	
	// 途中から再生を始める
	if(res.Basic_Basic_AdvancedFrame()!=0){
		UpdateInfo	update_info;
		for(int i=0 ; i<res.Basic_Basic_AdvancedFrame() ; ++i){
			Update(update_info);
		}
	}

	return true;
}
#endif

// ------------------------------------------------------------------
template<typename ResourceType>
bool	Root<ResourceType>::Initialize(const ResourceType &res, const fileformat::bmb::Header *header, const float gen_scale){

	ML_PTR_ASSERT(header);

	BASE_NODE::is_root_	= true;
	{
		// メモリの確保
		bool result = true;
		InitialAllocInfo info(1);
		result = BASE_NODE::Allocate(res);
		if(!result)return false;
		
		BASE_NODE::FixGenCount(res, info, gen_scale);
		result = BASE_NODE::AllocateParam();
		if(!result)return false;
	}
	{
		InitializeInfo info(BASE_NODE::node_base_array_, gen_scale);

		// ルートの初期化
		BASE_NODE::node_base_array_->SetBmbHeader(header);
		BASE_NODE::node_base_array_->SetEnableBm2(bm3::fileformat::bmb::RootInterChangedBm2((void*)header));
		BASE_NODE::node_base_array_->SetGenScale(gen_scale);
		BASE_NODE::node_base_array_->SetVertexBufferCount(VertexManager::instance().VertexBufferCount());
		BASE_NODE::node_base_array_->Initialize();
		// ノードの数を数えてメモリ確保
		{
			bool result = true;
			// emitter 情報を格納するメモリを確保
			result = BASE_NODE::node_base_array_->AllocResEmitterInfo(BASE_NODE::CalcEmitterCount());
			if(!result)return false;
			BASE_NODE::SetResEmitterInfo(info);
			BASE_NODE::node_base_array_->SortResEmitterInfo();

			result = BASE_NODE::node_base_array_->InitializeNodeList(BASE_NODE::node_base_array_->ResourceEmitterCount());
			if(!result)return false;
		}
		// ノードの数を数えてからＺソートの初期化（メモリの確保ができないから）
		if(BASE_NODE::node_base_array_->IsEnableZSort()){
			bool result = BASE_NODE::node_base_array_->InitializePrimZList(MaxPrimCount());
			if(!result)return false;
		}

		// 各ノードの初期化
		BASE_NODE::Initialize(res, info);

		// ルートは無条件でアクティブ
		BASE_NODE::ActivateNode(0, 1);
		BASE_NODE::ActivateNodeDependParent(0, 1, NULL);

		InitializePrimitive(info);
	}
	
	// 途中から再生を始める
	int advanced_frame = bm3::fileformat::bmb::RootAdvancedFrame((void*)header);
	if(advanced_frame!=0){
		UpdateInfo	update_info;
		for(int i=0 ; i<advanced_frame; ++i){
			Update(update_info);
		}
	}

	return true;
}

// ------------------------------------------------------------------
template<typename ResourceType>
bool	Root<ResourceType>::InitializePrimitive(InitializeInfo &info){
	bool result = true;
	{
		BASE_NODE::SetMaxParticleCount();
		result = BASE_NODE::node_base_array_->Renderer().Initialize(
			info,
			BASE_NODE::node_base_array_->ResourceEmitterCount(),
			VertexManager::instance().VertexBufferCount()
		);
		if(!result)return false;

		BASE_NODE::node_base_array_->ResetNodeCount();
	}
	return true;
}

// ------------------------------------------------------------------
template<typename ResourceType>
bool Root<ResourceType>::Initialize(InitInfo &init_info){
	return BASE_NODE::node_base_array_->Renderer().Initialize(init_info);
}

// ------------------------------------------------------------------
template<typename ResourceType>
bool	Root<ResourceType>::IsExpired() const{

	if(BASE_NODE::node_base_array_->IsExpired()==false){
		return BASE_NODE::Child_IsExpired();
	}
	return true;
}

// ------------------------------------------------------------------
template<typename ResourceType>
template<typename UpdateInfo>
void	Root<ResourceType>::Update(UpdateInfo &info){

	BASE_NODE::node_base_array_->ResetNodeCount();
	BASE_NODE::node_base_array_->Update(info);
	if(BASE_NODE::node_base_array_->IsExpired()==false){
		BASE_NODE::Update(info);
	}
}

// ------------------------------------------------------------------
template<typename ResourceType>
void	Root<ResourceType>::MakeVertex(DrawInfo &info){

	if(BASE_NODE::node_base_array_->IsEnableZSort()){
		BASE_NODE::node_base_array_->ResetZList();
		MakeVertexChild(info);
		BASE_NODE::node_base_array_->MakeZList();
	}
	else{
		MakeVertexChild(info);
	}
}

// ------------------------------------------------------------------
template<typename ResourceType>
void	Root<ResourceType>::MakeVertexChild(DrawInfo &info){
	int index = VertexManager::instance().MakeVertexIndex();
	BASE_NODE::node_base_array_->SetVertexBufferIndex(index);
	BASE_NODE::node_base_array_->Renderer().BeginVertexBuffer(info, index);
	{
		BASE_NODE::MakeVertex(info);
	}
	BASE_NODE::node_base_array_->Renderer().EndVertexBuffer(info, index);
}

// ------------------------------------------------------------------
template<typename ResourceType>
void	Root<ResourceType>::Draw(DrawInfo &info){

	int index = VertexManager::instance().DrawIndex();
	BASE_NODE::node_base_array_->SetVertexBufferIndex(index);

	if(BASE_NODE::node_base_array_->IsEnableZSort()){
		BASE_NODE::node_base_array_->Renderer().Draw(info, index, BASE_NODE::node_base_array_->ZList());
	}
	else{
		BASE_NODE::node_base_array_->Renderer().Draw(info, index);
	}
}

// ------------------------------------------------------------------
template<typename ResourceType>
int		Root<ResourceType>::NodeCount(){
	return BASE_NODE::node_base_array_->ResourceEmitterCount();
}

// ------------------------------------------------------------------
template<typename ResourceType>
void	Root<ResourceType>::NodeVertexInfo(int emitter_id, VertexInfo &info, int vertex_buffer_id){

	ML_ASSERT(emitter_id<NodeCount());
	ML_ASSERT(((unsigned int)emitter_id<BASE_NODE::node_base_array_->VertexBufferCount()));

	BASE_NODE::node_base_array_->Renderer().NodeVertexInfo(emitter_id, info, vertex_buffer_id);
}

// ------------------------------------------------------------------
template<typename ResourceType>
int		Root<ResourceType>::SwitchEffect(const ResourceType &res, const fileformat::bmb::Header *header){

	InitialAllocInfo	info(1);
	int result = BASE_NODE::SwitchResource(res, info);
	BASE_NODE::node_base_array_->SetBmbHeader(header);
	BASE_NODE::node_base_array_->Renderer().ResetEmitterResource(0);
	return result;
}


// ------------------------------------------------------------------
template<typename ResourceType>
void		Root<ResourceType>::RequestRetire(){

	BASE_NODE::node_base_array_->RequestRetire();
}

// ------------------------------------------------------------------
template<typename ResourceType>
void		Root<ResourceType>::Reset(ResetInfo &reset_info){

	BASE_NODE::node_base_array_->Initialize();

	InitializeInfo info(BASE_NODE::node_base_array_, BASE_NODE::node_base_array_->GenScale());
	BASE_NODE::Initialize(BASE_NODE::Resource(), info);

	// ルートは無条件でアクティブ
	BASE_NODE::ActivateNode(0, 1);
	BASE_NODE::ActivateNodeDependParent(0, 1, NULL);

	BASE_NODE::node_base_array_->Renderer().Reset(reset_info);

	// 途中から再生を始める
	int advanced_frame = bm3::fileformat::bmb::RootAdvancedFrame((const void*)BASE_NODE::node_base_array_->BmbHeader());
	if(advanced_frame!=0){
		UpdateInfo	update_info;
		for(int i=0 ; i<advanced_frame; ++i){
			Update(update_info);
		}
	}

}

// ------------------------------------------------------------------
template<typename ResourceType>
void		Root<ResourceType>::DestroyResource(){
	BASE_NODE::node_base_array_->Renderer().DestroyResource(0);
}

// ------------------------------------------------------------------
template<typename ResourceType>
void		Root<ResourceType>::RestoreResource(RestoreInfo &info){
	BASE_NODE::node_base_array_->Renderer().RestoreResource(
		info,
		0
	);
}

// ------------------------------------------------------------------
template<typename ResourceType>
void	Root<ResourceType>::SetMatrix(const ml::matrix44 &matrix){
	BASE_NODE::node_base_array_->SetMatrix(matrix);
}

// ------------------------------------------------------------------
template<typename ResourceType>
int		Root<ResourceType>::TotalVertexBufferSize() const {
	return BASE_NODE::node_base_array_->TotalVertexBufferSize();
}

// ------------------------------------------------------------------
template<typename ResourceType>
void		Root<ResourceType>::SetColorScale(const ColorType &color_scale){
	BASE_NODE::node_base_array_->SetColorScale(color_scale);
}

// ------------------------------------------------------------------
template<typename ResourceType>
const ColorType &		Root<ResourceType>::ColorScale(){
	return BASE_NODE::node_base_array_->ColorScale();
}

// ------------------------------------------------------------------
template<typename ResourceType>
void		Root<ResourceType>::SetOffsetPosition(const ml::vector3d &offset_position){
	BASE_NODE::node_base_array_->SetOffsetPosition(offset_position);
}

// ------------------------------------------------------------------
template<typename ResourceType>
const ml::vector3d &		Root<ResourceType>::OffsetPosition(){
	return BASE_NODE::node_base_array_->OffsetPosition();
}

// ------------------------------------------------------------------
template<typename ResourceType>
void	Root<ResourceType>::RegistryCreatePrimitiveCallback(void (*callback)(internal::CreatePrimitiveCallbackInfo &info, void *data), void *data){

	BASE_NODE::node_base_array_->RegistryCreatePrimitiveCallback(callback, data);
}

// ------------------------------------------------------------------
template<typename ResourceType>
void	Root<ResourceType>::RegistryDestoryPrimitiveCallback(void (*callback)(internal::DestoryPrimitiveCallbackInfo &info, void *data), void *data){
	BASE_NODE::node_base_array_->RegistryDestoryPrimitiveCallback(callback, data);
}

// ------------------------------------------------------------------
template<typename ResourceType>
int Root<ResourceType>::MaxVertexBufferSize() const {
	return BASE_NODE::MaxVertexBufferSize();
}

// ------------------------------------------------------------------
template<typename ResourceType>
int Root<ResourceType>::MaxPrimCount() const {
	return BASE_NODE::MaxPrimCount();
}

#ifdef BM3_PLATFORM_playstation3
	
// ------------------------------------------------------------------
template<typename ResourceType>
template<typename UpdateInfo>
void	Root<ResourceType>::RawSpuUpdate(UpdateInfo &info){

	BASE_NODE::node_base_array_->ResetNodeCount();
	BASE_NODE::RawSpuUpdate(info);

	RawSpuManager::instance().BeginTaskManager();
	BASE_NODE::MakeUpdateList(info);
	//info.BootSpurs();
	info.Sync();
//	RawSpuManager::instance().Update();	// SPUの起動
//	RawSpuManager::instance().EndTaskManager();
}

// ------------------------------------------------------------------
template<typename ResourceType>
template<typename UpdateInfo>
void	Root<ResourceType>::RawSpuUpdateField(UpdateInfo &info){
	BASE_NODE::RawSpuUpdateField(info);
}

// ------------------------------------------------------------------
template<typename ResourceType>
void	Root<ResourceType>::RawSpuMakeVertex(DrawInfo &info){

	int index = VertexManager::instance().MakeVertexIndex();
	BASE_NODE::node_base_array_->SetVertexBufferIndex(index);
	BASE_NODE::node_base_array_->Renderer().BeginVertexBuffer(info, index);
	{
		BASE_NODE::RawSpuMakeVertex(info);
		RawSpuManager::instance().MakeVertex();	// SPUの起動
	}
	BASE_NODE::node_base_array_->Renderer().EndVertexBuffer(info, index);
}
	
#endif	

#ifdef BM3_PLATFORM_xbox360
// ------------------------------------------------------------------
template<typename ResourceType>
BOOL Root<ResourceType>::IsVertexBufferBusy(){
	return BASE_NODE::node_base_array_->Renderer().IsVertexBufferBusy();
}
#endif


#undef	BASE_NODE

}	// namespace control
}	// namespace obj
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_OBJECT_CONTROL_BM3_OBJ_CONTROLROOT_INL_H
