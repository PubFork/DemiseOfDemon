#ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_FIELD_GLOBAL_H
#define BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_FIELD_GLOBAL_H

#include "../bm3_CoreType.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace field{
	

#if defined(BM3_TARGET_IDE)
	#define	BM3_FIELD_RESOURCE_TYPE(NODE_TYPE)	res::IdeNode
#endif
#if defined(BM3_TARGET_EMBEDDED)
	#define	BM3_FIELD_RESOURCE_TYPE(NODE_TYPE)	res::NODE_TYPE
#endif
	
#if defined(BM3_TARGET_IDE)
template<
	typename ResourceType,
	typename LifePolicy
>
static void FieldDraw(DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, LifePolicy &life){
	// Field•`‰æ
	// ƒvƒŠƒ~ƒeƒBƒu‚Ì”‚ÉŠÖŒW‚È‚­ˆê“x•`‰æ‚·‚ê‚Î‚¢‚¢
	if(res.ChildArray().Count() > 0){
		const typename ResourceType::ChildArrayType_	&child_array	= res.ChildArray();
		int												child_count		= ml::minimum(res.ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define	BM3_DRAW_FIELD(NAMESPACE, NODE_TYPE)													\
			const BM3_FIELD_RESOURCE_TYPE(NODE_TYPE) &resource =										\
				reinterpret_cast<const BM3_FIELD_RESOURCE_TYPE(NODE_TYPE) &>(child_array.Child(index));	\
			if(resource.IsSelected()){																	\
				NAMESPACE::FieldManipulator::Draw(draw_info, resource, matrix, life);					\
			}

		for(int index = 0;index != child_count;++ index){
			switch(child_array.Child(index).Basic_Meta_NodeType()){
				case NodeTypeConst_ControlNull:				break;
				case NodeTypeConst_ObjectReference:			break;
				case NodeTypeConst_EmitterQuad:				break;
				case NodeTypeConst_EmitterParticle:			break;
				case NodeTypeConst_EmitterSimpleParticle:	break;
				case NodeTypeConst_EmitterMassParticle:		break;
				case NodeTypeConst_EmitterModel:			break;
				case NodeTypeConst_EmitterStripe:			break;
				case NodeTypeConst_FieldGravity:		{	BM3_DRAW_FIELD(field::gravity		,Gravity);		}	break;
				case NodeTypeConst_FieldWind:			{	BM3_DRAW_FIELD(field::wind			,Wind);			}	break;
				case NodeTypeConst_FieldVortex:			{	BM3_DRAW_FIELD(field::vortex		,Vortex);		}	break;
				case NodeTypeConst_FieldRadialpower:	{	BM3_DRAW_FIELD(field::radialpower	,Radialpower);	}	break;
				case NodeTypeConst_FieldDeceleration:	{	BM3_DRAW_FIELD(field::deceleration	,Deceleration);	}	break;
				case NodeTypeConst_FieldCollision:		{	BM3_DRAW_FIELD(field::collision		,Collision);	}	break;
				case NodeTypeConst_FieldGoal:			{	BM3_DRAW_FIELD(field::goal			,Goal);			}	break;
				default:								ML_ASSERT(false);BM3_ASSUME(0);						break;
			}
		}
		#undef BM3_DRAW_FIELD
	}
}
#endif
	
#undef	BM3_FIELD_RESOURCE_TYPE
	
}	// namespace field
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_FIELD_GLOBAL_H
