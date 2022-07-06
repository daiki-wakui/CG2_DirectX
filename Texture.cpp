#include "Texture.h"

//�摜�ǂݍ���
void Texture::Load(HRESULT tResult, const wchar_t* image){
	result = tResult;

	result = LoadFromWICFile(
		image,
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	//�r�ݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);
}

//�~�b�v�}�b�v����
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

//���\�[�X�ݒ�
void Texture::ResourceSetting(){
	textureResouseDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResouseDesc.Format = metadata.format;
	textureResouseDesc.Width = metadata.width;
	textureResouseDesc.Height = (UINT)metadata.height;
	textureResouseDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResouseDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResouseDesc.SampleDesc.Count = 1;
}

//�V�F�[�_���\�[�X�r���[�ݒ�
void Texture::ShaderResourceViewSetting(){
	srvDesc.Format = textureResouseDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureResouseDesc.MipLevels;
}

//�e�N�X�`���o�b�t�@����
void Texture::CreateBaffer(HRESULT tResult, ID3D12Device* tDevice)
{
	result = tResult;
	device = tDevice;

	//�q�[�v�ݒ�
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

//�f�[�^�]��
void Texture::DetaTransfer(HRESULT tResult)
{
	result = tResult;

	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
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
