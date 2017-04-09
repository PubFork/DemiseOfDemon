#ifndef BM3_SDK_INC_BM3_CORE_BM3_EFFECT_H
#define BM3_SDK_INC_BM3_CORE_BM3_EFFECT_H

#include "object/control/bm3_obj_ControlRoot.h"
#include "object/bm3_obj_RootNodeBase.h"
#include "bm3_CoreType.h"
#include "bm3_VertexInfo.h"
#include "bm3_RestoreInfo.h"


namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

typedef	obj::internal::CreatePrimitiveCallbackInfo	CreatePrimitiveCallbackInfo;
typedef	obj::internal::DestoryPrimitiveCallbackInfo	DestoryPrimitiveCallbackInfo;

class DrawInfo;

namespace fileformat{
namespace bmb{
struct NodeHeader;
}	// namespace bmb
}	// namespace fileformat

/// @brief	エフェクトクラス
template<typename ResRootType = res::Root>
class Effect{

public:
	Effect();
	~Effect();

#if defined(BM3_TARGET_IDE)
	bool	Initialize(const ResRootType &res_root);
#endif
	/*!
		@brief	各ノードのメモリ確保と初期化を行う
		@param	res_root	ルートノードのリソース
		@param	header		bmbのヘッダ
		@param	gen_scale	生成数の割合
		@return	メモリ確保に失敗するとfalseを返す
	*/
	bool	Initialize(const ResRootType &res_root, const fileformat::bmb::Header *header, const float gen_scale);
	/*!
		@brief	各ノードですでに初期化できるものはしておく
		@param	init_info	InitInfo
		@return	初期化の成否
		@detail	現状マスパーティクルの初期メモリを確保する。
				ここで初期化されない場合は描画直前にメモリを確保する。
	*/
	bool	Initialize(InitInfo &init_info);

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

	/*!
		@brief	使用する頂点バッファのサイズを返す
		@return	なし
		@detail	発生するパーティクルの数によって毎フレームサイズが変更し得る。
				更新処理をした後に呼び出す必要がある。
	*/
	int		VertexBufferSize() const;
	/*!
		@brief	エフェクトのルートにマトリックスを設定する。継承関係も考慮される。
		@param	matrix	設定するマトリックス
		@return	なし
		@detail	更新処理をする前までに設定しておく必要がある。
				ルートに設定するので継承設定が反映される。
	*/
	void	SetMatrix(const ml::matrix44 &matrix);
	/*!
		@brief	エフェクトにカラーを掛け合わせる
		@param	color_scale	掛け合わせるカラー
		@return	なし
		@detail	頂点カラーに掛け合わされる。
				頂点作成までに設定しておく必要がある。
	*/
	void	SetColorScale(const ColorType &color_scale);
	/*!
		@brief	エフェクトに位置を足し合わせる
		@param	offset_position	足し合わせる位置
		@return	なし
		@detail	親なマトリックスなどの影響は受けず、単純に頂点座標に足し合わせるだけで他には何もしない。
				頂点作成までに設定しておく必要がある。
	*/
	void	SetOffsetPosition(const ml::vector3d &offset_position);

	/*!
		@brief	エフェクトが終了したかどうかを返す
		@return	エフェクトが終了したかどうか
	*/
	bool	IsExpired() const;
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
	void	Reset(ResetInfo &info);


	void	RegistryCreatePrimitiveCallback(void (*callback)(CreatePrimitiveCallbackInfo &info, void *data), void *data);
	void	RegistryDestoryPrimitiveCallback(void (*callback)(DestoryPrimitiveCallbackInfo &info, void *data), void *data);

	int		NodeCount();
	void	NodeVertexInfo(int emitter_id, VertexInfo &info, int vertex_buffer_id=0);

	int		SwitchEffect(const void *bmb_image);

	// random幅を考慮した際の最大頂点バッファサイズ
	int		MaxVertexBufferSize() const;

	
	void	DestroyResource();
	void	RestoreResource(RestoreInfo &info);
	
#ifdef BM3_PLATFORM_playstation3
	template<typename UpdateInfo>
	void	RawSpuUpdate(UpdateInfo &info);
	template<typename UpdateInfo>
	void	RawSpuUpdateField(UpdateInfo &info);
	void	RawSpuMakeVertex(DrawInfo &info);
#endif

#ifdef BM3_PLATFORM_xbox360
	/*!
		@brief	頂点バッファの使用状況を問い合わせる
		@return	頂点が使用されているかどうか
		@detail	どれか１つのマスパーティクルの頂点バッファが使用されている場合はTRUE
				すべてのマスパーティクルの頂点バッファが使用されていなければFALSEを返す
	*/
	BOOL IsVertexBufferBusy();
#endif

private:
	obj::control::Root<ResRootType>	obj_root_;
};

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#include "bm3_Effect_inl.h"

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BM3_EFFECT_H
