#ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_NONCOPYABLE_H
#define LIBMATCHLOCK_INC_ML_UTILITY_ML_NONCOPYABLE_H

namespace ml{

///	@addtogroup	utility
///	@{

///	@addtogroup	utility_noncopyable
///	@{

///	@brief	コピーさせない
///	@note	このクラスを継承するとコピーできなくなります
class noncopyable{
///	@cond internal

protected:
	noncopyable(){;}
	~noncopyable(){;}

private:
	noncopyable(const noncopyable &);
	const noncopyable &	operator=(const noncopyable &);
///	@endcond
};

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_NONCOPYABLE_H
