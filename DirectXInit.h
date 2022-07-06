#pragma once
#include <vector>
#include <string>
#include "Windows.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include "KeyBoard.h"
#include "Deeptest.h"

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
		XMFLOAT3 normal;
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

	
	////�[�x�r���[����
	//D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};

public:

	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;

	// �p�C�v�����X�e�[�g�̐���
	ID3D12PipelineState* pipelineState = nullptr;
	// ���[�g�V�O�l�`��
	ID3D12RootSignature* rootSignature;
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	HRESULT result;
	ID3D12Device* device = nullptr;

	//�[�x�N���X
	Deeptest Depth;

	ID3D12GraphicsCommandList* commandList = nullptr;

	//�f�o�b�N���C���[�̗L����
	ID3D12Debug* debugController;

	// �r���[�|�[�g�ݒ�R�}���h
	D3D12_VIEWPORT viewport{};
	// �V�U�[��`
	D3D12_RECT scissorRect{};

	// �����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;

	////�[�x�r���[�p�f�X�N���v�^�q�[�v����
	//ID3D12DescriptorHeap* dsvHeap = nullptr;

	//�[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v����
	//D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;

	//���_�f�[�^
	XMFLOAT3 vertices[24] = {};
	uint16_t indices[36]={};

	//std::vector <uint16_t> indices;

	UINT sizeVB;
	UINT sizeIB;

	D3D12_INDEX_BUFFER_VIEW ibView{};

	struct ConstBufferDataTransfrom {
		XMMATRIX mat;
	};

	ID3D12Resource* constBuffMaterial = nullptr;

	ID3D12Resource* constBuffTransform0 = nullptr;
	ConstBufferDataTransfrom* constMapTransform0 = nullptr;

	ID3D12Resource* constBuffTransform1 = nullptr;
	ConstBufferDataTransfrom* constMapTransform1 = nullptr;

	XMMATRIX matProjection;

	XMMATRIX matWorld;
	XMMATRIX matView;
	XMFLOAT3 eye;
	XMFLOAT3 target;
	XMFLOAT3 up;

	XMMATRIX matWorld1;

	struct Object3d
	{
		//�萔�o�b�t�@(�s��p)
		ID3D12Resource* constBuffTransform;

		//�萔�o�b�t�@�A�b�v(�s��p)
		ConstBufferDataTransfrom* constMapTransform;

		//�A�t�B���ϊ����
		XMFLOAT3 scale = { 1,1,1 };
		XMFLOAT3 rotation = { 0,0,0 };
		XMFLOAT3 position = { 0,0,0 };

		//���[���h�ϊ��s��
		XMMATRIX matWorld;

		//�e�I�u�W�F�N�g�ւ̃|�C���^
		Object3d* parent = nullptr;
	};

	const size_t kOjectConst = 50;

	Object3d object3ds[50];
	
	bool isTex = 0;

	DirectXInit();
	void Init(HWND& hwnd);
	void DrawingInit();
	void Update(KeyBoard &key);
	void DrawUpdate();
	void ResourceBarrier();
	void GraphicCommand(KeyBoard& key);
	void InitializeObject3d(Object3d* object, ID3D12Device* device);
	void CallObject3dInit();
	void UpdateObject3d(Object3d* object, XMMATRIX& matView, XMMATRIX& matProjection);
	void DrawObject3d(
		Object3d* object,
		ID3D12GraphicsCommandList* commandList,
		D3D12_VERTEX_BUFFER_VIEW& vbView,
		D3D12_INDEX_BUFFER_VIEW& ibView,
		UINT numIndices);
};


