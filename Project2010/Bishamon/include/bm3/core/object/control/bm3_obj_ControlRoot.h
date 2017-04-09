#ifndef BM3_SDK_INC_BM3_CORE_OBJECT_CONTROL_BM3_OBJ_CONTROLROOT_H
#define BM3_SDK_INC_BM3_CORE_OBJECT_CONTROL_BM3_OBJ_CONTROLROOT_H

#include "bm3_obj_ControlBase.h"
#include "bm3/core/object/bm3_obj_RootNodeBase.h"
#include "bm3/core/resource/bm3_res_Root.h"
#include "bm3/core/policy/Life/bm3_policy_Life_Infinity.h"
#include "bm3/core/policy/ChildGenerateTiming/bm3_policy_ChildGenerateTiming_Nothing.h"
#include <ml/type/ml_basic.h>
#include <ml/math/ml_random.h>
#include "bm3/core/bm3_VertexInfo.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class InitializeInfo;
class DrawInfo;

namespace fileformat{
namespace bmb{
struct NodeHeader;
}	// namespace bmb
}	// namespace fileformat

namespace obj{
namespace control{

template<typename ResourceType = res::Root>
class Root : public Base<ResourceType, RootNodeBase, policy::Life_Infinity, policy::ChildGenerateTiming_Nothing>{

public:
	typedef	Root			ThisType_;
	typedef	ResourceType	ResourceType_;

	Root();
	~Root();

#if defined(BM3_TARGET_IDE)
	bool	Initialize(const ResourceType &res);
#endif
	/*!
		@brief	各ノードのメモリ確保と初期化を行う
		@param	res_root	ルートノードのリソース
		@param	header		bmbのヘッダ
		@param	gen_scale	生成数の割合
		@return	メモリ確保に失敗するとfalseを返す
	*/
	bool	Initialize(const ResourceType &res, const fileformat::bmb::Header *header, const float gen_scale);
	bool	InitializePrimitive(InitializeInfo &info);
	/*!
		@brief	各ノードですでに初期化できるものはしておく
		@param	init_info	InitInfo
		@return	初期化の成否
		@detail	現状マスパーティクルの初期メモリを確保する。
				ここで初期化されない場合は描画直前にメモリを確保する。
	*/
	bool	Initialize(InitInfo &init_info);

	void	Finalize();

	/*!
		@brief	エフェクトが終了したかどうかを返す
		@return	エフェクトが終了したかどうか
	*/
	bool	IsExpired() const;

	/*!
		@brief	エフェクトを更新する
		@param	info	UpdateInfo
		@return	なし
	*/
	template<typename UpdateInfo>
	void	Update(UpdateInfo &info);
	/*!
		@brief	エフェクトの頂点を作成する
		@param	info	DrawInfo
		@return	なし
		@detail	指定された頂点バッファに書き込みを行う
	*/
	void	MakeVertex(DrawInfo &info);
	/*!
		@brief	エフェクトを描画する
		@param	info	DrawInfo
		@return	なし
	*/
	void	Draw(DrawInfo &info);

	int		NodeCount();
	void	NodeVertexInfo(int emitter_id, VertexInfo &info, int vertex_buffer_id = 0);

	int		SwitchEffect(const ResourceType &res, const fileformat::bmb::Header *header);

	/*!
		@brief	エフェクトの生成を強制的になくす。
		@return	なし。
		@detail	自然に消えたように見せる効果があるので余韻を残す場合に使用できる。
	*/
	void	RequestRetire();

	/*!
		@brief	エフェクトを初期状態に戻す。
		@param	info	ResetInfo
		@return	なし。
		@detail	メモリの再確保は行わず、初期状態に戻す。
	*/
	void	Reset(ResetInfo &reset_info);

	void	DestroyResource();
	void	RestoreResource(RestoreInfo &info);

	/*!
		@brief	エフェクトのルートにマトリックスを設定する。継承関係も考慮される。
		@param	matrix	設定するマトリックス
		@return	なし
		@detail	更新処理をする前までに設定しておく必要がある。
				ルートに設定するので継承設定が反映される。
	*/
	void	SetMatrix(const ml::matrix44 &matrix);
	/*!
		@brief	すべてのノードで使用する頂点バッファサイズの総和を返す
		@return すべてのノードで使用する頂点バッファサイズの総和
	*/
	int		TotalVertexBufferSize() const ;
	int		MaxVertexBufferSize() const;
	int		MaxPrimCount() const;

	/*!
		@brief	エフェクトにカラーを掛け合わせる
		@param	color_scale	掛け合わせるカラー
		@return	なし
		@detail	頂点カラーに掛け合わされる。
				頂点作成までに設定しておく必要がある。
	*/
	void					SetColorScale(const ColorType &color_scale);
	const ColorType &		ColorScale();

	/*!
		@brief	エフェクトに位置を足し合わせる
		@param	offset_position	足し合わせる位置
		@return	なし
		@detail	親なマトリックスなどの影響は受けず、単純に頂点座標に足し合わせるだけで他には何もしない。
				頂点作成までに設定しておく必要がある。
	*/
	void					SetOffsetPosition(const ml::vector3d &offset_position);
	const ml::vector3d &	OffsetPosition();

	void	RegistryCreatePrimitiveCallback(void (*callback)(internal::CreatePrimitiveCallbackInfo &info, void *data), void *data);
	void	RegistryDestoryPrimitiveCallback(void (*callback)(internal::DestoryPrimitiveCallbackInfo &info, void *data), void *data);
	
#ifdef BM3_PLATFORM_playstation3
	template<typename UpdateInfo>
	void	RawSpuUpdate(UpdateInfo &info);
	template<typename UpdateInfo>
	void	RawSpuUpdateField(UpdateInfo &info);
	void	RawSpuMakeVertex(DrawInfo &info);
#endif

#ifdef BM3_PLATFORM_xbox360
	BOOL IsVertexBufferBusy();
#endif

private:
	void	MakeVertexChild(DrawInfo &info);
#if defined(BM3_TARGET_IDE)
	void MakeEmitterInfo(const ResourceType &res);
#endif

};

}	// namespace control
}	// namespace obj
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#include "bm3_obj_ControlRoot_inl.h"

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_OBJECT_CONTROL_BM3_OBJ_CONTROLROOT_H
