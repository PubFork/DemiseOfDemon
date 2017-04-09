#ifndef BM3_SDK_INC_BM3_CORE_TYPE_BM3_VECTORBUFFER_H
#define BM3_SDK_INC_BM3_CORE_TYPE_BM3_VECTORBUFFER_H

#include "../../system/bm3_Config.h"
#include "bm3_Allocator.h"

#include "bm3_Vector.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
#if 0
template<typename T>
class VectorBuffer{

private:
	typedef	typename std::vector<T, Allocator<T> >		VectorBufferType;

public:
	typedef	typename VectorBufferType::iterator			Iterator;
	typedef	typename VectorBufferType::const_iterator	ConstIterator;

	VectorBuffer(){		;	}
	~VectorBuffer(){		;	}

	Iterator		Begin(){			return buffer_.begin();		}
	Iterator		End(){				return buffer_.end();		}
	ConstIterator	Begin() const{		return buffer_.begin();		}
	ConstIterator	End() const{		return buffer_.end();		}

	void			PushBack(T v){
		try{
			buffer_.push_back(v);
		}catch(...){
			// Šm•Û‚Å‚«‚È‚©‚Á‚½‚ç–³Ž‹
			;
		}
	}

	void			Set(int id, T v){	buffer_[id] = v;			}
	T				At(int id) const{	return buffer_.at(id);		}
	void			Resize(int size, T v){ buffer_.resize(size, v); }
	void			Assign(int size, T v){ buffer_.assign(size, v); }

	void			Clear(){			buffer_.clear();			}
	Iterator		Erase(Iterator i){	return buffer_.erase(i);	}

	bool			Empty() const{		return buffer_.empty();		}
	int				Size() const{		return int(buffer_.size());	}

private:
	VectorBufferType	buffer_;
	
};
#else

template<typename T>
class VectorBuffer{

public:
	typedef	typename Vector<T>::Iterator			Iterator;
	typedef	typename Vector<T>::ConstIterator		ConstIterator;
	

	VectorBuffer(){		;	}
	~VectorBuffer(){		;	}

	Iterator		Begin(){			return buffer_.begin();		}
	Iterator		End(){				return buffer_.end();		}
	ConstIterator	Begin() const{		return buffer_.begin();		}
	ConstIterator	End() const{		return buffer_.end();		}

	void			Set(int id, T v){	buffer_.set(id, v);			}
	T				At(int id) const{	return buffer_.at(id);		}
	bool			Resize(int size, T v){ return buffer_.resize(size, v); }
	void			Assign(int size, T v){ buffer_.assign(size, v); }

	void			Clear(){			buffer_.clear();			}
	Iterator		Erase(Iterator i){	return buffer_.erase(i);	}

	bool			Empty() const{		return buffer_.empty();		}
	int				Size() const{		return int(buffer_.size());	}

private:
	Vector<T>			buffer_;
	
};

#endif

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_TYPE_BM3_VECTORBUFFER_H
