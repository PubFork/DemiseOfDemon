#ifndef BM3_SDK_INC_BM3_CORE_POLICY_CHILDGENERATETIMING_BM3_POLICY_CHILDGENERATETIMING_ONCE_H
#define BM3_SDK_INC_BM3_CORE_POLICY_CHILDGENERATETIMING_BM3_POLICY_CHILDGENERATETIMING_ONCE_H

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace obj{
class RootNodeBase;
}

namespace policy{

/// @brief ChildGenerateTiming_Onceクラス。最初の一度だけ生成する。あとは生成しない
class ChildGenerateTiming_Once{

public:
	enum{
		IsGenerate = true
	};

	ChildGenerateTiming_Once(){
		;	
	}

	template<typename ResourceType>
	void	Initialize(const ResourceType & /*res*/, ml::random & /*random*/){
		child_count	= 1;
	}

	template<typename ResourceType, typename UpdateInfo>
	int		Update(UpdateInfo & /*info*/, const ResourceType & /*res*/, ml::random & /*random*/, const obj::RootNodeBase &root){
		
		if(root.IsRequestRetire() == false){
			if(child_count){
				child_count = 0;
				return 1;
			}
		}
		return 0;
	}

	template<typename ResourceType>
	bool	IsAllChildGenerated(const ResourceType & /*res*/, const obj::RootNodeBase &root) const{
		if(root.IsRequestRetire()){
			return true;
		}
		else{
			return (child_count==0);
		}
	}

	template<typename ResourceType>
	static int	CalcTotalGenerateNodeCount(const ResourceType & /*res*/){
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
	int	child_count;
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_POLICY_CHILDGENERATETIMING_BM3_POLICY_CHILDGENERATETIMING_ONCE_H
