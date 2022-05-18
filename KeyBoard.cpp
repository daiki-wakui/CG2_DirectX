#include "KeyBoard.h"
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <dinput.h>

using namespace DirectX;

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#define DIRECTINPUT_VERSION	0x0800


KeyBoard::KeyBoard(HRESULT &result, HWND &hwnd, WNDCLASSEX &w){

	//DirectInput�̏�����
	IDirectInput8* directInput = nullptr;
	result = DirectInput8Create(
		w.hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	//�L�[�{�[�h�f�o�C�X�̐���
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	//���̓f�[�^�`���̃Z�b�g
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);	//�W���`��
	assert(SUCCEEDED(result));

	//�r�����䃌�x���̃Z�b�g
	result = keyboard->SetCooperativeLevel(
		hwnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void KeyBoard::Update(){
	//�L�[�{�[�h���̎擾�J�n
	keyboard->Acquire();

	for (int i = 0; i < 256; i++) {
		oldkey[i] = key[i];
	}

	//�S�L�[�̓��͏�Ԃ��擾����
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