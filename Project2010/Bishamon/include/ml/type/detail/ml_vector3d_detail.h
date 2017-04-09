#ifndef LIBMATCHLOCK_INC_ML_TYPE_DETAIL_ML_VECTOR3D_DETAIL_H
#define LIBMATCHLOCK_INC_ML_TYPE_DETAIL_ML_VECTOR3D_DETAIL_H

namespace ml{

///	@cond detail
namespace detail{
///	@endcond

struct vector3d{
	enum{
		element_size = 4				///<—v‘f”
	};

	struct e{
		float	x_;					///<—v‘f
		float	y_;					///<—v‘f
		float	z_;					///<—v‘f
		float	w_;					///<—v‘f
	};

	union{
		e		e_;
		float	v_[element_size];	///<—v‘f
	};
};

///	@cond detail
}	// namespace detail
///	@endcond

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_TYPE_DETAIL_ML_VECTOR3D_DETAIL_H
