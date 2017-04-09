#ifndef BM3_SDK_INC_BM3_CORE_TYPE_BM3_CURVE_H
#define BM3_SDK_INC_BM3_CORE_TYPE_BM3_CURVE_H

#include <ml/utility/ml_assert.h>
#include <ml/math/ml_arithmetic.h>
#include <ml/type/ml_vector3d.h>
#include <ml/type/ml_int2type.h>
#include "../../system/bm3_Config.h"
#include "bm3_Keyframe.h"
#include "internal/bm3_InterpolateKeyframe.h"
#include <algorithm>

#if defined(BM3_TARGET_IDE)
#include <algorithm>
#include <map>
#endif

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace internal{

BM3_FORCEINLINE float *			ToArray(float &v){							return &v;		}
BM3_FORCEINLINE float *			ToArray(ml::vector3d &v){					return v.v_;	}
BM3_FORCEINLINE float *			ToArray(ml::color_rgba<float> &v){			return v.v_;	}
BM3_FORCEINLINE const float *	ToArray(const float &v){					return &v;		}
BM3_FORCEINLINE const float *	ToArray(const ml::vector3d &v){				return v.v_;	}
BM3_FORCEINLINE const float *	ToArray(const ml::color_rgba<float> &v){	return v.v_;	}

/// @brief BasicRandomクラス
struct BasicRandom{

	BM3_FORCEINLINE float					Value(float base_value, ml::random &random, float random_range){

		return random.f32_range(base_value, random_range);
	}

	BM3_FORCEINLINE ml::vector3d			Value(const ml::vector3d &base_value, ml::random &random, const ml::vector3d &random_range){

		return ml::vector3d().random_range(random, base_value, random_range);
//			ml::vector3d(
//				random.f32_range(base_value.e_.x_, random_range.e_.x_),
//				random.f32_range(base_value.e_.y_, random_range.e_.y_),
//				random.f32_range(base_value.e_.z_, random_range.e_.z_),
//				1.0f
//			);
	}

	BM3_FORCEINLINE ml::color_rgba<float>	Value(const ml::color_rgba<float> &base_value, ml::random &random, const ml::color_rgba<float> &random_range){

		return ml::color_rgba<float>().random_range_rgb(random, base_value, random_range);
//			ml::color_rgba<float>(
//				random.f32_range(base_value.e_.r_, random_range.e_.r_),
//				random.f32_range(base_value.e_.g_, random_range.e_.g_),
//				random.f32_range(base_value.e_.b_, random_range.e_.b_),
//	//			random.f32_range(base_value.e_.a_, random_range.e_.a_)
//				1.0						// アルファはここでは計算しない
//			);
	}
};

struct ChannelSyncRandom{

	BM3_FORCEINLINE float					Value(float base_value, ml::random &random, float random_range){

		return random.f32_range(base_value, random_range);		// チャンネルは1つなので変わらない
	}

	BM3_FORCEINLINE ml::vector3d			Value(const ml::vector3d &base_value, ml::random &random, const ml::vector3d &random_range){

		float r = random.f32_range(0.0f, 1.0f);

		return
			ml::vector3d(
				base_value.e_.x_ + random_range.e_.x_ * r,
				base_value.e_.y_ + random_range.e_.y_ * r,
				base_value.e_.z_ + random_range.e_.z_ * r,
				1.0f
			);
	}

	BM3_FORCEINLINE ml::color_rgba<float>	Value(const ml::color_rgba<float> &base_value, ml::random &random, const ml::color_rgba<float> &random_range){

		float r = random.f32_range(0.0f, 1.0f);

		return
			ml::color_rgba<float>(
				base_value.e_.r_ +  random_range.e_.r_ * r,
				base_value.e_.g_ +  random_range.e_.g_ * r,
				base_value.e_.b_ +  random_range.e_.b_ * r,
	//			base_value.e_.a_ +  random_range.e_.a_ * r
				1.0						// アルファはここでは計算しない
			);
	}
};

}	// namespace

#if defined(BM3_TARGET_IDE)
namespace ide{

template<typename T>
class Curve{

public:
	struct KeyframeWithFrame : public Keyframe<T>{
		KeyframeWithFrame() :
			frame_(0.0f),
			id_(-1){;}

		float	frame_;				// シリアライズ時のみ使われる
		int		id_;
	};

	typedef std::multimap<float, KeyframeWithFrame>	KeyframeMap;
	typedef typename KeyframeMap::iterator			KeyframeMapI;
	typedef typename KeyframeMap::const_iterator	KeyframeMapCI;
	typedef typename KeyframeMap::value_type		KeyframeMapT;

	typedef typename KeyframeMapI	KeyframeI;
	typedef typename KeyframeMapI	KeyframeCI;

	template<typename RandomType>
	T				RandomValue(float frame, ml::ui32 random_seed, const T &random_range) const{

		if(IsKeyframeEmpty()){
//			return T();

			ml::random	rand;
			{
				ml::random::seed_info	rsi;
				rsi.set1(random_seed);
				rand.set_seed(rsi);
			}
			return RandomType().Value(T(), rand, random_range);

		}else{
			KeyframeMapCI	front	= KeyframeBegin();
			KeyframeMapCI	back	= KeyframeEnd();
			-- back;

			if(frame <= front->first){
				ml::random	rand_left;
				{
					ml::random::seed_info	rsi;
					rsi.set1(ml::ui32(&front->first) + random_seed);
					rand_left.set_seed(rsi);
				}

				return RandomType().Value(front->second.Value(),  rand_left,  random_range);

			}else
			if(frame >= back->first){
				ml::random	rand_right;
				{
					ml::random::seed_info	rsi;
					rsi.set1(ml::ui32(&back->first) + random_seed);
					rand_right.set_seed(rsi);
				}

				return RandomType().Value(back->second.Value(), rand_right, random_range);

			}else{
				KeyframeMapCI right = keyframe_map_.lower_bound(frame);
				KeyframeMapCI left = right;
				-- left;

				float w    = right->first - left->first;				// フレーム間のはば
				float f    = frame - left->first;						// 指定フレーム位置
				float rate = (w == 0.0f) ? 0.0f : ml::division(f, w);	// 指定フレーム割合(0～1)

				ML_ASSERT(rate >= 0.0f);
				ML_ASSERT(rate <= 1.0f);

				T t;
				{
					ml::random	rand_left;
					{
						ml::random::seed_info	rsi;
						rsi.set1(ml::ui32(&left->first) + random_seed);
						rand_left.set_seed(rsi);
					}

					ml::random	rand_right;
					{
						ml::random::seed_info	rsi;
						rsi.set1(ml::ui32(&right->first) + random_seed);
						rand_right.set_seed(rsi);
					}

					internal::InterpolateKeyframe(
						internal::ToArray(t),
						rate,
						internal::ToArray(RandomType().Value(left->second.Value(),  rand_left,  random_range)),
						internal::ToArray(RandomType().Value(right->second.Value(), rand_right, random_range)),
						left->second.ElementInfoArray(),
						right->second.ElementInfoArray(),
						ml::int2type<KeyframeWithFrame::ValueElementCount>()
					);
				}
				return t;
			}
		}
	}

	Curve() :
		timeline_is_expanded_(false),
		timeline_panel_height_(160){
		;
	}

	KeyframeI Add(float frame){

		return
			keyframe_map_.insert(
				KeyframeMapT(
					frame,
					KeyframeWithFrame()
				)
			);
	}

	void	Remove(KeyframeI i){

		keyframe_map_.erase(i);
	}

	void	Remove(int index){

		ML_ASSERT(index >= 0);
		ML_ASSERT(index <  KeyframeCount());

		KeyframeMapI i = keyframe_map_.begin();
		std::advance(i, index);

		Remove(i);
	}

	int	KeyframeIndex(KeyframeI i){

		return std::distance(keyframe_map_.begin(), i);
	}

	int	KeyframeIndex(KeyframeCI i) const{

		return std::distance(keyframe_map_.begin(), i);
	}

	KeyframeMapCI	LowerKeyframe(float frame) const{

		return keyframe_map_.lower_bound(frame);
	}

	KeyframeMapCI	UpperKeyframe(float frame) const{

		return keyframe_map_.upper_bound(frame);
	}

	void	Clear(){

		keyframe_map_.clear();
	}

	int				KeyframeCount() const{		return int(keyframe_map_.size());	}
	KeyframeMapI	KeyframeBegin(){			return keyframe_map_.begin();		}
	KeyframeMapI	KeyframeEnd(){				return keyframe_map_.end();			}
	KeyframeMapCI	KeyframeBegin() const{		return keyframe_map_.begin();		}
	KeyframeMapCI	KeyframeEnd() const{		return keyframe_map_.end();			}
	bool			IsKeyframeEmpty() const{	return keyframe_map_.empty();		}
	const T&		KeyframeFrontValue() const { 
		ML_ASSERT(IsKeyframeEmpty()==false);
		return KeyframeBegin()->second.Value();
	}
	const T&		KeyframeBackValue() const { 
		ML_ASSERT(IsKeyframeEmpty()==false);
		KeyframeMapCI back = KeyframeEnd();
		-- back;
		return back->second.Value(); 
	}

	T				Value(float frame) const{

		if(IsKeyframeEmpty()){
			return T();

		}else{
			KeyframeMapCI	front	= KeyframeBegin();
			KeyframeMapCI	back	= KeyframeEnd();
			-- back;

			if(frame <= front->first){
				return front->second.Value();
			}else
			if(frame >= back->first){
				return back->second.Value();
			}else{
				KeyframeMapCI right = keyframe_map_.lower_bound(frame);
				KeyframeMapCI left = right;
				-- left;

				float w    = right->first - left->first;				// フレーム間のはば
				float f    = frame - left->first;						// 指定フレーム位置
				float rate = (w == 0.0f) ? 0.0f : ml::division(f, w);	// 指定フレーム割合(0～1)

				ML_ASSERT(rate >= 0.0f);
				ML_ASSERT(rate <= 1.0f);

				T t;
				{
					internal::InterpolateKeyframe(
						internal::ToArray(t),
						rate,
						internal::ToArray(left->second.Value()),
						internal::ToArray(right->second.Value()),
						left->second.ElementInfoArray(),
						right->second.ElementInfoArray(),
						ml::int2type<KeyframeWithFrame::ValueElementCount>()
					);
				}
				return t;
			}
		}
	}

	T				Value(float frame, ml::ui32 random_seed, const T &random_range) const{

		return RandomValue<internal::BasicRandom>(frame, random_seed, random_range);
	}

	T				ValueRandomChannelSync(float frame, ml::ui32 random_seed, const T &random_range) const{

		return RandomValue<internal::ChannelSyncRandom>(frame, random_seed, random_range);
	}

	bool	TimelineIsExpanded() const{		return timeline_is_expanded_;	}
	void	SetTimelineIsExpanded(bool i){	timeline_is_expanded_ = i;		}

	int		TimelinePanelHeight() const{	return timeline_panel_height_;	}
	void	SetTimelinePanelHeight(int i){	timeline_panel_height_ = i;		}

private:
	KeyframeMap	keyframe_map_;
	bool		timeline_is_expanded_;
	int			timeline_panel_height_;
};

}	// namespace ide
#endif	// #if defined(BM3_TARGET_IDE)

namespace embedded{

template<typename T>
class Curve{

public:
	struct KeyframeWithFrame : public Keyframe<T>{
		float	frame_;
	};

	struct CompareByFrame{
		bool operator()(const KeyframeWithFrame &l, const KeyframeWithFrame &r){
			return l.frame_ < r.frame_;
		}
	};

	typedef const KeyframeWithFrame *	KeyframeMapCI;

	template<typename RandomType>
	T				RandomValue(float frame, ml::ui32 random_seed, const T &random_range) const{

		if(IsKeyframeEmpty()){

			ml::random	rand;
			{
				ml::random::seed_info	rsi;
				rsi.set1(random_seed);
				rand.set_seed(rsi);
			}
			return RandomType().Value(T(), rand, random_range);

		}else{
			KeyframeMapCI	front	= KeyframeBegin();
			KeyframeMapCI	back	= KeyframeEnd();
			-- back;

			if(frame <= front->frame_){
				ml::random	rand_left;
				{
					ml::random::seed_info	rsi;
					rsi.set1(ml::ui32(front) + random_seed);
					rand_left.set_seed(rsi);
				}

				return RandomType().Value(front->Value(),  rand_left,  random_range);

			}else
			if(frame >= back->frame_){
				ml::random	rand_right;
				{
					ml::random::seed_info	rsi;
					rsi.set1(ml::ui32(back) + random_seed);
					rand_right.set_seed(rsi);
				}

				return RandomType().Value(back->Value(), rand_right, random_range);

			}else{
				KeyframeMapCI right = LowerKeyframe(frame);
				KeyframeMapCI left = right;
				-- left;

				float w    = right->frame_ - left->frame_;				// フレーム間のはば
				float f    = frame - left->frame_;						// 指定フレーム位置
//				float rate = (w == 0.0f) ? 0.0f : ml::division(f, w);	// 指定フレーム割合(0～1)
				float rate = (w == 0.0f) ? 0.0f : f * ml::inverse(w);	// 指定フレーム割合(0～1)

				ML_ASSERT(rate >= 0.0f);
				ML_ASSERT(rate <= 1.0f);

				T t;
				{
					ml::random	rand_left;
					{
						ml::random::seed_info	rsi;
						rsi.set1(ml::ui32(left) + random_seed);
						rand_left.set_seed(rsi);
					}

					ml::random	rand_right;
					{
						ml::random::seed_info	rsi;
						rsi.set1(ml::ui32(right) + random_seed);
						rand_right.set_seed(rsi);
					}

					T	left_value = RandomType().Value(left->Value(),  rand_left,  random_range);
					T	right_value = RandomType().Value(right->Value(), rand_right, random_range);
					internal::InterpolateKeyframe(
						internal::ToArray(t),
						rate,
						internal::ToArray(left_value),
						internal::ToArray(right_value),
						left->ElementInfoArray(),
						right->ElementInfoArray(),
						ml::int2type<KeyframeWithFrame::ValueElementCount>()
					);
				}
				return t;
			}
		}
	}

	int	KeyframeIndex(KeyframeMapCI i) const{

		return i - KeyframeBegin();
	}

	KeyframeMapCI	LowerKeyframe(float frame) const{

		KeyframeWithFrame	ekf;
		ekf.frame_ = frame;

		return std::lower_bound(KeyframeBegin(), KeyframeEnd(), ekf, CompareByFrame());
	}

	KeyframeMapCI	UpperKeyframe(float frame) const{

		KeyframeWithFrame	ekf;
		ekf.frame_ = frame;

		return std::upper_bound(KeyframeBegin(), KeyframeEnd(), ekf, CompareByFrame());
	}

	int				KeyframeCount() const{	ML_ASSERT(keyframe_count_ >= 0);	return keyframe_count_;						}
	KeyframeMapCI	KeyframeBegin() const{	ML_PTR_ASSERT(key_frame_begin_);	return key_frame_begin_;					}
	KeyframeMapCI	KeyframeEnd() const{										return KeyframeBegin() + KeyframeCount();	}
	bool			IsKeyframeEmpty() const{									return KeyframeCount() == 0;				}
	const T&		KeyframeFrontValue() const { 
		ML_ASSERT(IsKeyframeEmpty()==false);
		return KeyframeBegin()->Value();
	}
	const T&		KeyframeBackValue() const { 
		ML_ASSERT(IsKeyframeEmpty()==false);
		KeyframeMapCI back = KeyframeEnd();
		-- back;
		return back->Value(); 
	}

	T				Value(float frame) const{

		if(IsKeyframeEmpty()){
			return T();

		}else{
			KeyframeMapCI	front	= KeyframeBegin();
			KeyframeMapCI	back	= KeyframeEnd();
			-- back;

			if(frame <= front->frame_){
				return front->Value();
			}else
			if(frame >= back->frame_){
				return back->Value();
			}else{
				KeyframeMapCI right = LowerKeyframe(frame);
				KeyframeMapCI left = right;
				-- left;

				float w    = right->frame_ - left->frame_;				// フレーム間のはば
				float f    = frame - left->frame_;						// 指定フレーム位置
				float rate = (w == 0.0f) ? 0.0f : ml::division(f, w);	// 指定フレーム割合(0～1)

				ML_ASSERT(rate >= 0.0f);
				ML_ASSERT(rate <= 1.0f);

				T t;
				{
					internal::InterpolateKeyframe(
						internal::ToArray(t),
						rate,
						internal::ToArray(left->Value()),
						internal::ToArray(right->Value()),
						left->ElementInfoArray(),
						right->ElementInfoArray(),
						ml::int2type<KeyframeWithFrame::ValueElementCount>()
					);
				}
				return t;
			}
		}
	}

	T				Value(float frame, ml::ui32 random_seed, const T &random_range) const{

		return RandomValue<internal::BasicRandom>(frame, random_seed, random_range);
	}

	T				ValueRandomChannelSync(float frame, ml::ui32 random_seed, const T &random_range) const{

		return RandomValue<internal::ChannelSyncRandom>(frame, random_seed, random_range);
	}

	void	Mapping(const void *start){

		key_frame_begin_ =
			(key_frame_begin_offset_ == ml::ptr_size_ui(-1)) ?
				NULL :
				reinterpret_cast<KeyframeMapCI>(
					ml::ptr_size_ui(start) + key_frame_begin_offset_
				);
	}

	void	SetKeyframeCount(int c){				keyframe_count_ = c;			}
	void	SetKeyframeOffset(ml::ptr_size_ui o){	key_frame_begin_offset_ = o;	}

private:
	ml::si32			keyframe_count_;

	union{
		KeyframeMapCI		key_frame_begin_;
		ml::ptr_size_ui		key_frame_begin_offset_;
	};

	// 以下キーフレーム
};

}	// namespace embedded

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_TYPE_BM3_CURVE
