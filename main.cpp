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
		if (windows.gameloopExit(msg) == true || keyboard.keyInstantPush(DIK_ESCAPE)) {
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

			//X軸まわりに回転
			directX.eye.y = -100 * sinf(angle);
			directX.eye.z = -100 * cosf(angle);
			directX.matView = XMMatrixLookAtLH(XMLoadFloat3(&directX.eye), XMLoadFloat3(&directX.target), XMLoadFloat3(&directX.up));
		}

		//ワールド変換行列
		for (size_t i = 0; i < _countof(directX.object3ds); i++) {
			directX.UpdateObject3d(&directX.object3ds[i], directX.matView, directX.matProjection);
		}

		if (keyboard.keyPush(DIK_UP)) { directX.object3ds[0].position.y += 1.0f; }
		if (keyboard.keyPush(DIK_DOWN)){ directX.object3ds[0].position.y -= 1.0f; }
		if (keyboard.keyPush(DIK_RIGHT)){ directX.object3ds[0].position.x += 1.0f; }
		if (keyboard.keyPush(DIK_LEFT)){ directX.object3ds[0].position.x -= 1.0f; }
		if (keyboard.keyPush(DIK_R)) {directX.object3ds[0].rotation.x += 0.05f;}
		if (keyboard.keyPush(DIK_T)) {directX.object3ds[0].rotation.x -= 0.05f;}
		if (keyboard.keyPush(DIK_Y)) { directX.object3ds[0].rotation.y += 0.05f; }
		if (keyboard.keyPush(DIK_U)) { directX.object3ds[0].rotation.y -= 0.05f; }
		if (keyboard.keyPush(DIK_I)) { directX.object3ds[0].rotation.z += 0.05f; }
		if (keyboard.keyPush(DIK_O)) { directX.object3ds[0].rotation.z -= 0.05f; }

		directX.DrawUpdate(keyboard);
		keyboard.Update();

		//グラフィックコマンド
		directX.GraphicCommand(keyboard);

		directX.ResourceBarrier();
		//DirectX毎フレーム処理　ここまで

	}
	// ウィンドウクラスを登録解除
	windows.Release();

	return 0;
}