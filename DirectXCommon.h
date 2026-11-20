#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "WinApp.h"


class DirectXCommon
{
public:
	void Initialize();
	void DeviceCreate();
	void CommandInitialize();
	void CreateSwapChain();
	void DepthBufferCreation();
	void CreateDescriptorHeaps();
	void RTVInitializ();
	void DepthStencilViewInitializ();
	void FenceInitializ();
	void Viewport();
	void ScissorRect();
	void DxcCompilerCreate();
	void ImGuiInitialize();

	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, 2>SwapChainResources;

private:
	Microsoft::WRL::ComPtr<ID3D12Device>device;
	Microsoft::WRL::ComPtr<IDXGIFactory7>dxgiFactory;
	WinApp* winApp = nullptr;
};

