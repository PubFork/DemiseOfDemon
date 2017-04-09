#ifndef BM3_SDK_INC_BM3_CORE_TEXTURE_BM3_POLICY_TEXTURE_CROP_H
#define BM3_SDK_INC_BM3_CORE_TEXTURE_BM3_POLICY_TEXTURE_CROP_H

#include "../../bm3_Const.h"
#include "../../bm3_CoreType.h"
#include "../../bm3_CoreUtility.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{


/// @brief Texture_Cropクラス。テクスチャクロップの処理を行う
class Texture_Crop{

public:
	/*!
		クロップ切り替え用のカウンタとＩＤを初期化する
		@param	res		エミッターのリソースの参照
		@param	random	ml::randomクラス
		@param	crop	使用しない
		@param	crop_id	クロップのＩＤ
		@param	counter	クロップ切り替えのカウンタ
		@param	lt		レイヤタイプ
		@return	なし
	*/
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &random, TextureCrop &/*crop*/, int &crop_id, float &counter, TextureLayer::Type lt){

		if(Texture_Layer_CropRandom(res, lt)){
#ifdef BM3_TARGET_IDE
			Initialize(crop_id, counter, Texture_Layer_AnimationSpeed(res, lt), random, Texture_Layer_TextureCropAnimationCount(res, lt));
#else
			Initialize(crop_id, counter, Texture_Layer_AnimationSpeed(res, lt), random, Texture_Layer_TextureCropAnimationCount(res, lt));
#endif
		}
		else{
			Initialize(crop_id, counter, Texture_Layer_AnimationSpeed(res, lt));
		}
	}

	/*!
		レイヤー１のクロップ切り替え用のカウンタとＩＤを初期化する
		@param	res		エミッターのリソースの参照
		@param	random	ml::randomクラス
		@param	crop	使用しない
		@param	crop_id	クロップのＩＤ
		@param	counter	クロップ切り替えのカウンタ
		@param	type	アニメーションのタイプ
		@return	なし
	*/
	template<typename ResourceType>
	static BM3_FORCEINLINE void	InitializeLayer1(const ResourceType &res, ml::random &/*random*/, TextureCrop &/*crop*/, int &crop_id, float &counter, TextureAnimationTypeConst type){

		if(type == TextureAnimationTypeConst_Crop)	// 将来的にこのif文はなくなる
		{
			Initialize(crop_id, counter, res.Texture_Layer1_AnimationSpeed());
		}
	}
	
	/*!
		クロップ切り替え用のカウントを更新する
		@param	update_info	UpdateInfoクラス
		@param	res			エミッターのリソースの参照
		@param	crop		使用しない
		@param	crop_id		クロップ番号
		@param	counter		切り替えカウンタ
		@param	type		アニメーションのタイプ
		@param	lt			レイヤタイプ
		@return	なし
	*/
	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &update_info, const ResourceType &res, TextureCrop &/*crop*/, int &crop_id, float &counter, ml::random &random, TextureAnimationTypeConst /*type*/, TextureLayer::Type lt){

		if(Texture_Layer_CropRandom(res, lt)){
#ifdef BM3_TARGET_IDE
			Update(update_info, crop_id, counter, Texture_Layer_AnimationSpeed(res, lt), random, Texture_Layer_TextureCropAnimationCount(res, lt));
#else
			Update(update_info, crop_id, counter, Texture_Layer_AnimationSpeed(res, lt), random, Texture_Layer_TextureCropAnimationCount(res, lt));
#endif
		}
		else
		{
			Update(update_info, crop_id, counter, Texture_Layer_AnimationSpeed(res, lt));
		}
	}
	
	/*!
		レイヤー１のクロップ切り替え用のカウントを更新する
		@param	update_info	UpdateInfoクラス
		@param	res			エミッターのリソースの参照
		@param	crop		使用しない
		@param	crop_id		クロップ番号
		@param	counter		切り替えカウンタ
		@param	type		アニメーションのタイプ
		@return	なし
	*/
	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void	UpdateLayer1(UpdateInfo &update_info, const ResourceType &res, TextureCrop &/*crop*/, int &crop_id, float &counter, TextureAnimationTypeConst type){

		if(type == TextureAnimationTypeConst_Crop)	// 将来的にこのif文はなくなる
		{
			Update(update_info, crop_id, counter, res.Texture_Layer1_AnimationSpeed());
		}
	}

private:
	/*!
		クロップ切り替え用のカウンタとＩＤを初期化する
		@param	crop_id		クロップのＩＤ
		@param	counter		クロップ切り替えのカウンタ
		@param	sw_frame	切り替えフレーム数
		@return	なし
	*/
	static BM3_FORCEINLINE void	Initialize(int &crop_id, float &counter, int sw_frame){
		crop_id	= 0;
		counter	= (float)sw_frame;
	}

	/*!
		クロップ切り替え用のカウンタとＩＤを初期化する。クロップＩＤはランダムに切り替える。
		@param	crop_id			クロップのＩＤ
		@param	counter			クロップ切り替えのカウンタ
		@param	sw_frame		切り替えフレーム数
		@param	random			ml::random
		@param	max_crop_count	最大クロップ数
		@return	なし
	*/
	static BM3_FORCEINLINE void	Initialize(int &crop_id, float &counter, int sw_frame, ml::random &random, int max_crop_count){
		crop_id	= random.i32(max_crop_count);
		counter	= (float)sw_frame;
	}
	
	/*!
		クロップ切り替え用のカウントを更新する
		@param	update_info	UpdateInfoクラス
		@param	crop_id		クロップ番号
		@param	counter		切り替えカウンタ
		@param	sw_frame	切り替えフレーム数
		@return	なし
	*/
	template<typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &update_info, int &crop_id, float &counter, int sw_frame){

		ML_ASSERT(0<=counter);

		//-- counter;
		counter -= update_info.GetUpdateFrame();
		while(counter <= 0){
			counter = sw_frame + counter;
			++ crop_id;
		}
	}

	/*!
		クロップ切り替え用のカウントを更新する。クロップＩＤはランダムに切り替える。
		@param	update_info	UpdateInfoクラス
		@param	crop_id		クロップ番号
		@param	counter		切り替えカウンタ
		@param	sw_frame	切り替えフレーム数
		@param	random			ml::random
		@param	max_crop_count	最大クロップ数
		@return	なし
	*/
	template<typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &update_info, int &crop_id, float &counter, int sw_frame, ml::random &random, int max_crop_count){

		ML_ASSERT(0<=counter);

		//-- counter;
		counter -= update_info.GetUpdateFrame();

		while(counter <= 0){
			counter = sw_frame + counter;
			crop_id	= random.i32(max_crop_count);
		}

	}
	
};

}	// namespace impl


}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_TEXTURE_BM3_POLICY_TEXTURE_CROP_H
