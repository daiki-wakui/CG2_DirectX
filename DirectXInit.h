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
		XMFLOAT3 normal;
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

	
	////深度ビュー生成
	//D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};

public:

	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;

	// パイプランステートの生成
	ID3D12PipelineState* pipelineState = nullptr;
	// ルートシグネチャ
	ID3D12RootSignature* rootSignature;
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	HRESULT result;
	ID3D12Device* device = nullptr;

	//深度クラス
	Deeptest Depth;

	ID3D12GraphicsCommandList* commandList = nullptr;

	//デバックレイヤーの有効化
	ID3D12Debug* debugController;

	// ビューポート設定コマンド
	D3D12_VIEWPORT viewport{};
	// シザー矩形
	D3D12_RECT scissorRect{};

	// レンダーターゲットビューのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;

	////深度ビュー用デスクリプタヒープ生成
	//ID3D12DescriptorHeap* dsvHeap = nullptr;

	//深度ステンシルビュー用デスクリプタヒープ生成
	//D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;

	//頂点データ
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
		//定数バッファ(行列用)
		ID3D12Resource* constBuffTransform;

		//定数バッファアップ(行列用)
		ConstBufferDataTransfrom* constMapTransform;

		//アフィン変換情報
		XMFLOAT3 scale = { 1,1,1 };
		XMFLOAT3 rotation = { 0,0,0 };
		XMFLOAT3 position = { 0,0,0 };

		//ワールド変換行列
		XMMATRIX matWorld;

		//親オブジェクトへのポインタ
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


