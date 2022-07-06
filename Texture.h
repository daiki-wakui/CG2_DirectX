#pragma once

#include <d3d12.h>
#include <DirectXTex.h>
using namespace DirectX;

class Texture{
public:
	HRESULT result;
	ID3D12Device* device = nullptr;

////�摜�f�[�^���擾////
	TexMetadata metadata{};
	ScratchImage scratchImg{};

////�~�b�v�}�b�v����////
	ScratchImage mipChain{};

////���\�[�X�ݒ�////
	D3D12_RESOURCE_DESC textureResouseDesc{};

////�V�F�[�_���\�[�X�r���[�ݒ�////
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

	//�e�N�X�`���o�b�t�@����
	ID3D12Resource* texBuff = nullptr;
public:

	void Load(HRESULT tResult,const wchar_t* image);
	void CreateMipmap(HRESULT tResult);
	void ResourceSetting();
	void ShaderResourceViewSetting();
	void CreateBaffer(HRESULT tResult, ID3D12Device* tDevice);
	void DetaTransfer(HRESULT tResult);
};

