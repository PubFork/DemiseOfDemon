#ifdef BM3_PM_SIMPLE_SCALE
#define BM3_PM_CASE(S,T)	(((S) << (2*1)) | ((T) << (2*0)))
switch(BM3_PM_CASE(res.Scale_Basic_ScaleType(), res.Translate_Basic_TranslateType())){
	case BM3_PM_CASE(ScaleTypeConst_Constant,TranslateTypeConst_Constant):{
		;
		;
		;
		;
		break;
	}
	case BM3_PM_CASE(ScaleTypeConst_Constant,TranslateTypeConst_NewtonDynamics):{
		;
		p.translate_.speed_.mac(p.translate_.acceleration_,update_info.GetUpdateFrame());
		;
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(ScaleTypeConst_Constant,TranslateTypeConst_CurveValue):{
		;
		p.translate_.value_ = res.Translate_Value_Curve().Value(p.LifeRate(), random_seed, res.Translate_Value_Range());
		;
		p.translate_.value_.mac(p.generate_shape_.position_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(ScaleTypeConst_Constant,TranslateTypeConst_CurveSpeed):{
		;
			if(res.GenerateShape_Basic_Normal()){
		p.translate_.speed_.mul(p.generate_shape_.direction_ ,res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range()));
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
	else{
		p.translate_.speed_ = res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range());
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
		;
		;
		break;
	}
	case BM3_PM_CASE(ScaleTypeConst_NewtonDynamics,TranslateTypeConst_Constant):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ += p.scale_.acceleration_*update_info.GetUpdateFrame();
#else
	p.scale_.speed_.mac(p.scale_.acceleration_,update_info.GetUpdateFrame());
#endif
		;
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
	p.scale_.value_.maximum0();
#endif
		;
		break;
	}
	case BM3_PM_CASE(ScaleTypeConst_NewtonDynamics,TranslateTypeConst_NewtonDynamics):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ += p.scale_.acceleration_*update_info.GetUpdateFrame();
#else
	p.scale_.speed_.mac(p.scale_.acceleration_,update_info.GetUpdateFrame());
#endif
		p.translate_.speed_.mac(p.translate_.acceleration_,update_info.GetUpdateFrame());
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
	p.scale_.value_.maximum0();
#endif
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(ScaleTypeConst_NewtonDynamics,TranslateTypeConst_CurveValue):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ += p.scale_.acceleration_*update_info.GetUpdateFrame();
#else
	p.scale_.speed_.mac(p.scale_.acceleration_,update_info.GetUpdateFrame());
#endif
		p.translate_.value_ = res.Translate_Value_Curve().Value(p.LifeRate(), random_seed, res.Translate_Value_Range());
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
	p.scale_.value_.maximum0();
#endif
		p.translate_.value_.mac(p.generate_shape_.position_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(ScaleTypeConst_NewtonDynamics,TranslateTypeConst_CurveSpeed):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ += p.scale_.acceleration_*update_info.GetUpdateFrame();
#else
	p.scale_.speed_.mac(p.scale_.acceleration_,update_info.GetUpdateFrame());
#endif
			if(res.GenerateShape_Basic_Normal()){
		p.translate_.speed_.mul(p.generate_shape_.direction_ ,res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range()));
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
	else{
		p.translate_.speed_ = res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range());
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
	p.scale_.value_.maximum0();
#endif
		;
		break;
	}
	case BM3_PM_CASE(ScaleTypeConst_CurveValue,TranslateTypeConst_Constant):{
		#ifdef BM3_PM_SIMPLE_SCALE
		p.scale_.value_ = res.Scale_Value_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Value_RangeX());
		p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);

#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());

		if(res.Scale_Basic_Square()){
//			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f)*update_info.GetUpdateFrame();
			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f);
			p.scale_.value_.e_.y_ = p.scale_.value_.e_.x_;
			p.scale_.value_.e_.z_ = p.scale_.value_.e_.x_;
		}else{
			p.scale_.value_.maximum0();
		}
	#else
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());
		p.scale_.value_.maximum0();
	#endif
#endif
		;
		;
		;
		break;
	}
	case BM3_PM_CASE(ScaleTypeConst_CurveValue,TranslateTypeConst_NewtonDynamics):{
		#ifdef BM3_PM_SIMPLE_SCALE
		p.scale_.value_ = res.Scale_Value_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Value_RangeX());
		p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);

#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());

		if(res.Scale_Basic_Square()){
//			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f)*update_info.GetUpdateFrame();
			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f);
			p.scale_.value_.e_.y_ = p.scale_.value_.e_.x_;
			p.scale_.value_.e_.z_ = p.scale_.value_.e_.x_;
		}else{
			p.scale_.value_.maximum0();
		}
	#else
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());
		p.scale_.value_.maximum0();
	#endif
#endif
		p.translate_.speed_.mac(p.translate_.acceleration_,update_info.GetUpdateFrame());
		;
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(ScaleTypeConst_CurveValue,TranslateTypeConst_CurveValue):{
		#ifdef BM3_PM_SIMPLE_SCALE
		p.scale_.value_ = res.Scale_Value_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Value_RangeX());
		p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);

#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());

		if(res.Scale_Basic_Square()){
//			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f)*update_info.GetUpdateFrame();
			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f);
			p.scale_.value_.e_.y_ = p.scale_.value_.e_.x_;
			p.scale_.value_.e_.z_ = p.scale_.value_.e_.x_;
		}else{
			p.scale_.value_.maximum0();
		}
	#else
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());
		p.scale_.value_.maximum0();
	#endif
#endif
		p.translate_.value_ = res.Translate_Value_Curve().Value(p.LifeRate(), random_seed, res.Translate_Value_Range());
		;
		p.translate_.value_.mac(p.generate_shape_.position_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(ScaleTypeConst_CurveValue,TranslateTypeConst_CurveSpeed):{
		#ifdef BM3_PM_SIMPLE_SCALE
		p.scale_.value_ = res.Scale_Value_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Value_RangeX());
		p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);

#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());

		if(res.Scale_Basic_Square()){
//			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f)*update_info.GetUpdateFrame();
			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f);
			p.scale_.value_.e_.y_ = p.scale_.value_.e_.x_;
			p.scale_.value_.e_.z_ = p.scale_.value_.e_.x_;
		}else{
			p.scale_.value_.maximum0();
		}
	#else
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());
		p.scale_.value_.maximum0();
	#endif
#endif
			if(res.GenerateShape_Basic_Normal()){
		p.translate_.speed_.mul(p.generate_shape_.direction_ ,res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range()));
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
	else{
		p.translate_.speed_ = res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range());
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
		;
		;
		break;
	}
	case BM3_PM_CASE(ScaleTypeConst_CurveSpeed,TranslateTypeConst_Constant):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ = res.Scale_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Speed_RangeX());
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());

		if(res.Scale_Basic_Square()){
			p.scale_.speed_.e_.x_ *= update_info.GetUpdateFrame();
			p.scale_.speed_.e_.y_ = p.scale_.speed_.e_.x_;
			p.scale_.speed_.e_.z_ = p.scale_.speed_.e_.x_;
		}
	#else
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());
	#endif
#endif
		;
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#else
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#endif
#endif
		;
		break;
	}
	case BM3_PM_CASE(ScaleTypeConst_CurveSpeed,TranslateTypeConst_NewtonDynamics):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ = res.Scale_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Speed_RangeX());
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());

		if(res.Scale_Basic_Square()){
			p.scale_.speed_.e_.x_ *= update_info.GetUpdateFrame();
			p.scale_.speed_.e_.y_ = p.scale_.speed_.e_.x_;
			p.scale_.speed_.e_.z_ = p.scale_.speed_.e_.x_;
		}
	#else
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());
	#endif
#endif
		p.translate_.speed_.mac(p.translate_.acceleration_,update_info.GetUpdateFrame());
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#else
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#endif
#endif
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(ScaleTypeConst_CurveSpeed,TranslateTypeConst_CurveValue):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ = res.Scale_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Speed_RangeX());
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());

		if(res.Scale_Basic_Square()){
			p.scale_.speed_.e_.x_ *= update_info.GetUpdateFrame();
			p.scale_.speed_.e_.y_ = p.scale_.speed_.e_.x_;
			p.scale_.speed_.e_.z_ = p.scale_.speed_.e_.x_;
		}
	#else
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());
	#endif
#endif
		p.translate_.value_ = res.Translate_Value_Curve().Value(p.LifeRate(), random_seed, res.Translate_Value_Range());
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#else
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#endif
#endif
		p.translate_.value_.mac(p.generate_shape_.position_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(ScaleTypeConst_CurveSpeed,TranslateTypeConst_CurveSpeed):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ = res.Scale_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Speed_RangeX());
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());

		if(res.Scale_Basic_Square()){
			p.scale_.speed_.e_.x_ *= update_info.GetUpdateFrame();
			p.scale_.speed_.e_.y_ = p.scale_.speed_.e_.x_;
			p.scale_.speed_.e_.z_ = p.scale_.speed_.e_.x_;
		}
	#else
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());
	#endif
#endif
			if(res.GenerateShape_Basic_Normal()){
		p.translate_.speed_.mul(p.generate_shape_.direction_ ,res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range()));
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
	else{
		p.translate_.speed_ = res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range());
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#else
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#endif
#endif
		;
		break;
	}
}
#undef BM3_PM_CASE
#else // #ifdef BM3_PM_SIMPLE_SCALE
#define BM3_PM_CASE(R,S,T)	(((R) << (2*2)) | ((S) << (2*1)) | ((T) << (2*0)))
switch(BM3_PM_CASE(res.Rotate_Basic_RotateType(), res.Scale_Basic_ScaleType(), res.Translate_Basic_TranslateType())){
	case BM3_PM_CASE(RotateTypeConst_Constant,ScaleTypeConst_Constant,TranslateTypeConst_Constant):{
		;
		;
		;
		;
		;
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_Constant,ScaleTypeConst_Constant,TranslateTypeConst_NewtonDynamics):{
		;
		;
		p.translate_.speed_.mac(p.translate_.acceleration_,update_info.GetUpdateFrame());
		;
		;
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_Constant,ScaleTypeConst_Constant,TranslateTypeConst_CurveValue):{
		;
		;
		p.translate_.value_ = res.Translate_Value_Curve().Value(p.LifeRate(), random_seed, res.Translate_Value_Range());
		;
		;
		p.translate_.value_.mac(p.generate_shape_.position_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_Constant,ScaleTypeConst_Constant,TranslateTypeConst_CurveSpeed):{
		;
		;
			if(res.GenerateShape_Basic_Normal()){
		p.translate_.speed_.mul(p.generate_shape_.direction_ ,res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range()));
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
	else{
		p.translate_.speed_ = res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range());
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
		;
		;
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_Constant,ScaleTypeConst_NewtonDynamics,TranslateTypeConst_Constant):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ += p.scale_.acceleration_*update_info.GetUpdateFrame();
#else
	p.scale_.speed_.mac(p.scale_.acceleration_,update_info.GetUpdateFrame());
#endif
		;
		;
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
	p.scale_.value_.maximum0();
#endif
		;
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_Constant,ScaleTypeConst_NewtonDynamics,TranslateTypeConst_NewtonDynamics):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ += p.scale_.acceleration_*update_info.GetUpdateFrame();
#else
	p.scale_.speed_.mac(p.scale_.acceleration_,update_info.GetUpdateFrame());
#endif
		;
		p.translate_.speed_.mac(p.translate_.acceleration_,update_info.GetUpdateFrame());
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
	p.scale_.value_.maximum0();
#endif
		;
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_Constant,ScaleTypeConst_NewtonDynamics,TranslateTypeConst_CurveValue):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ += p.scale_.acceleration_*update_info.GetUpdateFrame();
#else
	p.scale_.speed_.mac(p.scale_.acceleration_,update_info.GetUpdateFrame());
#endif
		;
		p.translate_.value_ = res.Translate_Value_Curve().Value(p.LifeRate(), random_seed, res.Translate_Value_Range());
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
	p.scale_.value_.maximum0();
#endif
		;
		p.translate_.value_.mac(p.generate_shape_.position_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_Constant,ScaleTypeConst_NewtonDynamics,TranslateTypeConst_CurveSpeed):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ += p.scale_.acceleration_*update_info.GetUpdateFrame();
#else
	p.scale_.speed_.mac(p.scale_.acceleration_,update_info.GetUpdateFrame());
#endif
		;
			if(res.GenerateShape_Basic_Normal()){
		p.translate_.speed_.mul(p.generate_shape_.direction_ ,res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range()));
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
	else{
		p.translate_.speed_ = res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range());
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
	p.scale_.value_.maximum0();
#endif
		;
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_Constant,ScaleTypeConst_CurveValue,TranslateTypeConst_Constant):{
		#ifdef BM3_PM_SIMPLE_SCALE
		p.scale_.value_ = res.Scale_Value_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Value_RangeX());
		p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);

#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());

		if(res.Scale_Basic_Square()){
//			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f)*update_info.GetUpdateFrame();
			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f);
			p.scale_.value_.e_.y_ = p.scale_.value_.e_.x_;
			p.scale_.value_.e_.z_ = p.scale_.value_.e_.x_;
		}else{
			p.scale_.value_.maximum0();
		}
	#else
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());
		p.scale_.value_.maximum0();
	#endif
#endif
		;
		;
		;
		;
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_Constant,ScaleTypeConst_CurveValue,TranslateTypeConst_NewtonDynamics):{
		#ifdef BM3_PM_SIMPLE_SCALE
		p.scale_.value_ = res.Scale_Value_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Value_RangeX());
		p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);

#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());

		if(res.Scale_Basic_Square()){
//			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f)*update_info.GetUpdateFrame();
			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f);
			p.scale_.value_.e_.y_ = p.scale_.value_.e_.x_;
			p.scale_.value_.e_.z_ = p.scale_.value_.e_.x_;
		}else{
			p.scale_.value_.maximum0();
		}
	#else
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());
		p.scale_.value_.maximum0();
	#endif
#endif
		;
		p.translate_.speed_.mac(p.translate_.acceleration_,update_info.GetUpdateFrame());
		;
		;
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_Constant,ScaleTypeConst_CurveValue,TranslateTypeConst_CurveValue):{
		#ifdef BM3_PM_SIMPLE_SCALE
		p.scale_.value_ = res.Scale_Value_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Value_RangeX());
		p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);

#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());

		if(res.Scale_Basic_Square()){
//			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f)*update_info.GetUpdateFrame();
			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f);
			p.scale_.value_.e_.y_ = p.scale_.value_.e_.x_;
			p.scale_.value_.e_.z_ = p.scale_.value_.e_.x_;
		}else{
			p.scale_.value_.maximum0();
		}
	#else
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());
		p.scale_.value_.maximum0();
	#endif
#endif
		;
		p.translate_.value_ = res.Translate_Value_Curve().Value(p.LifeRate(), random_seed, res.Translate_Value_Range());
		;
		;
		p.translate_.value_.mac(p.generate_shape_.position_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_Constant,ScaleTypeConst_CurveValue,TranslateTypeConst_CurveSpeed):{
		#ifdef BM3_PM_SIMPLE_SCALE
		p.scale_.value_ = res.Scale_Value_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Value_RangeX());
		p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);

#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());

		if(res.Scale_Basic_Square()){
//			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f)*update_info.GetUpdateFrame();
			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f);
			p.scale_.value_.e_.y_ = p.scale_.value_.e_.x_;
			p.scale_.value_.e_.z_ = p.scale_.value_.e_.x_;
		}else{
			p.scale_.value_.maximum0();
		}
	#else
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());
		p.scale_.value_.maximum0();
	#endif
#endif
		;
			if(res.GenerateShape_Basic_Normal()){
		p.translate_.speed_.mul(p.generate_shape_.direction_ ,res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range()));
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
	else{
		p.translate_.speed_ = res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range());
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
		;
		;
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_Constant,ScaleTypeConst_CurveSpeed,TranslateTypeConst_Constant):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ = res.Scale_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Speed_RangeX());
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());

		if(res.Scale_Basic_Square()){
			p.scale_.speed_.e_.x_ *= update_info.GetUpdateFrame();
			p.scale_.speed_.e_.y_ = p.scale_.speed_.e_.x_;
			p.scale_.speed_.e_.z_ = p.scale_.speed_.e_.x_;
		}
	#else
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());
	#endif
#endif
		;
		;
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#else
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#endif
#endif
		;
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_Constant,ScaleTypeConst_CurveSpeed,TranslateTypeConst_NewtonDynamics):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ = res.Scale_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Speed_RangeX());
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());

		if(res.Scale_Basic_Square()){
			p.scale_.speed_.e_.x_ *= update_info.GetUpdateFrame();
			p.scale_.speed_.e_.y_ = p.scale_.speed_.e_.x_;
			p.scale_.speed_.e_.z_ = p.scale_.speed_.e_.x_;
		}
	#else
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());
	#endif
#endif
		;
		p.translate_.speed_.mac(p.translate_.acceleration_,update_info.GetUpdateFrame());
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#else
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#endif
#endif
		;
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_Constant,ScaleTypeConst_CurveSpeed,TranslateTypeConst_CurveValue):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ = res.Scale_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Speed_RangeX());
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());

		if(res.Scale_Basic_Square()){
			p.scale_.speed_.e_.x_ *= update_info.GetUpdateFrame();
			p.scale_.speed_.e_.y_ = p.scale_.speed_.e_.x_;
			p.scale_.speed_.e_.z_ = p.scale_.speed_.e_.x_;
		}
	#else
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());
	#endif
#endif
		;
		p.translate_.value_ = res.Translate_Value_Curve().Value(p.LifeRate(), random_seed, res.Translate_Value_Range());
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#else
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#endif
#endif
		;
		p.translate_.value_.mac(p.generate_shape_.position_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_Constant,ScaleTypeConst_CurveSpeed,TranslateTypeConst_CurveSpeed):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ = res.Scale_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Speed_RangeX());
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());

		if(res.Scale_Basic_Square()){
			p.scale_.speed_.e_.x_ *= update_info.GetUpdateFrame();
			p.scale_.speed_.e_.y_ = p.scale_.speed_.e_.x_;
			p.scale_.speed_.e_.z_ = p.scale_.speed_.e_.x_;
		}
	#else
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());
	#endif
#endif
		;
			if(res.GenerateShape_Basic_Normal()){
		p.translate_.speed_.mul(p.generate_shape_.direction_ ,res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range()));
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
	else{
		p.translate_.speed_ = res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range());
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#else
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#endif
#endif
		;
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_NewtonDynamics,ScaleTypeConst_Constant,TranslateTypeConst_Constant):{
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ += p.rotate_.acceleration_*update_info.GetUpdateFrame();
#else
	p.rotate_.speed_.mac(p.rotate_.acceleration_,update_info.GetUpdateFrame());
#endif
		;
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_NewtonDynamics,ScaleTypeConst_Constant,TranslateTypeConst_NewtonDynamics):{
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ += p.rotate_.acceleration_*update_info.GetUpdateFrame();
#else
	p.rotate_.speed_.mac(p.rotate_.acceleration_,update_info.GetUpdateFrame());
#endif
		p.translate_.speed_.mac(p.translate_.acceleration_,update_info.GetUpdateFrame());
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_NewtonDynamics,ScaleTypeConst_Constant,TranslateTypeConst_CurveValue):{
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ += p.rotate_.acceleration_*update_info.GetUpdateFrame();
#else
	p.rotate_.speed_.mac(p.rotate_.acceleration_,update_info.GetUpdateFrame());
#endif
		p.translate_.value_ = res.Translate_Value_Curve().Value(p.LifeRate(), random_seed, res.Translate_Value_Range());
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		p.translate_.value_.mac(p.generate_shape_.position_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_NewtonDynamics,ScaleTypeConst_Constant,TranslateTypeConst_CurveSpeed):{
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ += p.rotate_.acceleration_*update_info.GetUpdateFrame();
#else
	p.rotate_.speed_.mac(p.rotate_.acceleration_,update_info.GetUpdateFrame());
#endif
			if(res.GenerateShape_Basic_Normal()){
		p.translate_.speed_.mul(p.generate_shape_.direction_ ,res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range()));
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
	else{
		p.translate_.speed_ = res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range());
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_NewtonDynamics,ScaleTypeConst_NewtonDynamics,TranslateTypeConst_Constant):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ += p.scale_.acceleration_*update_info.GetUpdateFrame();
#else
	p.scale_.speed_.mac(p.scale_.acceleration_,update_info.GetUpdateFrame());
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ += p.rotate_.acceleration_*update_info.GetUpdateFrame();
#else
	p.rotate_.speed_.mac(p.rotate_.acceleration_,update_info.GetUpdateFrame());
#endif
		;
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
	p.scale_.value_.maximum0();
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_NewtonDynamics,ScaleTypeConst_NewtonDynamics,TranslateTypeConst_NewtonDynamics):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ += p.scale_.acceleration_*update_info.GetUpdateFrame();
#else
	p.scale_.speed_.mac(p.scale_.acceleration_,update_info.GetUpdateFrame());
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ += p.rotate_.acceleration_*update_info.GetUpdateFrame();
#else
	p.rotate_.speed_.mac(p.rotate_.acceleration_,update_info.GetUpdateFrame());
#endif
		p.translate_.speed_.mac(p.translate_.acceleration_,update_info.GetUpdateFrame());
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
	p.scale_.value_.maximum0();
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_NewtonDynamics,ScaleTypeConst_NewtonDynamics,TranslateTypeConst_CurveValue):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ += p.scale_.acceleration_*update_info.GetUpdateFrame();
#else
	p.scale_.speed_.mac(p.scale_.acceleration_,update_info.GetUpdateFrame());
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ += p.rotate_.acceleration_*update_info.GetUpdateFrame();
#else
	p.rotate_.speed_.mac(p.rotate_.acceleration_,update_info.GetUpdateFrame());
#endif
		p.translate_.value_ = res.Translate_Value_Curve().Value(p.LifeRate(), random_seed, res.Translate_Value_Range());
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
	p.scale_.value_.maximum0();
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		p.translate_.value_.mac(p.generate_shape_.position_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_NewtonDynamics,ScaleTypeConst_NewtonDynamics,TranslateTypeConst_CurveSpeed):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ += p.scale_.acceleration_*update_info.GetUpdateFrame();
#else
	p.scale_.speed_.mac(p.scale_.acceleration_,update_info.GetUpdateFrame());
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ += p.rotate_.acceleration_*update_info.GetUpdateFrame();
#else
	p.rotate_.speed_.mac(p.rotate_.acceleration_,update_info.GetUpdateFrame());
#endif
			if(res.GenerateShape_Basic_Normal()){
		p.translate_.speed_.mul(p.generate_shape_.direction_ ,res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range()));
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
	else{
		p.translate_.speed_ = res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range());
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
	p.scale_.value_.maximum0();
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_NewtonDynamics,ScaleTypeConst_CurveValue,TranslateTypeConst_Constant):{
		#ifdef BM3_PM_SIMPLE_SCALE
		p.scale_.value_ = res.Scale_Value_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Value_RangeX());
		p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);

#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());

		if(res.Scale_Basic_Square()){
//			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f)*update_info.GetUpdateFrame();
			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f);
			p.scale_.value_.e_.y_ = p.scale_.value_.e_.x_;
			p.scale_.value_.e_.z_ = p.scale_.value_.e_.x_;
		}else{
			p.scale_.value_.maximum0();
		}
	#else
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());
		p.scale_.value_.maximum0();
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ += p.rotate_.acceleration_*update_info.GetUpdateFrame();
#else
	p.rotate_.speed_.mac(p.rotate_.acceleration_,update_info.GetUpdateFrame());
#endif
		;
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_NewtonDynamics,ScaleTypeConst_CurveValue,TranslateTypeConst_NewtonDynamics):{
		#ifdef BM3_PM_SIMPLE_SCALE
		p.scale_.value_ = res.Scale_Value_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Value_RangeX());
		p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);

#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());

		if(res.Scale_Basic_Square()){
//			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f)*update_info.GetUpdateFrame();
			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f);
			p.scale_.value_.e_.y_ = p.scale_.value_.e_.x_;
			p.scale_.value_.e_.z_ = p.scale_.value_.e_.x_;
		}else{
			p.scale_.value_.maximum0();
		}
	#else
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());
		p.scale_.value_.maximum0();
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ += p.rotate_.acceleration_*update_info.GetUpdateFrame();
#else
	p.rotate_.speed_.mac(p.rotate_.acceleration_,update_info.GetUpdateFrame());
#endif
		p.translate_.speed_.mac(p.translate_.acceleration_,update_info.GetUpdateFrame());
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_NewtonDynamics,ScaleTypeConst_CurveValue,TranslateTypeConst_CurveValue):{
		#ifdef BM3_PM_SIMPLE_SCALE
		p.scale_.value_ = res.Scale_Value_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Value_RangeX());
		p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);

#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());

		if(res.Scale_Basic_Square()){
//			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f)*update_info.GetUpdateFrame();
			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f);
			p.scale_.value_.e_.y_ = p.scale_.value_.e_.x_;
			p.scale_.value_.e_.z_ = p.scale_.value_.e_.x_;
		}else{
			p.scale_.value_.maximum0();
		}
	#else
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());
		p.scale_.value_.maximum0();
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ += p.rotate_.acceleration_*update_info.GetUpdateFrame();
#else
	p.rotate_.speed_.mac(p.rotate_.acceleration_,update_info.GetUpdateFrame());
#endif
		p.translate_.value_ = res.Translate_Value_Curve().Value(p.LifeRate(), random_seed, res.Translate_Value_Range());
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		p.translate_.value_.mac(p.generate_shape_.position_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_NewtonDynamics,ScaleTypeConst_CurveValue,TranslateTypeConst_CurveSpeed):{
		#ifdef BM3_PM_SIMPLE_SCALE
		p.scale_.value_ = res.Scale_Value_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Value_RangeX());
		p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);

#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());

		if(res.Scale_Basic_Square()){
//			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f)*update_info.GetUpdateFrame();
			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f);
			p.scale_.value_.e_.y_ = p.scale_.value_.e_.x_;
			p.scale_.value_.e_.z_ = p.scale_.value_.e_.x_;
		}else{
			p.scale_.value_.maximum0();
		}
	#else
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());
		p.scale_.value_.maximum0();
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ += p.rotate_.acceleration_*update_info.GetUpdateFrame();
#else
	p.rotate_.speed_.mac(p.rotate_.acceleration_,update_info.GetUpdateFrame());
#endif
			if(res.GenerateShape_Basic_Normal()){
		p.translate_.speed_.mul(p.generate_shape_.direction_ ,res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range()));
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
	else{
		p.translate_.speed_ = res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range());
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_NewtonDynamics,ScaleTypeConst_CurveSpeed,TranslateTypeConst_Constant):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ = res.Scale_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Speed_RangeX());
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());

		if(res.Scale_Basic_Square()){
			p.scale_.speed_.e_.x_ *= update_info.GetUpdateFrame();
			p.scale_.speed_.e_.y_ = p.scale_.speed_.e_.x_;
			p.scale_.speed_.e_.z_ = p.scale_.speed_.e_.x_;
		}
	#else
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ += p.rotate_.acceleration_*update_info.GetUpdateFrame();
#else
	p.rotate_.speed_.mac(p.rotate_.acceleration_,update_info.GetUpdateFrame());
#endif
		;
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#else
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_NewtonDynamics,ScaleTypeConst_CurveSpeed,TranslateTypeConst_NewtonDynamics):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ = res.Scale_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Speed_RangeX());
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());

		if(res.Scale_Basic_Square()){
			p.scale_.speed_.e_.x_ *= update_info.GetUpdateFrame();
			p.scale_.speed_.e_.y_ = p.scale_.speed_.e_.x_;
			p.scale_.speed_.e_.z_ = p.scale_.speed_.e_.x_;
		}
	#else
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ += p.rotate_.acceleration_*update_info.GetUpdateFrame();
#else
	p.rotate_.speed_.mac(p.rotate_.acceleration_,update_info.GetUpdateFrame());
#endif
		p.translate_.speed_.mac(p.translate_.acceleration_,update_info.GetUpdateFrame());
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#else
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_NewtonDynamics,ScaleTypeConst_CurveSpeed,TranslateTypeConst_CurveValue):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ = res.Scale_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Speed_RangeX());
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());

		if(res.Scale_Basic_Square()){
			p.scale_.speed_.e_.x_ *= update_info.GetUpdateFrame();
			p.scale_.speed_.e_.y_ = p.scale_.speed_.e_.x_;
			p.scale_.speed_.e_.z_ = p.scale_.speed_.e_.x_;
		}
	#else
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ += p.rotate_.acceleration_*update_info.GetUpdateFrame();
#else
	p.rotate_.speed_.mac(p.rotate_.acceleration_,update_info.GetUpdateFrame());
#endif
		p.translate_.value_ = res.Translate_Value_Curve().Value(p.LifeRate(), random_seed, res.Translate_Value_Range());
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#else
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		p.translate_.value_.mac(p.generate_shape_.position_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_NewtonDynamics,ScaleTypeConst_CurveSpeed,TranslateTypeConst_CurveSpeed):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ = res.Scale_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Speed_RangeX());
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());

		if(res.Scale_Basic_Square()){
			p.scale_.speed_.e_.x_ *= update_info.GetUpdateFrame();
			p.scale_.speed_.e_.y_ = p.scale_.speed_.e_.x_;
			p.scale_.speed_.e_.z_ = p.scale_.speed_.e_.x_;
		}
	#else
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ += p.rotate_.acceleration_*update_info.GetUpdateFrame();
#else
	p.rotate_.speed_.mac(p.rotate_.acceleration_,update_info.GetUpdateFrame());
#endif
			if(res.GenerateShape_Basic_Normal()){
		p.translate_.speed_.mul(p.generate_shape_.direction_ ,res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range()));
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
	else{
		p.translate_.speed_ = res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range());
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#else
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveValue,ScaleTypeConst_Constant,TranslateTypeConst_Constant):{
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = res.Rotate_Value_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Value_RangeX());
#else
	p.rotate_.value_ = res.Rotate_Value_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Value_Range());
#endif
		;
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = ml::to_radian(p.rotate_.value_);
#else
	p.rotate_.value_ = p.rotate_.value_.to_radian();
#endif
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveValue,ScaleTypeConst_Constant,TranslateTypeConst_NewtonDynamics):{
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = res.Rotate_Value_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Value_RangeX());
#else
	p.rotate_.value_ = res.Rotate_Value_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Value_Range());
#endif
		p.translate_.speed_.mac(p.translate_.acceleration_,update_info.GetUpdateFrame());
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = ml::to_radian(p.rotate_.value_);
#else
	p.rotate_.value_ = p.rotate_.value_.to_radian();
#endif
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveValue,ScaleTypeConst_Constant,TranslateTypeConst_CurveValue):{
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = res.Rotate_Value_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Value_RangeX());
#else
	p.rotate_.value_ = res.Rotate_Value_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Value_Range());
#endif
		p.translate_.value_ = res.Translate_Value_Curve().Value(p.LifeRate(), random_seed, res.Translate_Value_Range());
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = ml::to_radian(p.rotate_.value_);
#else
	p.rotate_.value_ = p.rotate_.value_.to_radian();
#endif
		p.translate_.value_.mac(p.generate_shape_.position_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveValue,ScaleTypeConst_Constant,TranslateTypeConst_CurveSpeed):{
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = res.Rotate_Value_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Value_RangeX());
#else
	p.rotate_.value_ = res.Rotate_Value_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Value_Range());
#endif
			if(res.GenerateShape_Basic_Normal()){
		p.translate_.speed_.mul(p.generate_shape_.direction_ ,res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range()));
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
	else{
		p.translate_.speed_ = res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range());
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = ml::to_radian(p.rotate_.value_);
#else
	p.rotate_.value_ = p.rotate_.value_.to_radian();
#endif
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveValue,ScaleTypeConst_NewtonDynamics,TranslateTypeConst_Constant):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ += p.scale_.acceleration_*update_info.GetUpdateFrame();
#else
	p.scale_.speed_.mac(p.scale_.acceleration_,update_info.GetUpdateFrame());
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = res.Rotate_Value_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Value_RangeX());
#else
	p.rotate_.value_ = res.Rotate_Value_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Value_Range());
#endif
		;
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
	p.scale_.value_.maximum0();
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = ml::to_radian(p.rotate_.value_);
#else
	p.rotate_.value_ = p.rotate_.value_.to_radian();
#endif
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveValue,ScaleTypeConst_NewtonDynamics,TranslateTypeConst_NewtonDynamics):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ += p.scale_.acceleration_*update_info.GetUpdateFrame();
#else
	p.scale_.speed_.mac(p.scale_.acceleration_,update_info.GetUpdateFrame());
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = res.Rotate_Value_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Value_RangeX());
#else
	p.rotate_.value_ = res.Rotate_Value_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Value_Range());
#endif
		p.translate_.speed_.mac(p.translate_.acceleration_,update_info.GetUpdateFrame());
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
	p.scale_.value_.maximum0();
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = ml::to_radian(p.rotate_.value_);
#else
	p.rotate_.value_ = p.rotate_.value_.to_radian();
#endif
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveValue,ScaleTypeConst_NewtonDynamics,TranslateTypeConst_CurveValue):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ += p.scale_.acceleration_*update_info.GetUpdateFrame();
#else
	p.scale_.speed_.mac(p.scale_.acceleration_,update_info.GetUpdateFrame());
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = res.Rotate_Value_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Value_RangeX());
#else
	p.rotate_.value_ = res.Rotate_Value_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Value_Range());
#endif
		p.translate_.value_ = res.Translate_Value_Curve().Value(p.LifeRate(), random_seed, res.Translate_Value_Range());
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
	p.scale_.value_.maximum0();
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = ml::to_radian(p.rotate_.value_);
#else
	p.rotate_.value_ = p.rotate_.value_.to_radian();
#endif
		p.translate_.value_.mac(p.generate_shape_.position_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveValue,ScaleTypeConst_NewtonDynamics,TranslateTypeConst_CurveSpeed):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ += p.scale_.acceleration_*update_info.GetUpdateFrame();
#else
	p.scale_.speed_.mac(p.scale_.acceleration_,update_info.GetUpdateFrame());
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = res.Rotate_Value_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Value_RangeX());
#else
	p.rotate_.value_ = res.Rotate_Value_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Value_Range());
#endif
			if(res.GenerateShape_Basic_Normal()){
		p.translate_.speed_.mul(p.generate_shape_.direction_ ,res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range()));
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
	else{
		p.translate_.speed_ = res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range());
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
	p.scale_.value_.maximum0();
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = ml::to_radian(p.rotate_.value_);
#else
	p.rotate_.value_ = p.rotate_.value_.to_radian();
#endif
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveValue,ScaleTypeConst_CurveValue,TranslateTypeConst_Constant):{
		#ifdef BM3_PM_SIMPLE_SCALE
		p.scale_.value_ = res.Scale_Value_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Value_RangeX());
		p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);

#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());

		if(res.Scale_Basic_Square()){
//			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f)*update_info.GetUpdateFrame();
			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f);
			p.scale_.value_.e_.y_ = p.scale_.value_.e_.x_;
			p.scale_.value_.e_.z_ = p.scale_.value_.e_.x_;
		}else{
			p.scale_.value_.maximum0();
		}
	#else
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());
		p.scale_.value_.maximum0();
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = res.Rotate_Value_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Value_RangeX());
#else
	p.rotate_.value_ = res.Rotate_Value_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Value_Range());
#endif
		;
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = ml::to_radian(p.rotate_.value_);
#else
	p.rotate_.value_ = p.rotate_.value_.to_radian();
#endif
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveValue,ScaleTypeConst_CurveValue,TranslateTypeConst_NewtonDynamics):{
		#ifdef BM3_PM_SIMPLE_SCALE
		p.scale_.value_ = res.Scale_Value_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Value_RangeX());
		p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);

#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());

		if(res.Scale_Basic_Square()){
//			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f)*update_info.GetUpdateFrame();
			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f);
			p.scale_.value_.e_.y_ = p.scale_.value_.e_.x_;
			p.scale_.value_.e_.z_ = p.scale_.value_.e_.x_;
		}else{
			p.scale_.value_.maximum0();
		}
	#else
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());
		p.scale_.value_.maximum0();
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = res.Rotate_Value_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Value_RangeX());
#else
	p.rotate_.value_ = res.Rotate_Value_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Value_Range());
#endif
		p.translate_.speed_.mac(p.translate_.acceleration_,update_info.GetUpdateFrame());
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = ml::to_radian(p.rotate_.value_);
#else
	p.rotate_.value_ = p.rotate_.value_.to_radian();
#endif
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveValue,ScaleTypeConst_CurveValue,TranslateTypeConst_CurveValue):{
		#ifdef BM3_PM_SIMPLE_SCALE
		p.scale_.value_ = res.Scale_Value_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Value_RangeX());
		p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);

#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());

		if(res.Scale_Basic_Square()){
//			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f)*update_info.GetUpdateFrame();
			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f);
			p.scale_.value_.e_.y_ = p.scale_.value_.e_.x_;
			p.scale_.value_.e_.z_ = p.scale_.value_.e_.x_;
		}else{
			p.scale_.value_.maximum0();
		}
	#else
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());
		p.scale_.value_.maximum0();
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = res.Rotate_Value_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Value_RangeX());
#else
	p.rotate_.value_ = res.Rotate_Value_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Value_Range());
#endif
		p.translate_.value_ = res.Translate_Value_Curve().Value(p.LifeRate(), random_seed, res.Translate_Value_Range());
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = ml::to_radian(p.rotate_.value_);
#else
	p.rotate_.value_ = p.rotate_.value_.to_radian();
#endif
		p.translate_.value_.mac(p.generate_shape_.position_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveValue,ScaleTypeConst_CurveValue,TranslateTypeConst_CurveSpeed):{
		#ifdef BM3_PM_SIMPLE_SCALE
		p.scale_.value_ = res.Scale_Value_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Value_RangeX());
		p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);

#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());

		if(res.Scale_Basic_Square()){
//			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f)*update_info.GetUpdateFrame();
			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f);
			p.scale_.value_.e_.y_ = p.scale_.value_.e_.x_;
			p.scale_.value_.e_.z_ = p.scale_.value_.e_.x_;
		}else{
			p.scale_.value_.maximum0();
		}
	#else
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());
		p.scale_.value_.maximum0();
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = res.Rotate_Value_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Value_RangeX());
#else
	p.rotate_.value_ = res.Rotate_Value_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Value_Range());
#endif
			if(res.GenerateShape_Basic_Normal()){
		p.translate_.speed_.mul(p.generate_shape_.direction_ ,res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range()));
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
	else{
		p.translate_.speed_ = res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range());
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = ml::to_radian(p.rotate_.value_);
#else
	p.rotate_.value_ = p.rotate_.value_.to_radian();
#endif
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveValue,ScaleTypeConst_CurveSpeed,TranslateTypeConst_Constant):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ = res.Scale_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Speed_RangeX());
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());

		if(res.Scale_Basic_Square()){
			p.scale_.speed_.e_.x_ *= update_info.GetUpdateFrame();
			p.scale_.speed_.e_.y_ = p.scale_.speed_.e_.x_;
			p.scale_.speed_.e_.z_ = p.scale_.speed_.e_.x_;
		}
	#else
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = res.Rotate_Value_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Value_RangeX());
#else
	p.rotate_.value_ = res.Rotate_Value_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Value_Range());
#endif
		;
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#else
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = ml::to_radian(p.rotate_.value_);
#else
	p.rotate_.value_ = p.rotate_.value_.to_radian();
#endif
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveValue,ScaleTypeConst_CurveSpeed,TranslateTypeConst_NewtonDynamics):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ = res.Scale_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Speed_RangeX());
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());

		if(res.Scale_Basic_Square()){
			p.scale_.speed_.e_.x_ *= update_info.GetUpdateFrame();
			p.scale_.speed_.e_.y_ = p.scale_.speed_.e_.x_;
			p.scale_.speed_.e_.z_ = p.scale_.speed_.e_.x_;
		}
	#else
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = res.Rotate_Value_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Value_RangeX());
#else
	p.rotate_.value_ = res.Rotate_Value_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Value_Range());
#endif
		p.translate_.speed_.mac(p.translate_.acceleration_,update_info.GetUpdateFrame());
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#else
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = ml::to_radian(p.rotate_.value_);
#else
	p.rotate_.value_ = p.rotate_.value_.to_radian();
#endif
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveValue,ScaleTypeConst_CurveSpeed,TranslateTypeConst_CurveValue):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ = res.Scale_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Speed_RangeX());
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());

		if(res.Scale_Basic_Square()){
			p.scale_.speed_.e_.x_ *= update_info.GetUpdateFrame();
			p.scale_.speed_.e_.y_ = p.scale_.speed_.e_.x_;
			p.scale_.speed_.e_.z_ = p.scale_.speed_.e_.x_;
		}
	#else
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = res.Rotate_Value_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Value_RangeX());
#else
	p.rotate_.value_ = res.Rotate_Value_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Value_Range());
#endif
		p.translate_.value_ = res.Translate_Value_Curve().Value(p.LifeRate(), random_seed, res.Translate_Value_Range());
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#else
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = ml::to_radian(p.rotate_.value_);
#else
	p.rotate_.value_ = p.rotate_.value_.to_radian();
#endif
		p.translate_.value_.mac(p.generate_shape_.position_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveValue,ScaleTypeConst_CurveSpeed,TranslateTypeConst_CurveSpeed):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ = res.Scale_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Speed_RangeX());
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());

		if(res.Scale_Basic_Square()){
			p.scale_.speed_.e_.x_ *= update_info.GetUpdateFrame();
			p.scale_.speed_.e_.y_ = p.scale_.speed_.e_.x_;
			p.scale_.speed_.e_.z_ = p.scale_.speed_.e_.x_;
		}
	#else
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = res.Rotate_Value_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Value_RangeX());
#else
	p.rotate_.value_ = res.Rotate_Value_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Value_Range());
#endif
			if(res.GenerateShape_Basic_Normal()){
		p.translate_.speed_.mul(p.generate_shape_.direction_ ,res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range()));
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
	else{
		p.translate_.speed_ = res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range());
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#else
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = ml::to_radian(p.rotate_.value_);
#else
	p.rotate_.value_ = p.rotate_.value_.to_radian();
#endif
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveSpeed,ScaleTypeConst_Constant,TranslateTypeConst_Constant):{
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ = res.Rotate_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Speed_RangeX());
//	p.rotate_.speed_ = ml::to_radian(p.rotate_.speed_);
#else
	p.rotate_.speed_ = res.Rotate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Speed_Range());
	p.rotate_.speed_ = p.rotate_.speed_.to_radian();
#endif
		;
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveSpeed,ScaleTypeConst_Constant,TranslateTypeConst_NewtonDynamics):{
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ = res.Rotate_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Speed_RangeX());
//	p.rotate_.speed_ = ml::to_radian(p.rotate_.speed_);
#else
	p.rotate_.speed_ = res.Rotate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Speed_Range());
	p.rotate_.speed_ = p.rotate_.speed_.to_radian();
#endif
		p.translate_.speed_.mac(p.translate_.acceleration_,update_info.GetUpdateFrame());
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveSpeed,ScaleTypeConst_Constant,TranslateTypeConst_CurveValue):{
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ = res.Rotate_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Speed_RangeX());
//	p.rotate_.speed_ = ml::to_radian(p.rotate_.speed_);
#else
	p.rotate_.speed_ = res.Rotate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Speed_Range());
	p.rotate_.speed_ = p.rotate_.speed_.to_radian();
#endif
		p.translate_.value_ = res.Translate_Value_Curve().Value(p.LifeRate(), random_seed, res.Translate_Value_Range());
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		p.translate_.value_.mac(p.generate_shape_.position_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveSpeed,ScaleTypeConst_Constant,TranslateTypeConst_CurveSpeed):{
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ = res.Rotate_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Speed_RangeX());
//	p.rotate_.speed_ = ml::to_radian(p.rotate_.speed_);
#else
	p.rotate_.speed_ = res.Rotate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Speed_Range());
	p.rotate_.speed_ = p.rotate_.speed_.to_radian();
#endif
			if(res.GenerateShape_Basic_Normal()){
		p.translate_.speed_.mul(p.generate_shape_.direction_ ,res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range()));
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
	else{
		p.translate_.speed_ = res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range());
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveSpeed,ScaleTypeConst_NewtonDynamics,TranslateTypeConst_Constant):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ += p.scale_.acceleration_*update_info.GetUpdateFrame();
#else
	p.scale_.speed_.mac(p.scale_.acceleration_,update_info.GetUpdateFrame());
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ = res.Rotate_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Speed_RangeX());
//	p.rotate_.speed_ = ml::to_radian(p.rotate_.speed_);
#else
	p.rotate_.speed_ = res.Rotate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Speed_Range());
	p.rotate_.speed_ = p.rotate_.speed_.to_radian();
#endif
		;
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
	p.scale_.value_.maximum0();
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveSpeed,ScaleTypeConst_NewtonDynamics,TranslateTypeConst_NewtonDynamics):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ += p.scale_.acceleration_*update_info.GetUpdateFrame();
#else
	p.scale_.speed_.mac(p.scale_.acceleration_,update_info.GetUpdateFrame());
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ = res.Rotate_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Speed_RangeX());
//	p.rotate_.speed_ = ml::to_radian(p.rotate_.speed_);
#else
	p.rotate_.speed_ = res.Rotate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Speed_Range());
	p.rotate_.speed_ = p.rotate_.speed_.to_radian();
#endif
		p.translate_.speed_.mac(p.translate_.acceleration_,update_info.GetUpdateFrame());
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
	p.scale_.value_.maximum0();
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveSpeed,ScaleTypeConst_NewtonDynamics,TranslateTypeConst_CurveValue):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ += p.scale_.acceleration_*update_info.GetUpdateFrame();
#else
	p.scale_.speed_.mac(p.scale_.acceleration_,update_info.GetUpdateFrame());
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ = res.Rotate_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Speed_RangeX());
//	p.rotate_.speed_ = ml::to_radian(p.rotate_.speed_);
#else
	p.rotate_.speed_ = res.Rotate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Speed_Range());
	p.rotate_.speed_ = p.rotate_.speed_.to_radian();
#endif
		p.translate_.value_ = res.Translate_Value_Curve().Value(p.LifeRate(), random_seed, res.Translate_Value_Range());
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
	p.scale_.value_.maximum0();
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		p.translate_.value_.mac(p.generate_shape_.position_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveSpeed,ScaleTypeConst_NewtonDynamics,TranslateTypeConst_CurveSpeed):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ += p.scale_.acceleration_*update_info.GetUpdateFrame();
#else
	p.scale_.speed_.mac(p.scale_.acceleration_,update_info.GetUpdateFrame());
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ = res.Rotate_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Speed_RangeX());
//	p.rotate_.speed_ = ml::to_radian(p.rotate_.speed_);
#else
	p.rotate_.speed_ = res.Rotate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Speed_Range());
	p.rotate_.speed_ = p.rotate_.speed_.to_radian();
#endif
			if(res.GenerateShape_Basic_Normal()){
		p.translate_.speed_.mul(p.generate_shape_.direction_ ,res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range()));
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
	else{
		p.translate_.speed_ = res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range());
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
	p.scale_.value_.maximum0();
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveSpeed,ScaleTypeConst_CurveValue,TranslateTypeConst_Constant):{
		#ifdef BM3_PM_SIMPLE_SCALE
		p.scale_.value_ = res.Scale_Value_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Value_RangeX());
		p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);

#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());

		if(res.Scale_Basic_Square()){
//			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f)*update_info.GetUpdateFrame();
			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f);
			p.scale_.value_.e_.y_ = p.scale_.value_.e_.x_;
			p.scale_.value_.e_.z_ = p.scale_.value_.e_.x_;
		}else{
			p.scale_.value_.maximum0();
		}
	#else
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());
		p.scale_.value_.maximum0();
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ = res.Rotate_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Speed_RangeX());
//	p.rotate_.speed_ = ml::to_radian(p.rotate_.speed_);
#else
	p.rotate_.speed_ = res.Rotate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Speed_Range());
	p.rotate_.speed_ = p.rotate_.speed_.to_radian();
#endif
		;
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveSpeed,ScaleTypeConst_CurveValue,TranslateTypeConst_NewtonDynamics):{
		#ifdef BM3_PM_SIMPLE_SCALE
		p.scale_.value_ = res.Scale_Value_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Value_RangeX());
		p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);

#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());

		if(res.Scale_Basic_Square()){
//			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f)*update_info.GetUpdateFrame();
			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f);
			p.scale_.value_.e_.y_ = p.scale_.value_.e_.x_;
			p.scale_.value_.e_.z_ = p.scale_.value_.e_.x_;
		}else{
			p.scale_.value_.maximum0();
		}
	#else
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());
		p.scale_.value_.maximum0();
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ = res.Rotate_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Speed_RangeX());
//	p.rotate_.speed_ = ml::to_radian(p.rotate_.speed_);
#else
	p.rotate_.speed_ = res.Rotate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Speed_Range());
	p.rotate_.speed_ = p.rotate_.speed_.to_radian();
#endif
		p.translate_.speed_.mac(p.translate_.acceleration_,update_info.GetUpdateFrame());
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveSpeed,ScaleTypeConst_CurveValue,TranslateTypeConst_CurveValue):{
		#ifdef BM3_PM_SIMPLE_SCALE
		p.scale_.value_ = res.Scale_Value_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Value_RangeX());
		p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);

#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());

		if(res.Scale_Basic_Square()){
//			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f)*update_info.GetUpdateFrame();
			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f);
			p.scale_.value_.e_.y_ = p.scale_.value_.e_.x_;
			p.scale_.value_.e_.z_ = p.scale_.value_.e_.x_;
		}else{
			p.scale_.value_.maximum0();
		}
	#else
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());
		p.scale_.value_.maximum0();
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ = res.Rotate_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Speed_RangeX());
//	p.rotate_.speed_ = ml::to_radian(p.rotate_.speed_);
#else
	p.rotate_.speed_ = res.Rotate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Speed_Range());
	p.rotate_.speed_ = p.rotate_.speed_.to_radian();
#endif
		p.translate_.value_ = res.Translate_Value_Curve().Value(p.LifeRate(), random_seed, res.Translate_Value_Range());
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		p.translate_.value_.mac(p.generate_shape_.position_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveSpeed,ScaleTypeConst_CurveValue,TranslateTypeConst_CurveSpeed):{
		#ifdef BM3_PM_SIMPLE_SCALE
		p.scale_.value_ = res.Scale_Value_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Value_RangeX());
		p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);

#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());

		if(res.Scale_Basic_Square()){
//			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f)*update_info.GetUpdateFrame();
			p.scale_.value_.e_.x_ = ml::maximum(p.scale_.value_.e_.x_, 0.0f);
			p.scale_.value_.e_.y_ = p.scale_.value_.e_.x_;
			p.scale_.value_.e_.z_ = p.scale_.value_.e_.x_;
		}else{
			p.scale_.value_.maximum0();
		}
	#else
		p.scale_.value_ = res.Scale_Value_Curve().Value(p.LifeRate(), random_seed, res.Scale_Value_Range());
		p.scale_.value_.maximum0();
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ = res.Rotate_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Speed_RangeX());
//	p.rotate_.speed_ = ml::to_radian(p.rotate_.speed_);
#else
	p.rotate_.speed_ = res.Rotate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Speed_Range());
	p.rotate_.speed_ = p.rotate_.speed_.to_radian();
#endif
			if(res.GenerateShape_Basic_Normal()){
		p.translate_.speed_.mul(p.generate_shape_.direction_ ,res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range()));
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
	else{
		p.translate_.speed_ = res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range());
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
		;
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveSpeed,ScaleTypeConst_CurveSpeed,TranslateTypeConst_Constant):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ = res.Scale_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Speed_RangeX());
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());

		if(res.Scale_Basic_Square()){
			p.scale_.speed_.e_.x_ *= update_info.GetUpdateFrame();
			p.scale_.speed_.e_.y_ = p.scale_.speed_.e_.x_;
			p.scale_.speed_.e_.z_ = p.scale_.speed_.e_.x_;
		}
	#else
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ = res.Rotate_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Speed_RangeX());
//	p.rotate_.speed_ = ml::to_radian(p.rotate_.speed_);
#else
	p.rotate_.speed_ = res.Rotate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Speed_Range());
	p.rotate_.speed_ = p.rotate_.speed_.to_radian();
#endif
		;
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#else
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		;
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveSpeed,ScaleTypeConst_CurveSpeed,TranslateTypeConst_NewtonDynamics):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ = res.Scale_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Speed_RangeX());
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());

		if(res.Scale_Basic_Square()){
			p.scale_.speed_.e_.x_ *= update_info.GetUpdateFrame();
			p.scale_.speed_.e_.y_ = p.scale_.speed_.e_.x_;
			p.scale_.speed_.e_.z_ = p.scale_.speed_.e_.x_;
		}
	#else
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ = res.Rotate_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Speed_RangeX());
//	p.rotate_.speed_ = ml::to_radian(p.rotate_.speed_);
#else
	p.rotate_.speed_ = res.Rotate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Speed_Range());
	p.rotate_.speed_ = p.rotate_.speed_.to_radian();
#endif
		p.translate_.speed_.mac(p.translate_.acceleration_,update_info.GetUpdateFrame());
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#else
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveSpeed,ScaleTypeConst_CurveSpeed,TranslateTypeConst_CurveValue):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ = res.Scale_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Speed_RangeX());
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());

		if(res.Scale_Basic_Square()){
			p.scale_.speed_.e_.x_ *= update_info.GetUpdateFrame();
			p.scale_.speed_.e_.y_ = p.scale_.speed_.e_.x_;
			p.scale_.speed_.e_.z_ = p.scale_.speed_.e_.x_;
		}
	#else
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ = res.Rotate_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Speed_RangeX());
//	p.rotate_.speed_ = ml::to_radian(p.rotate_.speed_);
#else
	p.rotate_.speed_ = res.Rotate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Speed_Range());
	p.rotate_.speed_ = p.rotate_.speed_.to_radian();
#endif
		p.translate_.value_ = res.Translate_Value_Curve().Value(p.LifeRate(), random_seed, res.Translate_Value_Range());
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#else
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		p.translate_.value_.mac(p.generate_shape_.position_,update_info.GetUpdateFrame());
		break;
	}
	case BM3_PM_CASE(RotateTypeConst_CurveSpeed,ScaleTypeConst_CurveSpeed,TranslateTypeConst_CurveSpeed):{
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ = res.Scale_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Scale_Speed_RangeX());
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());

		if(res.Scale_Basic_Square()){
			p.scale_.speed_.e_.x_ *= update_info.GetUpdateFrame();
			p.scale_.speed_.e_.y_ = p.scale_.speed_.e_.x_;
			p.scale_.speed_.e_.z_ = p.scale_.speed_.e_.x_;
		}
	#else
		p.scale_.speed_ = res.Scale_Speed_Curve().Value(p.LifeRate(), random_seed, res.Scale_Speed_Range());
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ = res.Rotate_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Speed_RangeX());
//	p.rotate_.speed_ = ml::to_radian(p.rotate_.speed_);
#else
	p.rotate_.speed_ = res.Rotate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Speed_Range());
	p.rotate_.speed_ = p.rotate_.speed_.to_radian();
#endif
			if(res.GenerateShape_Basic_Normal()){
		p.translate_.speed_.mul(p.generate_shape_.direction_ ,res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range()));
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
	else{
		p.translate_.speed_ = res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range());
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
		#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	#ifdef BM3_PM_AllowFor_Scale_Basic_Square
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#else
		p.scale_.value_.mac(p.scale_.speed_,update_info.GetUpdateFrame());
		p.scale_.value_.maximum0();
	#endif
#endif
		#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_*update_info.GetUpdateFrame();
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
		;
		break;
	}
}
#undef BM3_PM_CASE
#endif	// #ifdef BM3_PM_SIMPLE_SCALE
