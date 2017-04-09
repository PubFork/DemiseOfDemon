#ifndef BM3_SDK_INC_BM3_CORE_BM3_VERTEXMANAGERINTERFACE_H
#define BM3_SDK_INC_BM3_CORE_BM3_VERTEXMANAGERINTERFACE_H

#include "../system/bm3_Config.h"
#include "bm3_CoreType.h"
#include "bm3_VertexContext.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

/// @brief BeginVertexInfo�N���X
struct BeginVertexInfo{
	BeginVertexInfo(int size, void* handle) :
		size_(size), handle_(handle){
	}
	
	void SetHandle(void* handle){	handle_	= handle;	}

	int	Size()		const {	return size_;			}
	void* Handle()	const {	return handle_;			}
	
private:
	int		size_;
	void*	handle_;
};

/// @brief EndVertexInfo�N���X
struct EndVertexInfo{
	EndVertexInfo(int size, void* handle) :
		size_(size), handle_(handle){
	}
	
	int	Size()		const {	return size_;			}
	void* Handle()	const {	return handle_;			}
private:
	int		size_;
	void*	handle_;
};
	
/// @brief VertexMemoryInfo�N���X
struct VertexMemoryInfo{
public:
	VertexMemoryInfo(int size, int max_size, void* handle) :
		size_(size), max_size_(max_size), handle_(handle){
	}
	
	void SetHandle(void* handle){	handle_	= handle;	}

	int	Size()		const {	return size_;			}
	int MaxSize()	const {	return max_size_;		}
	void* Handle()	const {	return handle_;			}
	
private:
	int		size_;
	int		max_size_;
	void*	handle_;
};

/// @brief VertexManagerInterface�N���X
class VertexManagerInterface{
public:
	virtual ~VertexManagerInterface(){}
	
	virtual void BeginEffect(BeginVertexInfo & info)	= 0;	//!< �G�t�F�N�g�쐬���ɌĂяo�����
	virtual void EndEffect(EndVertexInfo & info)		= 0;	//!< �G�t�F�N�g�I�����ɌĂяo�����

	virtual void BeginMakeVertex(DrawInfo &draw_info, VertexMemoryInfo &vertex_info)	= 0;//!< �G�t�F�N�g�̒��_�쐬�J�n���ƂɌĂяo�����
	virtual void EndMakeVertex(DrawInfo &draw_info, VertexMemoryInfo &vertex_info)	= 0;	//!< �G�t�F�N�g�̒��_�쐬�I�����ƂɌĂяo�����

	virtual void* Alloc(VertexMemoryInfo &info)		= 0;	//!< BeginMakeVertex��ɌĂяo����A�o�b�t�@��n��
	virtual bool Alloc(int size)		= 0;				//!< ���_�o�b�t�@�����������Ȃ��^�C�v�̃o�b�t�@���m�ۂ���Ƃ��ɃT�C�Y�̊m�F�̂ݍs��
	virtual void Free(void* handle, int size)	= 0;	//!< �G�t�F�N�g�f�X�g���N�^���ɌĂяo����A�o�b�t�@���J������
	virtual void Free(int size)	= 0;					//!< ���_�o�b�t�@�����������Ȃ��^�C�v�̃o�b�t�@�̃T�C�Y��߂�

	virtual int MakeVertexIndex() 	const	= 0;	//!< ���݂̒��_�o�b�t�@�C���f�b�N�X
	virtual int DrawIndex()			const	= 0;	//!< ���݂̕`��o�b�t�@�C���f�b�N�X
	virtual int VertexBufferCount()	const	= 0;	//!< ���_�o�b�t�@�̖{��

	virtual void PrepareDrawPrimitive(DrawInfo &draw_info, VertexContext &ctx)	= 0;//!< �`�揀��
	virtual void DrawPrimitive(DrawInfo &draw_info, VertexContext &ctx)	= 0;		//!< �`��
#ifdef BM3_PLATFORM_psp	
	virtual void DrawPrimitiveN(DrawInfo &draw_info, VertexContext &ctx)	= 0;		//!< �`��
#endif
	
#ifdef BM3_PLATFORM_playstation3
	virtual int LocationType()	= 0;	//!< ���_�o�b�t�@�̃^�C�v�i���C���i�}�b�v�ς݁j�����[�J���j

#endif
};

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BM3_VERTEXMANAGERINTERFACE_H
