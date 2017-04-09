#ifndef BM3_SDK_INC_BM3_CORE_BM3_RENDERDEVICEPOLICYBASE_H
#define BM3_SDK_INC_BM3_CORE_BM3_RENDERDEVICEPOLICYBASE_H

#include <ml/type/ml_matrix44.h>
#include <ml/type/ml_vector3d.h>
#include <ml/type/ml_color.h>
#include <ml/utility/ml_assert.h>
#include "../system/bm3_Config.h"
#include "bm3_CoreType.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class DrawInfo;

namespace internal{

/// @brief RenderDevicePolicyBaseクラス
class RenderDevicePolicyBase{

public:
	struct Future{
		enum Flag{
			COL		= 1<<0,				// カラー
			SPC		= 1<<1,				// スペキュラ
			LGT		= 1<<2,				// ライト
			SFT		= 1<<3,				// ソフトパーティクル
			TEX		= 1<<4,				// テクスチャ
			SFTN	= 1<<5,				// 正規化ソフトパーティクル
			FOG		= 1<<6,				// フォグ
			//
			Max		= 7,				// Flagの数をここに入れる
			All		= 1 << Max
		};
	};

	struct Function{
		enum Type{
			COL					= Future::COL													,
			COL_LGT				= Future::COL					| Future::LGT					,
			COL_SPC_LGT			= Future::COL	| Future::SPC	| Future::LGT					,
			COL_TEX				= Future::COL									| Future::TEX	,
			COL_LGT_TEX			= Future::COL					| Future::LGT	| Future::TEX	,
			COL_SPC_LGT_TEX		= Future::COL	| Future::SPC	| Future::LGT	| Future::TEX	,
			LGT					= 								  Future::LGT					,
			SPC_LGT				= 				  Future::SPC	| Future::LGT					,
			TEX					= 												  Future::TEX	,
			LGT_TEX				= 								  Future::LGT	| Future::TEX	,
			SPC_LGT_TEX			= 				  Future::SPC	| Future::LGT	| Future::TEX	,
			COL_FOG				= Future::COL													| Future::FOG,
			COL_LGT_FOG			= Future::COL					| Future::LGT					| Future::FOG,
			COL_SPC_LGT_FOG		= Future::COL	| Future::SPC	| Future::LGT					| Future::FOG,
			COL_FOG_TEX			= Future::COL									| Future::TEX	| Future::FOG,
			COL_LGT_FOG_TEX		= Future::COL					| Future::LGT	| Future::TEX	| Future::FOG,
			COL_SPC_LGT_FOG_TEX	= Future::COL	| Future::SPC	| Future::LGT	| Future::TEX	| Future::FOG,
			LGT_FOG				= 								  Future::LGT					| Future::FOG,
			SPC_LGT_FOG			= 				  Future::SPC	| Future::LGT					| Future::FOG,
			FOG_TEX				= 												  Future::TEX	| Future::FOG,
			LGT_FOG_TEX			= 								  Future::LGT	| Future::TEX	| Future::FOG,
			SPC_LGT_FOG_TEX		= 				  Future::SPC	| Future::LGT	| Future::TEX	| Future::FOG,

			COL_SFT					= Future::COL													| Future::SFT,
			COL_LGT_SFT				= Future::COL					| Future::LGT					| Future::SFT,
			COL_SPC_LGT_SFT			= Future::COL	| Future::SPC	| Future::LGT					| Future::SFT,
			COL_SFT_TEX				= Future::COL									| Future::TEX	| Future::SFT,
			COL_LGT_SFT_TEX			= Future::COL					| Future::LGT	| Future::TEX	| Future::SFT,
			COL_SPC_LGT_SFT_TEX		= Future::COL	| Future::SPC	| Future::LGT	| Future::TEX	| Future::SFT,
			LGT_SFT					= 								  Future::LGT					| Future::SFT,
			SPC_LGT_SFT				= 				  Future::SPC	| Future::LGT					| Future::SFT,
			SFT_TEX					= 												  Future::TEX	| Future::SFT,
			LGT_SFT_TEX				= 								  Future::LGT	| Future::TEX	| Future::SFT,
			SPC_LGT_SFT_TEX			= 				  Future::SPC	| Future::LGT	| Future::TEX	| Future::SFT,
			COL_SFT_FOG				= Future::COL													| Future::SFT | Future::FOG,
			COL_LGT_SFT_FOG			= Future::COL					| Future::LGT					| Future::SFT | Future::FOG,
			COL_SPC_LGT_SFT_FOG		= Future::COL	| Future::SPC	| Future::LGT					| Future::SFT | Future::FOG,
			COL_SFT_FOG_TEX			= Future::COL									| Future::TEX	| Future::SFT | Future::FOG,
			COL_LGT_SFT_FOG_TEX		= Future::COL					| Future::LGT	| Future::TEX	| Future::SFT | Future::FOG,
			COL_SPC_LGT_SFT_FOG_TEX	= Future::COL	| Future::SPC	| Future::LGT	| Future::TEX	| Future::SFT | Future::FOG,
			LGT_SFT_FOG				= 								  Future::LGT					| Future::SFT | Future::FOG,
			SPC_LGT_SFT_FOG			= 				  Future::SPC	| Future::LGT					| Future::SFT | Future::FOG,
			SFT_FOG_TEX				= 												  Future::TEX	| Future::SFT | Future::FOG,
			LGT_SFT_FOG_TEX			= 								  Future::LGT	| Future::TEX	| Future::SFT | Future::FOG,
			SPC_LGT_SFT_FOG_TEX		= 				  Future::SPC	| Future::LGT	| Future::TEX	| Future::SFT | Future::FOG,

			COL_SFTN					= Future::COL													| Future::SFT | Future::SFTN,
			COL_LGT_SFTN				= Future::COL					| Future::LGT					| Future::SFT | Future::SFTN,
			COL_SPC_LGT_SFTN			= Future::COL	| Future::SPC	| Future::LGT					| Future::SFT | Future::SFTN,
			COL_SFTN_TEX				= Future::COL									| Future::TEX	| Future::SFT | Future::SFTN,
			COL_LGT_SFTN_TEX			= Future::COL					| Future::LGT	| Future::TEX	| Future::SFT | Future::SFTN,
			COL_SPC_LGT_SFTN_TEX		= Future::COL	| Future::SPC	| Future::LGT	| Future::TEX	| Future::SFT | Future::SFTN,
			LGT_SFTN					= 								  Future::LGT					| Future::SFT | Future::SFTN,
			SPC_LGT_SFTN				= 				  Future::SPC	| Future::LGT					| Future::SFT | Future::SFTN,
			SFTN_TEX					= 												  Future::TEX	| Future::SFT | Future::SFTN,
			LGT_SFTN_TEX				= 								  Future::LGT	| Future::TEX	| Future::SFT | Future::SFTN,
			SPC_LGT_SFTN_TEX			= 				  Future::SPC	| Future::LGT	| Future::TEX	| Future::SFT | Future::SFTN,
			COL_SFTN_FOG				= Future::COL													| Future::SFT| Future::SFTN | Future::FOG,
			COL_LGT_SFTN_FOG			= Future::COL					| Future::LGT					| Future::SFT| Future::SFTN | Future::FOG,
			COL_SPC_LGT_SFTN_FOG		= Future::COL	| Future::SPC	| Future::LGT					| Future::SFT| Future::SFTN | Future::FOG,
			COL_SFTN_FOG_TEX			= Future::COL									| Future::TEX	| Future::SFT| Future::SFTN | Future::FOG,
			COL_LGT_SFTN_FOG_TEX		= Future::COL					| Future::LGT	| Future::TEX	| Future::SFT| Future::SFTN | Future::FOG,
			COL_SPC_LGT_SFTN_FOG_TEX	= Future::COL	| Future::SPC	| Future::LGT	| Future::TEX	| Future::SFT| Future::SFTN | Future::FOG,
			LGT_SFTN_FOG				= 								  Future::LGT					| Future::SFT| Future::SFTN | Future::FOG,
			SPC_LGT_SFTN_FOG			= 				  Future::SPC	| Future::LGT					| Future::SFT| Future::SFTN | Future::FOG,
			SFTN_FOG_TEX				= 												  Future::TEX	| Future::SFT| Future::SFTN | Future::FOG,
			LGT_SFTN_FOG_TEX			= 								  Future::LGT	| Future::TEX	| Future::SFT| Future::SFTN | Future::FOG,
			SPC_LGT_SFTN_FOG_TEX		= 				  Future::SPC	| Future::LGT	| Future::TEX	| Future::SFT| Future::SFTN | Future::FOG,
			MODEL = Future::COL|Future::SPC|Future::LGT|Future::TEX|Future::FOG,
			//
			Basic			= 0,			// 基本
			Undefined		= Future::All,	// 未定義
			Max				= 0xFFFFFFFF
		};

	static bool IsValid(Function::Type function){
		return
			(function == Function::Basic				) ||
			//
			(function == Function::COL						) ||
			(function == Function::COL_LGT					) ||
			(function == Function::COL_SPC_LGT				) ||
			(function == Function::COL_TEX					) ||
			(function == Function::COL_LGT_TEX				) ||
			(function == Function::COL_SPC_LGT_TEX			) ||
			(function == Function::LGT						) ||
			(function == Function::SPC_LGT					) ||
			(function == Function::TEX						) ||
			(function == Function::LGT_TEX					) ||
			(function == Function::SPC_LGT_TEX				) ||
			(function == Function::COL_FOG					) ||
			(function == Function::COL_LGT_FOG				) ||
			(function == Function::COL_SPC_LGT_FOG			) ||
			(function == Function::COL_FOG_TEX				) ||
			(function == Function::COL_LGT_FOG_TEX			) ||
			(function == Function::COL_SPC_LGT_FOG_TEX		) ||
			(function == Function::LGT_FOG					) ||
			(function == Function::SPC_LGT_FOG				) ||
			(function == Function::FOG_TEX					) ||
			(function == Function::LGT_FOG_TEX				) ||
			(function == Function::SPC_LGT_FOG_TEX			) ||
			//
			(function == Function::COL_SFT					) ||
			(function == Function::COL_LGT_SFT				) ||
			(function == Function::COL_SPC_LGT_SFT			) ||
			(function == Function::COL_SFT_TEX				) ||
			(function == Function::COL_LGT_SFT_TEX			) ||
			(function == Function::COL_SPC_LGT_SFT_TEX		) ||
			(function == Function::LGT_SFT					) ||
			(function == Function::SPC_LGT_SFT				) ||
			(function == Function::SFT_TEX					) ||
			(function == Function::LGT_SFT_TEX				) ||
			(function == Function::SPC_LGT_SFT_TEX			) ||
			(function == Function::COL_SFT_FOG				) ||
			(function == Function::COL_LGT_SFT_FOG			) ||
			(function == Function::COL_SPC_LGT_SFT_FOG		) ||
			(function == Function::COL_SFT_FOG_TEX			) ||
			(function == Function::COL_LGT_SFT_FOG_TEX		) ||
			(function == Function::COL_SPC_LGT_SFT_FOG_TEX	) ||
			(function == Function::LGT_SFT_FOG				) ||
			(function == Function::SPC_LGT_SFT_FOG			) ||
			(function == Function::SFT_FOG_TEX				) ||
			(function == Function::LGT_SFT_FOG_TEX			) ||
			(function == Function::SPC_LGT_SFT_FOG_TEX		) ||
			//
			(function == Function::COL_SFTN					) ||
			(function == Function::COL_LGT_SFTN				) ||
			(function == Function::COL_SPC_LGT_SFTN			) ||
			(function == Function::COL_SFTN_TEX				) ||
			(function == Function::COL_LGT_SFTN_TEX			) ||
			(function == Function::COL_SPC_LGT_SFTN_TEX		) ||
			(function == Function::LGT_SFTN					) ||
			(function == Function::SPC_LGT_SFTN				) ||
			(function == Function::SFTN_TEX					) ||
			(function == Function::LGT_SFTN_TEX				) ||
			(function == Function::SPC_LGT_SFTN_TEX			) ||

			(function == Function::COL_SFTN_FOG				) ||
			(function == Function::COL_LGT_SFTN_FOG			) ||
			(function == Function::COL_SPC_LGT_SFTN_FOG		) ||
			(function == Function::COL_SFTN_FOG_TEX			) ||
			(function == Function::COL_LGT_SFTN_FOG_TEX		) ||
			(function == Function::COL_SPC_LGT_SFTN_FOG_TEX	) ||
			(function == Function::LGT_SFTN_FOG				) ||
			(function == Function::SPC_LGT_SFTN_FOG			) ||
			(function == Function::SFTN_FOG_TEX				) ||
			(function == Function::LGT_SFTN_FOG_TEX			) ||
			(function == Function::SPC_LGT_SFTN_FOG_TEX		);
		}
	};


	struct DirectionalLight{
		ml::vector3d	direction_;
		ColorType		color_;
		ColorType		ambient_color_;
	};

protected:
	struct Register{
		enum Flag{
			Matrix					= 1<<0,
			DiffuseColor			= 1<<1,
			EmissionColor			= 1<<2,
			SpecularColorAndPower	= 1<<3,
			ColorScale				= 1<<4,
			OffsetPosition			= 1<<5,
			AmbientLightColor		= 1<<6,
			LightColor				= 1<<7,
			LightDirection			= 1<<8,
			Texture0Scroll			= 1<<9,
			Texture1Scroll			= 1<<10,
			CameraDirection			= 1<<11,
			SoftThreshold			= 1<<12,
			ModelVertexCount		= 1<<13,		// ModelVertexCount PointSpriteScaleは同じ場所
			PointSpriteScale		= 1<<13,		// ModelVertexCount PointSpriteScaleは同じ場所
			FogColor				= 1<<14,
			FogArea					= 1<<15,		// start, end, near, far
			FogDensity				= 1<<16,		
			
			//
			All						= 0xFFFFFFFF
		};
	};

	enum	ApplyType{
		ApplyType_No,
		ApplyType_Basic,
		ApplyType_InstancingModel,
		ApplyType_SimpleParticle,
		ApplyType_MassParticle,
		ApplyType_Model
	};

	enum	ApplyMassType{
		ApplyMass_Setup,
		ApplyMass_Render,
		ApplyMass_Update
	};

public:
	RenderDevicePolicyBase();
	~RenderDevicePolicyBase();

	void SetProjectionMatrix(const ml::matrix44 &m){	projection_matrix_		= m;	EnableParamChange(Register::Matrix);								}
	void SetViewMatrix(const ml::matrix44 &m){			view_matrix_			= m;	EnableParamChange(Register::Matrix);								}
	void SetWorldMatrix(const ml::matrix44 &m);

	void SetDiffuseColor(const ColorType &c){			diffuse_color_			= c;	EnableParamChange(Register::DiffuseColor);							}
	void SetEmissionColor(const ColorType &c){			emission_color_			= c;	EnableParamChange(Register::EmissionColor);							}
	void SetSpecularColor(const ColorType &c){			specular_color_and_power_.e_.r_	= c.e_.r_;
														specular_color_and_power_.e_.g_	= c.e_.g_;
														specular_color_and_power_.e_.b_	= c.e_.b_;
																						EnableParamChange(Register::SpecularColorAndPower);					}
	void SetSpecularPower(float pow){					ML_ASSERT(pow >= 0.0f);
														specular_color_and_power_.v_[3]	= pow;
																						EnableParamChange(Register::SpecularColorAndPower);					}

	void SetTextureDrawType(TextureDrawTypeConst t){	texture_draw_type_		= t;																		}
	void SetTexture0Scroll(const ml::vector3d &v){		texture0_scroll_		= v;	EnableParamChange(Register::Texture0Scroll);						}
	void SetTexture1Scroll(const ml::vector3d &v){		texture1_scroll_		= v;	EnableParamChange(Register::Texture1Scroll);						}

	void SetColorScale(const ColorType &c){				color_scale_			= c;	EnableParamChange(Register::ColorScale);							}
	void SetOffsetPosition(const ml::vector3d &p){		offset_position_		= p;	EnableParamChange(Register::OffsetPosition);						}

	void SetAmbientLightColor(const ColorType &c){		ambient_light_color_	= c;	EnableParamChange(Register::AmbientLightColor);						}
	void SetLightDirection(const ml::vector3d &v){		light_direction_		= v;	EnableParamChange(Register::LightDirection);						}
	void SetLightColor(const ColorType &c){				light_color_			= c;	EnableParamChange(Register::LightColor);							}

	void SetDefaultColor(){								SetDiffuseColor(ColorType(1.0f, 1.0f, 1.0f, 1.0f));
														SetColorScale(ColorType(1.0f, 1.0f, 1.0f, 1.0f));
														SetOffsetPosition(ml::vector3d(0.0f, 0.0f, 0.0f, 1.0f)); 											}

	void SetCameraDirection(const ml::vector3d &v){		camera_direction_		= v;	EnableParamChange(Register::CameraDirection);						}

	void SetSoftThreshold(float t){						soft_threshold_			= t;	EnableParamChange(Register::SoftThreshold);							}
	void SetModelVertexCount(float c){					model_vertex_count_		= c;	EnableParamChange(Register::ModelVertexCount);						}
	void SetPointSpriteScale(float s){					point_sprite_scale_		= s;	EnableParamChange(Register::PointSpriteScale);						}
	
	void SetFogColor(const ColorType &c){				fog_color_				= c;	EnableParamChange(Register::FogColor);								}
	void SetFogArea(const ml::vector3d &d){				fog_area_				= d;	EnableParamChange(Register::FogArea);								}
	void SetFogDensity(const float d){					fog_density_			= d;	EnableParamChange(Register::FogDensity);							}

	// 各機能の有効・無効の設定
	void EnableVertexColor(){							function_ = (Function::Type)((function_ | Future::COL) & function_mask_);	ML_ASSERT(Function::IsValid(function_));	}
	void EnableTexture(){								function_ = (Function::Type)((function_ | Future::TEX) & function_mask_);	ML_ASSERT(Function::IsValid(function_));	}
	void EnableSpecular(){								function_ = (Function::Type)((function_ | Future::SPC) & function_mask_);	ML_ASSERT(Function::IsValid(function_));	}
	void EnableLight(){									function_ = (Function::Type)((function_ | Future::LGT) & function_mask_);	ML_ASSERT(Function::IsValid(function_));	}
	void EnableSoft(){									function_ = (Function::Type)((function_ | Future::SFT) & function_mask_);	ML_ASSERT(Function::IsValid(function_));	}
	void EnableSoftN(){									function_ = (Function::Type)((function_ | Future::SFTN)& function_mask_);	ML_ASSERT(Function::IsValid(function_));	}
	void EnableFog(){									function_ = (Function::Type)((function_ | Future::FOG) & function_mask_);	ML_ASSERT(Function::IsValid(function_));	}

	void DisableVertexColor(){							function_ = (Function::Type)(function_ & ~Future::COL);	ML_ASSERT(Function::IsValid(function_));	}
	void DisableTexture(){								function_ = (Function::Type)(function_ & ~Future::TEX);	ML_ASSERT(Function::IsValid(function_));	}
	void DisableSpecular(){								function_ = (Function::Type)(function_ & ~Future::SPC);	ML_ASSERT(Function::IsValid(function_));	}
	void DisableLight(){								function_ = (Function::Type)(function_ & ~Future::LGT);	ML_ASSERT(Function::IsValid(function_));	}
	void DisableSoft(){									function_ = (Function::Type)(function_ & ~Future::SFT);	ML_ASSERT(Function::IsValid(function_));	}
	void DisableSoftN(){								function_ = (Function::Type)(function_ & ~Future::SFTN);ML_ASSERT(Function::IsValid(function_));	}
	void DisableFog(){									function_ = (Function::Type)(function_ & ~Future::FOG);	ML_ASSERT(Function::IsValid(function_));	}

	void DisableAllFuture(){							function_ = Function::Basic;																		}

	bool IsEnableVertexColor() const{					return (function_ & Future::COL) != 0;																}
	bool IsEnableTexture() const{						return (function_ & Future::TEX) != 0;																}
	bool IsEnableSpecular() const{						return (function_ & Future::SPC) != 0;																}
	bool IsEnableLight() const{							return (function_ & Future::LGT) != 0;																}
	bool IsEnableSoft() const{							return (function_ & Future::SFT) != 0;																}
	bool IsEnableSoftN() const{							return (function_ & Future::SFTN)!= 0;																}
	bool IsEnableFog() const{							return (function_ & Future::FOG) != 0;																}

	// 各機能マスクの有効・無効の設定
	void FunctionMask_EnableVertexColor(){				function_mask_ = (Function::Type)(function_mask_ | Future::COL);		}
	void FunctionMask_EnableTexture(){					function_mask_ = (Function::Type)(function_mask_ | Future::TEX);		}
	void FunctionMask_EnableSpecular(){					function_mask_ = (Function::Type)(function_mask_ | Future::SPC);		}
	void FunctionMask_EnableLight(){					function_mask_ = (Function::Type)(function_mask_ | Future::LGT);		}
	void FunctionMask_EnableSoft(){						function_mask_ = (Function::Type)(function_mask_ | Future::SFT);		}
	void FunctionMask_EnableSoftN(){					function_mask_ = (Function::Type)(function_mask_ | Future::SFTN);		}
	void FunctionMask_EnableFog(){						function_mask_ = (Function::Type)(function_mask_ | Future::FOG);		}

	void FunctionMask_DisableVertexColor(){				function_mask_ = (Function::Type)(function_mask_ & ~Future::COL);		}
	void FunctionMask_DisableTexture(){					function_mask_ = (Function::Type)(function_mask_ & ~Future::TEX);		}
	void FunctionMask_DisableSpecular(){				function_mask_ = (Function::Type)(function_mask_ & ~Future::SPC);		}
	void FunctionMask_DisableLight(){					function_mask_ = (Function::Type)(function_mask_ & ~Future::LGT);		}
	void FunctionMask_DisableSoft(){					function_mask_ = (Function::Type)(function_mask_ & ~Future::SFT);		}
	void FunctionMask_DisableSoftN(){					function_mask_ = (Function::Type)(function_mask_ & ~Future::SFTN);		}
	void FunctionMask_DisableFog(){						function_mask_ = (Function::Type)(function_mask_ & ~Future::FOG);		}

	bool FunctionMask_IsEnableVertexColor() const{		return (function_mask_ & Future::COL) != 0;								}
	bool FunctionMask_IsEnableTexture() const{			return (function_mask_ & Future::TEX) != 0;								}
	bool FunctionMask_IsEnableSpecular() const{			return (function_mask_ & Future::SPC) != 0;								}
	bool FunctionMask_IsEnableLight() const{			return (function_mask_ & Future::LGT) != 0;								}
	bool FunctionMask_IsEnableSoft() const{				return (function_mask_ & Future::SFT) != 0;								}
	bool FunctionMask_IsEnableSoftN() const{			return (function_mask_ & Future::SFTN)!= 0;								}
	bool FunctionMask_IsEnableFog() const{				return (function_mask_ & Future::FOG) != 0;								}

	void GetDirectionalLight(DirectionalLight &light){
		light.direction_		= light_direction_;
		light.color_			= light_color_;
		light.ambient_color_	= ambient_light_color_;
	}
	void SetDirectionalLight(const DirectionalLight &light){
		light_direction_		= light.direction_;
		light_color_			= light.color_;
		ambient_light_color_	= light.ambient_color_;
	}
	void NonDirectionalLight(){
		light_direction_		= ml::vector3d(0.0f, 0.0f, 0.0f, 0.0f);
		light_color_			= ColorType(0.0f, 0.0f, 0.0f, 1.0f);
		ambient_light_color_	= ColorType(1.0f, 1.0f, 1.0f, 1.0f);
	}
	

#ifdef BM3_PLATFORM_psp
	virtual void PreBeginRender(DrawInfo &draw_info);
#else
	void PreBeginRender(DrawInfo &draw_info);
#endif
	void PostBeginRender(DrawInfo &/*draw_info*/){;}
	void PostEndRender(DrawInfo &/*draw_info*/){;}
	void PreEndRender(DrawInfo &/*draw_info*/){;}

	
public:
	void SetBindShapeMatrix(ml::matrix44 *matrix) { bind_shape_matrix_ = matrix; }
	void SetJointMatrices(ml::matrix44 *matrices) { joint_matrices_ = matrices; }
	void SetModelMatrix(ml::matrix44 *matrix) { model_matrix_ = matrix; }
	void SetJointCount(int count) { joint_count_ = count; }
	ml::matrix44 *GetMatrixPalette() { return palette; }
	
protected:
	Function::Type			function_;
	Function::Type			old_function_;
	Function::Type			function_mask_;		// 機能マスク。機能はこの値とのアンドが取られる

	ml::matrix44			projection_matrix_;
	ml::matrix44			view_matrix_;
	ml::matrix44			world_matrix_;

	// todo ひとまず24個分確保しておく。今後、調整する
	ml::matrix44	palette[32];
	ml::matrix44	*bind_shape_matrix_;
	ml::matrix44	*joint_matrices_;
	ml::matrix44	*model_matrix_;
	int joint_count_;

// TODO:プリプロセッサではじこうとすると動かなくなる
//#if defined(BM3_TARGET_IDE)
	ml::matrix44			ide_effect_matrix_;
//#endif

	ColorType		diffuse_color_;
	ColorType		emission_color_;
	ColorType		specular_color_and_power_;
	ColorType		color_scale_;
	ml::vector3d	offset_position_;

	ml::vector3d	light_direction_;
	ColorType		light_color_;
	ColorType		ambient_light_color_;

	TextureDrawTypeConst	texture_draw_type_;
	TextureDrawTypeConst	old_texture_draw_type_;
	ml::vector3d			texture0_scroll_;
	ml::vector3d			texture1_scroll_;

	ml::vector3d			camera_direction_;

	float					soft_threshold_;
	float					model_vertex_count_;
	float					point_sprite_scale_;
	
	ColorType				fog_color_;
	ml::vector3d			fog_area_;
	float					fog_density_;

	bool IsParamChange(Register::Flag f) const{					return (register_change_ & f) != 0;							}
	void EnableParamChange(Register::Flag f){					register_change_ = Register::Flag(register_change_ | f);	}
	void DisableParamChange(Register::Flag f){					register_change_ = Register::Flag(register_change_ & ~f);	}

private:
	Register::Flag		register_change_;
};

}	// namespace internal
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BM3_RENDERDEVICEPOLICYBASE_H
