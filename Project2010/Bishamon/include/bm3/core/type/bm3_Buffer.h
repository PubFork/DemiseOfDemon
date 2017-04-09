#ifndef BM3_SDK_INC_BM3_CORE_TYPE_BM3_BUFFER_H
#define BM3_SDK_INC_BM3_CORE_TYPE_BM3_BUFFER_H

#include "../../system/bm3_Config.h"
#include "bm3_List.h"
#include "bm3_Allocator.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

#if 0
template<typename T>
class Buffer{

private:
	// ��xbox360�̏ꍇ�A���[�h�E�q�b�g�E�X�g�A�̋N�����ɂ����
	// �Ƃ���P�[�X�ł�
	// �Eupdate��`��p�P�b�g�����̃��[�v�� deque���list�̂ق��� 1�`2�����x����
	// �E���������list���deque�̂ق��� 1�`2�����x����

//	typedef	typename std::deque<T, Allocator<T> >	BufferType;
	typedef	typename std::list<T, Allocator<T> >	BufferType;

public:
	typedef	typename BufferType::iterator			Iterator;
	typedef	typename BufferType::const_iterator		ConstIterator;

	Buffer(){		;	}
	~Buffer(){		;	}

	Iterator		Begin(){			return buffer_.begin();		}
	Iterator		End(){				return buffer_.end();		}
	ConstIterator	Begin() const{		return buffer_.begin();		}
	ConstIterator	End() const{		return buffer_.end();		}

//	void			PushBack(T v)
	void			PushBack(const T &v)
	{
		try{
			buffer_.push_back(v);
		}catch(...){
			// �m�ۂł��Ȃ������疳��
			;
		}
	}

	void			Clear(){			buffer_.clear();			}
	Iterator		Erase(Iterator i){	return buffer_.erase(i);	}

	bool			Empty() const{		return buffer_.empty();		}
	int				Size() const{		return int(buffer_.size());	}

private:
	BufferType	buffer_;
};

#else

template<typename T>
class Buffer{

private:
public:
	typedef	typename List<T>::Iterator			Iterator;
	typedef	typename List<T>::ConstIterator		ConstIterator;

	Buffer(){		;	}
	~Buffer(){		;	}

	Iterator		Begin(){				return list_.Begin();		}
	Iterator		End(){					return list_.End();			}
	ConstIterator	ConstBegin() const{		return list_.ConstBegin();	}
	ConstIterator	ConstEnd() const{		return list_.ConstEnd();	}

	void			PushBack(const T &v){
		list_.PushBack(v);
	}

	void			Clear(){			list_.Clear();				}
	Iterator		Erase(Iterator i){	return list_.Erase(i);		}

	bool			Empty() const{		return list_.Empty();		}
	int				Size() const{		return int(list_.Size());	}

private:
	List<T>	list_;
};



#endif

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_TYPE_BM3_BUFFER_H
