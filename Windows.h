#pragma once

class Windows
{
private:
	//�E�B���h�E�v���V�[�W��
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	

public:

	Windows();

	void windowsForm(WNDCLASSEX &w, HWND &hwnd);
};

