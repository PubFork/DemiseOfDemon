#ifndef BM3_SDK_INC_BM3_CORE_BM3_UPDATEINFO_H
#define BM3_SDK_INC_BM3_CORE_BM3_UPDATEINFO_H

#include "../system/bm3_Config.h"
#include "fileformat/bm3_bmb.h"
#include "primitive/bm3_Primitive.h"					//
#include "resource/bm3_res_Particle.h"
#include "resource/bm3_res_SimpleParticle.h"
#include "resource/bm3_res_MassParticle.h"
#include "resource/bm3_res_Quad.h"
#include "resource/bm3_res_Model.h"
#include "resource/bm3_res_Stripe.h"
#include "bm3_CoreType.h"
#include <ml/type/ml_basic.h>
#include <ml/type/ml_type2type.h>


namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

template<NodeTypeConst N>	struct NodeTypeToResource{};
template<>	 				struct NodeTypeToResource<NodeTypeConst_EmitterQuad>{			typedef res::Quad			Type;	};
template<>	 				struct NodeTypeToResource<NodeTypeConst_EmitterParticle>{		typedef res::Particle		Type;	};
template<>	 				struct NodeTypeToResource<NodeTypeConst_EmitterSimpleParticle>{	typedef res::SimpleParticle	Type;	};
template<>	 				struct NodeTypeToResource<NodeTypeConst_EmitterMassParticle>{	typedef res::MassParticle	Type;	};
template<>	 				struct NodeTypeToResource<NodeTypeConst_EmitterModel>{			typedef res::Model			Type;	};
template<>	 				struct NodeTypeToResource<NodeTypeConst_EmitterStripe>{			typedef res::Stripe			Type;	};

/// @brief PrimitiveInfoクラス
struct PrimitiveInfo{
	PrimitiveInfo(){;}

	PrimitiveInfo(
		const fileformat::bmb::NodeHeader *node_header,
		const void *resource,
		void *active_primitive_begin,
		void *active_primitive_end
	) :
		node_header_(node_header),
		resource_(resource),
		active_primitive_begin_(active_primitive_begin),
		active_primitive_end_(active_primitive_end){;}

	const fileformat::bmb::NodeHeader	*node_header_;
	const void							*resource_;
	void								*active_primitive_begin_;
	void								*active_primitive_end_;
};

/// @brief UpdateInfoクラス
class UpdateInfo{
private:
	float					update_frame_;
	UpdateFixPoint::Type	update_fix_point_frame_;

public:
	virtual ~UpdateInfo(){}

	BM3_FORCEINLINE void	SetUpdateFrame(float frame){
		update_frame_				= frame;
		update_fix_point_frame_		= UpdateFixPoint::FloatToFixPoint(frame);
	};
	BM3_FORCEINLINE float					GetUpdateFrame() const{			return update_frame_;			}
	BM3_FORCEINLINE UpdateFixPoint::Type	GetFixPointUpdateFrame() const{	return update_fix_point_frame_;	}

	UpdateInfo() :
//		UpdateFrameRate_(1.0f),
		generate_child_scale_(1.0f){

		SetUpdateFrame(1.0f);
	}

	/*!
		@brief	プリミティブことに更新直前に呼び出される
		@param	info	プリミティブに関する情報オブジェクト
		@return なし
		@detail	このクラスを継承することでプリミティブの状態を変更、参照できる
	*/
	BM3_FORCEINLINE virtual void OnPrimitiveUpdateBegin(PrimitiveInfo &/*info*/){

		;
	}

	/*!
		@brief	プリミティブことに更新直後に呼び出される
		@param	info	プリミティブに関する情報オブジェクト
		@return なし
		@detail	このクラスを継承することでプリミティブの状態を変更、参照できる
	*/
	BM3_FORCEINLINE virtual void OnPrimitiveUpdateEnd(PrimitiveInfo &/*info*/){

		;
	}

	float	GenerateChildScale() const{ 		return generate_child_scale_;					}
	void	SetGenerateChildScale(float scale){	generate_child_scale_	= ml::maximum(0.0f, scale);	}

protected:
	float		generate_child_scale_;
};

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BM3_UPDATEINFO_H
