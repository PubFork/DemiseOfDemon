// �萔
uniform float PI = 3.141592;

// ���_�V�F�[�_�[�̓���
struct VS_INPUT {
    float4 pos              : POSITION;    // ���W( VERTEX3DSHADER�\���̂� pos �̒l )
    float4 diffuseColor		: COLOR0;
    float3 normal           : NORMAL0;     // �@��( VERTEX3DSHADER�\���̂� norm �̒l )
    float2 textureCoord0    : TEXCOORD0;   // �e�N�X�`�����W�O( VERTEX3DSHADER�\���̂� u, v �̒l )
};

// ���_�V�F�[�_�[�̏o��
struct VS_OUTPUT {
    float4 projectionPos	: POSITION;    // ���W( �ˉe��� )
    float4 diffuseColor     : COLOR0;
    float2 textureCoord0    : TEXCOORD0;   // �e�N�X�`�����W
	float3 normal			: TEXCOORD1;   // ���[���h�ϊ������@��
};


// ���W�X�^�[
float4x4 matView		: register( c6 );    // ���[���h���W���r���[���W�ɕϊ�����s��̓]�u�s��
float4x4 matProjection	: register( c2 );    // �r���[���W���ˉe���W�ɕϊ�����s��̓]�u�s��

float period			: register( c0 );
float amplitude			: register( c1 );

// main�֐�
VS_OUTPUT main( VS_INPUT input ) {
    VS_OUTPUT output;
    float4 worldPos;
    float4 viewPos;

	// �g��������
	input.pos.y += amplitude * sin( fmod( input.pos.z + input.pos.x, PI * 2 ) +
							( period - ( input.pos.x / 2 ) ) * PI * 2 ) - amplitude;

   
    worldPos = input.pos; // ���͂̒��_���W

    viewPos = mul( worldPos, matView );	// ���_���W���r���[��Ԃ̍��W�ɕϊ�����

    output.projectionPos = mul( viewPos, matProjection ); // �r���[��Ԃ̍��W���ˉe��Ԃ̍��W�ɕϊ�����

    output.textureCoord0 = input.textureCoord0;	// �e�N�X�`�����W
    
    output.diffuseColor = input.diffuseColor; // ���_�J���[
	
	output.normal = mul( input.normal, matView );

    return output;
}
