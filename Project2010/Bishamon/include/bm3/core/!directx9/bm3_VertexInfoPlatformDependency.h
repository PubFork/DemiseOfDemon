#ifndef BM3_SDK_INC_BM3_CORE_DIRECTX9_BM3_VERTEXINFOPLATFORMDEPENDENCY_H
#define BM3_SDK_INC_BM3_CORE_DIRECTX9_BM3_VERTEXINFOPLATFORMDEPENDENCY_H

#include <d3d9.h>
#include <d3dx9.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace internal{

	
#define MAX_ELEMENT_COUNT (7)	// 仮

struct VertexInfoPlatformDependency{
	
	D3DVERTEXELEMENT9*	Element() {				return element_; 		}
	unsigned int		ElementNum() const {	return element_num_;	}
	void	SetElementNum(unsigned int num)	{	element_num_ = num;		}
	
private:
	D3DVERTEXELEMENT9	element_[MAX_ELEMENT_COUNT];
	unsigned int 		element_num_;				// 頂点フォーマットの数
};
	
	

}	// namespace internal
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DIRECTX9_BM3_VERTEXINFOPLATFORMDEPENDENCY_H
