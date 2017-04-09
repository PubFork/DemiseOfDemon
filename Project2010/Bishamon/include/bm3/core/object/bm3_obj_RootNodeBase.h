#ifndef BM3_SDK_INC_BM3_CORE_OBJECT_BM3_OBJ_ROOTNODEBASE_H
#define BM3_SDK_INC_BM3_CORE_OBJECT_BM3_OBJ_ROOTNODEBASE_H

#include "bm3_obj_NodeBase.h"
#include "../../system/bm3_Config.h"
#include "../bm3_Const.h"
#include "../bm3_CoreType.h"
#include "../type/bm3_VectorBuffer.h"
#include "../bm3_PrimitiveRenderer.h"
#include "../fileformat/bm3_bmb.h"
#if defined(BM3_TARGET_IDE)
#include "../resource/bm3_res_IdeNode.h"
#endif

#include <ml/type/ml_matrix44.h>
#include <ml/utility/ml_assert.h>
#include "../policy/Life/bm3_policy_Life_Root.h"
#include "../bm3_UpdateInfo.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace obj{

namespace internal{
/// @brief CreatePrimitiveCallbackInfoクラス
struct CreatePrimitiveCallbackInfo{

	CreatePrimitiveCallbackInfo(){;}
	CreatePrimitiveCallbackInfo(
		const fileformat::bmb::NodeHeader *node_header,
		const void *resource,
		void *primitive_begin,
		void *primitive_end
	) :
		node_header_(node_header),
		resource_(resource),
		primitive_begin_(primitive_begin),
		primitive_end_(primitive_end){;}

	const fileformat::bmb::NodeHeader	*node_header_;
	const void							*resource_;
	void								*primitive_begin_;
	void								*primitive_end_;
};

/// @brief DestoryPrimitiveCallbackInfoクラス
struct DestoryPrimitiveCallbackInfo{

	DestoryPrimitiveCallbackInfo(){;}
	DestoryPrimitiveCallbackInfo(
		const fileformat::bmb::NodeHeader *node_header,
		const void *resource,
		void *primitive_begin,
		void *primitive_end
	) :
		node_header_(node_header),
		resource_(resource),
		primitive_begin_(primitive_begin),
		primitive_end_(primitive_end){;}

	const fileformat::bmb::NodeHeader	*node_header_;
	const void							*resource_;
	void								*primitive_begin_;
	void								*primitive_end_;
};
}	// namespce internal

/// @brief RootNodeBaseクラス
class RootNodeBase : public NodeBase{

public:
	RootNodeBase();
	~RootNodeBase();

	void Initialize(){						life_.Initialize(RootLife());	}
	void Update(UpdateInfo &update_info){	life_.Update(update_info);		}
	bool IsExpired() const{					return life_.IsExpired();		}

	void					SetColorScale(const ColorType &color_scale){			color_scale_ = color_scale;			}
	const ColorType &		ColorScale(){											return color_scale_;				}

	void					SetOffsetPosition(const ml::vector3d &offset_position){	offset_position_ = offset_position;	}
	const ml::vector3d &	OffsetPosition(){										return offset_position_;			}

	void			RequestRetire(){			flag_ |= Flag::RequestRetire;					}
	bool			IsRequestRetire() const{	return (flag_ & Flag::RequestRetire) != 0;		}

	void			AddChildCount(int emitter_index, int child_count){	child_count_list_.Set(emitter_index, child_count + child_count_list_.At(emitter_index));}
	void			AddPushCount(int emitter_index, int push_count){	push_count_list_.Set(emitter_index, push_count + push_count_list_.At(emitter_index));	}
	void			ResetChildCount(int emitter_index){					child_count_list_.Set(emitter_index, 0);	push_count_list_.Set(emitter_index, 0);		}

	unsigned int	ChildCount(int id)  {	return child_count_list_.At(id);	}
	unsigned int	PushCount(int id) {		return push_count_list_.At(id);		}

	bool		InitializeNodeList(int emtter_count){
		bool result = true;
		result = child_count_list_.Resize(emtter_count, 0);
		if(!result)return false;
		result = push_count_list_.Resize(emtter_count, 0);
		if(!result)return false;

		return true;
	}
	void			ResetNodeCount(){
		if(&child_count_list_)
			child_count_list_.Assign(ResourceEmitterCount(), 0);
		if(&push_count_list_)
			push_count_list_.Assign(ResourceEmitterCount(), 0);
	}

	void	RegistryCreatePrimitiveCallback(void (*callback)(internal::CreatePrimitiveCallbackInfo &info, void *data), void *data);
	void	RegistryDestoryPrimitiveCallback(void (*callback)(internal::DestoryPrimitiveCallbackInfo &info, void *data), void *data);

	void	CallCreatePrimitiveCallback(internal::CreatePrimitiveCallbackInfo &info){
		if(create_primitive_callback_){
			(*create_primitive_callback_)(info, create_primitive_callback_data_);
		}
	}
	void	CallDestoryPrimitiveCallback(internal::DestoryPrimitiveCallbackInfo &info){
		if(destory_primitive_callback_){
			(*destory_primitive_callback_)(info, destory_primitive_callback_data_);
		}
	}

	EmitterTypeConst	ResourceEmitterType(int emitter_index) const { 
		return res_emitter_info_[emitter_index].type_;
	}
	const void*	ResourceEmitter(int emitter_index) const { 
		return res_emitter_info_[emitter_index].resource_;
	}

	/*
		@brief	エミッター情報のメモリ確保
	*/
	bool AllocResEmitterInfo(int count);
	/*
		@brief	エミッターの数を返す
	*/
	int ResourceEmitterCount() const { return res_emitter_count_;			}
	/*
		@brief	エミッタの情報を設定する
	*/
	void SetResEmitterInfo(float prio, EmitterTypeConst type, const void* res);
	/*
		@brief	エミッター情報をプライオリティの値に基づきソートする
	*/
	void SortResEmitterInfo();

	static unsigned int RandomSeed();

	int EmitterIndex(const void* resource_address) const {
		int index=0;
		for(index=0 ; index<ResourceEmitterCount(); ++index){
			if(ResourceEmitter(index)==resource_address)	break;
		}

		ML_ASSERT(index<ResourceEmitterCount());
		return index;
	}

	//j プリミティブ一つ分の頂点バッファサイズ
	int		UnitVertexBufferSize(int emitter_index)	const{return primitive_renderer_.UnitVertexBufferSize(emitter_index, vertex_index_);	}
	//j 使用する頂点バッファサイズを取得する
	int		VertexBufferSize(int emitter_index)		const{return primitive_renderer_.VertexBufferSize(emitter_index, vertex_index_);	}
	//j 現在使用される全頂点バッファのサイズを取得する
	int		TotalVertexBufferSize()					const{return primitive_renderer_.TotalVertexBufferSize(vertex_index_);	}
	//j 指定されたインデックスのところから頂点バッファを取得する
	void*	VertexBuffer(int emitter_index, int size){	return primitive_renderer_.GetVertexBuffer(emitter_index, size, vertex_index_);	}
	void*	EmitterBuffer(int emitter_index){			return primitive_renderer_.EmitterBuffer(emitter_index, vertex_index_);		}
	//j 生成されたエミッターの生成子の数を取得する
	int		RendererPushCount(int emitter_index){		return primitive_renderer_.PushCount(emitter_index, vertex_index_);		}


	int		VertexBufferCount() const	{	return vertex_buffer_count_;	}
	PrimitiveRenderer& Renderer()		{	return primitive_renderer_;	}

	void	SetBmbHeader(const fileformat::bmb::Header *header){	bmb_header_	= header;			}
	const fileformat::bmb::Header* BmbHeader(){	return bmb_header_;	}

	void	SetGenScale(float scale){	gen_scale_	= scale;	}
	float	GenScale(){					return gen_scale_;		}

	void	SetVertexBufferCount(int count){	vertex_buffer_count_	= count;	}
	void	SetVertexBufferIndex(int index){	vertex_index_			= index;	}

#ifdef BM3_TARGET_IDE
	void	SetEnableWholeZSort(bool b){	is_whole_z_sort_	= b;	}
	bool	IsEnableZSort()	const{			return is_whole_z_sort_;	}

	void	SetResIdeNode(const res::IdeNode *res){	res_ide_node_ = res;		}
	int		RootLife() const{	return res_ide_node_->Basic_Basic_RootLife();	}
#else
	bool	IsEnableZSort()	const{	return fileformat::bmb::IsZSort(reinterpret_cast<const void*>(bmb_header_));	}
	int		RootLife() const{		return fileformat::bmb::RootLife(reinterpret_cast<const void*>(bmb_header_));	}
#endif

	bool	InitializePrimZList(int count);
	void	AddPrimZList(int index, PrimZList *z_list){	child_prim_z_list_[index]	= z_list;	}
	void	ResetZList();
	void	MakeZList();
	PrimZPList*	ZList(){	return prim_z_plist_current_;	}

	void	SetEnableBm2(bool flag){	is_interchanged_bm2_	= flag;		}
	bool	IsEnabledBm2() const {		return is_interchanged_bm2_;		}
protected:

#if defined(BM3_TARGET_IDE)
/*	template<typename ResourceType>
	void MakeEmitterInfo(const ResourceType &res){

		const bm3::res::IdeNode::ChildArrayType_	&child_array	= res.ChildArray();
		int											child_count		= child_array.Count();

		for(int i = 0;i != child_count;++ i){
			EmitterInfo		info;

			#define BM3_EMITTER_INFO()														\
				info.type_				= child_array.Child(i).Basic_Basic_EmitterType();	\
				info.resource_address_	= &child_array.Child(i);							\
				emitter_info_array_.push_back(info);

			switch(child_array.Child(i).Basic_Meta_NodeType()){
				case bm3::NodeTypeConst_ControlNull:									break;
				case bm3::NodeTypeConst_EmitterQuad:			BM3_EMITTER_INFO();		break;
				case bm3::NodeTypeConst_EmitterParticle:		BM3_EMITTER_INFO();		break;
				case bm3::NodeTypeConst_EmitterSimpleParticle:	BM3_EMITTER_INFO();		break;
				case bm3::NodeTypeConst_EmitterMassParticle:	BM3_EMITTER_INFO();		break;
				case bm3::NodeTypeConst_EmitterModel:			BM3_EMITTER_INFO();		break;
				case bm3::NodeTypeConst_EmitterStripe:			BM3_EMITTER_INFO();		break;
				case bm3::NodeTypeConst_FieldGravity:									break;
				case bm3::NodeTypeConst_FieldWind:										break;
				case bm3::NodeTypeConst_FieldVortex:									break;
				case bm3::NodeTypeConst_FieldRadialpower:								break;
				case bm3::NodeTypeConst_FieldDeceleration:								break;
				case bm3::NodeTypeConst_FieldCollision:									break;
				case bm3::NodeTypeConst_FieldGoal:										break;
				case NodeTypeConst_ObjectReference:			break;
				default:						BM3_ASSUME(0);	ML_ASSERT(false);		break;
			}
			MakeEmitterInfo(child_array.Child(i));
		}
	}

	EmitterInfoArray			emitter_info_array_;
*/
	bool						is_whole_z_sort_;
	const res::IdeNode			*res_ide_node_;
#endif
	PrimitiveRenderer				primitive_renderer_;
	const fileformat::bmb::Header	*bmb_header_;
	float							gen_scale_;
	bool							is_interchanged_bm2_;

	typedef	VectorBuffer<unsigned int>		NodeCountType;
	typedef	NodeCountType::Iterator			NodeCountI;
	typedef	NodeCountType::ConstIterator	NodeCountCI;

	NodeCountType							child_count_list_;
	NodeCountType							push_count_list_;
	unsigned int							vertex_buffer_count_;
	unsigned int							vertex_index_;

	int										res_emitter_count_;
	ResEmitterInfo							*res_emitter_info_;

private:
	void SwitchZList(PrimZPList **z0, PrimZPList **z1, int index);

	struct Flag{
		enum Bit{
			RequestRetire = 1<<0
		};
	};

	ColorType		color_scale_;
	ml::vector3d	offset_position_;

	ml::ui32		flag_;

	PrimZPList		prim_z_plist_[2];	// Whole Z Sort Manage
	PrimZPList*		prim_z_plist_current_;
	PrimZList		**child_prim_z_list_;

	policy::Life_Root	life_;


	void	(*create_primitive_callback_)(internal::CreatePrimitiveCallbackInfo &info, void *data);
	void	*create_primitive_callback_data_;
	void	(*destory_primitive_callback_)(internal::DestoryPrimitiveCallbackInfo &info, void *data);
	void	*destory_primitive_callback_data_;
};

}	// namespace obj
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_OBJECT_BM3_OBJ_ROOTNODEBASE_H
