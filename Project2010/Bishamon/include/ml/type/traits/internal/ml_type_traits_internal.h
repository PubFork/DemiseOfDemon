#ifndef LIBMATCHLOCK_INC_ML_TYPE_TRAITS_INTERNAL_ML_TYPE_TRAITS_INTERNAL_H
#define LIBMATCHLOCK_INC_ML_TYPE_TRAITS_INTERNAL_ML_TYPE_TRAITS_INTERNAL_H

namespace ml{

///	@cond internal

#define	ML_INTERNAL_BOOL_TRAIT(NAME)				\
	template<typename T>							\
	struct NAME{									\
		enum{										\
			value = false							\
		};											\
	}

#define	ML_INTERNAL_BOOL_TRAIT_TRUE(NAME, TYPE)		\
	template<>										\
	struct NAME<TYPE>{								\
		enum{										\
			value = true							\
		};											\
	}

#define	ML_INTERNAL_BOOL_TRAIT_TRUE_ARG(NAME, TYPE)	\
	template<typename T>							\
	struct NAME<TYPE>{								\
		enum{										\
			value = true							\
		};											\
	}

///	@endcond

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_TYPE_TRAITS_INTERNAL_ML_TYPE_TRAITS_INTERNAL_H
