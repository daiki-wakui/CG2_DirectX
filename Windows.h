#pragma once

class Windows
{
private:
	//ウィンドウプロシージャ
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	

public:

	Windows();

	void windowsForm(WNDCLASSEX &w, HWND &hwnd);
};

