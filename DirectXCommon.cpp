#include "DirectXCommon.h"
#include<cassert>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

using namespace Microsoft::WRL;

void DirectXCommon::Initialize()
{
	assert(winApp);
	this->winApp = winApp;


	void DirectXCommon::DeviceCreate()
	{
		//デバイス生成

		Microsoft::WRL::ComPtr<ID3D12Device> device = nullptr;
		D3D_FEATURE_LEVEL featureLevels[] =
		{
	D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0
		};

		const char* featureLevelsStrings[] = { "12.2","12.1","12.0" };
		for (size_t i = 0; i < _countof(featureLevels); ++i)
		{
			hr = D3D12CreateDevice(useAdapter.Get(), featureLevels[i], IID_PPV_ARGS(&device));
			if (SUCCEEDED(hr))
			{
				Log(std::format("FeatureLevel:{}\n", featureLevelsStrings[i]));
				break;

			}

		}
		assert(device != nullptr);
		Log("Complete create D3D12Device!!!\n");
	}

	void DirectXCommon::CommandInitialize()
	{
		// コマンド関連の生成

		Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue = nullptr;
		D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
		hr = device->CreateCommandQueue(&commandQueueDesc,
			IID_PPV_ARGS(&commandQueue));
		assert(SUCCEEDED(hr));
		//コマンドアロケーターを生成する
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator = nullptr;
		hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
		assert(SUCCEEDED(hr));
		//コマンドリストを生成する
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;
		hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr,
			IID_PPV_ARGS(&commandList));
		assert(SUCCEEDED(hr));
	}

	void DirectXCommon::CreateSwapChain()
	{
		// スワップチェーンの生成

		Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain = nullptr;
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
		swapChainDesc.Width = WinApp::kClientWidth;
		swapChainDesc.Height = WinApp::kClientHeight;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		//コマンドキュー、ウィンドウハンドル、設定を渡して生成する
		hr = dxgiFactory->CreateSwapChainForHwnd(commandQueue.Get(), winApp->GetHwnd(), &swapChainDesc, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(swapChain.GetAddressOf()));
		assert(SUCCEEDED(hr));
	}

	void DirectXCommon::DepthBufferCreation()
	{

		// 深度バッファの生成

		Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource = CreateDepthStencilTextureResource(device.Get(), WinApp::kClientWidth, WinApp::kClientHeight);

	}

	void DirectXCommon::CreateDescriptorHeaps()
	{
		// 各種でスクリプタヒープの生成

	//ディスクリプタヒープの生成
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap = CreatDescriptoHeap(device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false);
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap = CreatDescriptoHeap(device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 128, true);
		//DSV用のヒープ
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap = CreatDescriptoHeap(device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);

	}

	void DirectXCommon::RTVInitializ()
	{

		// レンダーターゲットビューの初期化

		//RTVの設定
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle = rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2]{};
		rtvHandles[0] = rtvStartHandle;
		device->CreateRenderTargetView(swapChaiResources[0].Get(), &rtvDesc, rtvHandles[0]);
		rtvHandles[1].ptr = rtvHandles[0].ptr + device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		device->CreateRenderTargetView(swapChaiResources[1].Get(), &rtvDesc, rtvHandles[1]);


	}

	void DirectXCommon::DepthStencilViewInitializ()
	{
		// 深度ステンシルビューの初期化

	//DSVの設定
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

		device->CreateDepthStencilView(depthStencilResource.Get(), &dsvDesc, dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	}

	void DirectXCommon::FenceInitializ()
	{
		// フェンスの生成

		//初期値でFence作成
		Microsoft::WRL::ComPtr<ID3D12Fence> fence = nullptr;
		uint16_t fenceValue = 0;
		hr = device->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
		assert(SUCCEEDED(hr));

	}

	void DirectXCommon::Viewport()
	{
		//ビューポート
		D3D12_VIEWPORT viewport{};
		viewport.Width = WinApp::kClientWidth;
		viewport.Height = WinApp::kClientHeight;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
	}

	void DirectXCommon::ScissorRect()
	{
		//シザー短形
		D3D12_RECT scissorRect{};

		scissorRect.left = 0;
		scissorRect.right = WinApp::kClientWidth;
		scissorRect.top = 0;
		scissorRect.bottom = WinApp::kClientHeight;

	}

	void DirectXCommon::DxcCompilerCreate()
	{
		// DCXコンパイラの生成

		//dxcCompilerの初期化
		Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils = nullptr;
		hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
		assert(SUCCEEDED(hr));
		Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler = nullptr;
		hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
		assert(SUCCEEDED(hr));

		//現時点でincludeはしないが、includeに対応するための設定を行っておく
		Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler = nullptr;
		hr = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
		assert(SUCCEEDED(hr));

	}

	void DirectXCommon::ImGuiInitialize()
	{
		//ImGuiの初期化

#ifdef  USE_IMGUI
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplWin32_Init(winApp->GetHwnd);
		ImGui_ImplDX12_Init(device.Get(), swapChainDesc.BufferCount, rtvDesc.Format, srvDescriptorHeap.Get(), srvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), srvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->Build();
#endif


	}


}
