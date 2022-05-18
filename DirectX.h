#pragma once
#include <vector>
#include <string>

class DirectX : public Windows{
private:
	ID3D12Device* device = nullptr;
	IDXGISwapChain4* swapChain = nullptr;
	ID3D12GraphicsCommandList* commandList = nullptr;
	ID3D12DescriptorHeap* rtvHeap = nullptr;
	ID3D12CommandQueue* commandQueue = nullptr;
	ID3D12CommandAllocator* commandAllocator = nullptr;

	// �t�F���X�̐���
	ID3D12Fence* fence = nullptr;
	UINT64 fenceVal = 0;

	// �f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};

	// �o�b�N�o�b�t�@
	std::vector<ID3D12Resource*> backBuffers;
	
	// 1.���\�[�X�o���A�ŏ������݉\�ɕύX
	D3D12_RESOURCE_BARRIER barrierDesc{};

public:

	DirectX();

	void DirectXInit(HRESULT& result, HWND& hwnd, D3D12_CPU_DESCRIPTOR_HANDLE &rtvHandle);
	void DirectXEvreyFrame(HRESULT& result, ID3D12GraphicsCommandList* &commandList, D3D12_CPU_DESCRIPTOR_HANDLE &rtvHandle);
	void ResourceBarrier(HRESULT& result, ID3D12GraphicsCommandList* &commandList);
};
