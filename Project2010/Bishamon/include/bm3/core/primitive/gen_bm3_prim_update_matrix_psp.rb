##################################################################################################################################################################
ScaleTypeConst_Constant_PRE							= ";"
ScaleTypeConst_Constant_POST						= ";"
ScaleTypeConst_NewtonDynamics_PRE				=<<DOC
#ifdef BM3_PM_SIMPLE_SCALE
	p.scale_.speed_ += p.scale_.acceleration_*update_info.GetUpdateFrame();
	p.scale_.value_ += p.scale_.speed_*update_info.GetUpdateFrame();
	p.scale_.value_ = ml::maximum(p.scale_.value_, 0.0f);
#else
	__asm__ (
		".set     push\\n"					
		".set     noreorder\\n"			
		"lv.t     c010, %2\\n"          // latency3 pitch1
		"lv.t     c020, %3\\n"          // latency3 pitch1
		"lv.t     c030, %4\\n"          // latency3 pitch1
		"vadd.t   c100, c010, c020\\n"  // latency5 pitch1
		"vadd.t   c110, c100, c030\\n"  // latency5 pitch1
		"vzero.t  c200\\n"              // latency3 pitch1
		"vmax.t   c120, c110, c200\\n"  // latency3 pitch1
		"sv.t     c100, %1\\n"          // latency0 pitch7
		"sv.t     c120, %0\\n"          // latency0 pitch7
		".set     pop\\n"					
		: "=m"(p.scale_.value_.v_), "=m"(p.scale_.speed_.v_)
		: "m"(p.scale_.acceleration_.v_), "m"(p.scale_.speed_.v_), "m"(p.scale_.value_.v_)
	);
//	p.scale_.speed_.add(p.scale_.acceleration_);
//	p.scale_.value_.add(p.scale_.speed_);
//	p.scale_.value_.maximum0();
#endif
DOC

ScaleTypeConst_NewtonDynamics_POST			= <<DOC
DOC

ScaleTypeConst_CurveValue_PRE						= <<DOC
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
DOC

ScaleTypeConst_CurveValue_POST					= ";"

ScaleTypeConst_CurveSpeed_PRE						= <<DOC
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
DOC

ScaleTypeConst_CurveSpeed_POST					= <<DOC
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
DOC

##################################################################################################################################################################
RotateTypeConst_Constant_PRE						= ";"
RotateTypeConst_Constant_POST						= ";"
RotateTypeConst_NewtonDynamics_PRE			= <<DOC
#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ += p.rotate_.acceleration_;
//	p.rotate_.value_ += p.rotate_.speed_;
#else
	__asm__ (
		".set			push\\n"					
		".set			noreorder\\n"			
		"lv.t			c010, %2\\n"
		"lv.t			c020, %3\\n"
		"vadd.t			c100, c010, c020\\n"		
		"lv.t			c030, %4\\n"
		"vadd.t			c110, c100, c030\\n"		
		"sv.t			c100, %1\\n"
		"sv.t			c110, %0\\n"
		".set			pop\\n"					
		: "=m"(p.rotate_.value_.v_), "=m"(p.rotate_.speed_.v_)
		: "m"(p.rotate_.acceleration_.v_), "m"(p.rotate_.speed_.v_), "m"(p.rotate_.value_.v_)
	);
//	p.rotate_.speed_.add(p.rotate_.acceleration_);
//	p.rotate_.value_.add(p.rotate_.speed_);
#endif
DOC

RotateTypeConst_NewtonDynamics_POST			= <<DOC
DOC

RotateTypeConst_CurveValue_PRE					= <<DOC
#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = res.Rotate_Value_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Value_RangeX());
#else
	p.rotate_.value_ = res.Rotate_Value_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Value_Range());
#endif
DOC

RotateTypeConst_CurveValue_POST					= <<DOC
#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ = ml::to_radian(p.rotate_.value_);
#else
	p.rotate_.value_ = p.rotate_.value_.to_radian();
#endif
DOC

RotateTypeConst_CurveSpeed_PRE					= <<DOC
#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.speed_ = res.Rotate_Speed_CurveX().Value(p.LifeRate(), random_seed, res.Rotate_Speed_RangeX());
//	p.rotate_.speed_ = ml::to_radian(p.rotate_.speed_);
#else
	p.rotate_.speed_ = res.Rotate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Rotate_Speed_Range());
	p.rotate_.speed_ = p.rotate_.speed_.to_radian();
#endif
DOC

RotateTypeConst_CurveSpeed_POST					= <<DOC
#ifdef BM3_PM_SIMPLE_SCALE
//	p.rotate_.value_ += p.rotate_.speed_;
#else
	p.rotate_.value_.mac(p.rotate_.speed_,update_info.GetUpdateFrame());
#endif
DOC

##################################################################################################################################################################
TranslateTypeConst_Constant_PRE					= ";"
TranslateTypeConst_Constant_POST				= ";"

TranslateTypeConst_NewtonDynamics_PRE		= <<DOC
	__asm__ (
		".set			push\\n"					
		".set			noreorder\\n"			
		"lv.t			c010, %2\\n"
		"lv.t			c020, %3\\n"
		"vadd.t			c100, c010, c020\\n"		
		"lv.t			c030, %4\\n"
		"vadd.t			c110, c100, c030\\n"		
		"sv.t			c100, %1\\n"
		"sv.t			c110, %0\\n"
		".set			pop\\n"					
		: "=m"(p.translate_.value_.v_), "=m"(p.translate_.speed_.v_)
		: "m"(p.translate_.acceleration_.v_), "m"(p.translate_.speed_.v_), "m"(p.translate_.value_.v_)
	);
//	p.translate_.speed_.add(p.translate_.acceleration_);
//	p.translate_.value_.add(p.translate_.speed_);
DOC

TranslateTypeConst_NewtonDynamics_POST	= <<DOC
DOC

TranslateTypeConst_CurveValue_PRE				= <<DOC
p.translate_.value_ = res.Translate_Value_Curve().Value(p.LifeRate(), random_seed, res.Translate_Value_Range());
DOC

TranslateTypeConst_CurveValue_POST			= <<DOC
p.translate_.value_.mac(p.generate_shape_.position_,update_info.GetUpdateFrame());
DOC

TranslateTypeConst_CurveSpeed_PRE				=<<DOC
	if(res.GenerateShape_Basic_Normal()){
		p.translate_.speed_.mul(p.generate_shape_.direction_ ,res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range()));
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
	else{
		p.translate_.speed_ = res.Translate_Speed_Curve().Value(p.LifeRate(), random_seed, res.Translate_Speed_Range());
		p.translate_.value_.mac(p.translate_.speed_,update_info.GetUpdateFrame());
	}
DOC

TranslateTypeConst_CurveSpeed_POST			= ";"

Scale = [
	{:type => "ScaleTypeConst_Constant",						:pre => ScaleTypeConst_Constant_PRE,						:post => ScaleTypeConst_Constant_POST},
	{:type => "ScaleTypeConst_NewtonDynamics",			:pre => ScaleTypeConst_NewtonDynamics_PRE,			:post => ScaleTypeConst_NewtonDynamics_POST},
	{:type => "ScaleTypeConst_CurveValue",					:pre => ScaleTypeConst_CurveValue_PRE,					:post => ScaleTypeConst_CurveValue_POST},
	{:type => "ScaleTypeConst_CurveSpeed",					:pre => ScaleTypeConst_CurveSpeed_PRE,					:post => ScaleTypeConst_CurveSpeed_POST},
]

Rotate = [
	{:type => "RotateTypeConst_Constant",						:pre => RotateTypeConst_Constant_PRE,						:post => RotateTypeConst_Constant_POST},
	{:type => "RotateTypeConst_NewtonDynamics",			:pre => RotateTypeConst_NewtonDynamics_PRE,			:post => RotateTypeConst_NewtonDynamics_POST},
	{:type => "RotateTypeConst_CurveValue",					:pre => RotateTypeConst_CurveValue_PRE,					:post => RotateTypeConst_CurveValue_POST},
	{:type => "RotateTypeConst_CurveSpeed",					:pre => RotateTypeConst_CurveSpeed_PRE,					:post => RotateTypeConst_CurveSpeed_POST},
]

Translate = [
	{:type => "TranslateTypeConst_Constant",				:pre => TranslateTypeConst_Constant_PRE,				:post => TranslateTypeConst_Constant_POST},
	{:type => "TranslateTypeConst_NewtonDynamics",	:pre => TranslateTypeConst_NewtonDynamics_PRE,	:post => TranslateTypeConst_NewtonDynamics_POST},
	{:type => "TranslateTypeConst_CurveValue",			:pre => TranslateTypeConst_CurveValue_PRE,			:post => TranslateTypeConst_CurveValue_POST},
	{:type => "TranslateTypeConst_CurveSpeed",			:pre => TranslateTypeConst_CurveSpeed_PRE,			:post => TranslateTypeConst_CurveSpeed_POST},
]

File.open("bm3_prim_update_matrix_psp.inc", "w") do |file|

	file.puts "#ifdef BM3_PM_SIMPLE_SCALE"

		file.puts "#define BM3_PM_CASE(S,T)	(((S) << (2*1)) | ((T) << (2*0)))"
		file.puts "switch(BM3_PM_CASE(res.Scale_Basic_ScaleType(), res.Translate_Basic_TranslateType())){"

		Scale.each do |s|
			Translate.each do |t|
				file.puts "\tcase BM3_PM_CASE(#{s[:type]},#{t[:type]}):{"

				file.puts "		#{s[:pre]}"
				file.puts "		#{t[:pre]}"
				file.puts "		#{s[:post]}"
				file.puts "		#{t[:post]}"

				file.puts "		break;"
				file.puts "	}"
			end
		end

		file.puts "}"
		file.puts "#undef BM3_PM_CASE"

	file.puts "#else // #ifdef BM3_PM_SIMPLE_SCALE"

		file.puts "#define BM3_PM_CASE(R,S,T)	(((R) << (2*2)) | ((S) << (2*1)) | ((T) << (2*0)))"
		file.puts "switch(BM3_PM_CASE(res.Rotate_Basic_RotateType(), res.Scale_Basic_ScaleType(), res.Translate_Basic_TranslateType())){"

		Rotate.each do |r|
			Scale.each do |s|
				Translate.each do |t|
					file.puts "\tcase BM3_PM_CASE(#{r[:type]},#{s[:type]},#{t[:type]}):{"

					file.puts "		#{s[:pre]}"
					file.puts "		#{s[:post]}"
					file.puts "		#{r[:pre]}"
					file.puts "		#{r[:post]}"
					file.puts "		#{t[:pre]}"
					file.puts "		#{t[:post]}"

					file.puts "		break;"
					file.puts "	}"
				end
			end
		end

		file.puts "}"
		file.puts "#undef BM3_PM_CASE"

	file.puts "#endif	// #ifdef BM3_PM_SIMPLE_SCALE"
end
