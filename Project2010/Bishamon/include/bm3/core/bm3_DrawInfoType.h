#ifndef BM3_SDK_INC_BM3_CORE_BM3_DRAWINFOTYPE_H
#define BM3_SDK_INC_BM3_CORE_BM3_DRAWINFOTYPE_H

#include <ml/type/ml_matrix44.h>
#include <ml/type/ml_vector3d.h>
#include "../system/bm3_Config.h"
#include "bm3_CoreType.h"
#include "bm3_VertexInfo.h"
#include "fileformat/bm3_bmb_NodeHeader.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
	
/// @brief	OnPrimitiveDrawInfoクラス
struct OnPrimitiveDrawInfo{

	OnPrimitiveDrawInfo(){;}

	OnPrimitiveDrawInfo(
		const void *resource,
		const VertexInfo *info
	) :
	resource_(resource),
	vertex_info_(info){
		;
	}

	const void							*resource_;
	const VertexInfo					*vertex_info_;
};
	
/// @brief	OnDrawInfoクラス
struct OnDrawInfo{

	OnDrawInfo(){;}

	OnDrawInfo(
		const fileformat::bmb::NodeHeader *node_header,
		const void *resource,
		const VertexInfo *info
	) :
	node_header_(node_header),
	resource_(resource),
	vertex_info_(info){
		;
	}

	const fileformat::bmb::NodeHeader	*node_header_;	// リソースのノード情報。ノードタイプなどが取得可能です。
	const void							*resource_;		// ノードごとのリソース情報。ツールで設定されたノードのパラメータを取得できます
	const VertexInfo					*vertex_info_;	// 頂点ストリーム情報を取得可能です。
};

///	@brief	OnSetRenderStateInfoクラス
struct OnSetRenderStateInfo{

	OnSetRenderStateInfo(){;}

	OnSetRenderStateInfo(
		const fileformat::bmb::NodeHeader *node_header,
		const void *resource
	) :
	node_header_(node_header),
	resource_(resource){
		;
	}

	const fileformat::bmb::NodeHeader	*node_header_;	// リソースのノード情報。ノードタイプなどが取得可能です。
	const void							*resource_;		// ノードごとのリソース情報。ツールで設定されたノードのパラメータを取得できます
};
	
///	@brief	ShaderInfoクラス
struct	ShaderInfo{
	ShaderInfo(){}
	ShaderInfo(
		const fileformat::bmb::NodeHeader	*node_header,
		const void							*resource,
		const ColorType						&color_scale,
		const ml::vector3d					&offset_position
	) :
		node_header_(node_header),
		resource_(resource),
		color_scale_(color_scale),
		offset_position_(offset_position),
		world_matrix_(NULL),
		texture_scroll_(NULL),
#ifdef BM3_PLATFORM_playstation3
		fog_influence_(0.0f),
		location_type_(0)
#else
		fog_influence_(0.0f)
#endif
		{
			;
		}

	const fileformat::bmb::NodeHeader	*node_header_;	// リソースのノード情報。ノードタイプなどが取得可能です
	const void							*resource_;		// ノードごとのリソース情報。ツールで設定されたノードのパラメータを取得です
	ColorType		color_scale_;		// ユーザーに設定されたカラースケール値
	ml::vector3d	offset_position_;	// ユーザーに設定された位置のオフセット
	ml::matrix44	*world_matrix_;		//モデルのみ使用
	ml::vector3d	*texture_scroll_;	//モデルのみ使用
	float			fog_influence_;		//モデルのみ使用
#ifdef BM3_PLATFORM_playstation3
	int				location_type_;
#endif
	bool			lighting_;
};

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BM3_DRAWINFOTYPE_H
