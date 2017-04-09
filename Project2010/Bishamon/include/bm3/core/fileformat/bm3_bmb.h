#ifndef BM3_SDK_INC_BM3_CORE_FILEFORMAT_BMB_H
#define BM3_SDK_INC_BM3_CORE_FILEFORMAT_BMB_H

#include <ml/type/ml_basic.h>
#include <ml/type/ml_vector3d.h>
#include <ml/type/ml_color.h>
#include <ml/utility/ml_assert.h>
#include <ml/utility/ml_make_sign.h>
#include "bm3/core/bm3_Const.h"
#include "bm3/core/bm3_CoreType.h"
#include "bm3/core/resource/policy/bm3_res_policy_StaticChildArray.h"
#include "bm3/utility/internal/bm3_MacroBegin.h"
#include "bm3_bmb_NodeHeader.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace fileformat{
namespace bmb{

struct Header{
	enum{
		//
		Sign						= ML_MAKE_SIGN_32('B','M','B','I'),
		ReverseSign					= ML_MAKE_SIGN_R32('B','M','B','I'),
		//
		Version						= 0x00000037,				// 0x00000037: 親のカラー継承を追加
																// 0x00000036: テクスチャクロップのランダム、ビルボードポリライン、モデルのライトのオンオフ追加
																// 0x00000035: 参照ノード追加
																// 0x00000034: 相対パス名追加
																// 0x00000033: Version1_5タグ追加
																// 0x00000032: 渦フィールドに集束パラメータ追加、bm2互換フラグをデフォルトでtrueに変更
																// 0x00000031: ルートに寿命追加（エフェクト全体の寿命）
																// 0x00000030: 背景画像ノード追加
																// 0x0000002F: ＢＭ２互換モード追加
																// 0x0000002E: エフェクト全体ソート追加
																// 0x0000002D: Header member 不具合修正
																// 0x0000002C: エフェクト全体寿命変数追加
																// 0x0000002B: 最適化ビットフィールド追加
																// 0x0000002A: フィールドにゴール機能追加
																// 0x00000029: フォグ追加
																// 0x00000028: Facingタイプ追加
																// 0x00000027: IDEにガイドを描画するように修正、マスパーティクルの生成形状に法線方向フラグを追加
																// 0x00000026: マスパーティクルにカラーカーブが追加されていたのを削除
																// 0x00000025: マスパーティクルに親の継承関係、ノイズに頻度、生成時間を追加
																// 0x00000024: マスパーティクルにテクスチャアニメ、無限リピートOn/Off機能、
																// 0x00000023: マスパーティクルにアルファアニメーション、スケールランダム、カラーランダム、生成形状、生成開始時間を追加
																// 0x00000022: アルファテスト追加、マスパーティクルに深度バッファ書き込み追加
																// 0x00000021: エミッタ内Zソート追加,フィールドの表示・非表示を追加
																// 0x00000020: マスパーティクル追加
																// 0x0000001F: 風、渦にスケールの影響を追加
																// 0x0000001E: 生成形状、ゴールフィールドにスケールを追加
																// 0x0000001D: 途中から再生させる機能追加
																// 0x0000001C: フィールドゴール追加
																// 0x0000001B: フィールド風、渦追加
																// 0x0000001A: ヌルエミッタにブレンド追加、継承関係にアルファを追加
																// 0x00000019: 文字コード変換機能追加、座標系変換機能追加
																// 0x00000018: ストライプの各関節のランダムを同期できるように修正
																// 0x00000017: シンプルパーティクル追加
																// 0x00000016: アルファのランダム、色チャンネル同期情報追加
																// 0x00000015: 生成形状モデル情報追加
																// 0x00000014: ルートのユーザーデータを保持するようにした
																// 0x00000013: ユーザーデータ情報追加
																// 0x00000012: ソフト系情報追加
																// 0x00000011: テクスチャレイヤー２情報追加
																// 0x00000010: フィールドの風とコリジョンを一旦閉じた
																// 0x0000000F: モデルテクスチャのアドレッシング、フィルタ情報追加
																// 0x0000000E: モデルテクスチャのスクロール情報追加
																// 0x0000000D: エミッターの情報を追加
																// 0x0000000C: フィールド追加
																// 0x0000000B: モデルの詳細項目追加
		//
		DataFlag_is_little_endian	= 1<<0,
		RuntimeFlag_is_mapped		= 1<<0
	};

	struct EmitterInfo{
		EmitterTypeConst	type_;
		ml::ui32			resource_offset_;
	};

	// +0x00
	ml::ui32	sign_;
	ml::ui32	version_;
	ml::ui8		data_flag_;
	ml::ui8		runtime_flag_;
	ml::ui8		pad_8_[2];
	ml::ui32	node_offset_;

	// +0x10
	ml::ui32	texture_count_;
	ml::ui32	texture_path_offset_;
	ml::ui32	texture_name_offset_;
	ml::ui32	pad1_;

	// +0x20
	ml::ui32	model_count_;
	ml::ui32	model_path_offset_;
	ml::ui32	model_name_offset_;
	ml::ui32	pad2_;

	// +0x30
	ml::ui32	model_texture_count_;
	ml::ui32	model_texture_path_offset_;
	ml::ui32	model_texture_name_offset_;
	ml::ui32	pad3_;

	// +0x40
	ml::ui32	bmsln_count_;
	ml::ui32	bmsln_path_offset_;
	ml::ui32	bmsln_name_offset_;
	ml::ui32	pad4_;
	
	// +0x50
	ml::ui32	emitter_count_;
	ml::ui32	emitter_offset_;
	ml::ui32	pad5_;
	ml::ui32	pad6_;

	// +0x60
	ml::ui32	root_userdata_offset_;
	ml::ui32	root_advanced_frame_offset_;
	ml::si32	root_total_lifetime_offset_;
	ml::si32	root_whole_z_sort_offset_;

	// +0x70
	ml::ui32	root_version1_5_offset_;
	ml::ui32	root_life_offset_;
	ml::ui32	string_offset_;
	ml::ui32	binary_offset_;
};

ML_STATIC_ASSERT((sizeof(Header) & 15) == 0);

}	// namespace bmb
}	// namespace fileformat
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#include "../../utility/internal/bm3_MacroEnd.h"

#include "bm3_bmb_pack.h"

#endif	// BM3_SDK_INC_BM3_CORE_FILEFORMAT_BMB_H
