#ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_SINGLETON_H
#define LIBMATCHLOCK_INC_ML_UTILITY_ML_SINGLETON_H

#if defined(BM3_PLATFORM_nw4c)
#pragma diag_suppress 261
#endif

#include "../type/ml_basic.h"
#include "ml_noncopyable.h"

namespace ml{

///	@addtogroup	utility
///	@{

///	@addtogroup	utility_singleton
///	@{

///	@brief	シングルトン作成クラス
template<typename T>
class singleton : noncopyable{

public:
	///	@brief		初期化
	static void	initialize();

	///	@brief		後処理
	static void	finalize();

	///	@brief		インスタンスを求める
	///	@return		インスタンス
	static T &	instance(){
#if 1
		return *reinterpret_cast<T *>(instance_);
#else
		union Cast{
			ml::max_size_basic_type *	mem_;
			T *							org_;
		};

		Cast c;
		c.mem_ = instance_;

		return *c.org_;
#endif
	}

private:
	typedef ml::max_size_basic_type	instance_type;
	enum{
		instance_size = (sizeof(T) + sizeof(instance_type) - 1) / sizeof(instance_type)
	};

	static ml::max_size_basic_type	instance_[instance_size];
};

///	@brief	シングルトン定義
#define	ML_SINGLETON_DEFINITION(NAME)	friend class ::ml::singleton<NAME>

///	@}

///	@}

}	// namespace ml

#include "ml_singleton_inl.h"

#endif	// #ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_SINGLETON_H
