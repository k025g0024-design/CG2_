#include<windows.h>
#include<cstdint>

//ウィンドウプローシージャ
LRESULT CALLBACK Windowproc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
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



//windowsアプリでエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE,_In_opt_ HINSTANCE,_In_ LPSTR,_In_ int )
{
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

	const int32_t window_width = 1280;
	const int32_t window_height = 720;

	RECT wrc = { 0,0,window_width ,window_height };

	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW,false);

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
	//ウィンドウの×ボタンが押されるまでループ
	ShowWindow(hwnd, SW_SHOW);
	MSG msg{};
	while (msg.message !=WM_QUIT)
	{
		//Windowにメッセージが来たら優先で処理する
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//ここにゲームの更新処理と描画処理を入れる
		}
	}
	//出力ウィンドウへの文字出力
	OutputDebugStringA("Hello,DirectX!\n");
	
	return 0;
}

