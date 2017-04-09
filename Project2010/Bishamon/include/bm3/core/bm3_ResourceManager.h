#ifndef BM3_SDK_INC_BM3_CORE_BM3_RESOURCEMANAGER_H
#define BM3_SDK_INC_BM3_CORE_BM3_RESOURCEMANAGER_H

#include "../system/bm3_Config.h"
#include "../utility/bm3_String.h"
#include "texture/bm3_tex_HandleBase.h"
#include "model/bm3_model_ModelHandleBase.h"
#include "bmsln/bm3_bmsln_HandleBase.h"
#include "bm3_RendererInfo.h"
#include "bm3_CoreType.h"

#if defined(BM3_PLATFORM_win_opengl)
	#include "!opengl/bm3_ResourceManagerPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_win_directx9)
	#include "!directx9/bm3_ResourceManagerPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_win_directx10)
	#include "!directx10/bm3_ResourceManagerPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_playstation3)
#include "!playstation3/bm3_ResourceManagerPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_xbox360)
	#include "!xbox360/bm3_ResourceManagerPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_psp)
	#include "!psp/bm3_ResourceManagerPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_gamebryodx9)
	#include "!gamebryodx9/bm3_ResourceManagerPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_opengles)
	#include "!opengles/bm3_ResourceManagerPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_nw4c)
	#include "!nw4c/bm3_ResourceManagerPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_psvita)
	#include "!psvita/bm3_ResourceManagerPlatformDependency.h"
#endif

#include <ml/utility/ml_singleton.h>

#if defined(BM3_TARGET_IDE)
#include "texture/bm3_tex_IdeHandle.h"
#include <ml/utility/ml_memory_stream.h>
#include <ctime>
#include <map>
#include "resource/bm3_res_IdeNode.h"
#endif

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

#if defined(BM3_TARGET_IDE)
class UpdateInfo;
#endif
class DrawInfo;
class HashInterface;

namespace internal{

#if defined(BM3_TARGET_IDE)
namespace ide{

class TextureLoader;
class ModelCreater;

class ResourceManagerImpl{

public:
	ResourceManagerImpl();
	~ResourceManagerImpl();

	texture::HandleBase	*TextureHandleFromName(const StringType &name, RendererInfo &renderer, TextureCategory::Type category = TextureCategory::Effect);
	model::HandleBase	*ModelHandleFromName(const StringType &name, RendererInfo &renderer);
	const void			*M3rImageFromName(const StringType &name);

	// bmslnÉnÉìÉhÉãÇéÊÇËèoÇ∑
	bmsln::HandleBase	*BmslnHandleFromName(const StringType &name);
	// ÉäÉ\Å[ÉXÇìoò^Ç∑ÇÈÅB2èdìoò^ÇÕÇµÇ»Ç¢
	void	RegisterBmsln(const std::wstring &name, const res::IdeNode *res);
	void	UnregisterAllBmsln();

	void	Reload();

	void	SetTextureDatabaseDir(const std::wstring &dir);
	void	SetModelDatabaseDir(const std::wstring &dir);
	void	SetBmslnDatabaseDir(const std::wstring &dir);

	const std::wstring &TextureDatabaseDir(){							return texture_data_base_dir_;	}
	const std::wstring &ModelDatabaseDir(){								return model_data_base_dir_;	}
	const std::wstring &BmslnDatabaseDir(){								return bmsln_data_base_dir_;	}

	bool	LoadTexture(const std::wstring &filename, ml::memory_stream &file_image, int &width, int &height);

	void	PrepareUpdate(UpdateInfo &update_info);
	void	PrepareDraw(DrawInfo &draw_info);

private:
	template<typename T>
	struct FileInfo{
		FileInfo() :
			is_req_reload_(false),
			handle_(NULL){;}

		~FileInfo(){
			delete handle_;
		}

		bool				is_req_reload_;

		std::wstring		filename_;
		FILETIME			file_age_;
		ml::memory_stream	file_image_;
		T *					handle_;
	};

	struct ResourceKey{
		std::wstring	name_;
		RendererInfo	renderer_info_;

		ResourceKey(){;}

		ResourceKey(const std::wstring &name, const RendererInfo &renderer_info) :
			name_(name),
			renderer_info_(renderer_info){;}

		bool operator<(const ResourceKey &s) const{	return ToString() < s.ToString();			}
		bool operator>(const ResourceKey &s) const{	return ToString() > s.ToString();			}
		std::wstring ToString() const{				return name_ + renderer_info_.ToString();	}
	};

	struct SimpleResourceKey{
		std::wstring	name_;

		SimpleResourceKey(){;}

		SimpleResourceKey(const std::wstring &name) :
			name_(name){;}

		bool operator<(const SimpleResourceKey &s) const{	return ToString() < s.ToString();			}
		bool operator>(const SimpleResourceKey &s) const{	return ToString() > s.ToString();			}
		std::wstring ToString() const{				return name_;	}
	};


//	typedef	FileInfo<texture::HandleBase>				TextureFileInfo;
	typedef	FileInfo<texture::ide::Handle>				TextureFileInfo;
	typedef	std::map<ResourceKey, TextureFileInfo>		TextureFileMap;
	typedef	TextureFileMap::iterator					TextureFileMapI;

	typedef	FileInfo<model::HandleBase>					ModelFileInfo;
	typedef	std::map<ResourceKey, ModelFileInfo>		ModelFileMap;
	typedef	ModelFileMap::iterator						ModelFileMapI;

	typedef	FileInfo<bmsln::HandleBase>					BmslnFileInfo;
	typedef	std::map<SimpleResourceKey, BmslnFileInfo>	BmslnFileMap;
	typedef	BmslnFileMap::iterator						BmslnFileMapI;

	typedef	FileInfo<ml::memory_stream>					M3rFileInfo;
	typedef	std::map<std::wstring, M3rFileInfo>			M3rFileMap;
	typedef	M3rFileMap::iterator						M3rFileMapI;

	std::wstring		texture_data_base_dir_;
	std::wstring		model_data_base_dir_;
	std::wstring		bmsln_data_base_dir_;

	TextureLoader *		texture_loader_;
	ModelCreater *		model_creater_;
	TextureFileMap		texture_file_map_;
	ModelFileMap		model_file_map_;
	M3rFileMap			m3r_file_map_;
	BmslnFileMap		bmsln_file_map_;

	void	RegisterTextureFromFile(const std::wstring &filename, const std::wstring &name, RendererInfo &renderer);
	void	RegisterModelFromFile(const std::wstring &filename, const std::wstring &name, RendererInfo &renderer);
	void	RegisterM3rFromFile(const std::wstring &filename, const std::wstring &name);
};

}	// namespace ide
#endif	// #if defined(BM3_TARGET_IDE)

namespace embedded{

class RegisterResourceProcessInterface : public RegisterResourceProcessInterfacePlatformDependency{
public:
	virtual ~RegisterResourceProcessInterface();

	virtual texture::HandleBase *TextureHandle(const wchar_t *name, TextureCategory::Type category) = 0;
	virtual model::HandleBase *ModelHandle(const wchar_t *name) = 0;
	virtual bmsln::HandleBase *BmslnHandle(const wchar_t * /*name*/){	ML_ASSERT(false);	return NULL;	}
};

class ResourceManagerImpl{

public:
	ResourceManagerImpl();
	~ResourceManagerImpl();

	enum{
		UseHashCount = 3
	};

	template<typename T>
	void	RegisterHash(T hash_array[]){
		texture_hash_	= &hash_array[0];
		model_hash_		= &hash_array[1];
		bmsln_hash_		= &hash_array[2];
	}

	texture::HandleBase	*TextureHandleFromName(const StringType &name, RendererInfo &renderer, TextureCategory::Type category = TextureCategory::Effect);
	texture::HandleBase	*TextureHandleFromName(const wchar_t *name, RendererInfo &renderer, TextureCategory::Type category = TextureCategory::Effect);

	model::HandleBase	*ModelHandleFromName(const StringType &name, RendererInfo &renderer);
	model::HandleBase	*ModelHandleFromName(const wchar_t *name, RendererInfo &renderer);
	const void			*M3rImageFromName(const StringType &name);
	const void			*M3rImageFromName(const wchar_t *name);

	bmsln::HandleBase	*BmslnHandleFromName(const StringType &name);
	bmsln::HandleBase	*BmslnHandleFromName(const wchar_t *name);

	void	UnregisterTexture(	const wchar_t *name);
	void	UnregisterModel(	const wchar_t *name);
	void	UnregisterBmsln(	const wchar_t *name);
	void	UnregisterAllTexture();
	void	UnregisterAllModel();
	void	UnregisterAllBmsln();

	bool	IsRegisteredTexture(const wchar_t *name) const;
	bool	IsRegisteredModel(	const wchar_t *name) const;
	bool	IsRegisteredBmsln(	const wchar_t *name) const;

	void	RegisterResource(const void *bmb_image, RegisterResourceProcessInterface &rrp);
	void	RegisterTexture(const wchar_t *name, texture::HandleBase *handle);
	void	RegisterModel(	const wchar_t *name, model::HandleBase *handle);
	void	RegisterBmsln(	const wchar_t *name, bmsln::HandleBase *handle);

private:
	HashInterface	*texture_hash_;
	HashInterface	*model_hash_;
	HashInterface	*bmsln_hash_;
};
}	// namespace embedded

}	// namespace internal

#if defined(BM3_TARGET_IDE)
	typedef	ml::singleton<internal::ide::ResourceManagerImpl>		ResourceManager;
#endif
#if defined(BM3_TARGET_EMBEDDED)
	typedef	ml::singleton<internal::embedded::ResourceManagerImpl>	ResourceManager;
	typedef	internal::embedded::RegisterResourceProcessInterface	RegisterResourceProcessInterface;

	enum{
		ResourceManager_UseHashCount	= internal::embedded::ResourceManagerImpl::UseHashCount
	};
#endif

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BM3_RESOURCEMANAGER_H
