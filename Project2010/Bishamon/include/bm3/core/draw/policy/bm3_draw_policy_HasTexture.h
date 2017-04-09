#ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_HASTEXTURE_H
#define BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_HASTEXTURE_H

#include "bm3/system/bm3_Config.h"
#include "bm3_draw_policy_Common.h"
#include "../../bm3_DrawInfo.h"
#include "../../bm3_CoreType.h"
#include "../../texture/bm3_tex_HandleBase.h"
#include "../../bm3_RenderStateManager.h"
#include "../../bm3_ResourceManager.h"
#include "../../bm3_ProfileManager.h"

// 特殊化するために
#include "../../resource/bm3_res_SimpleParticle.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace draw{
namespace policy{

class HasTexture{

protected:
	texture::HandleBase	*texture_handle_[TextureLayer::Max];

	HasTexture(){

		ML_STATIC_ASSERT(TextureLayer::_1 == 0);
		ML_STATIC_ASSERT(TextureLayer::_2 == 1);
		ML_STATIC_ASSERT(TextureLayer::Max == 2);

		texture_handle_[TextureLayer::_1] = NULL;
		texture_handle_[TextureLayer::_2] = NULL;
	}

	template<typename ResourceType>
	void	SetTexture(DrawInfo &draw_info, const ResourceType &res){

		TextureDrawTypeConst	tdt	= res.Texture_Basic_TextureDrawType();
		{
			texture_handle_[TextureLayer::_1] = ResourceManager::instance().TextureHandleFromName(res.Texture_Layer1_PictureName(), draw_info.Renderer());
			if(IsEnableTextureLayer1(res)){
				ProfileManager::instance().Draw_AddTextureSwitchCount(1);

				texture_handle_[TextureLayer::_1]->Bind(TextureLayer::_1, draw_info);

#if defined(BM3_PLATFORM_nw4c)
				RenderStateManager::instance().SetTextureAddressFilterType(draw_info, TextureLayer::_1, res.Texture_Layer1_TextureAddressType(), res.Texture_Layer1_TextureFilterType());
#else
				RenderStateManager::instance().SetTextureAddressType(draw_info, TextureLayer::_1, res.Texture_Layer1_TextureAddressType());
				RenderStateManager::instance().SetTextureFilterType( draw_info, TextureLayer::_1, res.Texture_Layer1_TextureFilterType());
#endif

				RenderStateManager::instance().EnableTexture();
			}
		}

		{
			texture_handle_[TextureLayer::_2] = ResourceManager::instance().TextureHandleFromName(res.Texture_Layer2_PictureName(), draw_info.Renderer());
			if(IsEnableTextureLayer2(res)){
				ProfileManager::instance().Draw_AddTextureSwitchCount(1);

				texture_handle_[TextureLayer::_2]->Bind(TextureLayer::_2, draw_info);
#if defined(BM3_PLATFORM_nw4c)
				RenderStateManager::instance().SetTextureAddressFilterType(draw_info, TextureLayer::_2, res.Texture_Layer2_TextureAddressType(), res.Texture_Layer2_TextureFilterType());
#else
				RenderStateManager::instance().SetTextureAddressType(draw_info, TextureLayer::_2, res.Texture_Layer2_TextureAddressType());
				RenderStateManager::instance().SetTextureFilterType( draw_info, TextureLayer::_2, res.Texture_Layer2_TextureFilterType());
#endif
				RenderStateManager::instance().EnableTexture();
			}
			else{
				tdt	= TextureDrawTypeConst_Basic;					
			}
		}

		RenderStateManager::instance().SetTextureDrawType(tdt);
	}

	template<typename ResourceType>
	void	SetTextureType(const ResourceType &res){

		RendererInfo	render_info;	// 現状使われいないのでダミー
		texture_handle_[TextureLayer::_1] = ResourceManager::instance().TextureHandleFromName(res.Texture_Layer1_PictureName(), render_info);
		texture_handle_[TextureLayer::_2] = ResourceManager::instance().TextureHandleFromName(res.Texture_Layer2_PictureName(), render_info);

		TextureDrawTypeConst	tdt	= res.Texture_Basic_TextureDrawType();
		if(IsEnableTextureLayer1(res)){			RenderStateManager::instance().EnableTexture();		}

		if(IsEnableTextureLayer2(res)){			RenderStateManager::instance().EnableTexture();		}
		else{									tdt	= TextureDrawTypeConst_Basic;					}

		RenderStateManager::instance().SetTextureDrawType(tdt);
	}

	template<typename ResourceType>
	bool	IsEnableTextureLayer1(const ResourceType &res) const{
		return
			(res.Texture_Layer1_TextureAnimationType() != TextureAnimationTypeConst_Nothing) &&
			texture_handle_[TextureLayer::_1];
	}

	template<typename ResourceType>
	bool	IsEnableTextureLayer2(const ResourceType &res) const{
		return
			(res.Texture_Layer2_TextureAnimationType() != TextureAnimationTypeConst_Nothing) &&
			texture_handle_[TextureLayer::_2];
	}

	// 以下、シンプルパーティクル用
	void	SetTexture(DrawInfo &draw_info, const res::SimpleParticle &res){
		ProfileManager::instance().Draw_AddTextureSwitchCount(1);

		texture_handle_[TextureLayer::_1] = ResourceManager::instance().TextureHandleFromName(res.Texture_Layer1_PictureName(), draw_info.Renderer());

		if(texture_handle_[TextureLayer::_1]){
			texture_handle_[TextureLayer::_1]->Bind(TextureLayer::_1, draw_info);
#if defined(BM3_PLATFORM_nw4c)
			RenderStateManager::instance().SetTextureAddressFilterType( draw_info, TextureLayer::_1, TextureAddressTypeConst_Repeat, res.Texture_Layer1_TextureFilterType());
#else
			RenderStateManager::instance().SetTextureFilterType( draw_info, TextureLayer::_1, res.Texture_Layer1_TextureFilterType());
#endif
			RenderStateManager::instance().EnableTexture();
		}
	}

	void	SetTextureType(const res::SimpleParticle &res){

		RendererInfo	render_info;	// 現状使われいないのでダミー
		texture_handle_[TextureLayer::_1] = ResourceManager::instance().TextureHandleFromName(res.Texture_Layer1_PictureName(), render_info);

		if(texture_handle_[TextureLayer::_1]){
			RenderStateManager::instance().EnableTexture();
		}
	}
};

}	// namespace policy
}	// namespace draw
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_HASTEXTURE_H
