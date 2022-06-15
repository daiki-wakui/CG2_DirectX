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

		if (keyboard.keyPush(DIK_D) || keyboard.keyPush(DIK_A)) {
			if (keyboard.keyPush(DIK_D)) {
				angle += XMConvertToRadians(1.0f);
			}
			else if (keyboard.keyPush(DIK_A)) {
				angle -= XMConvertToRadians(1.0f);
			}

			directX.eye.x = -100 * sinf(angle);
			directX.eye.z = -100 * cosf(angle);
			directX.matView = XMMatrixLookAtLH(XMLoadFloat3(&directX.eye), XMLoadFloat3(&directX.target), XMLoadFloat3(&directX.up));
		}

		directX.constMapTransform->mat = directX.matView * directX.matProjection;

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