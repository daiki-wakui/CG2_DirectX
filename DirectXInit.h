#pragma once
#include <vector>
#include <string>
#include "Windows.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
//���w���C�u����
#include <DirectXMath.h>
using namespace DirectX;
//D3D�R���p�C��
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <DirectXTex.h>

class DirectXInit : public Windows {
private:

	//���_�f�[�^�\����
	struct Vertex {
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

	//DirectX�����������p�ϐ�
	
	IDXGIFactory7* dxgiFactory = nullptr;
	IDXGISwapChain4* swapChain = nullptr;
	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12CommandQueue* commandQueue = nullptr;
	ID3D12DescriptorHeap* rtvHeap = nullptr;

	//�X���b�v�`�F�[���̐ݒ�
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	// �f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	// �o�b�N�o�b�t�@
	std::vector<ID3D12Resource*> backBuffers;
	// �t�F���X�̐���
	ID3D12Fence* fence = nullptr;
	UINT64 fenceVal = 0;
	//���\�[�X�o���A
	D3D12_RESOURCE_BARRIER barrierDesc{};
	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	// ���_�o�b�t�@�̐���
	ID3D12Resource* vertBuff = nullptr;
	//�@���_�o�b�t�@�̃f�[�^�]��
	Vertex* vertMap = nullptr;


	ID3DBlob* vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ID3DBlob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g

	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};


	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};

	// ���[�g�V�O�l�`���̃V���A���C�Y
	ID3DBlob* rootSigBlob = nullptr;

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};

	//���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[3] = {};

	//�ݒ������SRV�p�f�X�N���v�^�q�[�v�𐶐�
	ID3D12DescriptorHeap* srvHeap = nullptr;

	ID3D12Resource* constBuffMaterial = nullptr;
	ID3D12Resource* constBuffTransform = nullptr;

public:
	// �p�C�v�����X�e�[�g�̐���
	ID3D12PipelineState* pipelineState = nullptr;
	// ���[�g�V�O�l�`��
	ID3D12RootSignature* rootSignature;
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	HRESULT result;
	ID3D12Device* device = nullptr;

	ID3D12GraphicsCommandList* commandList = nullptr;

	//�f�o�b�N���C���[�̗L����
	ID3D12Debug* debugController;

	// �r���[�|�[�g�ݒ�R�}���h
	D3D12_VIEWPORT viewport{};
	// �V�U�[��`
	D3D12_RECT scissorRect{};

	// �����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;

	//���_�f�[�^
	XMFLOAT3 vertices[6] = {};
	uint16_t indices[6]={};

	//std::vector <uint16_t> indices;

	UINT sizeVB;
	UINT sizeIB;

	D3D12_INDEX_BUFFER_VIEW ibView{};

	DirectXInit();
	void Init(HWND& hwnd);
	void DrawingInit();
	void Update();
	void ResourceBarrier();
	void GraphicCommand();
};

