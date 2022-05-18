#pragma once
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <dinput.h>

using namespace DirectX;

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#define DIRECTINPUT_VERSION	0x0800

class KeyBoard{
private:

	IDirectInputDevice8* keyboard = nullptr;
	BYTE key[256] = {};
	BYTE oldkey[256] = {};

public:

	KeyBoard(HRESULT &result, HWND &hwnd, WNDCLASSEX &w);

	void Update();

	bool keyPush(uint8_t key);
	bool keyRelease(uint8_t key);
	bool keyInstantPush(uint8_t key);
	bool keyInstantRelease(uint8_t key);

};

