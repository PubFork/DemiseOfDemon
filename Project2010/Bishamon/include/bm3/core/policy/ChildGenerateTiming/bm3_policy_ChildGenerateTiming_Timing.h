#ifndef BM3_SDK_INC_BM3_CORE_POLICY_CHILDGENERATETIMING_BM3_POLICY_CHILDGENERATETIMING_TIMING_H
#define BM3_SDK_INC_BM3_CORE_POLICY_CHILDGENERATETIMING_BM3_POLICY_CHILDGENERATETIMING_TIMING_H

#include "../../object/bm3_obj_RootNodeBase.h"
#include "../../bm3_CoreType.h"
#include <ml/utility/ml_assert.h>
#include <ml/utility/ml_cast.h>
#include <float.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

/// @brief ChildGenerateTiming_Timingクラス
class ChildGenerateTiming_Timing{

public:
	enum{
		IsGenerate = true
	};

	ChildGenerateTiming_Timing(){
		;
	}

	template<typename ResourceType>
	void	Initialize(const ResourceType &res){

		if(res.Generate_Basic_GenerateType() == GenerateTypeConst_Timing){
			repeat_frame_offset_ = 0.0f;

			repeat_count_ = res.Generate_Timing_RepeatCount();
			ML_ASSERT(repeat_count_ > 0);
		}
	}

	template<typename ResourceType, typename LifeType, typename UpdateInfo>
	int		Update(UpdateInfo &info, const ResourceType &res, ml::random &/*random*/, const LifeType &life, const obj::RootNodeBase &root){

		if(res.Generate_Basic_GenerateType() == GenerateTypeConst_Timing){

			if(root.IsRequestRetire() == false){

				if(IsAllChildGenerated(res, root) == false){

					int count = 0;
					{
						float current	= life.LifeRate();
						float next		= current + (life.LifeFrameStep()*info.GetUpdateFrame());
						ML_ASSERT(current <= next);

						GenerateTimingCurveType::KeyframeMapCI b = res.Generate_Timing_Timing().LowerKeyframe(current - repeat_frame_offset_);
						GenerateTimingCurveType::KeyframeMapCI e = res.Generate_Timing_Timing().UpperKeyframe(next    - repeat_frame_offset_);

						GenerateTimingCurveType::KeyframeMapCI i = b;
						for(;i != e;++ i){
#if defined(BM3_TARGET_IDE)
							count += ml::numeric_cast<int>(i->second.Value());
#endif
#if defined(BM3_TARGET_EMBEDDED)
							count += ml::numeric_cast<int>(i->Value());
#endif
						}

						// リピート指定で、リピート終わりのタイミングだったら
						if(e == res.Generate_Timing_Timing().KeyframeEnd()){
							repeat_count_ = ml::maximum(0, repeat_count_ - 1);
							repeat_frame_offset_ = next;
						}
					}
					return ml::maximum(0, count);
				}
			}
		}

		return 0;
	}

	// すべての子を生成したか
	template<typename ResourceType, typename LifePolicy>
	bool	IsAllChildGenerated(const ResourceType &res, const obj::RootNodeBase &root, const LifePolicy &life) const{

		if(res.Generate_Basic_GenerateType() == GenerateTypeConst_Timing){
			if(root.IsRequestRetire()){
				return true;
			}else{
				if(res.Generate_Timing_EndlessRepeat())	return false;
				switch(res.Basic_Basic_ExpirationType()){
					case ExpirationTypeConst_Life:				return life.IsExpired();
					case ExpirationTypeConst_AllChildExpired:	return (repeat_count_ == 0);
					default:	ML_ASSERT(false);				return false;
				}
/*				
				switch(res.Basic_Basic_ExpirationType()){
					case ExpirationTypeConst_Life:				
						return life.IsExpired();
					case ExpirationTypeConst_AllChildExpired:
						return
							(res.Generate_Timing_EndlessRepeat() == false) &&
							(repeat_count_ == 0);
					default:	ML_ASSERT(false);	return false;
				}
*/
			}
		}else{
			return false;
		}
	}

	// ライフを考慮しない
	template<typename ResourceType>
	bool	IsAllChildGenerated(const ResourceType &res, const obj::RootNodeBase &root) const{

		if(res.Generate_Basic_GenerateType() == GenerateTypeConst_Timing){
			if(root.IsRequestRetire()){
				return true;
			}else{
				return
					(res.Generate_Timing_EndlessRepeat() == false) &&
					(repeat_count_ == 0);
			}
		}else{
			return false;
		}
	}

	template<typename ResourceType>
	static int	CalcTotalGenerateNodeCount(const ResourceType &res){

		if(res.Generate_Basic_GenerateType() == GenerateTypeConst_Timing){

			int max_lifespan = res.Basic_Basic_Life() + res.Basic_Basic_LifeRange();// + res.Basic_Basic_ActiveStart() + res.Basic_Basic_ActiveStartRange();
			return GenerateNodeCount(res, max_lifespan);
		}

		return 0;
	}

	template<typename ResourceType>
	static int	CalcTotalGenerateNodeCount(const ResourceType &res, int child_lifespan){
		if(res.Generate_Basic_GenerateType() == GenerateTypeConst_Timing){

			int max_lifespan = res.Basic_Basic_Life() + res.Basic_Basic_LifeRange();// + res.Basic_Basic_ActiveStart() + res.Basic_Basic_ActiveStartRange();
			return GenerateNodeCount(res, ml::maximum(max_lifespan, child_lifespan));
		}

		return 0;
	}

	template<typename ResourceType>
	static int	CalcTotalLifeSpan(const ResourceType &res){
		int total_lifespan = 0;
		if(res.Generate_Basic_GenerateType() == GenerateTypeConst_Timing){

			if(res.Generate_Timing_Timing().IsKeyframeEmpty()){
				;
			}
			else{
				if(res.Generate_Timing_EndlessRepeat()){
					total_lifespan = -1;
				}
				else{
					int max_lifespan = res.Basic_Basic_Life() + res.Basic_Basic_LifeRange() + res.Basic_Basic_ActiveStart() + res.Basic_Basic_ActiveStartRange();
					const float lifespan = res.Generate_Timing_Timing().KeyframeBackValue() * res.Basic_Basic_Life();
					total_lifespan = (int)(lifespan * res.Generate_Timing_RepeatCount() + max_lifespan);
				}
			}
		}

		return total_lifespan;
	}

	template<typename ResourceType>
	void	SwitchResource(const ResourceType &res){
		if(res.Generate_Basic_GenerateType() == GenerateTypeConst_Timing){

			repeat_count_ = res.Generate_Timing_RepeatCount();
			ML_ASSERT(repeat_count_ > 0);
		}
	}

private:
	int		repeat_count_;
	float	repeat_frame_offset_;

	template<typename ResourceType>
	static int	GenerateNodeCount(const ResourceType &res, int max_lifespan){

		int count = 0;
		if(res.Generate_Timing_Timing().IsKeyframeEmpty()){
			;
		}
		else{
			int total_generate_count	= 0;
			GenerateTimingCurveType::KeyframeMapCI	i	= res.Generate_Timing_Timing().KeyframeBegin();
			for(; i!=res.Generate_Timing_Timing().KeyframeEnd() ; ++i){
#if defined(BM3_TARGET_IDE)
				total_generate_count	+= (int)i->second.Value();
#else
				total_generate_count	+= (int)i->Value();
#endif
			}

			const float lifespan = res.Generate_Timing_Timing().KeyframeBackValue() * res.Basic_Basic_Life();

			// トータルリピート時間より最大寿命の方が長いとき
			if(res.Generate_Timing_EndlessRepeat() || (max_lifespan < (lifespan * res.Generate_Constant_RepeatTime()))){
				// 寿命より生成間隔がかなり小さいとき、第一項が0になる可能性があるため最後に＋1
				count = (int)(total_generate_count * (int)((float)max_lifespan / lifespan + 1)) +1;
			}
			else{
				count = total_generate_count * res.Generate_Timing_RepeatCount();
			}
		}
	
		return count;
	}
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_POLICY_CHILDGENERATETIMING_BM3_POLICY_CHILDGENERATETIMING_TIMING_H
