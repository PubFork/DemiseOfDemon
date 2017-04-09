#ifndef LIBMATCHLOCK_INC_ML_TYPE_DETAIL_ML_COLOR_DETAIL_h
#define LIBMATCHLOCK_INC_ML_TYPE_DETAIL_ML_COLOR_DETAIL_h

namespace ml{

///	@cond detail
namespace detail{
///	@endcond

template<typename T>
struct color_rgb{
	enum{
		element_size = 3			///<�v�f��
	};

	struct e{
		T	r_;					///<��
		T	g_;					///<��
		T	b_;					///<��
	};

	union{
		e	e_;
		T	v_[element_size];	///<�v�f��
	};
};

template<typename T>
struct color_argb{
	enum{
		element_size = 4			///<�v�f��
	};

	struct e{
		T	a_;					///<�A���t�@
		T	r_;					///<��
		T	g_;					///<��
		T	b_;					///<��
	};

	union{
		e	e_;
		T	v_[element_size];	///<�v�f��
	};
};

template<typename T>
struct color_rgba{
	enum{
		element_size = 4			///<�v�f��
	};

	struct e{
		T	r_;					///<��
		T	g_;					///<��
		T	b_;					///<��
		T	a_;					///<�A���t�@
	};

	union{
		e	e_;
		T	v_[element_size];	///<�v�f��
	};
};

///	@cond detail
}	// namespace detail
///	@endcond

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_TYPE_DETAIL_ML_COLOR_DETAIL_h
