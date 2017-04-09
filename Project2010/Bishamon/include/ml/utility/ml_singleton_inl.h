#ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_SINGLETON_INL_H
#define LIBMATCHLOCK_INC_ML_UTILITY_ML_SINGLETON_INL_H

namespace ml{

template<typename T>
typename singleton<T>::instance_type	singleton<T>::instance_[singleton<T>::instance_size];

template<typename T>
void singleton<T>::initialize(){

	new (instance_) T;
}

template<typename T>
void singleton<T>::finalize(){

	reinterpret_cast<T *>(instance_)->~T();
}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_SINGLETON_INL_H
