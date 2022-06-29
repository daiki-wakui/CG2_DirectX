#include "Basic.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET{

	float4 texcolor = float4(tex.Sample(smp, input.uv));

	//�����̌v�Z
	float3 light = normalize(float3(1,-1,1));	//�E�����@�����̃��C�g
	float3 diffuse = saturate(dot(-light, input.normal));	//diffuse��[0,1]�͈̔͂�Clamp����
	float brightness = diffuse + 0.3f;	//�A���r�G���g����0.3�Ƃ��Čv�Z

	return float4(texcolor.rgb * brightness, texcolor.a) * color;

	//�g�U���ˌ��̌v�Z
	//float3 light = normalize(float3(1,-1,1));	//�E�����@�����̃��C�g
	//float brightness = dot(-light, input.normal);	//�����ւ̃x�N�g���Ɩ@���x�N�g���̓���
	//return float4(brightness, brightness, brightness, 1);	//�P�x��RGB�ё�����ďo��

	//return float4(input.normal,1);
	//return float4(tex.Sample(smp, input.uv));
	//return float4(1, 1, 1, 1);
}
