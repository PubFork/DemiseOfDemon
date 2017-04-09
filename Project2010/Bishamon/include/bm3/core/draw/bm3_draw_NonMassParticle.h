#ifndef BM3_SDK_INC_BM3_CORE_DRAW_BM3_DRAW_NONMASSPARTICLE_H
#define BM3_SDK_INC_BM3_CORE_DRAW_BM3_DRAW_NONMASSPARTICLE_H

#include "../../system/bm3_Config.h"
#include "policy/bm3_draw_policy_Position6.h"
#include "policy/bm3_draw_policy_Color6.h"
#include "policy/bm3_draw_policy_Texture6.h"
#include "policy/bm3_draw_policy_Common.h"
#include "policy/bm3_draw_policy_HasTexture.h"
#include "policy/bm3_draw_policy_DefaultRenderState.h"
#include "../bm3_CoreType.h"
#include "../bm3_RenderStateManager.h"
#include "../bm3_ProfileManager.h"
#include "../bm3_ResourceManager.h"
#include "bm3_draw_Common.h"
#include "../object/bm3_obj_RootNodeBase.h"
#include "../bm3_VertexInfo.h"
#include "../bm3_VertexManager.h"

#include <ml/type/ml_matrix44.h>
#include <ml/type/ml_color.h>
#include <ml/utility/ml_simpletimer.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class DrawInfo;

namespace draw{

/// @brief MassParticleƒNƒ‰ƒX
class MassParticle :
	public policy::HasTexture,
	public policy::DefaultRenderState{

public:
	MassParticle(){}
	~MassParticle(){
		Finalize();
	}

	template<typename ResourceType>
	void	Initialize(InitializeInfo & /*info*/, const ResourceType &/*res*/){
	}

	template<typename ResourceType>
	void	Initialize(InitInfo & /*info*/, const ResourceType & /*res*/){
	}
		
	void Reset(ResetInfo & /*reset_info*/){
	}
		
	void	Finalize(){
	}

	template<
		typename ResourceType
	>
	void	Begin(DrawInfo & /*draw_info*/, const ResourceType & /*res*/){
	}

	template<
		typename ResourceType
	>
	void	End(DrawInfo & /*draw_info*/, ShaderInfo & /*shader_info*/, VertexContext & /*ctx*/, int /*child_count*/, int /*push_count*/, const ResourceType & /*res*/){

	}

	template<
		typename PrimitiveType,
		typename ResourceType
	>
	void	PushRange(const PrimitiveType * /*prim*/, DrawInfo & /*draw_info*/, const PacketDrawInfo &/*packet_draw_info*/, const ResourceType &/*res*/, bool /*is_updated*/){
	}

	// BM2ŒÝŠ·
	template<
		typename PrimitiveType,
		typename ResourceType
	>
	void	PushRangeBm2(const PrimitiveType * /*prim*/, DrawInfo & /*draw_info*/, const PacketDrawInfo &/*packet_draw_info*/, const ResourceType &/*res*/, bool /*is_updated*/){
	}

	void BeginMakeVertex(obj::RootNodeBase & /*root*/, int /*emitter_index*/){
	}

	void EndMakeVertex(obj::RootNodeBase & /*root*/, int /*emitter_index*/){
	}

		int  TotalResourceSize(){	return 0;	}
	int VertexBufferSize(int child_count){ return child_count * VertexPerPrimitiveCount * sizeof(VertexType); }
	int VertexBufferSize(int child_count, int /*push_count*/){ return VertexBufferSize(child_count); }
	int TotalVertexBufferSize(int child_count, int /*push_count*/){ return VertexBufferSize(child_count) + TotalResourceSize(); }

	template<typename ResourceType>
	void GetVertexInfo(VertexInfo & /*info*/, int /*child_count*/, int /*push_count*/, const ResourceType & /*res*/){
	}

protected:
	template<
		typename ResourceType
	>
	void	EndCore(DrawInfo & /*draw_info*/, EmitterTypeConst /*emitter_type*/, VertexContext & /*ctx*/, int /*child_count*/, ResourceType & /*res*/){
	}

private:
	enum{
		VertexPerPrimitiveCount	= 6
	};

	typedef	draw::policy::PositionType	PositionType;

	struct VertexType{
		TextureCoordinate	texture_;
		draw::policy::ByteColorRgba			color_;
		PositionType		position_;
	};
};

}	// namespace draw
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3


#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DRAW_BM3_DRAW_NONMASSPARTICLE_H
