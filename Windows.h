#pragma once
#include <Windows.h>

class Windows {
private:
	//ウィンドウプロシージャ
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:
	WNDCLASSEX w{};
	HWND hwnd{};

	Windows();
	void create();
	void Release();
	bool gameloopExit(MSG& msg);
};
