//�}�e���A��
cbuffer ConstBufferDataMaterial : register(b0) {
	float4 color;	//�F(RGBA)
};

//���_�V�F�[�_�̏o�͍\����
struct VSOutput {
	//�V�X�e���p���_���W
	float4 svpos : SV_POSITION;
	//�@���x�N�g��
	float3 normal : NORMAL;
	//uv�l
	float2 uv : TEXCOORD;
};

cbuffer ConstBufferDataTransform : register(b1) {
	matrix mat;	//3D�ϊ��s��
};