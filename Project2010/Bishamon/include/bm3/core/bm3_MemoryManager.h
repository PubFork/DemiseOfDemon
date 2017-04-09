#ifndef BM3_SDK_INC_BM3_CORE_BM3_MEMORYMANAGER_H
#define BM3_SDK_INC_BM3_CORE_BM3_MEMORYMANAGER_H

#include "../system/bm3_Config.h"
#include "bm3_MemoryAllocator.h"
#include "bm3_ProfileManager.h"
#include "bm3_CoreType.h"
#include <ml/utility/ml_singleton.h>
#include <ml/utility/ml_assert.h>
#include <ml/utility/ml_offset_pointer.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace internal{

/// @brief	MemoryManagerImplÉNÉâÉX
class MemoryManagerImpl{

public:
	MemoryManagerImpl();
	~MemoryManagerImpl();

	template<typename EffectType>
	EffectType *	AllocEffect(int byte_size){
		EffectType *e =
			reinterpret_cast<EffectType *>(
				AllocEffectCore(byte_size)
			);

		ML_PTR_ASSERT(e);

		return (e!=NULL) ? new(e) EffectType : e;
	}

	template<typename EffectType>
	void	FreeEffect(EffectType *effect){

		if(effect){
			effect->~EffectType();

			FreeEffectCore(effect);
		}
	}

	template<typename EffectType>
	void	FreeEffectAllocFailed(EffectType *effect){
		if(effect){
			FreeEffectCore(effect);
		}
	}

	template<typename T>
	T *	AllocEmitter(int byte_size){
		return reinterpret_cast<T *>(AllocEmitterCore(byte_size));
	}
	template<typename T>
	void	FreeEmitter(T *memory){
		FreeEmitterCore(memory);
	}

	template<typename T>
	T *	AllocPrimitive(int byte_size){
		return reinterpret_cast<T *>(AllocPrimitiveCore(byte_size));
	}
	template<typename T>
	void	FreePrimitive(T *memory){
		FreePrimitiveCore(memory);
	}

	template<typename T>
	T *	AllocDraw(int byte_size){
		return reinterpret_cast<T *>(AllocDrawCore(byte_size));
	}
	template<typename T>
	void	FreeDraw(T *memory){
		FreeDrawCore(memory);
	}

	void *	General_Alloc(		int byte_size){	return	Alloc(	byte_size,	MemoryCategory::General);	}
	void	General_Free(		void *memory){			Free(	memory,		MemoryCategory::General);	}
	int		General_MaxSize() const{			return	MaxSize(			MemoryCategory::General);	}

	void *	Shader_Alloc(		int byte_size){	return	Alloc(	byte_size,	MemoryCategory::Shader);	}
	void	Shader_Free(		void *memory){			Free(	memory,		MemoryCategory::Shader);	}
	int		Shader_MaxSize() const{				return	MaxSize(			MemoryCategory::Shader);	}

	void	RegisterAllocator(MemoryAllocatorInterface &mai);
	void	UnregisterAllocator();

private:
	void *	AllocEffectCore(	int byte_size){	return	Alloc(byte_size,	MemoryCategory::General);	}
	void	FreeEffectCore(		void *memory){			Free(memory,		MemoryCategory::General);	}
	void *	AllocEmitterCore(	int byte_size){	return	Alloc(byte_size,	MemoryCategory::General);	}
	void	FreeEmitterCore(	void *memory){			Free(memory,		MemoryCategory::General);	}
	void *	AllocPrimitiveCore(	int byte_size){	return	Alloc(byte_size,	MemoryCategory::General);	}
	void	FreePrimitiveCore(	void *memory){			Free(memory,		MemoryCategory::General);	}
	void *	AllocDrawCore(		int byte_size){	return	Alloc(byte_size,	MemoryCategory::General);	}
	void	FreeDrawCore(		void *memory){			Free(memory,		MemoryCategory::General);	}

	struct HeaderCore{
		int						byte_size_;
		MemoryCategory::Type	type_;
	};

	struct HeaderAlign{
		enum{
			#if defined(BM3_PLATFORM_playstation3)
				AlignSize = 128
			#else
				AlignSize = 16
			#endif
		};

		ML_STATIC_ASSERT(sizeof(HeaderCore) <= AlignSize);

		ml::ui8	pad_[AlignSize - sizeof(HeaderCore)];
	};

	struct Header : public HeaderCore, public HeaderAlign{};

	ML_STATIC_ASSERT((sizeof(Header) % HeaderAlign::AlignSize) == 0);

	void	*Alloc(int byte_size, MemoryCategory::Type type){

		ML_PTR_ASSERT(mai_);

		if(byte_size > 0){
#if 1
			void *m = mai_->Alloc(sizeof(Header) + byte_size, type);

			if(m != NULL){
				Header *header = reinterpret_cast<Header *>(m);
				header->byte_size_	= byte_size;
				header->type_		= type;

				ProfileManager::instance().Memory_AddAllocatedSize(byte_size);

				return ml::offset_pointer<void *>(m, sizeof(Header));
			}else{
				return NULL;
			}
#else
			return mai_->Alloc(byte_size, type);
#endif
		}else{
			return NULL;
		}
	}

	void	Free(void *memory, MemoryCategory::Type type){

		ML_PTR_ASSERT(mai_);

		if(memory){
#if 1
//			void	*m		= ml::offset_pointer<void *>(memory, -sizeof(Header));		// åxçêÇ™èoÇÈ
			void	*m		= (void *)(int(memory) - sizeof(Header));
			Header	*header	= reinterpret_cast<Header *>(m);

			ML_ASSERT(header->type_ == type);

			ProfileManager::instance().Memory_AddAllocatedSize(-header->byte_size_);

			mai_->Free(m, type);
#else
			mai_->Free(memory, type);
#endif
		}
	}

	int		MaxSize(MemoryCategory::Type type) const{

		ML_PTR_ASSERT(mai_);
		return mai_->MaxSize(type);
	}

	MemoryAllocatorInterface	*mai_;
};

}	// namespace internal

typedef	ml::singleton<internal::MemoryManagerImpl>	MemoryManager;

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BM3_MEMORYMANAGER_H
