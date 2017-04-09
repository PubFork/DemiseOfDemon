// ピクセルシェーダーの入力
struct PS_INPUT {
	float4 diffuseColor     : COLOR0;
	float2 textureCoord0    : TEXCOORD0;
	float2 vpos				: VPOS;
};

// ピクセルシェーダーの出力
struct PS_OUTPUT {
	float4 color		: COLOR0;
};

// レジスター
sampler  Texture		: register( s0 );

uniform float time			: register( c0 );
uniform float2 resolution	: register( c1 );
uniform float textureState	: register( c3 );

// main関数
PS_OUTPUT main( PS_INPUT input ) {
	// 定数
	int MAX_ITER = 4;
	float4 DEFAULT_COLOR = float4( 0, 100, 255, 255 ) / 255;	// デフォルトカラー
	float ALPHA_COLOR = 0.5;	// 透明度

	// 水面の模様設定
	float2 v_texCoord = input.vpos / resolution;
	float2 p =  v_texCoord * 8.0 - float2( 20.0 , 20.0 );
	float2 i = p;
	float c = 1.3;
	float inten = 2.99;
	for ( int n = 0; n < MAX_ITER; n++ ) {
		float t = time * (1.0 - ( 3.0 / float( n + 1 ) ) );
		
		i = p + float2( cos( t - i.x ) + sin( t + i.y ),
					    sin( t - i.y ) + cos( t + i.x ) );
					 
		c += 1.0 / length( float2( p.x / ( sin( i.x + t ) / inten ),
								   p.y / ( cos(i.y + t ) / inten ) ) );
	}
	c /= ( float )MAX_ITER;
	c = 1.5 - sqrt( c );

	// 色の設定
	PS_OUTPUT output;
	float2 uv = input.textureCoord0;
	float4 color;
	if ( textureState > 0 ) {
		color = tex2D( Texture, uv ) / 255;
	} else {
		color = DEFAULT_COLOR / 255;
	}
	color.rgb *= ( 1.0 / ( 1.0 - ( c + 0.05 ) ) ) * 10;	// 模様の合成
	output.color = color;
	output.color.a = ALPHA_COLOR;
	return output;
}
