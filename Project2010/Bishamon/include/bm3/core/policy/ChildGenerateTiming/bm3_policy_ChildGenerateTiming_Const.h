#ifndef BM3_SDK_INC_BM3_CORE_POLICY_CHILDGENERATETIMING_BM3_POLICY_CHILDGENERATETIMING_CONST_H
#define BM3_SDK_INC_BM3_CORE_POLICY_CHILDGENERATETIMING_BM3_POLICY_CHILDGENERATETIMING_CONST_H

#include "../../object/bm3_obj_RootNodeBase.h"
#include <ml/utility/ml_assert.h>
#include <ml/utility/ml_cast.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

/// @brief ChildGenerateTiming_Const�N���X
class ChildGenerateTiming_Const{

public:
	enum{
		IsGenerate = true
	};

	ChildGenerateTiming_Const() :
		generate_pick_(0.0f),
		generated_count_(0),
		max_generate_count_(0){
		;
	}

	template<typename ResourceType>
	void	Initialize(const ResourceType &res){

		if(res.Generate_Basic_GenerateType() == GenerateTypeConst_Constant){

			ML_ASSERT(res.Generate_Constant_GenerateCount() > 0);
			ML_ASSERT(res.Generate_Constant_RepeatTime() > 0);

			max_generate_count_	= res.Generate_Constant_GenerateCount() * res.Generate_Constant_RepeatTime();
			ML_ASSERT(max_generate_count_ > 0);

			generated_count_	= 0;
			interval_frame_		= 0;
			ML_ASSERT(interval_frame_ >= 0);

			// �v�Z�덷�ɂ�萶�����������ɕς��A����Ă������������ƍ���Ȃ����Ƃ�����B
			// �ŏI�I�ɐ����l�̒l�ɒ����Đ����������̂���������ɏ��Ȃ��l�ɂ�����͔����ɑ傫���l�����Ă������ق�������B
			generate_per_time_	= ml::numeric_cast<float>(res.Generate_Constant_GenerateCount()) * ml::inverse<float>(ml::numeric_cast<float>(res.Generate_Constant_GenerateTime())) + 0.000001f;
//			generate_per_time_	= ml::numeric_cast<float>(res.Generate_Constant_GenerateCount()) / ml::numeric_cast<float>(res.Generate_Constant_GenerateTime());

			if(res.Generate_Constant_GenerateCount() < res.Generate_Constant_GenerateTime()){
				// �ŏ��̃t���[���ōŒ჏���V���b�g����
				generate_pick_ = 1.0f - generate_per_time_;
			}
			else{
				generate_pick_	= 0;
			}

		}
	}

	template<typename ResourceType, typename LifeType, typename UpdateInfo>
	int		Update(UpdateInfo &info, const ResourceType &res, ml::random &random, const LifeType & /*life*/, const obj::RootNodeBase &root){

		if(res.Generate_Basic_GenerateType() == GenerateTypeConst_Constant){

			if(root.IsRequestRetire() == false){

				ML_ASSERT(max_generate_count_ > 0);

				if(IsAllChildGenerated(res, root) == false){

					if(interval_frame_ > 0){
						-- interval_frame_;

					}else{
						// �P�A�b�v�f�[�g������̐�������������
						generate_pick_ += generate_per_time_ * info.GetUpdateFrame();

						// �������𓾂�
						int c = ml::numeric_cast<int>(generate_pick_);
						if( !res.Generate_Constant_EndlessRepeat() ) 
							c = ml::minimum(c,max_generate_count_-generated_count_);

						if(c >= 1){
							generated_count_	+= c;
							generate_pick_		-= ml::numeric_cast<float>(c);
							if( root.IsEnabledBm2() == true )
							{
								if( !((generated_count_) % res.Generate_Constant_GenerateCount()) )
								{
									interval_frame_		= MakeIntervalFrame(res, random);
								}
								else
								{
									// ������ɗ���̂͐������ł̗]�肪���������ꍇ�ɃC���^�[�o���������z���Ȃ��悤�ɂ��邽�߁B
									interval_frame_		= 0;
									random.i32(res.Generate_Constant_IntervalRange());
								}
							}
							else
							{
								interval_frame_		= MakeIntervalFrame(res, random);
							}
							return c;
						}
					}
				}
			}
		}

		return 0;
	}

	// ���ׂĂ̎q�𐶐�������
	template<typename ResourceType, typename LifePolicy>
	bool	IsAllChildGenerated(const ResourceType &res, const obj::RootNodeBase &root, const LifePolicy &life) const{

		if(res.Generate_Basic_GenerateType() == GenerateTypeConst_Constant){
			if(root.IsRequestRetire()){
				return true;
			}else{
				if(res.Generate_Constant_EndlessRepeat())	return false;
				switch(res.Basic_Basic_ExpirationType()){
					case ExpirationTypeConst_Life:				return life.IsExpired() || (generated_count_ >= max_generate_count_);
					case ExpirationTypeConst_AllChildExpired:	return (generated_count_ >= max_generate_count_);
					default:	ML_ASSERT(false);				return false;
				}
/*
				switch(res.Basic_Basic_ExpirationType()){
					case ExpirationTypeConst_Life:				
						return life.IsExpired() || 
							((res.Generate_Constant_EndlessRepeat() == false) && (generated_count_ >= max_generate_count_));
					case ExpirationTypeConst_AllChildExpired:
						return
							(res.Generate_Constant_EndlessRepeat() == false) &&
							(generated_count_ >= max_generate_count_);
					default:	ML_ASSERT(false);	return false;
				}
*/
			}
		}else{
			return false;
		}
	}

	// life���l�����Ȃ�
	template<typename ResourceType>
	bool	IsAllChildGenerated(const ResourceType &res, const obj::RootNodeBase &root) const{

		if(res.Generate_Basic_GenerateType() == GenerateTypeConst_Constant){
			if(root.IsRequestRetire()){
				return true;
			}else{
				return
					(res.Generate_Constant_EndlessRepeat() == false) &&
					(generated_count_ >= max_generate_count_);
			}
		}else{
			return false;
		}
	}
	
	template<typename ResourceType>
	static int	CalcTotalGenerateNodeCount(const ResourceType &res){

		if(res.Generate_Basic_GenerateType() == GenerateTypeConst_Constant){

			int max_lifespan = res.Basic_Basic_Life() + res.Basic_Basic_LifeRange();// + res.Basic_Basic_ActiveStart() + res.Basic_Basic_ActiveStartRange();
			return GenerateNodeCount(res, max_lifespan);
		}

		return 0;
	}

	template<typename ResourceType>
	static int	CalcTotalGenerateNodeCount(const ResourceType &res, int child_lifespan){
		if(res.Generate_Basic_GenerateType() == GenerateTypeConst_Constant){
			int max_lifespan = res.Basic_Basic_Life() + res.Basic_Basic_LifeRange();
			return GenerateNodeCount(res, ml::maximum(max_lifespan, child_lifespan));
		}

		return 0;
	}

	template<typename ResourceType>
	static int	CalcTotalLifeSpan(const ResourceType &res){
		int total_lifespan = 0;

		if(res.Generate_Basic_GenerateType() == GenerateTypeConst_Constant){

			if(res.Generate_Constant_EndlessRepeat()){
				total_lifespan	= -1;
			}
			else{
				int max_lifespan	= res.Basic_Basic_Life() + res.Basic_Basic_LifeRange() + res.Basic_Basic_ActiveStart() + res.Basic_Basic_ActiveStartRange();
				int interval		= res.Generate_Constant_Interval() + res.Generate_Constant_IntervalRange();

				total_lifespan	= (res.Generate_Constant_GenerateCount() * interval + res.Generate_Constant_GenerateTime())
									* res.Generate_Constant_RepeatTime() 
									+ max_lifespan;
			}
		}

		return total_lifespan;
	}

	template<typename ResourceType>
	void	SwitchResource(const ResourceType &res){
		if(res.Generate_Basic_GenerateType() == GenerateTypeConst_Constant){

			ML_ASSERT(res.Generate_Constant_GenerateCount() > 0);
			ML_ASSERT(res.Generate_Constant_RepeatTime() > 0);

			max_generate_count_	= res.Generate_Constant_GenerateCount() * res.Generate_Constant_RepeatTime();
			ML_ASSERT(max_generate_count_ > 0);

			generate_per_time_	= ml::numeric_cast<float>(res.Generate_Constant_GenerateCount()) / res.Generate_Constant_GenerateTime();
		}
	}

private:
	float	generate_pick_;			// ��␔
	int		generated_count_;		// ����������
	int		max_generate_count_;	// �ő吶�����i�񖳌����s�[�g���j
	int		interval_frame_;		// ��������^�C�~���O��
	float	generate_per_time_;

	// interval���Ԃ����
	template<typename ResourceType>
	static int	MakeIntervalFrame(const ResourceType &res, ml::random &random){

		return res.Generate_Constant_Interval() + random.i32(res.Generate_Constant_IntervalRange());
	}

	template<typename ResourceType>
	static int	GenerateNodeCount(const ResourceType &res, int max_lifespan){
		int count = 0;
		// �g�[�^�����s�[�g���Ԃ��ő�����̕��������Ƃ�
		int generate_span = res.Generate_Constant_GenerateTime() + res.Generate_Constant_Interval();
//		if(res.Generate_Constant_EndlessRepeat() || (max_lifespan < (res.Generate_Constant_GenerateTime() * res.Generate_Constant_RepeatTime()))){
		if(res.Generate_Constant_EndlessRepeat() || (max_lifespan < (generate_span * res.Generate_Constant_RepeatTime()))){
			float exact_count	= (float)(res.Generate_Constant_GenerateCount() * max_lifespan) / (float)generate_span;
			count = (int)( ((int)exact_count < exact_count) ? exact_count + 1 : exact_count);
		}
		else{
			count = res.Generate_Constant_GenerateCount() * res.Generate_Constant_RepeatTime();
		}

		return count;
	}
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_POLICY_CHILDGENERATETIMING_BM3_POLICY_CHILDGENERATETIMING_CONST_H
