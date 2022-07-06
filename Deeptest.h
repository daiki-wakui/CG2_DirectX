#pragma once
#include <d3d12.h>
#include "Global.h"

class Deeptest{
public:

////設定周り////

	//深度バッファリソース設定
	D3D12_RESOURCE_DESC depthResourceDesc{};
	
	//深度値用ヒーププロパティ
	D3D12_HEAP_PROPERTIES deptHeapProp{};
	
	//深度値のクリア値
	D3D12_CLEAR_VALUE depthClearValue{};
	
////生成周り////

	HRESULT result;
	ID3D12Device* device = nullptr;

	//リソース生成
	ID3D12Resource* depthBuff = nullptr;

	//深度ビュー用デスクリプタヒープ生成
	ID3D12DescriptorHeap* dsvHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};

	//深度ビュー生成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};

	//深度ステンシルビュー用デスクリプタヒープ生成
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;

	// グラフィックスパイプライン設定
	//D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	
public:
	Deeptest();

	//設定
	void Setting();
	//生成
	void Create(HRESULT dResult, ID3D12Device* dDevice);

	//void RenderTargetViewSet();

	//グラフィックパイプライン生成時に深度テストの設定するやつ
	//void DepthStencilState();
};

