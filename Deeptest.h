#pragma once
#include <d3d12.h>
#include "Global.h"

class Deeptest{
public:

////�ݒ����////

	//�[�x�o�b�t�@���\�[�X�ݒ�
	D3D12_RESOURCE_DESC depthResourceDesc{};
	
	//�[�x�l�p�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES deptHeapProp{};
	
	//�[�x�l�̃N���A�l
	D3D12_CLEAR_VALUE depthClearValue{};
	
////��������////

	HRESULT result;
	ID3D12Device* device = nullptr;

	//���\�[�X����
	ID3D12Resource* depthBuff = nullptr;

	//�[�x�r���[�p�f�X�N���v�^�q�[�v����
	ID3D12DescriptorHeap* dsvHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};

	//�[�x�r���[����
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};

	//�[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v����
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;

	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	//D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	
public:
	Deeptest();

	//�ݒ�
	void Setting();
	//����
	void Create(HRESULT dResult, ID3D12Device* dDevice);

	//void RenderTargetViewSet();

	//�O���t�B�b�N�p�C�v���C���������ɐ[�x�e�X�g�̐ݒ肷����
	//void DepthStencilState();
};

