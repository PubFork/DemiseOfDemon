#ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_PRIM_MASSARTICLE_H
#define BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_PRIM_MASSARTICLE_H

#include "bm3_Primitive.h"
#include "../bm3_UpdateInfo.h"
#include "../field/bm3_field_Gravity.h"
#include "../field/bm3_field_Wind.h"
#include "../field/bm3_field_Vortex.h"
#include "../field/bm3_field_Radialpower.h"
#include "../field/bm3_field_Deceleration.h"
#include "../field/bm3_field_Collision.h"
#include "../resource/bm3_res_Gravity.h"
#include "../resource/bm3_res_Wind.h"
#include "../resource/bm3_res_Vortex.h"
#include "../resource/bm3_res_Radialpower.h"
#include "../resource/bm3_res_Deceleration.h"
#include "../resource/bm3_res_Collision.h"
#include "../policy/Field/bm3_policy_Bounding_Sphere.h"
#include <ml/utility/ml_simpletimer.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class DrawInfo;

namespace prim{
namespace massparticle{

#if defined(BM3_TARGET_IDE)
	#define	BM3_FIELD_RESOURCE_TYPE(NODE_TYPE)	res::IdeNode
#endif
#if defined(BM3_TARGET_EMBEDDED)
	#define	BM3_FIELD_RESOURCE_TYPE(NODE_TYPE)	res::NODE_TYPE
#endif


/*!
	@brief	マスパーティクルエミッタ一つ一つのパラメータの処理
	@note	このエミッターに関連するフィールドの処理も行っている
			関数は他のエミッタと同じインターフェイスになっている
*/
class PrimitiveManipulator{

public:
	/*!
		@brief	各種パラメータの初期化
		@param	prim_begin	エミッターの先頭ポインタ
		@param	prim_end	エミッターの終了ポインタ
		@param	res			エミッターリソースクラスの参照
		@param	random		ランダムクラス
		@param	random_seed	デフォルトのランダムシード（同期用）
		@return なし
	*/
	template<
		typename PrimitiveType,
		typename ResourceType
	>
	static void Initialize(PrimitiveType *prim_begin, PrimitiveType *prim_end, const ResourceType &res, ml::random random, unsigned int /*random_seed*/){

		ML_INITIALIZE_TIMER;
		ML_PTR_ASSERT(prim_begin);
		ML_PTR_ASSERT(prim_end);

		{
			

			for(PrimitiveType *p = prim_begin ;p != prim_end ;++ p){

				p->Initialize(res, random);

				p->scale_				= 1.0f;
				p->position_.set_unit();
				p->velocity_			= 0.0f;
				p->gravity_.set_unit();
				p->deceleration_		= 1.0f;
				p->noise_scale_			= 1.0f;
				p->noise_frequency_		= 1.0f;
				p->alpha_				= 1.0f;
				p->color_center_.value_.e_.r_	= 1.0f;
				p->color_center_.value_.e_.g_	= 1.0f;
				p->color_center_.value_.e_.b_	= 1.0f;
				p->color_center_.value_.e_.a_	= 1.0f;

				p->is_updated_	= false;
			}
		}
	}

	template<typename PrimitiveType, typename ResourceType>
	static void InitializeField(PrimitiveType * /*prim_begin*/, PrimitiveType * /*prim_end*/, ml::ui8*  /*field_begin*/, const ResourceType & /*res*/, ml::random & /*random*/){
		;// フィールドは扱っていない
	}
	
	template<typename PrimitiveType, typename ResourceType, typename UpdateInfo>
	static int Update(PrimitiveType *prim_begin, PrimitiveType *prim_end, int start_prim_index, int **active_node_index, UpdateInfo &update_info, const ResourceType &res, ml::random & /*random*/){
		
		ML_UPDATE_TIMER;
		ML_PTR_ASSERT(prim_begin);
		ML_PTR_ASSERT(prim_end);

#if defined(BM3_TARGET_EMBEDDED)
		PrimitiveInfo uipi(
			&res.node_header_,
			reinterpret_cast<const void *>(&res),
			prim_begin,
			prim_end
		);
		update_info.OnPrimitiveUpdateBegin(uipi);
#endif

		int active_count			= 0;
		int	*temp_active_node_index	= *active_node_index;
		for(PrimitiveType *prim = prim_begin ; prim != prim_end ; ++prim, ++start_prim_index){

			PrimitiveType &p = *prim;

			PrefetchData<sizeof(PrimitiveType)>(prim + 1);
//			PrefetchData<sizeof(read)>(read + 2);

			bool	is_first_frame	= p.IsFirstFrame();
			p.life_.Update(update_info);

			if(p.IsActive()){
				*temp_active_node_index = start_prim_index;

				if(is_first_frame == false){
					
					Update(p, update_info);
				}
				temp_active_node_index++;
				active_count++;
			}
		}

#if defined(BM3_TARGET_EMBEDDED)
		update_info.OnPrimitiveUpdateEnd(uipi);
#endif
		return active_count;
	}

	template<typename PrimitiveType, typename ResourceType, typename UpdateInfo>
	static int Update(UpdateInfo &update_info, const ResourceType &res, PrimitiveType *prim_begin, int *active_node_index, int active_node_count, const PrimitiveUpdateInfo & /*pui*/, int begin_count){
		ML_UPDATE_TIMER;
		ML_PTR_ASSERT(prim_begin);

		for(int i=0 ; i<active_node_count ; ++i){
			int index = active_node_index[i] - begin_count;
			if(active_node_count<index)	return i;

//			PrefetchData<sizeof(PrimitiveType)>(prim + 1);

			Update(prim_begin[index], update_info);
		}

		return active_node_count;
	}

	template<typename PrimitiveType>
	static BM3_FORCEINLINE void Update(PrimitiveType &p, UpdateInfo &update_info){
		p.is_updated_	= true;
		p.generate_scale_	= update_info.GenerateChildScale();
		p.update_frame_ = update_info.GetUpdateFrame();
	}

	template<typename PrimitiveType, typename ResourceType>
	static void ZSort(PrimitiveType *prim_begin, int active_node_count, int *active_node_index_array, DrawInfo &draw_info, const ResourceType & /*res*/, WholeZInfo &z_info, PrimZList &z_list){
		ML_MAKEVERTEX_TIMER;

		if(z_info.is_enabled_){
			for(int i=0 ; i<active_node_count ; ++i){
				int	index = active_node_index_array[i];
				PrimitiveType	&prim	= prim_begin[index];

				ml::vector3d	pos	= prim.parent_matrix_->translate();
#ifdef BM3_TARGET_IDE
				const ml::vector3d	&view_pos	= draw_info.ViewMatrix().translate();
				pos.mul(draw_info.IdeEffectMatrix());
#else
				const ml::vector3d	&view_pos	= draw_info.InverseViewMatrix().translate();
#endif
				pos.mul(draw_info.ViewMatrix());
				prim.SetZ(pos.e_.z_ - view_pos.e_.z_ );

				PrimZInfo &pzi		= z_list.Push();
				pzi.emitter_index_	= z_info.emitter_index_;
				pzi.draw_count_		= 1;
				pzi.z_value_		= prim.GetZ();
				pzi.vertex_buffer_	= ml::offset_pointer<void*>(z_info.vertex_head_, i*z_info.prim_vtx_size_);
			}
		}
	}
#if 0
	template<typename PrimitiveType, typename ResourceType>
	static void Prepare(PrimitiveType *prim, int active_node_count, int *active_node_index_array, UpdateInfo & /*update_info*/, const ResourceType &res, const obj::RootNodeBase & /*root*/){

		ML_MAKEVERTEX_TIMER;
		ML_PTR_ASSERT(prim);
		ML_PTR_ASSERT(active_node_index_array);

		for(int k=0 ;k < active_node_count; ++ k){
			int index	= active_node_index_array[k];
			PrimitiveType	*p		= &prim[index];
			
			PrefetchData<sizeof(PrimitiveType)>(&prim[active_node_index_array[k+1]]);

		}
	}
#endif	
	template<typename PrimitiveType, typename ResourceType>
	static void PrepareField(PrimitiveType * /*prim*/, int /*active_node_count*/, int * /*active_node_index_array*/, ml::ui8* /*field_buffer*/, int /*field_all_size*/, UpdateInfo & /*update_info*/, const ResourceType & /*res*/, const obj::RootNodeBase & /*root*/){
	}

	template<typename ResourceType>
	static void SetFacingSetting(PacketDrawInfo & /*pdi*/, DrawInfo & /*draw_info*/, const ResourceType & /*res*/){
	}

	template<typename PrimitiveType, typename ResourceType, typename DrawType>
	static void MakeVertex(DrawType &dst, const PrimitiveType *prim, int /*active_node_count*/, int *active_node_index_array, DrawInfo &draw_info, const PacketDrawInfo &packet_draw_info, const ResourceType &res, void*  /*vertex_buffer*/, bool is_interchanged_bm2){
		
		ML_MAKEVERTEX_TIMER;
		ML_PTR_ASSERT(prim);

		int k=0;
		if(is_interchanged_bm2==true)
		{
	//		for(int k=0 ;k < active_node_count; ++ k){
				int index	= active_node_index_array[k];
				PrimitiveType	&p		= *const_cast<PrimitiveType *>(&prim[index]);

				dst.template PushRangeBm2<>(	prim, draw_info, packet_draw_info, res, p.is_updated_);
				p.is_updated_	= false;
	//		}
		}
		else
		{
	//		for(int k=0 ;k < active_node_count; ++ k){
				int index	= active_node_index_array[k];
				PrimitiveType	&p		= *const_cast<PrimitiveType *>(&prim[index]);

				dst.template PushRange<>(	prim, draw_info, packet_draw_info, res, p.is_updated_);
				p.is_updated_	= false;
	//		}
		}

	}

	template<typename PrimitiveType, typename ResourceType>
	static void SwitchResource(PrimitiveType * /*prim_begin*/, int /*active_node_count*/, int * /*active_node_index_array*/, const ResourceType & /*res*/, const ResourceType & /*old_res*/, ml::random & /*random*/){

	}
	
	template<typename PrimitiveType, typename ResourceType>
	static void SwitchResourceField(PrimitiveType * /*prim*/, int /*active_node_count*/, int * /*active_node_index_array*/, ml::ui8* /*field_buffer*/, int /*field_all_size*/, const ResourceType & /*res*/, const ResourceType & /*old_res*/, ml::random & /*random*/){
	}

	template<typename PrimitiveType, typename ResourceType>
	static void SetDriveMatrix(prim::Packet *packet_array, PrimitiveType *prim_begin, int active_node_count, int *active_node_index_array, const ResourceType & /*res*/, ParentParam * /*parent_param*/){

		for(int i=0 ; i<active_node_count ; ++i){
			int				index	= active_node_index_array[i];
			PrimitiveType	&p		= prim_begin[index];
			prim::Packet	*packet	= &packet_array[index];

			ml::matrix44	&m = packet->generate_time_hierarchy_parent_matrix_;
			ml::vector3d	&v = packet->generate_time_hierarchy_parent_matrix_row_length_;

			m.set_unit();
			v.set_unit();

			p.SetParentMatrix(m);
			p.SetParentScale(v);
		}
	}
};

}	// namespace massparticle
}	// namespace prim
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_PRIM_MASSPARTICLE_H
