#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>
#include "KeyBoard.h"
#include "Windows.h"
#include "DirectX.h"
#include "Global.h"

#include <DirectXMath.h>
using namespace DirectX;

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WNDCLASSEX w{};
	HWND hwnd{};

	Windows* win = new Windows;

	win->windowsForm(w,hwnd);

	MSG msg{}; // メッセージ

	//DirectX初期化処理　ここから
#ifdef _DEBUG
//デバッグレイヤーをオンに
	ID3D12Debug* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
	}
#endif

	HRESULT result;
	DirectX *directX = new DirectX;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = {};
	
	directX->DirectXInit(result, hwnd, rtvHandle);

	KeyBoard keyboard(result,hwnd,w);

	//DirectX初期化処理　ここまで

	//描画初期化処理
	
	//定数バッファ用データ構造体(マテリアル)
	struct ConstBufferDataMaterial {
		XMFLOAT4 color;	//色(RGBA)
	};

	//インデックスデータ(03_04)
	uint16_t indices[] = {
		0,1,2,
		1,2,3,
	};

	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
	
	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; //インデックスに入る分のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//インデックスバッファの生成
	//ID3D12Resource* indexBuff = nullptr;
	//result = device->CreateCommittedResource(
	//	&heapProp, // ヒープ設定
	//	D3D12_HEAP_FLAG_NONE,
	//	&resDesc, // リソース設定
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&indexBuff));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	/*XMFLOAT3* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));*/
	// 全頂点に対して
	//for (int i = 0; i < _countof(indices); i++) {
	//	indexMap[i] = indices[i]; // 座標をコピー
	//}
	// 繋がりを解除
	//Buff->Unmap(0, nullptr);

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用

	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ID3D12Resource* constBuffMaterial = nullptr;
	//定数バッファの生成
	/*result = device->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial));
	assert(SUCCEEDED(result));*/

	//定数バッファのマッピング
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);	//マッピング
	assert(SUCCEEDED(result));

	//値を書き込むと自動的に転送される
	constMapMaterial->color = XMFLOAT4(1, 1, 1, 1.0f);

	//ゲームループ
	while (true) {
		// メッセージがある?
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg); // キー入力メッセージの処理
			DispatchMessage(&msg); // プロシージャにメッセージを送る
		}
		// ✖ボタンで終了メッセージが来たらゲームループを抜ける
		if (msg.message == WM_QUIT) {
			break;
		}
		ID3D12GraphicsCommandList* commandList = nullptr;

		// DirectX毎フレーム処理 ここから
		directX->DirectXEvreyFrame(result, commandList, rtvHandle);
		// 1.リソースバリアで書き込み可能に変更
		
		// 2.描画先の変更

		// 3.画面クリア R G B A

		// 4.描画コマンド　ここから
		keyboard.Update();

		if (keyboard.keyPush(DIK_SPACE)) {
			FLOAT clearColor[] = { 0.75f,0.1f, 0.1f,0.0f };
			commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		}

		// 頂点データ
		XMFLOAT3 vertices[] = {
			{ -0.5f, -0.5f, 0.0f }, // 左下
			{ +0.5f, -0.5f, 0.0f }, // 右下
			{ -0.5f, +0.5f, 0.0f }, // 左上
		};

		// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
		UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));

		// 頂点バッファの設定
		D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
		// リソース設定
		D3D12_RESOURCE_DESC resDesc{};
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Width = sizeVB; // 頂点データ全体のサイズ
		resDesc.Height = 1;
		resDesc.DepthOrArraySize = 1;
		resDesc.MipLevels = 1;
		resDesc.SampleDesc.Count = 1;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// 頂点バッファの生成
		//ID3D12Resource* vertBuff = nullptr;
		//result = device->CreateCommittedResource(
		//	&heapProp, // ヒープ設定
		//	D3D12_HEAP_FLAG_NONE,
		//	&resDesc, // リソース設定
		//	D3D12_RESOURCE_STATE_GENERIC_READ,
		//	nullptr,
		//	IID_PPV_ARGS(&vertBuff));
		//assert(SUCCEEDED(result));

		// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
		//XMFLOAT3* vertMap = nullptr;
		//result = vertBuff->Map(0, nullptr, (void**)&vertMap);
		//assert(SUCCEEDED(result));
		//// 全頂点に対して
		//for (int i = 0; i < _countof(vertices); i++) {
		//	vertMap[i] = vertices[i]; // 座標をコピー
		//}
		// 繋がりを解除
		//vertBuff->Unmap(0, nullptr);

		// 頂点バッファビューの作成
		D3D12_VERTEX_BUFFER_VIEW vbView{};
		// GPU仮想アドレス
		//vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
		// 頂点バッファのサイズ
		vbView.SizeInBytes = sizeVB;
		// 頂点1つ分のデータサイズ
		vbView.StrideInBytes = sizeof(XMFLOAT3);

		ID3DBlob* vsBlob = nullptr; // 頂点シェーダオブジェクト
		ID3DBlob* psBlob = nullptr; // ピクセルシェーダオブジェクト
		ID3DBlob* errorBlob = nullptr; // エラーオブジェクト
		// 頂点シェーダの読み込みとコンパイル
		result = D3DCompileFromFile(
			L"BasicVS.hlsl", // シェーダファイル名
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
			"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
			0,
			&vsBlob, &errorBlob);

		// エラーなら
		if (FAILED(result)) {
			// errorBlobからエラー内容をstring型にコピー
			std::string error;
			error.resize(errorBlob->GetBufferSize());
			std::copy_n((char*)errorBlob->GetBufferPointer(),
				errorBlob->GetBufferSize(),
				error.begin());
			error += "\n";
			// エラー内容を出力ウィンドウに表示
			OutputDebugStringA(error.c_str());
			assert(0);
		}

		// ピクセルシェーダの読み込みとコンパイル
		result = D3DCompileFromFile(
			L"BasicPS.hlsl", // シェーダファイル名
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
			"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
			0,
			&psBlob, &errorBlob);

		// エラーなら
		if (FAILED(result)) {
			// errorBlobからエラー内容をstring型にコピー
			std::string error;
			error.resize(errorBlob->GetBufferSize());
			std::copy_n((char*)errorBlob->GetBufferPointer(),
				errorBlob->GetBufferSize(),
				error.begin());
			error += "\n";
			// エラー内容を出力ウィンドウに表示
			OutputDebugStringA(error.c_str());
			assert(0);
		}

		// 頂点レイアウト
		D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
			{
				"POSITION",						//セマンティック名
				0,								//同じセマンティック名が複数あるときに使うインデックス(0でよい)
				DXGI_FORMAT_R32G32B32_FLOAT,	//要素数とビット数を表す(XYZの3つでfloat型なのでR32G32B32_FLOAT)
				0,								//入力スロットインデックス
				D3D12_APPEND_ALIGNED_ELEMENT,	//データのオフセット値(D3D12_APPEND_ALIGNED_ELEMENだと自動設定)
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	//入力データ種別(標準はD3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA)
				0								//一度に描画するインスタンス数(0でよい)
			}, 
		};

		// グラフィックスパイプライン設定
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

		// シェーダーの設定
		pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
		pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
		pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
		pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

		// サンプルマスクの設定
		pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

		// ラスタライザの設定
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // カリングしない
		pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
		pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

		// ブレンドステート(ブレンド設定)
		D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
		blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;// RBGA全てのチャンネルを描画

		//共通設定(アルファ値)
		blenddesc.BlendEnable = true;	//ブレンドを有効にする
		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
		blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;	//ソースの値を100%使う
		blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//テストの値を0%使う

		//加算合成
		//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	//加算
		//blenddesc.SrcBlend = D3D12_BLEND_ONE;	//ソースの値を100%使う
		//blenddesc.DestBlend = D3D12_BLEND_ONE;	//テストの値を100%使う

		////減算合成
		//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	//テストからソースを減算
		//blenddesc.SrcBlend = D3D12_BLEND_ONE;	//ソースの値を100%使う
		//blenddesc.DestBlend = D3D12_BLEND_ONE;	//テストの値を100%使う

		//色反転
		//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	//加算
		//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;	//1.0f-デストカラーの値
		//blenddesc.DestBlend = D3D12_BLEND_ZERO;	//使わない

		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	//加算
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;	//ソースのアルファ値
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	//1.0f-ソースのアルファ値

		// 頂点レイアウトの設定
		pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
		pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

		// 図形の形状設定
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		// その他の設定
		pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
		pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
		pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

		//ルートパラメータの設定(03_02)
		D3D12_ROOT_PARAMETER rootParam = {};
		rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
		rootParam.Descriptor.ShaderRegister - 0;	//定数バッファ番号
		rootParam.Descriptor.RegisterSpace = 0;	//デフォルト値
		rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える

		// ルートシグネチャ
		ID3D12RootSignature* rootSignature;
		// ルートシグネチャの設定
		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		//＋03_02
		rootSignatureDesc.pParameters = &rootParam;	//ルートパラメータの先頭アドレス
		rootSignatureDesc.NumParameters = 1;	//ルートパラメータ数

		// ルートシグネチャのシリアライズ
		/*ID3DBlob* rootSigBlob = nullptr;
		result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
			&rootSigBlob, &errorBlob);
		assert(SUCCEEDED(result));
		result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
			IID_PPV_ARGS(&rootSignature));
		assert(SUCCEEDED(result));
		rootSigBlob->Release();*/
		// パイプラインにルートシグネチャをセット
		pipelineDesc.pRootSignature = rootSignature;

		// パイプランステートの生成
		/*ID3D12PipelineState* pipelineState = nullptr;
		result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
		assert(SUCCEEDED(result));*/

		// ビューポート設定コマンド
		D3D12_VIEWPORT viewport{};
		viewport.Width = window_width;	//横幅
		viewport.Height = window_height;	//縦幅
		viewport.TopLeftX = 0;	//左上X
		viewport.TopLeftY = 0;	//左上Y
		viewport.MinDepth = 0.0f;	//最小深度
		viewport.MaxDepth = 1.0f;	//最大深度
		// ビューポート設定コマンドを、コマンドリストに追加する
		commandList->RSSetViewports(1, &viewport);

		// シザー矩形の設定コマンド
		D3D12_RECT scissorRect{};
		scissorRect.left = 0; // 切り抜き座標左
		scissorRect.right = scissorRect.left + window_width; // 切り抜き座標右
		scissorRect.top = 0; // 切り抜き座標上
		scissorRect.bottom = scissorRect.top + window_height; // 切り抜き座標下
		// シザー矩形設定コマンドを、コマンドリストに追加する
		commandList->RSSetScissorRects(1, &scissorRect);

		// パイプラインステートとルートシグネチャの設定コマンド
		/*commandList->SetPipelineState(pipelineState);
		commandList->SetGraphicsRootSignature(rootSignature);*/

		// プリミティブ形状の設定コマンド
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // リストの形状

		// 頂点バッファビューの設定コマンド
		commandList->IASetVertexBuffers(0, 1, &vbView);

		//定数バッファビュー(CBV)の設定コマンド(03_02)
		//(0)は配列番号のゼロ
		commandList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

		// 描画コマンド
		commandList->DrawInstanced(_countof(vertices), 1, 0, 0); // 全ての頂点を使って描画
		// 4.描画コマンド　ここまで

		// 5.リソースバリアを戻す
		directX->ResourceBarrier(result,commandList);

		// DirectX毎フレーム処理 ここまで
	}

	// ウィンドウクラスを登録解除
	UnregisterClass(w.lpszClassName, w.hInstance);

	//コンソールへの文字入力
	OutputDebugStringA("Hello,DirectX!!\n");

	return 0;
}
