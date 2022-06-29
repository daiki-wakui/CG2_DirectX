#include <windows.h>
#include "Windows.h"
#include "DirectXInit.h"
#include "Global.h"
#include "KeyBoard.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//windowsクラス
	Windows windows;

	//DirectXクラス
	DirectXInit directX;

	//WindowsAPIの生成
	windows.create();
	MSG msg{}; // メッセージ

	//DirectX初期化処理ここから
	float angle = 0.0f;

	XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
	XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };
	XMFLOAT3 position = { 0.0f,0.0f,0.0f };

	directX.Init(windows.hwnd);
	KeyBoard keyboard(directX.result, windows.hwnd, windows.w);
	//DirectX初期化処理ここまで

	//描画初期化処理
	directX.DrawingInit();

	//ゲームループ
	while (true) {

		// ✖ボタンで終了メッセージが来たら
		if (windows.gameloopExit(msg) == true) {
			break;	//終了
		}

		//DirectX毎フレーム処理　ここから

		//カメラ回転
		if (keyboard.keyPush(DIK_D) || keyboard.keyPush(DIK_A)) {
			if (keyboard.keyPush(DIK_D)) {
				angle += XMConvertToRadians(1.0f);
			}
			else if (keyboard.keyPush(DIK_A)) {
				angle -= XMConvertToRadians(1.0f);
			}

			//Y軸まわりに回転
			directX.eye.x = -100 * sinf(angle);
			directX.eye.z = -100 * cosf(angle);
			directX.matView = XMMatrixLookAtLH(XMLoadFloat3(&directX.eye), XMLoadFloat3(&directX.target), XMLoadFloat3(&directX.up));
		}

		if (keyboard.keyPush(DIK_W) || keyboard.keyPush(DIK_S)) {
			if (keyboard.keyPush(DIK_W)) {
				angle += XMConvertToRadians(1.0f);
			}
			else if (keyboard.keyPush(DIK_S)) {
				angle -= XMConvertToRadians(1.0f);
			}

			//Y軸まわりに回転
			directX.eye.y = -100 * sinf(angle);
			directX.eye.z = -100 * cosf(angle);
			directX.matView = XMMatrixLookAtLH(XMLoadFloat3(&directX.eye), XMLoadFloat3(&directX.target), XMLoadFloat3(&directX.up));
		}


		if (keyboard.keyPush(DIK_UP) || keyboard.keyPush(DIK_DOWN) || keyboard.keyPush(DIK_RIGHT) || keyboard.keyPush(DIK_LEFT)) {
			if (keyboard.keyPush(DIK_UP)) {
				position.z += 1.0f;
			}
			else if (keyboard.keyPush(DIK_DOWN)) {
				position.z -= 1.0f;
			}

			if (keyboard.keyPush(DIK_RIGHT)) {
				position.x += 1.0f;
			}
			else if (keyboard.keyPush(DIK_LEFT)) {
				position.x -= 1.0f;
			}
		}

		//ワールド変換行列
		

		//スケーリング
		XMMATRIX matScale;
		matScale = XMMatrixScaling(scale.x, scale.y, scale.z);

		//回転
		XMMATRIX matRot;
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(rotation.z);	//Z 0度回転
		matRot *= XMMatrixRotationX(rotation.x);	//X 0度回転
		matRot *= XMMatrixRotationY(rotation.y);	//Y 0度回転

		//平行移動
		XMMATRIX matTrans;
		matTrans = XMMatrixTranslation(position.x, position.y, position.z);

		directX.matWorld = XMMatrixIdentity();
		directX.matWorld *= matScale;
		directX.matWorld *= matRot;
		directX.matWorld *= matTrans;

		//定数バッファにデータ転送
		directX.constMapTransform->mat = directX.matWorld * directX.matView * directX.matProjection;

		directX.DrawUpdate();
		keyboard.Update();

		if (keyboard.keyPush(DIK_SPACE)) {
			FLOAT clearColor[] = { 0.75f,0.1f, 0.1f,0.0f };
			directX.commandList->ClearRenderTargetView(directX.rtvHandle, clearColor, 0, nullptr);
		}



		//グラフィックコマンド
		directX.GraphicCommand();

		directX.ResourceBarrier();
		//DirectX毎フレーム処理　ここまで

	}
	// ウィンドウクラスを登録解除
	windows.Release();

	return 0;
}