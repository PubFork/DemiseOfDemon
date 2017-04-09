#ifndef BM3_SDK_INC_BM3_CORE_POLICY_CHILDGENERATETIMING_BM3_POLICY_CHILDGENERATETIMING_GENERATE_H
#define BM3_SDK_INC_BM3_CORE_POLICY_CHILDGENERATETIMING_BM3_POLICY_CHILDGENERATETIMING_GENERATE_H

#include "bm3_policy_ChildGenerateTiming_Const.h"
#include "bm3_policy_ChildGenerateTiming_Timing.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace obj{
class RootNodeBase;
}

namespace policy{

/// @brief ChildGenerateTiming_GenerateÉNÉâÉX
template<typename LifePolicy>
class ChildGenerateTiming_Generate{

public:
	enum{
		IsGenerate = true
	};

	ChildGenerateTiming_Generate(){
		;	
	}

	template<typename ResourceType>
	void	Initialize(const ResourceType &res, ml::random &random){

		life_.Initialize(res, random);
		const_.Initialize(res);
		timing_.Initialize(res);
	}

	template<typename ResourceType, typename UpdateInfo>
	int		Update(UpdateInfo &info, const ResourceType &res, ml::random &random, const obj::RootNodeBase &root){

		ML_STATIC_ASSERT(GenerateTypeConst_Constant == 0);
		ML_STATIC_ASSERT(GenerateTypeConst_Timing   == 1);
		ML_STATIC_ASSERT(GenerateTypeConst_MAX      == 2);

		life_.Update(info);
		if(life_.IsStarted()){
			return
				(res.Generate_Basic_GenerateType() == GenerateTypeConst_Constant) ?
					const_.Update(info, res, random, life_, root) :
					timing_.Update(info, res, random, life_, root);
		}
		else{
			return 0;
		}
	}

	// Ç∑Ç◊ÇƒÇÃéqÇê∂ê¨ÇµÇΩÇ©
	template<typename ResourceType>
	bool	IsAllChildGenerated(const ResourceType &res, const obj::RootNodeBase &root) const{

		ML_STATIC_ASSERT(GenerateTypeConst_Constant == 0);
		ML_STATIC_ASSERT(GenerateTypeConst_Timing   == 1);
		ML_STATIC_ASSERT(GenerateTypeConst_MAX      == 2);

		return
			(res.Generate_Basic_GenerateType() == GenerateTypeConst_Constant) ?
				const_.IsAllChildGenerated(res, root) :
				timing_.IsAllChildGenerated(res, root);
	}

	template<typename ResourceType>
	static int	CalcTotalGenerateNodeCount(const ResourceType &res){

		return
			(res.Generate_Basic_GenerateType() == GenerateTypeConst_Constant) ?
				ChildGenerateTiming_Const::CalcTotalGenerateNodeCount(res) :
				ChildGenerateTiming_Timing::CalcTotalGenerateNodeCount(res);
	}

	template<typename ResourceType>
	static int	CalcTotalGenerateNodeCount(const ResourceType &res, int child_lifespan){

		return
			(res.Generate_Basic_GenerateType() == GenerateTypeConst_Constant) ?
				ChildGenerateTiming_Const::CalcTotalGenerateNodeCount(res, child_lifespan) :
				ChildGenerateTiming_Timing::CalcTotalGenerateNodeCount(res, child_lifespan);
	}

	template<typename ResourceType>
	static int	CalcTotalLifeSpan(const ResourceType &res){
		return
			(res.Generate_Basic_GenerateType() == GenerateTypeConst_Constant) ?
				ChildGenerateTiming_Const::CalcTotalLifeSpan(res) :
				ChildGenerateTiming_Timing::CalcTotalLifeSpan(res);
	}

	template<typename ResourceType>
	void	SwitchResource(const ResourceType &res){

		const_.SwitchResource(res);
		timing_.SwitchResource(res);
	}

private:
	ChildGenerateTiming_Const	const_;
	ChildGenerateTiming_Timing	timing_;
	LifePolicy					life_;
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_POLICY_CHILDGENERATETIMING_BM3_POLICY_CHILDGENERATETIMING_GENERATE_H
