#ifndef BM3_SDK_INC_BM3_CORE_TYPE_BM3_TEXTURE_H
#define BM3_SDK_INC_BM3_CORE_TYPE_BM3_TEXTURE_H

#include <ml/utility/ml_assert.h>
#include "../../system/bm3_Config.h"
#include "../bm3_CoreType.h"
#include "../bm3_Const.h"
#if defined(BM3_TARGET_IDE)
#include <vector>
#endif

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

#if defined(BM3_TARGET_IDE)
namespace ide{

/// @brief TextureƒNƒ‰ƒX
class Texture{

public:
	typedef	std::vector<TexturePattern>		TexturePatternArray;
	typedef	TexturePatternArray::iterator	TexturePatternArrayI;

	Texture() :
		is_repeat_pattern_(true){
		;
	}

	TexturePatternArrayI	AddPattern(int frame_count, float left, float top, float width, float height){
		pattern_array_.push_back(TexturePattern(frame_count, left, top, width, height));

		return pattern_array_.end() - 1;
	}

	void	RemovePattern(TexturePatternArrayI i){

		pattern_array_.erase(i);
	}

	void	RemovePattern(int index){

		ML_ASSERT(index >= 0);
		ML_ASSERT(index <  PatternCount());

		TexturePatternArrayI i = pattern_array_.begin();
		std::advance(i, index);

		RemovePattern(i);
	}

	void	Clear(){

		pattern_array_.clear();
	}

	int		PatternIndex(TexturePatternArrayI i){

		return std::distance(pattern_array_.begin(), i);
	}

	int						PatternCount() const{		return int(pattern_array_.size());	}
	TexturePatternArrayI	PatternBegin(){				return pattern_array_.begin();		}
	TexturePatternArrayI	PatternEnd(){				return pattern_array_.end();		}
	const TexturePattern &	Pattern(int index) const{
		static const TexturePattern default_pattern_(1, 0.0f, 0.0f, 1.0f, 1.0f);

		return
			((index >= 0) && (index < PatternCount())) ?
				pattern_array_[index] :
				default_pattern_;
	}

	bool					IsRepeatPattern() const{	return is_repeat_pattern_;			}
	void					SetIsRepeatPattern(bool i){	is_repeat_pattern_ = i;				}

private:
	TexturePatternArray	pattern_array_;
	bool				is_repeat_pattern_;
};

}	// namespace ide
#endif	// #if defined(BM3_TARGET_IDE)

namespace embedded{
class Texture{

public:
	typedef	const TexturePattern *	TexturePatternArrayI;

	int						PatternCount() const{		return pattern_count_;					}
	TexturePatternArrayI	PatternBegin(){				return pattern_begin_;					}
	TexturePatternArrayI	PatternEnd(){				return pattern_begin_ + pattern_count_;	}
	const TexturePattern &	Pattern(int index) const{
		static const TexturePattern default_pattern_(1, 0.0f, 0.0f, 1.0f, 1.0f);

		return
			((index >= 0) && (index < PatternCount())) ?
				pattern_begin_[index] :
				default_pattern_;
	}

	bool					IsRepeatPattern() const{	return is_repeat_pattern_ != 0;		}

	void	Mapping(const void *start){

		pattern_begin_ =
			(pattern_begin_offset_ == ml::ptr_size_ui(-1)) ?
				NULL :
				reinterpret_cast<TexturePatternArrayI>(
					ml::ptr_size_ui(start) + pattern_begin_offset_
				);
	}

	void	SetIsRepeatPattern(bool i){				is_repeat_pattern_ = i ? 1 : 0;		}
	void	SetPatternCount(int c){					pattern_count_ = c;					}
	void	SetPatternOffset(ml::ptr_size_ui o){	pattern_begin_offset_ = o;			}

private:
	ml::si32			is_repeat_pattern_;
	ml::si32			pattern_count_;

	union{
		TexturePatternArrayI	pattern_begin_;
		ml::ptr_size_ui			pattern_begin_offset_;
	};
};

}	// namespace embedded

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_TYPE_BM3_TEXTURE_H
