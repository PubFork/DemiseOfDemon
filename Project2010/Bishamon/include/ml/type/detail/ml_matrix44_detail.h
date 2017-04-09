#ifndef LIBMATCHLOCK_INC_ML_TYPE_DETAIL_ML_MATRIX44_DETAIL_H
#define LIBMATCHLOCK_INC_ML_TYPE_DETAIL_ML_MATRIX44_DETAIL_H

namespace ml{

///	@cond detail
namespace detail{
///	@endcond

struct matrix44{
	enum{
		row_size	= 4,		///<�s�T�C�Y
		column_size	= 4			///<��T�C�Y
	};

	struct e{
		float m00_;			///<�v�f
		float m01_;			///<�v�f
		float m02_;			///<�v�f
		float m03_;			///<�v�f

		float m10_;			///<�v�f
		float m11_;			///<�v�f
		float m12_;			///<�v�f
		float m13_;			///<�v�f

		float m20_;			///<�v�f
		float m21_;			///<�v�f
		float m22_;			///<�v�f
		float m23_;			///<�v�f

		float m30_;			///<�v�f
		float m31_;			///<�v�f
		float m32_;			///<�v�f
		float m33_;			///<�v�f
	};

	union{
		e		e_;
		float	m_[row_size][column_size];		///<�v�f�i�Q�����j
		float	mm_[row_size * column_size];	///<�v�f�i�P�����j
	};
};

///	@cond detail
}	// namespace detail
///	@endcond

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_TYPE_DETAIL_ML_MATRIX44_DETAIL_H
