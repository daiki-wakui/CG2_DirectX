//マテリアル
cbuffer ConstBufferDataMaterial : register(b0) {
	float4 color;	//色(RGBA)
};

//頂点シェーダの出力構造体
struct VSOutput {
	//システム用頂点座標
	float4 svpos : SV_POSITION;
	//uv値
	float2 uv : TEXCOORD;
};