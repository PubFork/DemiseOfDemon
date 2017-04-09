#ifndef BM3_SDK_INC_BM3_CORE_FILEFORMAT_BMB_COMMON_H
#define BM3_SDK_INC_BM3_CORE_FILEFORMAT_BMB_COMMON_H

#include "../../system/bm3_Config.h"
#include "../bm3_CoreTypeSwitch.h"
#include "bm3_bmb_UpdateBmbInfo.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace fileformat{
namespace bmb{

struct NodeHeader;

/*!
	@brief	bmbイメージを初期化する（ポインタの付け替え）
	@param	image	bmbイメージの先頭ポインタ
	@return	なし
*/
void Initialize(void *image);
/*!
	@brief	bmbファイルの妥当性をチェックする
	@param	image	bmbイメージの先頭ポインタ
	@return	bmbファイルの妥当かどうか
*/
bool IsValidateData(const void *image);
/*!
	@brief	初期化済みかどうかを返す
	@param	image	bmbイメージの先頭ポインタ
	@return	初期化済みかどうか
*/
bool IsInitializedData(const void *image);
/*!
	@brief	ルートノードのポインタを返す
	@param	image	bmbイメージの先頭ポインタ
	@return	ルートノードのポインタ
*/
const void *RootNodeRes(const void *image);
/*!
	@brief	使用しているテクスチャの枚数を返す
	@param	image	bmbイメージの先頭ポインタ
	@return	使用しているテクスチャの枚数
*/
int TextureCount(const void *image);
/*!
	@brief	使用しているモデルデータの数を返す
	@param	image	bmbイメージの先頭ポインタ
	@return	使用しているモデルデータの数を
*/
int ModelCount(const void *image);
/*!
	@brief	モデルデータで使用しているテクスチャの数を返す
	@param	image	bmbイメージの先頭ポインタ
	@return	モデルデータで使用しているテクスチャの数
*/
int ModelTextureCount(const void *image);
/*!
	@brief	参照ノードのbmslnの数を返す
	@param	image	bmbイメージの先頭ポインタ
	@return	参照ノードのbmslnの数
*/
int BmslnCount(const void *image);
/*!
	@brief	エミッターノードの数を返す
	@param	image	bmbイメージの先頭ポインタ
	@return	エミッターノードの数
*/
int EmitterCount(const void *image);

/*!
	@brief	テクスチャの名前を返す
	@param	image	bmbイメージの先頭ポインタ
	@param	index	テクスチャのインデックス
	@return	テクスチャの名前
*/
const wchar_t *TextureNameFromIndex(const void *image, int index);
/*!
	@brief	bmslnからテクスチャへの相対パス名を返す
	@param	image	bmbイメージの先頭ポインタ
	@param	index	テクスチャのインデックス
	@return	bmslnからテクスチャへの相対パス名
*/
const wchar_t *TexturePathFromIndex(const void *image, int index);
/*!
	@brief	モデルの名前を返す
	@param	image	bmbイメージの先頭ポインタ
	@param	index	モデルのインデックス
	@return	モデルの名前
*/
const wchar_t *ModelNameFromIndex(const void *image, int index);
/*!
	@brief	bmslnからモデルへの相対パス名を返す
	@param	image	bmbイメージの先頭ポインタ
	@param	index	モデルのインデックス
	@return	bmslnからモデルへの相対パス名
*/
const wchar_t *ModelPathFromIndex(const void *image, int index);
/*!
	@brief	モデルテクスチャの名前を返す
	@param	image	bmbイメージの先頭ポインタ
	@param	index	モデルテクスチャのインデックス
	@return	モデルテクスチャの名前
*/
const wchar_t *ModelTextureNameFromIndex(const void *image, int index);
/*!
	@brief	bmslnからモデルテクスチャへの相対パス名を返す（モデルテクスチャはモデルと同じ場所にある必要がある）
	@param	image	bmbイメージの先頭ポインタ
	@param	index	モデルのインデックス
	@return	bmslnからモデルテクスチャへの相対パス名
*/
const wchar_t *ModelTexturePathFromIndex(const void *image, int index);
/*!
	@brief	bmslnの名前を返す
	@param	image	bmbイメージの先頭ポインタ
	@param	index	bmslnのインデックス
	@return	bmslnの名前
*/
const wchar_t *BmslnNameFromIndex(const void *image, int index);
/*!
	@brief	bmslnから参照ノード用のbmslnへの相対パス名を返す
	@param	image	bmbイメージの先頭ポインタ
	@param	index	参照ノード用のbmslnのインデックス
	@return	bmslnから参照ノード用のbmslnへの相対パス名
*/
const wchar_t *BmslnPathFromIndex(const void *image, int index);

/*!
	@brief	ルートのユーザーデータへのポインタを返す
	@param	image	bmbイメージの先頭ポインタ
	@return	ルートのユーザーデータへのポインタ
*/
const UserDataType *RootUserData(const void *image);
/*!
	@brief	繰り上げフレームを返す
	@param	image	bmbイメージの先頭ポインタ
	@return	繰り上げフレーム
*/
int RootAdvancedFrame(const void *image);
/*!
	@brief	bm2互換フラグがセットされているかどうかを返す
	@param	image	bmbイメージの先頭ポインタ
	@return	bm2互換フラグがセットされているかどうか
*/
bool RootInterChangedBm2(const void *image);
/*!
	@brief	version1.5フラグがセットされているかどうかを返す
	@param	image	bmbイメージの先頭ポインタ
	@return	version1.5フラグがセットされているかどうか
*/
bool RootVersion1_5(const void *bmb_image);
/*!
	@brief	エフェクト全体の寿命を返す
	@param	image	bmbイメージの先頭ポインタ
	@return	エフェクト全体の寿命
	@detail	エフェクト全体の寿命を返す。
			ランダムが設定されている場合はランダムが最大値を取った場合寿命を返す
*/
int TotalLifeTime(const void *bmb_image);
/*!
	@brief	エフェクトのZソートが有効かどうかを返す
	@param	image	bmbイメージの先頭ポインタ
	@return	エフェクトのZソートが有効かどうか
*/
bool IsZSort(const void *bmb_image);
/*!
	@brief	ルートノードの寿命を返す
	@param	image	bmbイメージの先頭ポインタ
	@return	ルートノードの寿命
	@detail	ルートノードの寿命を返す。
			0の場合は設定が無いものとする
*/
int RootLife(const void *bmb_image);

/*!
	@brief	bmbの中身を書き換える
	@param	info	bmb更新情報
	@param	image	bmbイメージの先頭ポインタ
	@return	なし
	@detail	UpdateBmbInfoを継承したクラスを作成する。
			呼び出された関数からデータの内容を書き換える。
*/
void UpdateBmb(UpdateBmbInfo &info, void *bmb_image);
	
struct ChildNodeMapping{
	static int Execute(NodeHeader &node_header, const wchar_t *string_chunk, const void *binary_chunk);
};

}	// namespace bmb
}	// namespace fileformat
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// BM3_SDK_INC_BM3_CORE_FILEFORMAT_BMB_COMMON_H
