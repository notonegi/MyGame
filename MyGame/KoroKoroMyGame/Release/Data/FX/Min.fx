//������������������������������������������������������
// Min.fx
// Author   : MasayaHayashi
// �X�V���� : 
//			  2018/09/28 �Œ���̃V�F�[�_�[�쐬
//������������������������������������������������������


///////////////////////////////////////////////////////////////////////////////////
//�^��`
///////////////////////////////////////////////////////////////////////////////////
struct VS_OUT
{
	float4 f4Position : POSITION;
	float2 f2Texture : TEXCOORD0;
};
///////////////////////////////////////////////////////////////////////////////////
//�O���[�o��
///////////////////////////////////////////////////////////////////////////////////

float4x4 mWVP; //���[���h���灨�r���[���v���W�F�N�V�����܂ł̍��W�ϊ��s��@

texture texDecal; //���b�V���̃e�N�X�`��

sampler Sampler = sampler_state //�T���v���[
{
	Texture = <texDecal>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;
	AddressU = Clamp;
	AddressV = Clamp;
};

/////////////////////////////////////////////////////////////////////
//�o�[�e�b�N�X�E�V�F�[�_�[
///////////////////////////////////////////////////////////////////////////////////
VS_OUT VS(float4 f4Position : POSITION, float2 f2Texture : TEXCOORD)
{
	VS_OUT Out;
	Out.f4Position = mul(f4Position, mWVP);
	Out.f2Texture = f2Texture;
	return Out;
}

///////////////////////////////////////////////////////////////////////////////////
//�s�N�Z���E�V�F�[�_�[
///////////////////////////////////////////////////////////////////////////////////
float4 PS(float2 f2Texture : TEXCOORD) : COLOR
{
	return tex2D(Sampler, f2Texture);
}

///////////////////////////////////////////////////////////////////////////////////
// �e�N�j�b�N
///////////////////////////////////////////////////////////////////////////////////
technique tecMinimum
{
	pass P0
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();
	}
}