#ifndef BM3_SDK_INC_BM3_CORE_POLICY_CHILDGENERATETIMING_BM3_POLICY_CHILDGENERATETIMING_Nothing_H
#define BM3_SDK_INC_BM3_CORE_POLICY_CHILDGENERATETIMING_BM3_POLICY_CHILDGENERATETIMING_Nothing_H

#include "../../object/bm3_obj_RootNodeBase.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

/// @brief ChildGenerateTiming_NothingÉNÉâÉX
class ChildGenerateTiming_Nothing{

public:
	enum{
		IsGenerate = false
	};

	ChildGenerateTiming_Nothing(){
		;
	}

	template<typename ResourceType>
	void	Initialize(const ResourceType & /*res*/, ml::random &/*random*/){

		// âΩÇ‡ÇµÇ»Ç¢
		;
	}

	template<typename ResourceType, typename UpdateInfo>
	int		Update(UpdateInfo &/*info*/, const ResourceType &/*res*/, ml::random &/*random*/, const obj::RootNodeBase &/*root*/){

		// âΩÇ‡ÇµÇ»Ç¢
		return 0;
	}

	// Ç∑Ç◊ÇƒÇÃéqÇê∂ê¨ÇµÇΩÇ©
	template<typename ResourceType>
	bool	IsAllChildGenerated(const ResourceType &/*res*/, const obj::RootNodeBase &/*root*/) const{

		return false;
	}

	template<typename ResourceType>
	static int	CalcTotalGenerateNodeCount(const ResourceType & /*res*/){
		return 1;
	}

	template<typename ResourceType>
	static int	CalcTotalGenerateNodeCount(const ResourceType & /*res*/, int /*child_lifespan*/){

		return 1;
	}

	template<typename ResourceType>
	static int	CalcTotalLifeSpan(const ResourceType & /*res*/){
		return -1;
	}

	template<typename ResourceType>
	void	SwitchResource(const ResourceType & /*res*/){
	}


private:
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_POLICY_CHILDGENERATETIMING_BM3_POLICY_CHILDGENERATETIMING_Nothing_H
