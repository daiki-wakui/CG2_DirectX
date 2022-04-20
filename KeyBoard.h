#pragma once
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <dinput.h>

using namespace DirectX;

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#define DIRECTINPUT_VERSION	0x0800

class KeyBoard
{
public:

	bool keyPush(uint8_t key, uint8_t oldkey);
	bool keyRelease(uint8_t key, uint8_t oldkey);
	bool keyInstantPush(uint8_t key, uint8_t oldkey);
	bool keyInstantRelease(uint8_t key, uint8_t oldkey);


};

