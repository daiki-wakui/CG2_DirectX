#pragma once
#include <vector>
#include <string>
#include "Windows.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
//数学ライブラリ
#include <DirectXMath.h>
using namespace DirectX;
//D3Dコンパイラ
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <DirectXTex.h>

class DirectXInit : public Windows {
private:

	//頂点データ構造体
	struct Vertex {
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

	//DirectX初期化処理用変数
	
	IDXGIFactory7* dxgiFactory = nullptr;
	IDXGISwapChain4* swapChain = nullptr;
	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12CommandQueue* commandQueue = nullptr;
	ID3D12DescriptorHeap* rtvHeap = nullptr;

	//スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	// バックバッファ
	std::vector<ID3D12Resource*> backBuffers;
	// フェンスの生成
	ID3D12Fence* fence = nullptr;
	UINT64 fenceVal = 0;
	//リソースバリア
	D3D12_RESOURCE_BARRIER barrierDesc{};
	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	// 頂点バッファの生成
	ID3D12Resource* vertBuff = nullptr;
	//　頂点バッファのデータ転送
	Vertex* vertMap = nullptr;


	ID3DBlob* vsBlob = nullptr; // 頂点シェーダオブジェクト
	ID3DBlob* psBlob = nullptr; // ピクセルシェーダオブジェクト
	ID3DBlob* errorBlob = nullptr; // エラーオブジェクト

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};


	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};

	// ルートシグネチャのシリアライズ
	ID3DBlob* rootSigBlob = nullptr;

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};

	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};

	//ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParams[3] = {};

	//設定を元にSRV用デスクリプタヒープを生成
	ID3D12DescriptorHeap* srvHeap = nullptr;

	ID3D12Resource* constBuffMaterial = nullptr;
	ID3D12Resource* constBuffTransform = nullptr;

public:
	// パイプランステートの生成
	ID3D12PipelineState* pipelineState = nullptr;
	// ルートシグネチャ
	ID3D12RootSignature* rootSignature;
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	HRESULT result;
	ID3D12Device* device = nullptr;

	ID3D12GraphicsCommandList* commandList = nullptr;

	//デバックレイヤーの有効化
	ID3D12Debug* debugController;

	// ビューポート設定コマンド
	D3D12_VIEWPORT viewport{};
	// シザー矩形
	D3D12_RECT scissorRect{};

	// レンダーターゲットビューのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;

	//頂点データ
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

