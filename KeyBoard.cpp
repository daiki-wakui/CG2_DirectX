#include "KeyBoard.h"
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <dinput.h>

using namespace DirectX;

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#define DIRECTINPUT_VERSION	0x0800


KeyBoard::KeyBoard(HRESULT &result, HWND &hwnd, WNDCLASSEX &w){

	//DirectInputの初期化
	IDirectInput8* directInput = nullptr;
	result = DirectInput8Create(
		w.hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	//キーボードデバイスの生成
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	//入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);	//標準形式
	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = keyboard->SetCooperativeLevel(
		hwnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void KeyBoard::Update(){
	//キーボード情報の取得開始
	keyboard->Acquire();

	for (int i = 0; i < 256; i++) {
		oldkey[i] = key[i];
	}

	//全キーの入力状態を取得する
	keyboard->GetDeviceState(sizeof(key), key);
}

bool KeyBoard::keyPush(uint8_t key) {
	return this->key[key];
}

bool KeyBoard::keyRelease(uint8_t key){
	return !this->key[key];
}

bool KeyBoard::keyInstantPush(uint8_t key){
	return this->key[key] && !oldkey[key];
}

bool KeyBoard::keyInstantRelease(uint8_t key){
	return !this->key[key] && oldkey[key];
}