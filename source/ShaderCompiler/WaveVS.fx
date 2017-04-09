// 定数
uniform float PI = 3.141592;

// 頂点シェーダーの入力
struct VS_INPUT {
    float4 pos              : POSITION;    // 座標( VERTEX3DSHADER構造体の pos の値 )
    float4 diffuseColor		: COLOR0;
    float3 normal           : NORMAL0;     // 法線( VERTEX3DSHADER構造体の norm の値 )
    float2 textureCoord0    : TEXCOORD0;   // テクスチャ座標０( VERTEX3DSHADER構造体の u, v の値 )
};

// 頂点シェーダーの出力
struct VS_OUTPUT {
    float4 projectionPos	: POSITION;    // 座標( 射影空間 )
    float4 diffuseColor     : COLOR0;
    float2 textureCoord0    : TEXCOORD0;   // テクスチャ座標
	float3 normal			: TEXCOORD1;   // ワールド変換した法線
};


// レジスター
float4x4 matView		: register( c6 );    // ワールド座標をビュー座標に変換する行列の転置行列
float4x4 matProjection	: register( c2 );    // ビュー座標を射影座標に変換する行列の転置行列

float period			: register( c0 );
float amplitude			: register( c1 );

// main関数
VS_OUTPUT main( VS_INPUT input ) {
    VS_OUTPUT output;
    float4 worldPos;
    float4 viewPos;

	// 波動方程式
	input.pos.y += amplitude * sin( fmod( input.pos.z + input.pos.x, PI * 2 ) +
							( period - ( input.pos.x / 2 ) ) * PI * 2 ) - amplitude;

   
    worldPos = input.pos; // 入力の頂点座標

    viewPos = mul( worldPos, matView );	// 頂点座標をビュー空間の座標に変換する

    output.projectionPos = mul( viewPos, matProjection ); // ビュー空間の座標を射影空間の座標に変換する

    output.textureCoord0 = input.textureCoord0;	// テクスチャ座標
    
    output.diffuseColor = input.diffuseColor; // 頂点カラー
	
	output.normal = mul( input.normal, matView );

    return output;
}
