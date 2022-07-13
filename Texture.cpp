#include "Texture.h"

//画像読み込み
void Texture::Load(HRESULT tResult, ID3D12Device* tDevice, const wchar_t* image){
	result = tResult;

	result = LoadFromWICFile(
		image,
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	//詠み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

	CreateMipmap(tResult);

	ResourceSetting();

	ShaderResourceViewSetting();

	CreateBaffer(tResult, tDevice);

	DetaTransfer(tResult);
}

//ミップマップ生成
void Texture::CreateMipmap(HRESULT tResult){
	result = tResult;

	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}
}

//リソース設定
void Texture::ResourceSetting(){
	textureResouseDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResouseDesc.Format = metadata.format;
	textureResouseDesc.Width = metadata.width;
	textureResouseDesc.Height = (UINT)metadata.height;
	textureResouseDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResouseDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResouseDesc.SampleDesc.Count = 1;
}

//シェーダリソースビュー設定
void Texture::ShaderResourceViewSetting(){
	srvDesc.Format = textureResouseDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureResouseDesc.MipLevels;
}

//テクスチャバッファ生成
void Texture::CreateBaffer(HRESULT tResult, ID3D12Device* tDevice)
{
	result = tResult;
	device = tDevice;

	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	result = device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResouseDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff));
}

//データ転送
void Texture::DetaTransfer(HRESULT tResult)
{
	result = tResult;

	//テクスチャバッファにデータ転送
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		const Image* img = scratchImg.GetImage(i, 0, 0);
		result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);
		assert(SUCCEEDED(result));
	}
}
