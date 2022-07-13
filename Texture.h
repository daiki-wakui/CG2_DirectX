#pragma once

#include <d3d12.h>
#include <DirectXTex.h>
using namespace DirectX;

class Texture{
public:
	HRESULT result;
	ID3D12Device* device = nullptr;

////画像データを取得////
	TexMetadata metadata{};
	ScratchImage scratchImg{};

////ミップマップ生成////
	ScratchImage mipChain{};

////リソース設定////
	D3D12_RESOURCE_DESC textureResouseDesc{};

////シェーダリソースビュー設定////
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

	//テクスチャバッファ生成
	ID3D12Resource* texBuff = nullptr;
public:

	void Load(HRESULT tResult, ID3D12Device* tDevice,const wchar_t* image);
	void CreateMipmap(HRESULT tResult);
	void ResourceSetting();
	void ShaderResourceViewSetting();
	void CreateBaffer(HRESULT tResult, ID3D12Device* tDevice);
	void DetaTransfer(HRESULT tResult);
};

