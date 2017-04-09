#ifndef BM3_SDK_INC_BM3_UTILITY_BM3_STRING_H
#define BM3_SDK_INC_BM3_UTILITY_BM3_STRING_H

#include <ml/utility/ml_assert.h>
#if defined(BM3_TARGET_IDE)
#include <ml/utility/ml_string.h>		// ml::strcmp
#endif
#if defined(BM3_PLATFORM_playstation3)
#include <ml/utility/ml_string.h>		// ml::strcmp
#endif
#if defined(BM3_PLATFORM_win_opengl)
#include <ml/utility/ml_string.h>		// ml::strcmp
#endif
#if defined(BM3_PLATFORM_opengles)
#include <ml/utility/ml_string.h>		// ml::strcmp
#endif
#if defined(BM3_PLATFORM_nw4c)
#include <ml/utility/ml_string.h>		// ml::strcmp
#endif
#include <ml/type/ml_basic.h>			// ml::ptr_size_ui
#include "../system/bm3_Config.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

#if defined(BM3_TARGET_IDE)

template<typename CharType = wchar_t>
class DynamicString{

private:
	typedef	std::basic_string<CharType>	StringType;

public:
	DynamicString(){								;	}
	DynamicString(const CharType *s)   : data_(s){	;	}
	DynamicString(const StringType &s) : data_(s){	;	}

	void				Set(const CharType *s){	data_ = s;				}
	const CharType *	Get() const{			return data_.c_str();	}
	bool				IsEmpty() const{		return data_.empty();	}

	bool operator==(const DynamicString &s) const{	return data_ == s.data_;	}
	bool operator!=(const DynamicString &s) const{	return data_ != s.data_;	}
	bool operator<( const DynamicString &s) const{	return data_ <  s.data_;	}

	void	Mapping(const void * /*start*/){
		;
	}

private:
	StringType	data_;
};

#endif	// #if defined(BM3_TARGET_IDE)

template<typename CharType = wchar_t>
class StaticString{

public:
	const CharType *	Get() const{		ML_PTR_ASSERT(data_);	return data_;				}
	bool				IsEmpty() const{	return (data_ == NULL) || (ml::strlen(Get()) == 0);	}

	bool operator==(const StaticString &s) const{	return ml::strcmp(Get(), s.Get()) == 0;	}
	bool operator!=(const StaticString &s) const{	return ml::strcmp(Get(), s.Get()) != 0;	}
	bool operator<( const StaticString &s) const{	return ml::strcmp(Get(), s.Get()) <  0;	}

	void	Mapping(const void *start){
		ML_PTR_ASSERT(start);

		data_ =
			(offset_ == ml::ptr_size_ui(-1)) ?
				NULL :
				reinterpret_cast<const CharType *>(
					ml::ptr_size_ui(start) + offset_
				);
	}

	void	SetOffset(ml::ptr_size_ui o){	offset_ = o;	}
	int		Offset() const{					return offset_;	}

private:
	union{
		const CharType *	data_;
		ml::ptr_size_ui		offset_;
	};
};

ML_STATIC_ASSERT(sizeof(StaticString<char>)    == sizeof(ml::ptr_size_ui));
ML_STATIC_ASSERT(sizeof(StaticString<wchar_t>) == sizeof(ml::ptr_size_ui));

#if defined(BM3_TARGET_IDE)
	typedef	DynamicString<>		StringType;
#endif
#if defined(BM3_TARGET_EMBEDDED)
	typedef	StaticString<>		StringType;
#endif

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_UTILITY_BM3_STRING_H
