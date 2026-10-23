#include "WinApp.h"
#include<cstdint>
#include <windows.h>
#include "externals/imgui/imgui.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//ウィンドウプローシージャ//
LRESULT CALLBACK WinApp::Windowproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
#ifdef  USE_IMGUI
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
	{
		return true;
	}
#endif
	//メッセージの種類によって処理を分ける
	switch (msg)
	{
		//ウィンドウが破棄された
	case WM_DESTROY:
		//OSに対してアプリの終了を伝える
		PostQuitMessage(0);

		return 0;
	}
	//デフォルトのメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);

}

void WinApp::Initialize()
{

	HRESULT hr = S_FALSE;
	hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	
	WNDCLASS wc{};

	//ウィンドウプローシージャ
	wc.lpfnWndProc = Windowproc;
	//ウィンドウクラスの名前
	wc.lpszClassName = L"CG2windowClass";
	//インスタンスハンドル
	wc.hInstance = GetModuleHandle(nullptr);
	//カーソル
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	RegisterClass(&wc);

	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

	RECT wrc = { 0,0,kClientWidth ,kClientHeight };

	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);


	HWND hwnd = CreateWindow
	(
		wc.lpszClassName, //クラス名
		L"CG2", //ウィンドウタイトル
		WS_OVERLAPPEDWINDOW, //ウィンドウスタイル
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left, //ウィンドウ幅
		wrc.bottom - wrc.top, //ウィンドウ高
		nullptr, //親ウィンドウハンドル
		nullptr, //メニューハンドル
		wc.hInstance, //インスタンスハンドル
		nullptr
	);


	/*ウィンドウの×ボタンが押されるまでループ*/
	ShowWindow(hwnd, SW_SHOW);
}

void WinApp::Update()
{

}
