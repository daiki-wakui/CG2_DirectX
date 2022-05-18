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

	// フェンスの生成
	ID3D12Fence* fence = nullptr;
	UINT64 fenceVal = 0;

	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};

	// バックバッファ
	std::vector<ID3D12Resource*> backBuffers;
	
	// 1.リソースバリアで書き込み可能に変更
	D3D12_RESOURCE_BARRIER barrierDesc{};

public:

	DirectX();

	void DirectXInit(HRESULT& result, HWND& hwnd, D3D12_CPU_DESCRIPTOR_HANDLE &rtvHandle);
	void DirectXEvreyFrame(HRESULT& result, ID3D12GraphicsCommandList* &commandList, D3D12_CPU_DESCRIPTOR_HANDLE &rtvHandle);
	void ResourceBarrier(HRESULT& result, ID3D12GraphicsCommandList* &commandList);
};
