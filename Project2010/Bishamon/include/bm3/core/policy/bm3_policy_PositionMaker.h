#ifndef BM3_SDK_INC_BM3_CORE_POLICY_POSITIONMAKER_H
#define BM3_SDK_INC_BM3_CORE_POLICY_POSITIONMAKER_H

#include "../bm3_Const.h"
#include <ml/type/ml_vector3d.h>
#include <ml/math/ml_math_const.h>
#include <ml/math/ml_triangular.h>
#include "../bm3_ResourceManager.h"
#include "../bm3_RendererInfo.h"
#include <m3dg/model/m3dg_model.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief PositionMakerクラス。 指定の形状のどこかの位置を与える
class PositionMaker{
	
	struct Callback{
		struct Arg{
			Arg(ml::random *random, ml::vector3d *value, ml::vector3d *direction) :
				random_(random),
				value_(value),
				direction_(direction){;}

			ml::random		*random_;
			ml::vector3d	*value_;
			ml::vector3d	*direction_;
		};

		static void Func(const m3dg::VertexArrayInfo &info, void *user_data){

			Arg &arg = *reinterpret_cast<Arg *>(user_data);
			{
				ML_ASSERT((info.vertex_count_ % 3) == 0);

				int		tri_count	= (int)(info.vertex_count_ * ml::inverse3);
				float	measure		= arg.random_->f32(info.all_triangle_measure_);

				const float *i = std::lower_bound(info.triangle_addition_measure_, info.triangle_addition_measure_ + tri_count, measure);

				int tri_no = i - info.triangle_addition_measure_;
				tri_no = ml::clamp(tri_no - 1, 0, tri_count - 1);

				const m3dg::VertexArrayInfo::IndexType *index = info.index_array_ + tri_no * 3;

				const ml::f32 *pos0    = ml::offset_pointer<const ml::f32 *>(info.position_array_, info.position_array_stride_ * index[0]);
				const ml::f32 *pos1    = ml::offset_pointer<const ml::f32 *>(info.position_array_, info.position_array_stride_ * index[1]);
				const ml::f32 *pos2    = ml::offset_pointer<const ml::f32 *>(info.position_array_, info.position_array_stride_ * index[2]);

				const ml::f32 *normal0 = ml::offset_pointer<const ml::f32 *>(info.normal_array_,   info.normal_array_stride_   * index[0]);
				const ml::f32 *normal1 = ml::offset_pointer<const ml::f32 *>(info.normal_array_,   info.normal_array_stride_   * index[1]);
				const ml::f32 *normal2 = ml::offset_pointer<const ml::f32 *>(info.normal_array_,   info.normal_array_stride_   * index[2]);

				float r0 = arg.random_->f32(1.0f);
				float r1 = arg.random_->f32(1.0f - r0);
				float r2 = 1.0f - r0 - r1;

				float px = pos0[0] * r0 + pos1[0] * r1 + pos2[0] * r2;
				float py = pos0[1] * r0 + pos1[1] * r1 + pos2[1] * r2;
				float pz = pos0[2] * r0 + pos1[2] * r1 + pos2[2] * r2;

				float nx = normal0[0] * r0 + normal1[0] * r1 + normal2[0] * r2;
				float ny = normal0[1] * r0 + normal1[1] * r1 + normal2[1] * r2;
				float nz = normal0[2] * r0 + normal1[2] * r1 + normal2[2] * r2;

				arg.value_->e_.x_ = px;
				arg.value_->e_.y_ = py;
				arg.value_->e_.z_ = pz;
				arg.value_->e_.w_ = 1.0f;

#if defined(BM3_PLATFORM_win_directx9) || defined(BM3_PLATFORM_xbox360) || defined(BM3_PLATFORM_gamebryodx9) || defined(BM3_PLATFORM_playstation3)
				arg.direction_->e_.x_ = -nx;
				arg.direction_->e_.y_ = -ny;
				arg.direction_->e_.z_ = -nz;
#else
				arg.direction_->e_.x_ = +nx;
				arg.direction_->e_.y_ = +ny;
				arg.direction_->e_.z_ = +nz;
#endif

				arg.direction_->e_.w_ = 1.0f;
			}
		}
	};
	
private:
	
public:	
	static BM3_FORCEINLINE void Point(ml::vector3d &dst, ml::random & /*random*/){
		dst.set_unit();
	}

	static BM3_FORCEINLINE void Sphere(ml::vector3d &dst, ml::random &random, const float inside_rate, const ml::vector3d &outside, const float horizon_s, const float horizon_e, const float vertical_s, const float vertical_e){
		{
			dst.mul(
				outside, 
				ml::pow(random.f32(inside_rate, 1.0f) ,ml::inverse3)
			);
			dst.e_.w_	= 1.0f;
		}
		{
			float	angle_h		= ml::to_radian(random.f32(	ml::minimum(horizon_s	,horizon_e),
															ml::maximum(horizon_s	,horizon_e)));

			float	angle_cos_v;
			{
				// 古いバージョンではエンドが180を超えることがあるのでコンバータができるまでの応急処置。
				float	end = (180.0f < vertical_e) ? vertical_e*0.5f : vertical_e;
				angle_cos_v = 1.0f - random.f32(ml::minimum(vertical_s		,end),
												ml::maximum(vertical_s		,end)) * ml::inverse(180.0f) * 2;
			}
//				float	angle_cos_v = 1.0f - random.f32(ml::minimum(vertical_s		,vertical_e),
//															ml::maximum(vertical_s		,vertical_e)) * ml::inverse(180.0f) * 2;
			float	angle_sin_v = ml::sqrtf(ml::maximum(0.0f, 1.0f-angle_cos_v*angle_cos_v));

			dst.e_.x_ *= angle_sin_v * ml::sinf(angle_h);
			dst.e_.y_ *= angle_cos_v;
			dst.e_.z_ *= angle_sin_v * ml::cosf(angle_h);
		}
	}

	static BM3_FORCEINLINE void Cylinder(ml::vector3d &dst, ml::random &random, const float inside_rate, const ml::vector3d &outside, const float horizon_s, const float horizon_e){
		{
			float	radius = random.f32(inside_rate, 1.0f);
			dst.e_.x_	= outside.e_.x_ * radius;
			dst.e_.y_	= outside.e_.y_ * random.f32(-0.5f,0.5f);
			dst.e_.z_	= outside.e_.z_ * radius;
			dst.e_.w_	= 1.0f;
		}

		{
			float				h[2];
			ml::sin_cos(random.f32(	ml::minimum(ml::to_radian(horizon_s)	,ml::to_radian(horizon_e)),
									ml::maximum(ml::to_radian(horizon_s)	,ml::to_radian(horizon_e)))	,h[0], h[1]);
			dst.e_.x_	*= h[1];
			dst.e_.z_	*= h[0];
		}
	}

	static BM3_FORCEINLINE void Cube(ml::vector3d &dst, ml::random &random, const float inside_rate, const ml::vector3d &outside){
		{
			int		e = random.i32(0, 3);
			float	r = random.f32(-1.0f, 1.0f) * (1.000001f - inside_rate);	// 1.0fからひくと必ず0になってしまうので僅かに大きくしておく
			(r > 0.0f) ? r += inside_rate : r -= inside_rate;

//			dst.random_range(random ,ml::vector3d::unit ,outside);
			dst.random_range_base0(random ,outside);
					if(e==0){	dst.e_.x_	= r * outside.e_.x_;	}
			else	if(e==1){	dst.e_.y_	= r * outside.e_.y_;	}
			else	if(e==2){	dst.e_.z_	= r * outside.e_.z_;	}
		}
	}
	
	// モデルは処理が重くなるので方向も決めてしまう
	static BM3_FORCEINLINE void Model(ml::vector3d &dst, ml::vector3d &dst_direction, ml::random &random, const StringType &name, const ml::vector3d &scale){
		
		const void *m3r_image = ResourceManager::instance().M3rImageFromName(name);

		if(m3r_image){
			Callback::Arg arg(&random, &dst, &dst_direction);

			m3dg::model::ForeachVertexArray(
				*reinterpret_cast<const m3dg::fileformat::m3r::Header *>(m3r_image),
				&Callback::Func,
				&arg
			);

			dst.mul(scale);
		}
	}
};


}	// namespace impl

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_POLICY_POSITIONMAKER_H

