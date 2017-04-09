#ifndef BM3_SDK_INC_BM3_CORE_BM3_SYSTE_H
#define BM3_SDK_INC_BM3_CORE_BM3_SYSTE_H

#include "bm3_MemoryManager.h"
#include "bm3_Effect.h"
#include "../system/bm3_Config.h"
#include <ml/utility/ml_singleton.h>
#include "fileformat/bm3_bmb.h"
#include "object/bm3_obj_RootNodeBase.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

typedef	Effect<res::Null>*	EffectHandle;

namespace internal{

/// @brief	SystemImplƒNƒ‰ƒX
class SystemImpl{

public:

public:
	SystemImpl();
	~SystemImpl();

	void	Initialize();
	void	Finalize();

#if defined(BM3_TARGET_IDE)
	template<typename ResRootType>
	Effect<ResRootType>	*CreateEffect(const ResRootType &res_root){

		ML_ASSERT(is_initialized_);

		typedef Effect<ResRootType>	EffectType;

		EffectType *e = MemoryManager::instance().AllocEffect<EffectType>(sizeof(EffectType));
		if(e){
			bool result = e->Initialize(res_root);
			if(!result){
				typedef Effect<ResRootType>	EffectType;
				MemoryManager::instance().FreeEffectAllocFailed<EffectType>(e);
				e = NULL;
			}
		}
		return e;
	}

#endif

	template<typename ResRootType>
	Effect<ResRootType>	*CreateEffect(const ResRootType &res_root, const fileformat::bmb::Header *header){

		ML_ASSERT(is_initialized_);

		typedef Effect<ResRootType>	EffectType;

		EffectType *e = MemoryManager::instance().AllocEffect<EffectType>(sizeof(EffectType));
		if(e){
			bool result = e->Initialize(res_root, header, generate_scale_);
			if(!result){
				ReleaseEffect(e);
				e = NULL;
			}
		}
		return e;
	}

	EffectHandle	CreateEffect(const void *bmb_image);

	template<typename ResRootType>
	void ReleaseEffect(Effect<ResRootType> *effect){

		ML_ASSERT(is_initialized_);

		typedef Effect<ResRootType>	EffectType;

		MemoryManager::instance().FreeEffect<EffectType>(effect);
	}

	void SetMaxGenerateScale(float scale){	generate_scale_	= scale;	}
	float MaxGenerateScale() const {		return generate_scale_;		}
	void SetFramePerSecond(float fps) { frame_per_second_ = fps; }
	float FramePerSecond() const { return frame_per_second_; }

private:
	bool			is_initialized_;
	float			generate_scale_;
	float			frame_per_second_;
};

}	// namespace internal

typedef	ml::singleton<internal::SystemImpl>	System;

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BM3_SYSTE_H
