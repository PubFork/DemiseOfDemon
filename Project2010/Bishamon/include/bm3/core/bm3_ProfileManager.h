#ifndef BM3_SDK_INC_BM3_CORE_BM3_PROFILEMANAGER_H
#define BM3_SDK_INC_BM3_CORE_BM3_PROFILEMANAGER_H

#include "../system/bm3_Config.h"
#include "bm3_Const.h"
#include <algorithm>
#include <ml/utility/ml_array.h>
#include <ml/utility/ml_singleton.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace internal{

/// @brief ProfileManagerImplƒNƒ‰ƒX
class ProfileManagerImpl{

public:
	ProfileManagerImpl(){

		;
	}
	~ProfileManagerImpl(){

		;
	}

	void	Draw_Reset(){														draw_info_.Reset();									}

	int		Draw_VertexProcessCount() const{									return draw_info_.VertexProcessCount();				}
	int		Draw_PrimitiveProcessCount(EmitterTypeConst type) const{			return draw_info_.PrimitiveProcessCount(type);		}
	int		Draw_TextureSwitchCount() const{									return draw_info_.TextureSwitchCount();				}
	int		Draw_ShaderSwitchCount() const{										return draw_info_.ShaderSwitchCount();				}
	int		Draw_VertexArrayDrawCount() const{									return draw_info_.VertexArrayDrawCount();			}

	void	Draw_AddVertexProcessCount(int count){								draw_info_.AddVertexProcessCount(count);			}
	void	Draw_AddPrimitiveProcessCount(EmitterTypeConst type, int count){	draw_info_.AddPrimitiveProcessCount(type, count);	}
	void	Draw_AddTextureSwitchCount(int count){								draw_info_.AddTextureSwitchCount(count);			}
	void	Draw_AddShaderSwitchCount(int count){								draw_info_.AddShaderSwitchCount(count);				}
	void	Draw_AddVertexArrayDrawCount(int count){							draw_info_.AddVertexArrayDrawCount(count);			}


	void	Memory_Reset(){						memory_info_.Reset();					}

	int		Memory_AllocatedSize() const{		return memory_info_.AllocatedSize();	}
	void	Memory_AddAllocatedSize(int size){	memory_info_.AddAllocatedSize(size);	}

private:
	struct DrawInfo{
		DrawInfo(){
			Reset();
		}

		void Reset(){
			process_vertex_count_	= 0;
			std::fill(ml::array_begin(process_primitive_count_), ml::array_end(process_primitive_count_), 0);
			texture_switch_count_	= 0;
			shader_switch_count_	= 0;
			vertex_draw_count_		= 0;
		}

		int		VertexProcessCount() const{									return process_vertex_count_;				}
		int		PrimitiveProcessCount(EmitterTypeConst type) const{			return process_primitive_count_[type];		}
		int		TextureSwitchCount() const{									return texture_switch_count_;				}
		int		ShaderSwitchCount() const{									return shader_switch_count_;				}
		int		VertexArrayDrawCount() const{								return vertex_draw_count_;					}

		void	AddVertexProcessCount(int count){							process_vertex_count_			+= count;	}
		void	AddPrimitiveProcessCount(EmitterTypeConst type, int count){	process_primitive_count_[type]	+= count;	}
		void	AddTextureSwitchCount(int count){							texture_switch_count_			+= count;	}
		void	AddShaderSwitchCount(int count){							shader_switch_count_			+= count;	}
		void	AddVertexArrayDrawCount(int count){							vertex_draw_count_				+= count;	}

		int		process_vertex_count_;
		int		process_primitive_count_[EmitterTypeConst_MAX];
		int		texture_switch_count_;
		int		shader_switch_count_;
		int		vertex_draw_count_;
	};

	struct MemoryInfo{
		MemoryInfo(){
			Reset();
		}

		void Reset(){
			allocated_size_ = 0;
		}

		int		AllocatedSize() const{		return allocated_size_;		}
		void	AddAllocatedSize(int size){	allocated_size_ += size;	}

		int	allocated_size_;
	};

	DrawInfo	draw_info_;
	MemoryInfo	memory_info_;
};

}	// namespace internal

typedef	ml::singleton<internal::ProfileManagerImpl>	ProfileManager;

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BM3_PROFILEMANAGER_H
